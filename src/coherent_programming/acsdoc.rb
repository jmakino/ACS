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
# 2004/3/22 Major rewrite to make it more easily extensible.
#
# Basic change: all functions are changed to (if not already designed
# that way) taking string(s) as an argument and output the processed
# string as an argument
#
#==============================================================================

module Acsdoc

  @@prompt = "|gravity>"

  @@imgcount = 0
#
# takes a single string which contains a command
# and creates processed string which contain output
#
  def add_output(s,  dirname, tag)
    ostring = "";
    a = s.split
    indent = s.index(tag) 
    noout = false
    noout = true if tag == ":command:" 
    tmpname = ".acsdoc.command-out"
    tmpcommand = ".acsdoc.command-file"
    prompt = " "* indent + @@prompt
    commandline = a[1..a.size].join(" ").chomp
    ostring = ostring +  "---\n"     unless noout
    fullcommand = "cd #{dirname}; "+commandline
    unless noout
      fullcommand  += ">& " + tmpname
      print "Generating output of \"#{commandline}\"...\n" 
    else
      print "Executing command \"#{commandline}\"...\n" 
    end
    print "command to run = ", fullcommand, "\n" if $DEBUG
    open(tmpcommand,"w+"){ |f|  f.print fullcommand + "\n"}
    system("cat  #{tmpcommand}") if $DEBUG
    system("csh -f #{tmpcommand}");
    unless noout
       ostring = ostring +  prompt + commandline + "\n" if tag == ":commandoutput:"
      output = `cat #{dirname}/#{tmpname}`
      output.each{|x| ostring+=  " "*indent + x}
      ostring+= "---\n"
    end
    ostring
  end

#
# takes an array of strings, starting 
#
  def command_with_input(s,tag, instring, dirname, showout )
    ostring = ""
    a = s.chomp.split
    endtag = a.pop
    indata=""
    until (line= instring.shift.chomp) =~ /#{endtag}/
      if $DEBUG
	p indata
	print "\n"
	p line
	print "\n"
      end
      indata += line+ "\n"
    end
