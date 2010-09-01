# -*- coding: utf-8 -*-
require "kconv"
require "logger"
require "rubygems"
require "hpricot"
require "tokyocabinet"
include TokyoCabinet
require "MeCab"

DATA_PATH = "../patent"

#文をparseし名詞のみに絞ってbug-of-wordsを返す
def parse_sentence(sentence)
  c = MeCab::Tagger.new(ARGV.join(" "))
  n = c.parseToNode(sentence)
  word_count = Hash.new{|h,k|h[k] = 0}
  while n do
    pos = n.feature.split(",").first
    word_count[n.surface] += 1 if pos == "名詞"
    n = n.next
  end
  return word_count
end

# def parse_sequential(param)
#   year = param[:year]
#   type = param[:type]
#   err = param[:error_flag]
#   db = TokyoCabinet::HDB.new
#   db.open("#{DATA_PATH}/db/#{year}/#{type}.hdb", HDB::OWRITER | HDB::OCREAT)
#   db.close
# end

if __FILE__ == $0
  puts parse_sentence("隣の客はよく柿食う客だ")
end
