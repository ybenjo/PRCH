# -*- coding: utf-8 -*-
#bayon用のファイルを標準出力に出力します

require "rubygems"
require "hpricot"
require "tokyocabinet"
include TokyoCabinet

def count_per_company(year)
  count = Hash.new{|h, k|h[k] = 0}
  db = TokyoCabinet::HDB.new
  db_company = TokyoCabinet::HDB.new
  db_company.open("../patent/db/#{year}/patent_id_app_name.hdb", HDB::OWRITER | HDB::OCREAT)
  db.open("../patent/db/#{year}/mecab.hdb", HDB::OWRITER | HDB::OCREAT)
  db.each_key do |id|
    company = db_company.get(id)
    count[company] += 1
  end
  db.close
  db_company.close

  db.open("../patent/db/#{year}/mecab_error.hdb", HDB::OWRITER | HDB::OCREAT)
  db_company.open("../patent/db/#{year}/patent_id_app_name_error.hdb", HDB::OWRITER | HDB::OCREAT)
  db.each_key do |id|
    company = db_company.get(id)
    count[company] += 1
  end
  db.close
  db_company.close
  
  puts "total:#{count.size}"
  count.to_a.sort{|a,b|b[1] <=> a[1]}.each do |e|
    puts e.join("\t")
  end
  

  
end

if $0 == __FILE__
  year = ARGV[0]
  count_per_company(year)
end
