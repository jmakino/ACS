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
  if s == "\n"
    print " \n"
  else
    print s
  end
  if s =~ /^\\header/
    s = gets until s != "\n"
    print "\n"
    print "* ", s
  end
end
