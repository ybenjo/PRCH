# -*- coding: utf-8 -*-
require "kconv"
require "logger"
require "rubygems"
require "hpricot"
require "tokyocabinet"
include TokyoCabinet
require "MeCab"
require "./db_util.rb"

DATA_PATH = "../patent"

#文をparseし名詞のみのbug-of-wordsを返す
def parse_sentence(sentence)
  c = MeCab::Tagger.new(ARGV.join(" "))
  n = c.parseToNode(sentence)
  bow = Hash.new{|h,k|h[k] = 0}
  while n do
    pos = n.feature.split(",").first
    word = n.surface.downcase
    bow[word] += 1 if pos == "名詞"
    n = n.next
  end
  return bow
end

#bug-of-wordsを w_1,c_1 \t w_2, c_2 \tな文字列に変換
def conv_word_count_to_str(bow)
  return bow.to_a.map{|e|e.join(",")}.join("\t")
end

#指定した年の本文全てをparseして/year/mecab.hdbに格納
def conv_sequential(param)
  year = param[:year]
  error_flag = param[:flag]
  
  db_get = TokyoCabinet::HDB.new
  
  if !error_flag
    db_get.open("#{DATA_PATH}/db/#{year}/body.hdb", HDB::OWRITER | HDB::OCREAT)
  else
    db_get.open("#{DATA_PATH}/db/#{year}/body_error.hdb", HDB::OWRITER | HDB::OCREAT)
  end

  count = 0.0
  size = db_get.size
  
  db_get.each do |id, body|
    bow = parse_sentence(body)
    wc = conv_word_count_to_str(bow)
    put_entry(:key => id,
              :value => wc,
              :type => "mecab",
              :year => year,
              :flag => error_flag
              )
    count += 1
    puts "Now #{count / size * 100}%"
  end
  db_get.close
end

if __FILE__ == $0
  year = ARGV[0]
  error_flag = ARGV[1] == "true" ? true : false
  conv_sequential(:year => year,
                  :flag => error_flag
                  )
end
