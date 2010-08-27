# -*- coding: utf-8 -*-
require "kconv"
require "rubygems"
require "hpricot"
require "tokyocabinet"
include TokyoCabinet

DATA_PATH = "../patent"

#tokyocabinetからの読み書き
def get_entry(key, year, db_type)
  key = param[:key]
  year = param[:year]
  db_type = param[:db_type]

  db = TokyoCabinet::HDB.new

  #まずは#{year}.hdbから読み込む
  db.open("#{DATA_PATH}/db/#{year}_#{db_type}.hdb", HDB::OWRITER | HDB::OCREAT)
  value_1 = db.get(key)
  db.close

  #その後#{year}_error.hdbも読み込む
  #valueが存在しない場合nilが返ってくる
  db.open("#{DATA_PATH}/db/#{year}_#{db_type}_error.hdb", HDB::OWRITER | HDB::OCREAT)
  value_2 = db.get(key)
  db.close

  #value_2がnilの場合value_1のみを返し、nilで無い場合、Marshal.loadしたものを返す
  return value_2.nil? ? value_1 : [value_1, value_2]
end

def put_entry(param)
  key = param[:key]
  value = param[:value]
  year = param[:year]
  db_type = param[:type]
  error_flag = param[:flag]
  
  db = TokyoCabinet::HDB.new
  
  #error_flagがtrueの場合は#{year}_#{db_type}_error.hdbを開く
  if error_flag
    db.open("#{DATA_PATH}/db/#{year}_#{db_type}_error.hdb", HDB::OWRITER | HDB::OCREAT)
  else
    db.open("#{DATA_PATH}/db/#{year}_#{db_type}.hdb", HDB::OWRITER | HDB::OCREAT)    
  end

  db.put(key, value)
  db.close
end

#ex. get_kind("H16", "1995091152.xml")
#カインドを取得するだけのメソッド
def get_kind(year, file_name)
  open("#{DATA_PATH}/#{year}/#{file_name}"){|f|
    xml = Hpricot(f.read)
    return (xml/:kind).inner_text.toutf8
  }
end

#ex. parse_xml("H16", "1995091152.xml")
def parse_and_put_xml(year, file_name)

  #kindが特許公報で無い場合falseを返して終了
  kind = get_kind(year, file_name)
  return false if kind !~ /特許公報/
    
  error_flag = kind =~ /訂正/ ? true : false
  
  open("#{DATA_PATH}/#{year}/#{file_name}"){|f|
    xml = Hpricot(f.read)
    
    #訂正公報の場合、"publication-reference"/"document-id"/"doc-number"が2つ存在する
    #特許番号、本文、IPCのmain-clsfを取得
    #ipcは先頭三文字(ex. A01)までを取得する(これはクラスと呼ばれる分類)
    patent_id = (xml/"publication-reference"/"document-id"/"doc-number").first.inner_text.to_i
    body = (xml/"description").inner_text.toutf8
    ipc = (xml/"classification-ipc"/"main-clsf").inner_text.toutf8[0..2]

    #app_id|nameを取得
    #これは特許を申請した会社のid|name
    #idは空の場合nilでは無く謎スペースが入っているので、スペースがマッチしたらnilを返しておく
    app_id = (xml/"applicant"/"registered-number").first.inner_text.toutf8
    app_id = nil if app_id[0] == " "
    app_name = (xml/"applicant"/"name").first.inner_text.toutf8

    #tokyocabinetへの書き込み
    #本来ならネストしたhashをMarshal.dumpするところだがC/C++で叩く予定があるので
    #今回は属性ごとに別のデータベースファイルを作る。非常に煩雑
    put_entry :key => patent_id, :value => body, :type => "body", :year => year, :flag => error_flag
    put_entry :key => patent_id, :value => ipc, :type => "ipc", :year => year, :flag => error_flag
    put_entry :key => patent_id, :value => app_name, :type => "app_name", :year => year, :flag => error_flag

    #app_idがnilで場合で無い場合のみ、nameとidを紐付ける
    put_entry :key => app_name, :value => app_id, :type => "app_name", :year => year, :flag => error_flag if !app_id.nil?
  }
end

#ex. parse_all_xml("H16")
def parse_and_put_all_xml(year)
  Dir.foreach("#{DATA_PATH}/#{year}") do |file_name|
    #特許公報のみ使用するためのチェック
    next if file_name == "." || file_name == ".."
    parse_and_put_xml(year, file_name)
  end
end

if $0 == __FILE__
  year = ARGV[0]
  parse_and_put_all_xml(year)
end
