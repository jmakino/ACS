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
#    s
# usage:  ruby acsdoc.rb [--keep-dot-files] [rdoc option]... [file]...
#
# 2004/3/22 Major rewrite to make it more easily extensible.
#
# Basic change: all functions are changed to (if not already designed
# that way) taking string(s) as an argument and output the processed
# string as an argument
#
#==============================================================================
#$DEBUG=1
module Rdoctotex

  @@wordreplace=[
    [/\+(\w|(\\_))+\+/,"{\\tt", "}"],
    [/\*\w+\*/,"{\\bf", "}"],
    [/\\_\w+\\_/,"{\\it", "}"]
  ]
  
  @@tagreplace=[
    ["<tt>","</tt>","{\\tt", "}"],
    ["<b>","</b>","{\\bf", "}"],
    ["<em>","</em>","{\\it", "}"],
    ["<i>","</i>","{\\it", "}"],
    ["<tex>","</tex>","", ""]
  ]
  
  @@header=[
    "part","chapter","section","subsection","subsubsetion",
    "subsubsubsection"]

  @@listtypes=[
    ["paragraph",""],
    ["ulist*","itemize"],
    ["ulist-","itemize"],
    ["nlist", "enumerate"],
    ["verbatim","verbatim"]]

  @@imglinkcount = 0
  @@latexscalingforimage = 0.5

  @@charstoescape =/(#)|(\{)|(\})|(\_)|(\{)|(\\)/

  @@addtional_preambles_for_tex = ""
  @@addtional_commands_for_tex = ""

  def escapetexspecialcharacters(instring)
    s=instring.gsub(@@charstoescape){|word| "\\"+word}
    s
  end
  
  def wordmarkup(instr)
    @@wordreplace.each do |x| instr.gsub!(x[0]) do |word|
	if /_/ =~word[1,1]
	  start = 2 ; cut=4
	else
	  start = 1; cut=2
	end
	x[1]+ " " + word[start,word.length-cut] +x[2]
      end
    end
    instr
  end

  def process_wordmarkup(instring,dirname)
    ostring = []
    instring.each{|s| ostring.push(wordmarkup(s))}
  end

  def tagmarkup(instr)
    @@tagreplace.each do |x| 
      instr.gsub!(x[0]) { |word|x[2]+ " " }
      instr.gsub!(x[1]) { |word|x[3] }
    end
    instr
  end

  def process_tagmarkup(instring,dirname)
    ostring = []
    instring.each{|s| ostring.push(tagmarkup(s))}
  end

  def process_headers(instring)
    ostring=[]
    afterverbatim = nil
    while s=instring.shift
      header_candidate =s.split[0] 
      if /^=+$/ =~ header_candidate
	ostring.push("\\"+@@header[header_candidate.length]+"{")
	header_text = s.sub(/=+/," ")
	while /^\s+$/ =~ (s = instring.shift )
	  header_text += s + " "
	end
	header_text += "}"
	ostring.push(header_text)
	instring.unshift(s)
      elsif /^---+/ =~ header_candidate
	if afterverbatim
	  ostring.push("\\hrule\n\n\\bigskip\n\n")
	else
	  ostring.push("\n\\bigskip\n\n\\hrule\n\n")
	end
	afterverbatim = nil
      else
	ostring.push(s)
      end
      if /\\end\{verbatim/  =~ s
	afterverbatim = 1  
      elsif /\S+/ =~ s
	afterverbatim = nil
      end
    end
    ostring
  end
  
  def process_include(instring)
    ostring=[] 
    while s=instring.shift
      if /^(\s*)\:include\:\s*(\S+)$/  =~ s
	indent = $1
	infile= open($2,"r")
#	print "indent = ", indent, "\n"
	while ss=infile.gets
	  ostring.push(indent+ss.chomp!)
	end
	infile.close
      else
	ostring.push(s)
      end
    end
    ostring
  end
  
  def process_link(instring)
    ostring=[] 
    while s=instring.shift
      if /(^|\s)link\:(\S+)/  =~ s
	imglinkfile = $2
	imgbase =".imgs/"
	imgdir =  imgbase
	epsfilename = @@imglinkcount.to_s+".eps"
	Dir.mkdir(imgdir) unless File.exist?(imgdir)
	print "cd #{imgdir}; convert  #{imglinkfile} #{epsfilename}\n"
	system "cd #{imgdir}; convert  #{imglinkfile} #{epsfilename}"
	s.sub!(/(^|\s)link\:(\S+)/,
	       "\n\\scalebox{#{@@latexscalingforimage}}{\\includegraphics{#{imgbase+epsfilename}}}\n")
	@@imglinkcount+=1
      end
      ostring.push(s)
    end
    ostring
  end

  @@intex_state = 0

  def process_tex_special_chars(instring)
    s = instring
    ostring =""
    while s.length > 0
      if @@intex_state == 0
	texloc = /<tex>/ =~ s
	if texloc
	  ostring+=escapetexspecialcharacters(s[0,texloc])+"<tex>"
	  @@intex_state = 1
	  s = s[texloc+5,s.length]
	else
	  ostring += escapetexspecialcharacters(s)
	  s=""
	end
      else
	texloc = /<\/tex>/ =~ s
	if texloc
	  ostring+=s[0,texloc]+"</tex>"
	  @@intex_state = 0
	  s = s[texloc+6,s.length]
	else
	  ostring += s
	  s=""
	end
      end
    end
    ostring
  end
  
  def process_single_paragraphs_lists_etc(instring,indent,type,new)
    s_prev = ""
    ostr=[]
    ostr.push("\\begin{"+@@listtypes[type][1]+"}")  if new and (type >0)

    intex = nil

#
# intex is used to suppress formatting. As you can see from the code below, 
# suppression can take place only if "<tex>" appears as single item in one 
# line, and suppression stops only of "</tex>" appears as single item in one
# line. So if you use them in a more complex ways, it might cause strange 
# problems.
#
    
    while s=instring.shift
      header_candidate =s.split[0] 
      new_item = nil
      new_type = 1
      if @@intex_state == 1
	new_type = type
	new_indet = indent
	s1 = s
      elsif (/^(\*|\-|\d+\.)$/ =~ header_candidate) 
	new_type = 1+[/\*/,/\-/,/\d+\./].collect{
	  |x| x=~ header_candidate}.index(0)

	s1 = s.sub(/\S+\s/){|x| " "*x.length} 
	new_indent = /\S/ =~ s1
	new_item = 1
      else
	new_indent = /\S/ =~ s
	new_indent = indent if /^\s*$/ =~s 
	s1=s
      end
      if type == 4 and /^\s+/ =~s 
	s1 = s
	new_type=4
	new_item = nil
      end
	  
      new_type = 4 if new_indent > 0 and new_item == nil
      s1= process_tex_special_chars(s1) unless new_type == 4
      if new_indent > indent
	instring.unshift(s)
	new=1
	new = nil if new_type == type and type == 4 
#	print "type =  #{new_type}, #{type}, #{new}\n"
	if new 
	  ostr+= process_single_paragraphs_lists_etc(instring,new_indent,
						     new_type,new)
	else
	  ostr.push s1
	  instring.shift
	end
      elsif new_indent == indent
	if new_item
	  if new_type != type
	    ostr.push("\\end{"+@@listtypes[type][1]+"}")
	    instring.unshift(s)
	    ostr+= process_single_paragraphs_lists_etc(instring,new_indent,
						       new_type,1)
	  end
	  ostr.push("\\item ") if new_type < 4
	end
	ostr.push s1
      else
	if type == 4 and new_type==4
	  indent = new_indent
	else
	  ostr.push("\\end{"+@@listtypes[type][1]+"}")
	end
	instring.unshift(s)
	return ostr
      end
      s_prev = s
    end	
    ostr
  end
  
  
  def convert_to_latex(instring,dirname)
    s=process_include(instring)
    s=process_single_paragraphs_lists_etc(s,0,0,1)
    s=process_link(s)
    s=process_wordmarkup(s,dirname)
    s=process_headers(s)
    s = process_tagmarkup(s,dirname).join("\n")
    s= <<-END_TEXSOURCE
    \\documentclass{book}
    \\usepackage{graphicx}
    #{@@addtional_preambles_for_tex}
    \\begin{document}
       #{@@addtional_commands_for_tex}
       #{s}
    \\end{document}
    END_TEXSOURCE
  end

end



module Acsdoc
  include Rdoctotex

  @@prompt = "|gravity>"

  @@imgcount = 0
  @@addtional_preambles_for_inline_tex = ""
  @@addtional_commands_for_inline_tex = ""

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
      fullcommand  = "cd #{dirname}; #{commandline}" 
    unless noout
      fullcommand  = "cd #{dirname}; (#{commandline})>&  #{tmpname}" 
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
#      fullcommand  +=   " >& " + tmpname 
      fullcommand  = "(#{fullcommand})" if showout
      if showout
	fullcommand = "cd #{dirname}; (( #{commandline} < #{tmpinname}  ))>& #{tmpname}"
      end
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


  def convert_link_to_rdoc_style(instring,dirname)
    ostring=[] 
    while s=instring.shift
      if /(^|\s)link\:(\S+)/  =~ s
	print "rdoc style line #{s}\n"
	preceedingspaces = $1
	imglinkfile = $2
	s.sub!(/(^|\s)link\:(\S+)/,
	       "#{preceedingspaces}link:../#{dirname}/#{imglinkfile}")
	print "converted to: #{s}\n"
      end
      ostring.push(s)
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
    \\usepackage{graphicx}
    #{@@addtional_preambles_for_inline_tex}
    \\begin{document}
       \\pagestyle{empty}
       \\thispagestyle{empty}
       #{@@addtional_commands_for_inline_tex}
       #{texcode}
    \\end{document}
    END_TEXSOURCE
    texsource.close
    system("cat "+texname)
    system "latex #{texname}"
    system "dvips -o #{texbase}.ps -l 1 -x 1900 -E #{texbase}"
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


  def prep_cp(infile, outfile,tolatex_flag)
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
    s = convert_link_to_rdoc_style(instring,dirname)
    tmpstring=prep_cp_string(s,dirname).split("\n");
    if tolatex_flag
      tmp2= Rdoctotex::convert_to_latex(tmpstring,dirname);
    else
      tmp2= find_and_process_tex_inlines(tmpstring,dirname);
    end
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
    classname = "noname"
    while s = ifile.gets
      s.gsub!(/([^\t]{8})|([^\t]*)\t/n){[$+].pack("A8")}
      if s.split[0]== "class"
	classname = s.split[1]
      elsif s.split[0]== "module"
	classname = s.split[1]
      elsif s.split[0]== "def"
	method_name = s.gsub(/\(/," ").split[1]
	print method_name if $DEBUG
	outfile = File.dirname(infile)+"/."+File.basename(infile) +
	  "+" + method_name
	outfilelong=outfile+ "+" +classname
	if oname[0...method_level].index(outfile) != nil
	  raise "Too many defs for file #{infile} line #{ARGF.lineno}"
	end
	oname[method_level]=[outfile,outfilelong]
	ofile[method_level] = [open(outfile, "w+"),open(outfilelong, "w+")]
	olevel[method_level] = s.index("def")
	method_level += 1
      elsif  s.split[0] == "end"
	loc = s.index("end")
	# check if this "end" corresponds to the deepest level "def"
	if method_level > 0
	  lastlevel = method_level-1
	  if olevel[lastlevel] == loc 
	    ofile[lastlevel].each{|x| x.print s}
	    ofile[lastlevel].each{|x| x.close}
	    method_level -= 1
	  elsif olevel[lastlevel] > loc 
	    raise "Unexpected \"end\" with level #{loc} for expected level "+
	      "#{olevel[lastlevel]} at line #{ARGF.lineno}"
	  end
	end
      end
      for i in 0...method_level do
	ofile[i].each{|x| x.print s}
      end
    end
    ifile.close
  end

  def initacs
    acsdocinitfile=nil
    for fname in [ENV["ACSDOCINITRC"], ".acsdocinitrc", ENV["HOME"]+"/.acsdocinitrc"]
      if acsdocinitfile == nil and fname != nil and File.exist?(fname)
	acsdocinitfile = fname 
      end
    end
    
    if acsdocinitfile
      print "Loading initialization file #{acsdocinitfile}\n"
      load acsdocinitfile
    else
      print "No config file found\n"
    end
  end
end  

# :segment start: acsdoc
include Acsdoc
initacs
del_flag = true
del_file_list = Array.new
tolatex_flag = false

ARGV.collect! do |a|
  if a =~ /\.cp$/
    unless tolatex_flag 
      dot_a = File.dirname(a)+"/."+File.basename(a);
    else
      dot_a = File.dirname(a)+"/"+File.basename(a,".cp")+ ".tex"
    end
    prep_cp(a, dot_a, tolatex_flag)
    a = dot_a
    del_file_list.push(dot_a)
  elsif a =~ /\.rb$/
    prep_rb(a)
    prep_rb_defs(a)
    a
  elsif a == "--keep-dot-files"
    del_flag = false
    a = ""
  elsif a == "--tolatex"
    tolatex_flag = true
    del_flag = false
    a = ""
  end
  a
end

if  (ENV["LANG"] == "ja_JP.eucJP") or ($KCODE == "EUC") 
  coptions = "--charset=EUC-JP"
else
  coptions = " "
end

system("rdoc #{coptions} #{ARGV.join(" ")}") unless tolatex_flag

if del_flag
  del_file_list.each do |f|
    File.delete(f)
  end
end
# :segment end:



