require "optparse"

config = Hash.new
OptionParser.new {|opt|
  opt.on('-i [OPTION]') {|v| config[:i] = v}
  opt.on('-d [OPTION]') {|v| config[:d] = v}
  opt.parse!(ARGV)
}

bow = config[:i]
ts = config[:d]

open(ts){|f|
  f.each{ |l|
    id, time = l.chomp.split("\t")
    ts[id] = time
  }
}

open(bow.sub(/\.txt/, "_with_date.txt"), "w"){ |fw|
  open(bow){ |fr|
    fr.each{ |l|
      ary = l.chomp.split("\t")
      puts "#{ary[0]}\t#{ts[ary[0]]}\t#{ary[1]}\t#{ary[2]}"
    }
  }
}
