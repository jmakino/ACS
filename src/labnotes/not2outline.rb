#!/usr/local/bin/ruby -w
#
#  not2outline.rb
#
#  this script converts the "all.not" file to usage in emacs outline mode
#
#  usage:  not2outline.rb < all.not > allout.not
#          emacs allout.not
#

while s = gets
  s = " " + s if s == "\n"
  if s =~ /^\\header/
    s = "* " + s 
    print "\n"       # to suppress blank line before header line in folded mode
  end
  print s
end
