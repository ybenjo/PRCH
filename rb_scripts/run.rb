["body", "claim"].each do |sec|
  ["noun", "verb"].each do |hinsi|
    puts "ruby ./parse_text_mecab.rb #{ARGV[0]} #{sec} #{hinsi}"
    system "ruby ./parse_text_mecab.rb #{ARGV[0]} #{sec} #{hinsi}"
  end
end
