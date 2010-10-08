# -*- coding: utf-8 -*-
#LDA用のbag-of-wordsファイルとその辞書ファイルを生成するスクリプト
year = ARGV[0].upcase

new_word = Hash.new
old_new_word = Hash.new{|h, k|h[k] = Hash.new}
["body", "claim"].each do |e|
  puts "reading ../patent/lda_data/#{year}_all_#{e}.word"
  open("../patent/lda_data/#{year}_all_#{e}.word"){|f|
    f.each do |l|
      old_id, word = l.chomp.split("\t")
      if !old_new_word[e].include?(old_id)
        if new_word.include?(word)
          old_new_word[e][old_id] = new_word[word]
        else
          new_word[word] = new_word.size
          old_new_word[e][old_id] = new_word[word]
        end
      end
    end
  }
end

puts "writing ../patent/lda_data/#{year}_all_all_documument.word"
open("../patent/lda_data/#{year}_all_all_document.word", "w"){|f_word|
  new_word.each_pair do |word, word_id|
    f_word.puts "#{word_id}\t#{word}"
  end
}

new_doc = Hash.new
old_new_doc = Hash.new{|h, k|h[k] = Hash.new}
["body", "claim"].each do |e|
  puts "reading ../patent/lda_data/#{year}_all_#{e}.doc"
  open("../patent/lda_data/#{year}_all_#{e}.doc"){|f|
    f.each do |l|
      old_id, word = l.chomp.split("\t")
#       if !new_doc.include?(word)
#         new_doc[word] = new_doc.size
#         old_new_doc[e][old_id] = new_doc[word]
      #       end

      if !old_new_doc[e].include?(old_id)
        if new_doc.include?(word)
          old_new_doc[e][old_id] = new_doc[word]
        else
          new_doc[word] = new_doc.size
          old_new_doc[e][old_id] = new_doc[word]
        end
      end
      
    end
  }  
end

puts "writing ../patent/lda_data/#{year}_all_all_document.doc"
open("../patent/lda_data/#{year}_all_all_documument.doc", "w"){|f_doc|
  new_doc.each_pair do |word, word_id|
    f_doc.puts "#{word_id}\t#{word}"
  end
}

total = Hash.new{0}
open("../patent/lda_data/#{year}_all_all_document_prev.txt", "w"){|f_all|
  puts "writing ../patent/lda_data/#{year}_all_all_document_prev.txt"
  ["body", "claim"].each do |e|
    puts "reading ../patent/lda_data/#{year}_all_#{e}.txt"
    open("../patent/lda_data/#{year}_all_#{e}.txt"){|f|
      f.each do |l|
        old_doc, old_word, count = l.chomp.split("\t")
        f_all.puts "#{old_new_doc[e][old_doc]}\t#{old_new_word[e][old_word]}\t#{count.to_i}"
      end
    }
  end
}
