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

module Acsdoc

  def add_output(s, ofile, dirname)
    ofile.print "---\n"
    a = s.split
    indent = s.index(":output:") 
    tmpname = ".acsdoc.command-out"
    prompt = " "* indent + ">"
    commandline = a[1..a.size].join(" ").chomp
    fullcommand = "cd #{dirname}; "+commandline + " >& " + tmpname
    print "command to run = ", fullcommand, "\n"
    system(fullcommand)
    ofile.print prompt + commandline, "\n"
    ofile.print `cat #{dirname}/#{tmpname}`.each{|x| x = " "*indent + x}
    ofile.print "---\n"
  end

  def prep_cp(infile, outfile)
    begin
      ifile = open(infile, "r")
    rescue
      raise "#{infile} does not exist"
    end
    dirname = File.dirname(infile)
    ofile = open(outfile, "w+")
    while s = ifile.gets
      s.gsub!(/<p>/, '<i>[Jun: ')
      s.gsub!(/<\/p>/, ' -- Piet] </i>')
      s.gsub!(/<j>/, '<i>[Piet ')
      s.gsub!(/<\/j>/, ' -- Jun] </i>')
      if s =~ /:in.*code:/ and s.index("\":inccode:\"")==nil
	s.sub!(/:in.*code:/, ':include:')
	ofile.print "---\n"
	ofile.print s
	ofile.print "---\n"
      elsif loc = s.index(":output:")  and s.index("\":output:\"")==nil
	add_output(s, ofile, dirname)
      else
	ofile.print s
      end
    end
    ifile.close
    ofile.close
  end

  # :segment start: prep_rb
  def prep_rb(infile)
    # :segment start: open-input-file
    begin
      ifile = open(infile, "r")
    rescue
      raise "#{infile} does not exist"
    end
    # :segment end:
    segment_level = 0
    ofile = Array.new;
    oname = Array.new;
    while s = ifile.gets
      if loc = s.index(":segment start:")  and s.index("\":segment start:\"")==nil
	segment_name = s[loc,s.length].split[2];
	print segment_name;
	outfile = File.dirname(infile)+"/."+File.basename(infile) +
	  "-" + segment_name
	if oname[0...segment_level].index(outfile) != nil
	  raise "Too many #{s} for file #{infile} line #{ARGF.lineno}"
	end
	oname[segment_level]=outfile
	ofile[segment_level] = open(outfile, "w+")
	segment_level += 1
      elsif  s.index(":segment end:") and  s.index("\":segment end:\"")  ==nil
	segment_level -= 1
	if (segment_level >= 0)
	  ofile[segment_level].close
	else
	  raise "Too many #{s} for file #{infile} line #{ARGF.lineno}"
	end
      else
	for i in 0...segment_level do
	  ofile[i].print s
	end
      end
    end
    ifile.close
  end
  # :segment end: prep_rb

  # :segment start: acsdoc
  def acsdoc()
    
    del_flag = true
    del_file_list = Array.new
    
    ARGV.collect! do |a|
      if a =~ /\.cp$/
	dot_a = File.dirname(a)+"/."+File.basename(a);
	prep_cp(a, dot_a)
	a = dot_a
	del_file_list.push(dot_a)
      elsif a =~ /\.rb$/
	prep_rb(a)
	a
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
  end
  # :segment end:
end

include Acsdoc
acsdoc







