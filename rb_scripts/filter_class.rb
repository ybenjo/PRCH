# -*- coding: utf-8 -*-
$KCODE='u'
class Filter
  def initialize(params)
    if !params[:bow]
      puts "No input file."
      puts "-i bag-of-words-file" if !params[:bow]
      exit(1)
    end

    @bow = Hash.new
    @date = Hash.new
    
    puts "Reading #{params[:bow]}"
    open(params[:bow]){|f|
      f.each{|l|
        if l.chomp.split("\t").size == 4
          doc_id, word_id, date, count = l.chomp.split("\t").map{|v|v.to_i}
          @date[doc_id] = date
        else
          doc_id, word_id,  count = l.chomp.split("\t").map{|v|v.to_i}
        end
        @bow[word_id] = [] if !@bow.include?(word_id)
        @bow[word_id].push [doc_id, count]
      }
    }
    
    @dic = Hash.new
    @dic_path = params[:dic] || params[:bow].sub(/\.txt/, ".word")
    if !File.exist?(@dic_path)
      puts "No dictionary file in #{@dic_path}"
      exit(1)
    else
      puts "Reading #{@dic_path}"
      open(@dic_path){|f|
        f.each{|l|
          word_id, word = l.chomp.split("\t")
          @dic[word_id.to_i] = word
        }
      }
    end
    
    @h_size = params[:h_size].nil? ? nil : params[:h_size].to_i
    @a_size = params[:a_size].nil? ? nil : params[:a_size].to_i
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
        @bow.delete(id)
      end
    }
  end

  def del_all
    del_number_symbol_kanji_num
    del_hiragana
    del_alphabet
    delete_selected_id
  end

  def output
    name = @dic_path.sub(/\.word/,"")

    name += "_h_#{@h_size}" if @h_size
    name += "_a_#{@a_size}" if @a_size
    
    open("#{name}.txt", "w"){|f|
      @bow.each_pair do |w_id, ary|
        next if w_id.nil?
        ary.each do |e|
          if @date.empty?
            f.puts "#{e[0]}\t#{w_id}\t#{e[1]}"
          else
            f.puts "#{e[0]}\t#{@date[e[0]]}\t#{w_id}\t#{e[1]}"
          end
        end
      end
    }
    
    open("#{name}.word", "w"){|f|
      @dic.each_pair do |w_id, word|
        f.puts "#{w_id}\t#{word}"
      end
    }
  end
end
