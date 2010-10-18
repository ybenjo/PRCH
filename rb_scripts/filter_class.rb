# -*- coding: utf-8 -*-
$KCODE='u'
class Filter
  def initialize(params)
    if !params[:bow]
      puts "No input file."
      puts "-i bag-of-words-file" if !params[:bow]
      exit(1)
    end

    @date = Hash.new
    @bow_name = params[:bow]
    
    @dic = Hash.new
    @dic.default = nil
    @dic_path = params[:dic] || @bow_name.sub(/\.txt/, ".word")
    if !File.exist?(@dic_path)
      puts "No dictionary file in #{@dic_path}"
      exit(1)
    else
      puts "Reading #{@dic_path}"
      open(@dic_path){|f|
        f.each{|l|
          word_id, word = l.chomp.split("\t")
          @dic[word_id] = word
        }
      }
    end
    
    @h_size = params[:h_size].nil? ? nil : params[:h_size].to_i
    @a_size = params[:a_size].nil? ? nil : params[:a_size].to_i
    @k_size = params[:k_size].nil? ? nil : params[:k_size].to_i
    @deleted_id = []
  end
  
  def del_hiragana
    if !@h_size
      puts "Skip del_hiragana"
      return 
    end
    
    @dic.each_pair do |id, word|
      if word =~ /^(?:\xE3\x81[\x81-\xBF]|\xE3\x82[\x80-\x93])+$/ && word.split(//).size <= @h_size
        @deleted_id.push id
      end
    end
  end

  def del_alphabet
    if !@a_size
      puts "Skip del_alphabet"
      return
    end
    
    @dic.each_pair do |id, word|
      if word =~ /^[a-zA-Z]+$/ && word.split(//).size <= @a_size
        @deleted_id.push id
      end
    end
  end

  def del_katakana
    if !@k_size
      puts "Skip del_katakana"
      return
    end
    
    @dic.each_pair do |id, word|
      if word =~ /^[ァ-ヴ]+$/ && word.split(//).size <= @k_size
        @deleted_id.push id
      end
    end
  end

  def del_number_symbol_kanji_num
    @dic.each_pair do |id, word|
      if word =~ /^\d+$/ || word =~ /^(一|二|三|四|五|六|七|八|九)$/ || word =~ /^([^a-zA-Z0-9ぁ-ん一-龠ァ-ヴ])+$/
        @deleted_id.push id
      end
    end
  end

  def delete_selected_id
    log_path = @dic_path.sub(/\.word/,".log")
    open(log_path, "w"){|f|
      puts "Writing #{log_path}"
      @deleted_id.each do |id|
        f.puts "#{id}\t#{@dic[id]}"
        @dic.delete(id)
      end
    }
  end

  def del_all
    del_number_symbol_kanji_num
    del_hiragana
    del_alphabet
    del_katakana
    delete_selected_id
  end

  def output
    name = @bow_name.sub(/\.txt/,"")

    name += "_h_#{@h_size}" if @h_size
    name += "_a_#{@a_size}" if @a_size
    name += "_k_#{@k_size}" if @k_size


    puts "Writing #{name}.txt"
    open("#{name}.txt", "w"){|f_w|
      open(@bow_name){|f_read|
        f_read.each{|l|
          ary = l.chomp.split("\t")
          
          if ary.size == 4
            d_id, time, w_id, count = ary
          else
            d_id, w_id, count = ary
          end
          
          if !@dic[w_id].nil?
            if ary.size == 4
               f_w.puts "#{d_id}\t#{time}\t#{w_id}\t#{count}"
            else
              f_w.puts "#{d_id}\t#{w_id}\t#{count}"
            end
          end
          
        }
      }
    }
  end
  
end
