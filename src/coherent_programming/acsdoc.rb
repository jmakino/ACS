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

  @@prompt = "|gravity>"

  def add_output(s, ofile, dirname, tag)
    a = s.split
    indent = s.index(tag) 
    noout = false
    noout = true if tag == ":command:" 
    tmpname = ".acsdoc.command-out"
    tmpcommand = ".acsdoc.command-file"
    prompt = " "* indent + @@prompt
    commandline = a[1..a.size].join(" ").chomp
    ofile.print "---\n"     unless noout
    outfile = open(tmpname, "w+")
    fullcommand = "cd #{dirname}; "+commandline
    fullcommand  += ">& " + tmpname unless noout
    print "Generating output of \"#{commandline}\"...\n"
    print "command to run = ", fullcommand, "\n" if $DEBUG
    open(tmpcommand,"w+"){ |f|  f.print fullcommand + "\n"}
    system("cat  #{tmpcommand}") if $DEBUG
    system("csh -f #{tmpcommand}");
    outfile.close
    unless noout
      ofile.print prompt + commandline, "\n" if tag == ":commandoutput:"
      output = `cat #{dirname}/#{tmpname}`
      output.each{|x| ofile.print " "*indent + x}
      ofile.print "---\n"
    end
  end

  def command_with_input(s,tag, ifile, ofile, dirname )
    a = s.chomp.split
    endtag = a.pop
    indata=""
    until (line= ifile.gets.chomp) == endtag
      p indata
      print "\n"
      p line
      print "\n"
      indata += line+ "\n"
    end
    print "indata:\ ", indata, "indata end\n"
    tmpinname =   ".acsdoc.command-in"
    open(dirname+"/"+tmpinname, "w+"){|f| f.print indata}
        indent = s.index(tag) 
    tmpname = ".acsdoc.command-out"
    tmpcommand = ".acsdoc.command-file"
    prompt = " "* indent + @@prompt
    commandline = a[1..a.size].join(" ").chomp
    ofile.print "---\n"
    outfile = open(tmpname, "w+")
    fullcommand = "cd #{dirname}; "+commandline
    fullcommand  += "<" + tmpinname +  " >& " + tmpname 
    print "Generating output of \"#{commandline}\"...\n"
    print "command to run = ", fullcommand, "\n" if $DEBUG
    open(tmpcommand,"w+"){ |f|  f.print fullcommand + "\n"}
    system("cat  #{tmpcommand}") if $DEBUG
    system("csh -f #{tmpcommand}");
    outfile.close
    ofile.print prompt + commandline, "\n"
    output = indata + `cat #{dirname}/#{tmpname}`
    output.each{|x| ofile.print " "*indent + x}
    ofile.print "---\n"
  end

  def set_prompt(s)
    a = s.split
    @@prompt = a[1]
    print "New prompit is #{@@prompt}\n"
  end

  def check_tag(s, tagname)
    loc = s.index(tagname)
    loc = nil if loc and s.index('"'+tagname+'"')
    loc
  end

# prep_cp handles the following tags (in the form of, for example,
# ":output:" in the case of "output" below: 

# * "inccode (or  /inc.*code/) filename" includes the file specified by 
#   the filename, between horizontal lines
# * "output command and args" includes the output of the command (with args) 
#   Both stderr and stdout are included
# * "commandoutput command and args" includes the command line itself and
#   its output (prompt default is "|gravity>"
# * "prompt new_prompt" sets new prompt
# * "command command and args" just run the command, but discard the output
# * "commandinputoutput args endtag" runs the command, taking the lines 
#   as input, untill endtag is reached.
# 
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
#      elsif loc = s.index(":output:")  and s.index("\":output:\"")==nil
      elsif loc = check_tag(s,":output:")
	add_output(s, ofile, dirname, ":output:")
      elsif loc = s.index(":commandoutput:")  and s.index("\":commandoutput:\"")==nil
	add_output(s, ofile, dirname, ":commandoutput:")
      elsif loc = s.index(":command:")  and s.index("\":command:\"")==nil
	add_output(s, ofile, dirname, ":command:")
      elsif loc = s.index(":prompt:")  and s.index("\":prompt:\"")==nil
	set_prompt(s)
      elsif loc = check_tag(s,":commandinputoutput:")
	command_with_input(s,":commandinputoutput:", ifile, ofile, dirname )
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
	print segment_name if $DEBUG
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







