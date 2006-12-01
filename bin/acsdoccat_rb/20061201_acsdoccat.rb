#!/usr/bin/ruby
#
# acsdoccat.rb
#
# This script takes a number of files and just concatinates them,
# but adding one more blank lines between each file
# usage:  ruby acsdoccat.rb   [file]...
#
# 2006/7/25 created
first = true
ARGV.each{|x|
  if first
     first = false
  else
     print "\n"
  end
  s = open(x,"r"){|f| f.gets(nil)}
  print s
}
