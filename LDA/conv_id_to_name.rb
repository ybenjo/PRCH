f_input = ARGV[0]
f_dic = ARGV[1]

dic = Hash.new
open(f_dic){|f|
  f.each do |l|
    id, word = l.chomp.split("\t")
    dic[id] = word
  end
}


open(f_input+"_conv", "w"){|f_write|
  open(f_input){|f|
    f.each do |l|
      if l !~ /^#/
        id = l.split(",")[0]
        f_write.puts dic[id] + "," + l.split(",")[1..-1].join(",")
      else
        f_write.puts l
      end
    end
  }
}
