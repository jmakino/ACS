#!/usr/bin/env ruby
require "acs"
require "vpl"
a = open("sample.dat"){|x| x.gets(nil)}.collect{|x| x.split}
VirtualPlotter.new.data(a).plot

