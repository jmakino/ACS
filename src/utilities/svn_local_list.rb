#!/usr/bin/ruby
#
#  svn_local_list.rb
#
#  you can run this script in any directory under svn (subversion) control,
#  to obtain the list of files that are partu of svn, even those files that
#  have been added, but not yet committed.
#
#  usage: ruby svn_local_list.rb
#
#==============================================================================

fname = ".svn/entries"
if FileTest.exist?(fname)
  entry_file = open(fname, "r")
else
  print "The current directory is not under subversion control\n"
  exit
end
while s = entry_file.gets
  if s =~ /name=\"/
    file_name = $'
    file_name.chop!                                        # remove trailing \n
    file_name.chop!                                        # remove trailing "
    unless file_name == "svn:this_dir"
      print file_name, "\n"
    end
  end
end
