# -*- coding: utf-8 -*-
require "rubygems"
require "tokyocabinet"
include TokyoCabinet

#tokyocabinetからの読み書き
def get_entry(param)
  key = param[:key]
  year = param[:year]
  type = param[:type]

  db = TokyoCabinet::HDB.new

  #まずは#{year}.hdbから読み込む
  db.open("#{DATA_PATH}/db/#{year}/#{type}.hdb", HDB::OWRITER | HDB::OCREAT)
  value_1 = db.get(key)
  db.close

  #その後#{year}/error.hdbも読み込む
  #valueが存在しない場合nilが返ってくる
  db.open("#{DATA_PATH}/db/#{year}/#{type}_error.hdb", HDB::OWRITER | HDB::OCREAT)
  value_2 = db.get(key)
  db.close

  #value_2がnilの場合value_1のみを返し、nilで無い場合、Marshal.loadしたものを返す
  return value_2.nil? ? value_1 : [value_1, value_2]
end

def put_entry(param)
  key = param[:key]
  value = param[:value]
  year = param[:year]
  type = param[:type]
  error_flag = param[:flag]
  
  db = TokyoCabinet::HDB.new
  
  #error_flagがtrueの場合は#{year}/#{type}_error.hdbを開く
  if error_flag
    db.open("#{DATA_PATH}/db/#{year}/#{type}_error.hdb", HDB::OWRITER | HDB::OCREAT)
  else
    db.open("#{DATA_PATH}/db/#{year}/#{type}.hdb", HDB::OWRITER | HDB::OCREAT)    
  end

  db.put(key, value)
  db.close
end
