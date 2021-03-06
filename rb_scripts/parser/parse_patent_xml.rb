# -*- coding: utf-8 -*-
require "kconv"
require "logger"
require "rubygems"
require "hpricot"
require "./db_util.rb"

DATA_PATH = "../patent"

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
  #ログ出力
  $log.info("Reading #{file_name}.")

  #kindが特許公報で無い場合falseを返して終了
  kind = get_kind(year, file_name)
  if kind !~ /特許公報/
    $log.error("#{file_name}は特許公報ではない")
    return false
  end

  #訂正情報の判定
  error_flag = kind =~ /訂正/ ? true : false
  
  open("#{DATA_PATH}/#{year}/#{file_name}"){|f|
    xml = Hpricot(f.read)
    
    #訂正公報の場合、"publication-reference"/"document-id"/"doc-number"が2つ存在する
    #特許番号、本文、タイトル、IPCのmain-clsfを取得
    patent_id = (xml/"publication-reference"/"document-id"/"doc-number").first.inner_text.to_i

    #タイトルが存在しない場合、処理を終了させる
    tmp_title = (xml/"invention-title")
    if tmp_title.size == 0
      $log.error("#{file_name}にはタイトルが存在しない")
      return false
    end
    
    title = tmp_title.first.inner_text.toutf8
    
    body = (xml/"description").inner_text.toutf8

    #ipcは先頭三文字(ex. A01)までを取得する(これはクラスと呼ばれる分類)
    ipc = (xml/"classification-ipc"/"main-clsf").inner_text.toutf8[0..2]

    #app_id|nameを取得
    #これは特許を申請した会社のid|name
    #app_id|nameが存在しない場合があるのでその判定を行う
    tmp_id = (xml/"applicant"/"registered-number")
    if tmp_id.size > 0
      app_id = tmp_id.first.inner_text.toutf8
      
      #idは空の場合nilでは無く謎スペースが入っているので、スペースがマッチしたらnilを返しておく
      app_id = nil if app_id[0] == " "
      app_name = (xml/"applicant"/"name").first.inner_text.toutf8
    else
      app_id = nil
    end
    
    #tokyocabinetへの書き込み
    #本来ならネストしたhashをMarshal.dumpするところだがC/C++で叩く予定があるので
    #今回は属性ごとに別のデータベースファイルを作る。非常に煩雑
    put_entry :key => patent_id, :value => title, :type => "title", :year => year, :flag => error_flag
    put_entry :key => patent_id, :value => body, :type => "body", :year => year, :flag => error_flag
    put_entry :key => patent_id, :value => ipc, :type => "ipc", :year => year, :flag => error_flag

    #app_idがnilで場合で無い場合のみ、app_name => app_id 及び patent_id => app_name を紐付ける
    if !app_id.nil?
      put_entry :key => app_name, :value => app_id, :type => "app_name", :year => year, :flag => error_flag 
      put_entry :key => patent_id, :value => app_name, :type => "app_name", :year => year, :flag => error_flag
    end
    
    #訂正の場合、訂正理由を残しておく
    if error_flag
      reason = (xml/"jp:gist-of-correction").inner_text.toutf8
      put_entry :key => patent_id, :value => reason, :type => "reason", :year => year, :flag => error_flag
    end
  }
end

#ex. parse_all_xml("H16")
def parse_and_put_all_xml(year)
  file_size = Dir.entries("#{DATA_PATH}/#{year}").size - 2
  count = 0.0
  
  Dir.foreach("#{DATA_PATH}/#{year}") do |file_name|
    #特許公報のみ使用するためのチェック
    next if file_name == "." || file_name == ".."
    parse_and_put_xml(year, file_name)
    count += 1
    puts "Now #{count / file_size * 100}%"
  end
end

if $0 == __FILE__
  year = ARGV[0]

  #めんどくさいのでグローバル変数で
  $log = Logger.new("#{DATA_PATH}/logs/#{year}.log")
  
  parse_and_put_all_xml(year)
end
