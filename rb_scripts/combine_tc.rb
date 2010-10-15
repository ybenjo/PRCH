# -*- coding: utf-8 -*-
require 'rubygems'
require 'tokyocabinet'
include TokyoCabinet

def get_entry(param)
  key = param[:key]
  year = param[:year]
  type = param[:type]

  db = TokyoCabinet::HDB.new
  db.open("#{DATA_PATH}/db/#{year}/#{type}.hdb", HDB::OWRITER | HDB::OCREAT)
  val = db.get(key)
  db.close

  return val
end

def write_entry(key, val, year)
  db = TokyoCabinet::HDB.new
  db.open("#{DATA_PATH}/db/#{year}/combine_#{year}.hdb", HDB::OWRITER | HDB::OCREAT)
  db.put(key, Marshal.dump(val))
  db.close
end

DATA_PATH = "../patent"

def rw(year)
  db_title = TokyoCabinet::HDB.new
  db_title.open("#{DATA_PATH}/db/#{year}/title.hdb", HDB::OWRITER | HDB::OCREAT)
  size = db_title.size
  read_count = 0
  db_title.each_pair do |id, v|
    read_count += 1
    puts "#{read_count} / #{size}"
    w_db = Hash.new
    w_db[:title] = v
    #date, ipc
    w_db[:ipc] = get_entry :key => id, :year => year, :type => "ipc"
    date = get_entry :key => id, :year => year, :type => "date"
    #dateは西暦だけ取り出す
    w_db[:date] = date.to_s[0..3]

    #body_noun
    body = Hash.new
    db_body = (get_entry :key => id, :year => year, :type => "body_noun")
    next if db_body.nil?
    db_body.split("\t").each do |e|
      word, count = e.split(",")
      count = count.to_i
      body[word] = count
    end
    w_db[:body_noun] = body

    #body_verb
    body_verb = Hash.new
    db_body_verb = (get_entry :key => id, :year => year, :type => "body_verb")
    next if db_body_verb.nil?
    db_body_verb.split("\t").each do |e|
      word, count = e.split(",")
      count = count.to_i
      body_verb[word] = count
    end
    w_db[:body_verb] = body_verb

    #claim_noun
    claim = Hash.new
    db_claim = (get_entry :key => id, :year => year, :type => "claim_noun")
    next if db_claim.nil?
    db_claim.split("\t").each do |e|
      word, count = e.split(",")
      count = count.to_i
      claim[word] = count
    end
    w_db[:claim_noun] = claim

    #claim_verb
    claim_verb = Hash.new
    db_claim_verb = (get_entry :key => id, :year => year, :type => "claim_verb")
    next if db_claim_verb.nil?
    db_claim_verb.split("\t").each do |e|
      word, count = e.split(",")
      count = count.to_i
      claim_verb[word] = count
    end
    w_db[:claim_verb] = claim_verb

    w_db[:app_name] = get_entry :key => id, :year => year, :type => "patent_id_app_name"
    w_db[:app_id] = get_entry :key => w_db[:app_name], :year => year, :type => "app_name_app_id"

    write_entry(id, w_db, year)
  end
  db_title.close
end

if $0 == __FILE__
  rw(ARGV[0].upcase)
end

