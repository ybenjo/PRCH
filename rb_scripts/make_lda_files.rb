# -*- coding: utf-8 -*-
#指定された年度, ジャンルにおける
#body/claim/allの不完全bag of words(重複), 辞書, doc_id - dateファイルを生成する。
#不完全bag of wordsはcombine_all_lda.ccで結合させる
require "optparse"
require 'rubygems'
require 'tokyocabinet'
include TokyoCabinet


config = Hash.new
OptionParser.new {|opt|
  opt.on('-o [OPTION]') {|v| config[:dir] = v}
  opt.on('-g [OPTION]') {|v| config[:genre] = v}
  opt.on('-y [OPTION]') {|v| config[:year] = v.upcase}
  opt.parse!(ARGV)
}

dir = config[:dir] || "../patent/lda_data/new"
year = config[:year]
genre = config[:genre] || :all

name_base = "#{dir}/#{year}_#{genre}"

word_id = Hash.new{|h,k|h[k] = 0}
doc_id = Hash.new{|h,k|h[k] = 0}

DATA_PATH = "../patent"

open("#{name_base}.word", "w"){|f_word|
  open("#{name_base}.date", "w"){|f_date|
    open("#{name_base}.doc", "w"){|f_doc|
      open("#{name_base}","w"){|f_all|
        open("#{name_base}_claim.txt","w"){|f_claim|
          open("#{name_base}_body.txt","w"){|f_body|
            db_read = TokyoCabinet::HDB.new
            db_read.open("#{DATA_PATH}/db/#{year}/combine_#{year}.hdb", HDB::OWRITER | HDB::OCREAT)
            db_read.each_pair do |id, val|
              values = Marshal.load(val)
              next if genre != :all && values[:ipc] !~ /^#{genre}/
              
              if !doc_id.include?(id)
                doc_id[id] = doc_id.size
                f_doc.puts "#{doc_id[id]}\t#{id}"
              end
              f_date.puts "#{doc_id[id]}\t#{values[:date]}"
              
              [:body_noun, :body_verb].each do |sec|
                values[sec].each do |word, count|
                  if !word_id.include?(word)
                    word_id[word] = word_id.size
                    f_word.puts "#{word_id[word]}\t#{word}"
                  end
                  f_body.puts "#{doc_id[id]}\t#{word_id[word]}\t#{count}"
                  f_all.puts "#{doc_id[id]}\t#{word_id[word]}\t#{count}"
                end
              end

              [:claim_noun, :claim_verb].each do |sec|
                values[sec].each do |word, count|
                  if !word_id.include?(word)
                    word_id[word] = word_id.size
                    f_word.puts "#{word_id[word]}\t#{word}"
                  end
                  f_claim.puts "#{doc_id[id]}\t#{word_id[word]}\t#{count}"
                  f_all.puts "#{doc_id[id]}\t#{word_id[word]}\t#{count}"
                end
              end
            end
            db_read.close
          }
        }
      }
    }
  }
}
