#!/usr/bin/ruby
#
# acsdoc.rb
#
# This script takes a number of files and runs rdoc ("rdoc.sourceforge.net")
# on them.  All options normally given to rdoc on the command line are
# passed on to rdoc.
#
# As an internal detail, this script generates for each .cp file "filename.cp"
# a temporary file ".filename.cp", which is removed before the script finishes.
# If acsdoc.rb is invoked with the option "--keep-dot-files", these temporary
# files are not removed
#
# usage:  ruby acsdoc.rb [--keep-dot-files] [rdoc option]... [file]...
#
#==============================================================================

def prep_cp(infile, outfile)
  if not ifile = open(infile, "r")
    raise "#{infile} does not exist"
  end
  ofile = open(outfile, "w+")
  while s = ifile.gets
    if s =~ /:in.*code:/
      s.sub!(/:in.*code:/, ':include:')
      ofile.print "---\n"
      ofile.print s
      ofile.print "---\n"
    else
      ofile.print s
    end
  end
  ifile.close
  ofile.close
end

del_flag = true

del_file_list = Array.new

ARGV.collect! do |a|
  if a =~ /\.cp$/
    dot_a = '.'+a
    prep_cp(a, dot_a)
    a = dot_a
    del_file_list.push(dot_a)
  elsif a == "--keep-dot-files"
    del_flag = false
    a = ""
  end
  a
end

system("rdoc #{ARGV.join(" ")}")

if del_flag
  del_file_list.each do |f|
    File.delete(f)
  end
end





