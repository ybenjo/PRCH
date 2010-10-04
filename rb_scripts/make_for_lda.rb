# -*- coding: utf-8 -*-
#LDA用のbag-of-wordsファイルとその辞書ファイルを生成するスクリプト
require "yaml"
require "optparse"
require "rubygems"
require "tokyocabinet"
include TokyoCabinet

#引数

config = Hash.new

OptionParser.new {|opt|
  opt.on('-o [OPTION]') {|v| config[:dir] = v}
  opt.on('-g [OPTION]') {|v| config[:genre] = v || :all}
  opt.on('-y [OPTION]') {|v| config[:year] = v}
  opt.on('-s [OPTION]') {|v| config[:section] = v.to_sym || :body}
  opt.on('-e [OPTION]') {|v| config[:flag] = v || false}
  opt.parse!(ARGV)
}

dir = config[:dir] || "../patent/lda_data"
genre = config[:genre] || :all
year = config[:year]
section = config[:section] || :body
flag = config[:flag] || false
fname_tail = flag ? "_error" : ""
NAME_BASE = "#{dir}/#{year}_#{genre}_#{section}#{fname_tail}"
DATA_PATH = "../patent"

word_id = Hash.new{|h,k|h[k] = 0}
doc_id = Hash.new{|h,k|h[k] = 0}

open("#{NAME_BASE}.txt","w"){|f|
  db_read = TokyoCabinet::HDB.new

  db_read.open("#{DATA_PATH}/db/#{year}/debug_#{year}#{fname_tail}.hdb", HDB::OWRITER | HDB::OCREAT)
  db_read.each_pair do |id, val|
    values = Marshal.load(val)
    next if genre != :all && values[:ipc] !~ /^#{genre}/
    [section, "#{section}_verb".to_sym].each do |sec|
      values[sec].each do |word, count|
        #ここにフィルタを仕込む
        word_id[word] = word_id.size if !word_id.include?(word)
        doc_id[id] = doc_id.size if !doc_id.include?(id)
        f.puts "#{doc_id[id]}\t#{word_id[word]}\t#{count}"
      end
    end
  end
  db_read.close
}

open("#{NAME_BASE}.word", "w"){|f|
  word_id.each_pair do |id, wid|
    f.puts "#{wid}\t#{id}"
  end
}

open("#{NAME_BASE}.doc", "w"){|f|
  doc_id.each_pair do |id, did|
    f.puts "#{did}\t#{id}"
  end
}
