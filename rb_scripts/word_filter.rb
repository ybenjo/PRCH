# -*- coding: utf-8 -*-
require "optparse"
require "./filter_class.rb"

if $0 == __FILE__
  params = Hash.new
  OptionParser.new {|opt|
    opt.on("-i [OPTION]"){|v| params[:bow] = v}
    opt.on("-d [OPTION]"){|v| params[:dic] = v}
    opt.on("-h [OPTION]"){|v| params[:h_size] = v}
    opt.on("-a [OPTION]"){|v| params[:a_size] = v}
    opt.parse!(ARGV)
  }
  
  filter = Filter.new(params)
  filter.del_all
  filter.output
end