#    print "indata:\ ", indata, "indata end\n" 
    tmpinname =   ".acsdoc.command-in"
    open(dirname+"/"+tmpinname, "w+"){|f| f.print indata}
        indent = s.index(tag) 
    tmpname = ".acsdoc.command-out"
    tmpcommand = ".acsdoc.command-file"
    prompt = " "* indent + @@prompt
    commandline = a[1..a.size].join(" ").chomp
    ostring = ostring +  "---\n"
    fullcommand = "cd #{dirname}; (" +commandline+ "<" + tmpinname + " )"
    if showout
      fullcommand  +=   " >& " + tmpname if showout
      print "Generating output of \"#{commandline}\"...\n" 
    else
      print "Executing command \"#{commandline}\"...\n" 
    end
    print "command to run = ", fullcommand, "\n" if $DEBUG
    open(tmpcommand,"w+"){ |f|  f.print fullcommand + "\n"}
    system("cat  #{tmpcommand}") if $DEBUG
    system("csh -f #{tmpcommand}");
    ostring = ostring +  prompt + commandline+ "\n"
    output = indata 
    output += `cat #{dirname}/#{tmpname}`  if  showout
    output.each{|x| ostring = ostring +  " "*indent + x}
    ostring = ostring +  "---\n"
    ostring
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
  def prep_cp_string(instring,dirname)
    ostring = ""
    while s = instring.shift
      if s.index("#") == 0
        print "comment line skipped ",s, "\n"
      elsif s =~ /:in.*code:/ and s.index("\":inccode:\"")==nil
	s.sub!(/:in.*code:/, ':include:')
	ostring = ostring +  "---\n"
	ostring = ostring +  s
	ostring = ostring +  "---\n"
      elsif loc = check_tag(s,":output:")
	ostring = ostring +  add_output(s, dirname, ":output:")
      elsif loc = s.index(":commandoutput:")  and s.index("\":commandoutput:\"")==nil
	ostring = ostring +  add_output(s,  dirname, ":commandoutput:")
      elsif loc = s.index(":command:")  and s.index("\":command:\"")==nil
	ostring = ostring +  add_output(s,  dirname, ":command:")
      elsif loc = s.index(":prompt:")  and s.index("\":prompt:\"")==nil
	set_prompt(s)
      elsif loc = check_tag(s,":commandinputoutput:")
	ostring = ostring +  command_with_input(s,":commandinputoutput:", 
				      instring,dirname,true)
      elsif loc = check_tag(s,":commandinput:")
	ostring = ostring +  command_with_input(s,":commandinput:", instring,
				      dirname,false)
      else
	ostring = ostring +  s
      end
    end
    ostring
  end

  def process_texcode(texcode,dirname)
    imgbase =".imgs/"
    imgdir =  imgbase 
    Dir.mkdir(imgdir) unless File.exist?(imgdir)
    texname = "tmp.tex"
    texbase = "tmp"
    texsource=open(texname,"w+")
    texsource.print <<-END_TEXSOURCE
    \\documentclass{article}
    \\begin{document}
       \\pagestyle{empty}
       \\thispagestyle{empty}
       #{texcode}
    \\end{document}
    END_TEXSOURCE
    texsource.close
    system("cat "+texname)
    system "latex #{texname}"
    system "dvips -o #{texbase}.ps -l 1 -x 1440 -E #{texbase}"
    system "rm -f #{texbase}.jpeg; convert  #{texbase}.ps  #{texbase}.jpeg "
    imgname = imgbase + @@imgcount.to_s + ".jpeg"
    if File.exist?(texbase+".jpeg")
      system "mv -f #{texbase}.jpeg #{imgname}"
    elsif File.exist?(texbase+".jpeg.0")
      system "mv -f #{texbase}.jpeg.0 #{imgname}"
    else
      raise "Failed to create the jpeg file #{texbase}.jpeg"
    end
    @@imgcount+=1
    "link:../#{imgname}"
  end

  def process_tex_inlines(s,instring,dirname)
    if s.index("<tex>")-1==  s.index("\\<tex>")
      print "s is escaped ", s, "\n"
      return [s[s.index("<tex>")+5,s.length],s[0,s.index("<tex>")+5]]
    end
    ostring = s[0,s.index("<tex>")]
    s = s[s.index("<tex>")+5,s.length]
    texcode = ""
    while s.index("</tex>")==nil
      texcode += s + "\n"
      s = instring.shift
      if s == nil 
	raise "End of file reached while searching for </tex>: #{$infile}"
      end
    end
    texcode += s[0,s.index("</tex>")];
    s = s[s.index("</tex>")+6,s.length]
    texresult=    process_texcode(texcode,dirname)
    ostring += texresult
    [s,ostring]
  end

  def find_and_process_tex_inlines(instring,dirname)
    ostring=""
    while s = instring.shift
      while loc = s.index("<tex>")  
	r = process_tex_inlines(s,instring,dirname)
	ostring +=r[1]
	s = r[0]
      end
      ostring += s+"\n"
    end
    ostring
  end


  def prep_cp(infile, outfile)
    $infile = infile
    begin
      ifile = open(infile, "r")
    rescue
      raise "#{infile} does not exist"
    end
    dirname = File.dirname(infile)
    ofile = open(outfile, "w+")
    instring = []
    while s = ifile.gets
      instring.push(s)
    end
    ifile.close
    tmpstring=prep_cp_string(instring,dirname).split("\n");
    tmp2= find_and_process_tex_inlines(tmpstring,dirname);
    ofile.print tmp2
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

  def prep_rb_defs(infile)
    ifile = open(infile, "r")
    method_level = 0
    ofile = Array.new;
    oname = Array.new;
    olevel = Array.new;
    while s = ifile.gets
      s.gsub!(/([^\t]{8})|([^\t]*)\t/n){[$+].pack("A8")}
      if s.split[0]== "def"
	method_name = s.gsub(/\(/," ").split[1]
	print method_name if $DEBUG
	outfile = File.dirname(infile)+"/."+File.basename(infile) +
	  "+" + method_name
	if oname[0...method_level].index(outfile) != nil
	  raise "Too many defs for file #{infile} line #{ARGF.lineno}"
	end
	oname[method_level]=outfile
	ofile[method_level] = open(outfile, "w+")
	olevel[method_level] = s.index("def")
	method_level += 1
      elsif  s.split[0] == "end"
	loc = s.index("end")
	# check if this "end" corresponds to the deepest level "def"
	if method_level > 0
	  lastlevel = method_level-1
	  if olevel[lastlevel] == loc 
	    ofile[lastlevel].print s
	    ofile[lastlevel].close
	    method_level -= 1
	  elsif olevel[lastlevel] > loc 
	    raise "Unexpected \"end\" with level #{loc} for expected level "+
	      "#{olevel[lastlevel]} at line #{ARGF.lineno}"
	  end
	end
      end
      for i in 0...method_level do
	ofile[i].print s
      end
    end
    ifile.close
  end
  
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
	prep_rb_defs(a)
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







