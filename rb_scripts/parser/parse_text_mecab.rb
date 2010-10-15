# -*- coding: utf-8 -*-
require "MeCab"
require "kconv"
require "logger"
require "rubygems"
require "hpricot"
require "tokyocabinet"
include TokyoCabinet
require "./db_util.rb"
require "kconv"
require "jcode"
$KCODE = 'u'

DATA_PATH = "../patent"

#文をparseし名詞のみのbug-of-wordsを返す
#今回は全角英数を半角英数に変換するのみに留め、
#文字数などでのフィルタリングを行わない
def parse_sentence(sentence, hinsi)

  check_string = ""
  if hinsi == "noun"
    check_string = "名詞"
  elsif hinsi == "verb"
    check_string = "動詞"
  else
    puts "check_string is #{check_string}"
    exit 1
  end
  
  c = MeCab::Tagger.new(ARGV.join(" "))
  n = c.parseToNode(sentence)
  bow = Hash.new{|h,k|h[k] = 0}
  while n do
    pos = n.feature.split(",").first
    word = n.surface.tr("ａ-ｚＡ-Ｚ０-９", "a-zA-Z0-9")
    bow[word] += 1 if pos == check_string
    n = n.next
  end
  return bow
end

#bug-of-wordsを w_1,c_1 \t w_2, c_2 \tな文字列に変換
def conv_word_count_to_str(bow)
  return bow.to_a.map{|e|e.join(",")}.join("\t")
end

#指定した年の本文全てをparseして/year/#{section}_#{hinsi}.hdbに格納
def conv_sequential(param)
  year = param[:year]
  section = param[:section]
  error_flag = param[:flag]
  hinsi = param[:hinsi]
  
  db_get = TokyoCabinet::HDB.new
  
  if !error_flag
    db_get.open("#{DATA_PATH}/db/#{year}/#{section}.hdb", HDB::OWRITER | HDB::OCREAT)
  else
    db_get.open("#{DATA_PATH}/db/#{year}/#{section}_error.hdb", HDB::OWRITER | HDB::OCREAT)
  end

  count = 0.0
  size = db_get.size
  
  db_get.each do |id, body|
    bow = parse_sentence(body, hinsi)
    wc = conv_word_count_to_str(bow)
    put_entry(:key => id,
              :value => wc,
              :type => "#{section}_#{hinsi}",
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
  section = ARGV[1] || "body"
  hinsi = ARGV[2] || "noun"
  error_flag = ARGV[3] == "true" ? true : false
  conv_sequential(
                  :year => year,
                  :flag => error_flag,
                  :section => section,
                  :hinsi => hinsi
                  )
end
