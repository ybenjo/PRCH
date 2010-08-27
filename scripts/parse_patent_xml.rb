# -*- coding: utf-8 -*-
require "kconv"
require "rubygems"
require "hpricot"
require "tokyocabinet"

DATA_PATH = "../patent"

#tokyocabinetからの読み書き
def get_entry(key, year)
  db = TokyoCabinet::HDB.new

  #まずは#{year}.hdbから読み込む
  db.open("#{DATA_PATH}/db/#{year}.hdb", HDB::OWRITER | HDB::OCREAT)
  value_1 = Marshal.load(db.get(key))
  db.close

  #その後#{year}_error.hdbも読み込む
  #valueが存在しない場合nilが返ってくる
  db.open("#{DATA_PATH}/db/#{year}_error.hdb", HDB::OWRITER | HDB::OCREAT)
  value_2 = db.get(key)
  db.close

  #value_2がnilの場合value_1のみを返し、nilで無い場合、Marshal.loadしたものを返す
  return value_2.nil? ? value_1 : [value_1, Marshal.load(value_2)]
end

def put_entry(key, value, year, error_flag)
  db = TokyoCabinet::HDB.new
  
  #error_flagがtrueの場合は#{year}_error.hdbを開く
  if error_flag
    db.open("#{DATA_PATH}/db/#{year}_error.hdb", HDB::OWRITER | HDB::OCREAT)
  else
    db.open("#{DATA_PATH}/db/#{year}.hdb", HDB::OWRITER | HDB::OCREAT)    
  end

  db.put(key, Marshal.dump(value))
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
def parse_xml(year, file_name, error_flag)
  open("#{DATA_PATH}/#{year}/#{file_name}"){|f|
    xml = Hpricot(f.read)
    #訂正公報の場合、"publication-reference"/"document-id"/"doc-number"が2つ存在する
    id = (xml/"publication-reference"/"document-id"/"doc-number").first.inner_text.to_i
  }
end

if $0 == __FILE__
  year = "H16"
  Dir.foreach("#{DATA_PATH}/#{year}") do |fname|

    #特許公報のみ使用するためのチェック
    next if fname == "." || fname == ".."
    kind = get_kind(year, fname)
    next if kind !~ /特許公報/
    
    error_flag = kind =~ /訂正/ ? true : false
    
    parse_xml(year, fname, error_flag)
  end
end
