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
# usage:  ruby acsdoc.rb [--keep-dot-files] [--tolatex]  [rdoc option]... [file]...
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
    [/(^|\W)\+([a-zA-Z_]+)\+($|\W)/,"{\\tt", "}"],
    [/(^|\W)\*(\w+)\*($|\W)/,"{\\bf", "}"],
    [/(^|\W)\\_(\w+)\\_($|\W)/,"{\\it", "}"]
  ]
  
  @@tagreplace=[
    ["<tt>","</tt>","{\\tt", "}"],
    ["<b>","</b>","{\\bf", "}"],
    ["<em>","</em>","{\\it", "}"],
    ["<i>","</i>","{\\it", "}"],
    ["<tex>","</tex>","", ""]
  ]
  
  @@header=[
    "part","chapter","section","subsection","subsubsection",
    "subsubsubsection"]

  @@listtypes=[
    ["paragraph",""],
    ["ulist*","itemize"],
    ["ulist-","itemize"],
    ["nlist", "enumerate"],
    ["verbatim","verbatim"]]

  @@imglinkcount = 0
  @@latexscalingforimage = 0.5

  @@charstotexmath =[
    [/\$/,"\\$"],
    [/\\</,"$<$"],
    [/\\>/,"$>$"],
    [/\\([^\$])/,'$\\backslash$\1'],
    ["{","\\{"],
    ["}","\\}"],
    [/#/,"\\#"],
    [/~/,"$\\sim$"],
    [/\_/,"\\_"],
    [/\^/,"$\\hat{\\ }$"]]

  @@basic_preambles_for_tex = "\\documentclass{book}\n\\usepackage{graphicx}"
  @@additional_preambles_for_tex = ""
  @@additional_commands_for_tex = ""

  def escapetexspecialcharacters(instring)
    s = instring
    @@charstotexmath.each{|x| s=s.gsub(x[0],x[1])}
    s
  end
  
  def wordmarkup(instr)
    @@wordreplace.each do |x| instr.gsub!(x[0]) do |word|
	$1 + x[1]+ " " + $2 +x[2] + $3
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


  def process_refmarkup(instring)
    ostring = []
    instring.each{|x| 
      ostring.push(x.gsub(/ref\(((\w|\d|\:)*)\)/){|s|  "\\ref{"+ $1 + "}"})}
    ostring
  end

  def process_tex_equations_for_latex(s,instring)
    eqtype = s
    texcode = "<tex>\n\\begin{#{eqtype}}\n"
    while (s = instring.shift ) =~ /\S/
      texcode += s + "\n"
      if s == nil 
	raise "End of file reached while searching for newline: #{$infile}"
      end
    end
    texcode += "\\end{#{eqtype}}\n</tex>\n"
  end


  def latex_process_tex_equations(instring)
    ostring=[]
    while s = instring.shift
      if s =~ /^\s*:(equation|eqnarray):\s*$/
	ostring.push(process_tex_equations_for_latex($1,instring))
      else
	ostring.push(s)
      end
    end
    ostring
  end


  def latex_copy_figure_file(figurefilename,dirname, figure_number)
    imgbase =".imgs/"
    imgdir =  imgbase 
    Dir.mkdir(imgdir) unless File.exist?(imgdir)
    imgname = imgbase + figure_number.to_s + "_" +
      File.basename(figurefilename) + ".eps"
    p imgname  if $DEBUG
    if figurefilename =~ /eps$/ 
      system "/bin/cp -p #{dirname}/#{figurefilename} #{imgname}"
    else
      system "convert #{dirname}/#{figurefilename} #{imgname}"
    end
    system "mv -f #{imgname}.0 #{imgname}"if File.exist?(imgname + ".0")
    imgname
  end

@@latex_figure_number = 0
  def latex_process_figures(s,instring,dirname)
    @@latex_figure_number += 1

    a=s.chomp.split
    figurefilename,size,label = a[1],a[2],a[3]
    caption = ""
    while (s = instring.shift ) =~ /\S/
      caption += s + "\n"
      if s == nil 
	raise "End of file reached while searching for newline: #{$infile}"
      end
    end
    filename_to_link = latex_copy_figure_file(figurefilename,dirname,
					      @@latex_figure_number)
    texstring = <<-END

<tex>
\\begin{figure}
\\begin{center}
    \\includegraphics[width=#{size}]{#{filename_to_link}}
\\caption{#{caption}}
\\label{#{label}}
\\end{center}
\\end{figure}
</tex>
END
  end


  def latex_find_and_process_figures(instring,dirname)
    ostring=[]
    while s = instring.shift
      p s  if $DEBUG
      if s =~ /^\s*:figure:\s*/
	ostring.push(latex_process_figures(s,instring,dirname))
      else
	ostring.push(s)
      end
    end
    ostring
  end

  def process_headers(instring)
    
    nosectionnumber = nil
    
    ostring=[]
    afterverbatim = nil
    inabstract = nil
    while s=instring.shift
      header_candidate =s.split[0] 
      if /^=+$/ =~ header_candidate
	header_text = s.sub(/=+/," ")
	while /^\s+$/ =~ (s = instring.shift )
	  header_text += s + " "
	end
	p header_text
	if header_text =~ /^\s*Abstract\s*$/ then
	  inabstract = true
	  header = "\\begin{abstract}"
	else
	    header = ""
	  if inabstract 
	    header = "\\end{abstract}\n\n" 
	    inabstract = nil
	  end
	  sectiontype= nosectionnumber ? "*":""
	  header+= "\\"+@@header[header_candidate.length]+sectiontype+"{" +
	    header_text + "}"
	  if nosectionnumber
	    header+= "\n\\addcontentsline{toc}{" +
	      @@header[header_candidate.length]+"}{" +
	      header_text + "}"
	    nosectionnumber=nil
	  end
	end
	ostring.push(header)
	instring.unshift(s)
      elsif /^---+/ =~ header_candidate
	if afterverbatim
	  ostring.push("\\hrule\n\n\\bigskip\n\n")
	else
	  ostring.push("\n\\bigskip\n\n\\hrule\n\n")
	end
	afterverbatim = nil
      elsif /^\s*:nosectionnumber:\s*$/ =~ header_candidate
	nosectionnumber = true
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
	print "convert  #{imglinkfile} #{imgdir}#{epsfilename}\n"
	system "convert  #{imglinkfile} #{imgdir}#{epsfilename}"
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
  
  def process_single_paragraphs_lists_etc(instring,indent,type,new,vlimit)
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
      if new_indent > indent and new_item == nil
	new_type = 4 
      end
      s1= process_tex_special_chars(s1) unless new_type == 4
      if new_indent > indent
	instring.unshift(s)
	new=1
	new = nil if new_type == type and type == 4 
	vlimit = indent+1 if new and new_type == 4
	if new 
	  ostr+= process_single_paragraphs_lists_etc(instring,new_indent,
						     new_type,new,
						     vlimit)
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
						       new_type,1,vlimit)
	  end
	  ostr.push("\\item ") if new_type < 4
	end
	ostr.push s1
      else
	if type == 4 and new_type==4 and new_indent > vlimit
	  indent = new_indent
	  ostr.push s1
	else
	  ostr.push("\\end{"+@@listtypes[type][1]+"}")
	  instring.unshift(s)
	  return ostr
	end
      end
      s_prev = s
    end	
    ostr
  end

  def post_process_verbatim(instring)
    ostr=[]
    while s=instring.shift
      if s =~ /\\end\{verbatim}/
	unless (last = ostr.pop) =~ /^\s*$/
	  ostr.push last
	end
      end
      ostr.push s
    end
    ostr
  end


  def process_toc_latex(instring)
    ostring=[]
    while s=instring.shift
      if /:tableofcontents:/ =~ s
	ostring.push("\\newpage\n\\tableofcontents\n\\newpage\n")
      else
	ostring.push(s)
      end
    end
    ostring
  end
  

  
  
  def convert_to_latex(instring,dirname)
    s=process_include(instring)
    s=latex_process_tex_equations(s)
    s=latex_find_and_process_figures(s,dirname)
    s=process_single_paragraphs_lists_etc(s,0,0,1,0)
    s=post_process_verbatim(s)
    s=process_link(s)
    s=process_wordmarkup(s,dirname)
    s=process_refmarkup(s)
    s=process_headers(s)
    s=process_toc_latex(s)
    s = process_tagmarkup(s,dirname).join("\n")
    s= <<-END_TEXSOURCE
    #{@@basic_preambles_for_tex}
    #{@@additional_preambles_for_tex}
    \\begin{document}
       #{@@additional_commands_for_tex}
       #{s}
    \\end{document}
    END_TEXSOURCE
  end

end



module Acsdoc
  include Rdoctotex

  @@prompt = "|gravity> "

  @@imgcount = 0
  @@additional_preambles_for_inline_tex = ""
  @@additional_commands_for_inline_tex = ""

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
    if showout then ostring else "" end
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
    #{@@additional_preambles_for_inline_tex}
    \\begin{document}
       \\pagestyle{empty}
       \\thispagestyle{empty}
       #{@@additional_commands_for_inline_tex}
       #{texcode}
       \\thispagestyle{empty}
    \\end{document}
    END_TEXSOURCE
    texsource.close
    system("cat "+texname)
    if  (ENV["LANG"] == "ja_JP.eucJP") or ($KCODE == "EUC") 
      latexcommand = "platex"
    else
      latexcommand = "latex"
    end

    system "#{latexcommand} #{texname}"
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

#
# <tex> xxx </tex> is passed to tex process and converted to 
# an image link
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

  @@tex_equation_number = 0
  @@tex_labels = {}

  def process_tex_equations(s,instring,dirname)
    @@tex_equation_number += 1
    eqtype = s
    namelabel = ""
    texcode = "\\setcounter{equation}{#{@@tex_equation_number-1}}\n\\begin{#{eqtype}}\n"
    while (s = instring.shift ) =~ /\S/
      if s =~ /\\label\{(\S+)\}/ 
	@@tex_labels[$1]=@@tex_equation_number
	namelabel = "<name>" + $1 + "</name>\n"
      end
      texcode += s + "\n"
      if s == nil 
	raise "End of file reached while searching for newline: #{$infile}"
      end
    end
    texcode += "\\end{#{eqtype}}\n"
    pp = process_texcode(texcode,dirname)
    namelabel = namelabel + pp+"\n\n"
  end


  def find_and_process_tex_equations(instring,dirname)
    is = instring.split("\n");
    ostring=""
    while s = is.shift
      p s  if $DEBUG
      if s =~ /^\s*:(equation|eqnarray):\s*$/
	r = process_tex_equations($1,is,dirname)
	ostring +=r
      else
	ostring += s+"\n"
      end
    end
    ostring
  end

  def process_nosectionnumber(realtag,s,taggedstring,dirname)
    print "Enter process_nosectionnumber\n"
    p realtag
    p s
    p taggedstring
    p dirname
    ""
  end

  def find_and_process_generic_tag(instring,dirname,tagname,funcname)
    tagexpr = Regexp.compile("^\\s*:"+tagname+"\\s*:")
    is = instring.split("\n");
    ostring=""
    while s = is.shift
      p s  if $DEBUG
      if s =~ tagexpr
	realtag = $1
	taggedstring = ""
	while (ss = is.shift ) =~ /\S/
	  taggedstring += ss + "\n"
	  if ss == nil 
	    raise "End of file reached while searching for newline: #{$infile}"
	  end
	end
	ostring +=send(funcname,realtag,s,taggedstring,dirname)
      else
	ostring += s+"\n"
      end
    end
    ostring
  end
  
  def copy_figure_file(figurefilename,dirname, figure_number)
    imgbase =".imgs/"
    imgdir =  imgbase 
    Dir.mkdir(imgdir) unless File.exist?(imgdir)
    imgname = imgbase + figure_number.to_s + "_" +
      File.basename(figurefilename) + ".jpeg"
    p imgname  if $DEBUG
    system "convert #{dirname}/#{figurefilename} #{imgname}"
    system "mv -f #{imgname}.0 #{imgname}"if File.exist?(imgname + ".0")
    "../"+imgname
  end

  @@figure_number =0

  def process_figures(s,instring,dirname)
    @@figure_number += 1
    a=s.chomp.split
    figurefilename,size,label = a[1],a[2],a[3]
    @@tex_labels[label]=@@figure_number
    namelabel = "<name>" + label + "</name>\n"
    caption = ""
    while (s = instring.shift ) =~ /\S/
      caption += s + "\n"
      if s == nil 
	raise "End of file reached while searching for newline: #{$infile}"
      end
    end
    filename_to_link = copy_figure_file(figurefilename,dirname,@@figure_number)
    namelabel + "link:"+filename_to_link+"\n\n Figure " +
      @@figure_number.to_s + ": " + caption + "\n\n"
  end


  def find_and_process_figures(instring,dirname)
    is = instring.split("\n");
    ostring=""
    while s = is.shift
      p s  if $DEBUG
      if s =~ /^\s*:figure:\s*/
	r = process_figures(s,is,dirname)
	ostring +=r
      else
	ostring += s+"\n"
      end
    end
    ostring
  end


  def process_tex_labels(instring,dirname)
    instring.gsub(/ref\(((\w|\d|\:)*)\)/){|s| 
      "<ntaga>"+ $1 + "</ntaga><ntagb>"+ @@tex_labels[$1].to_s+ "</ntagb>"}
  end

@@sectionheadercounter=0
@@sectionheaders=[]

  def process_section_headers(instring,filename)
    instring.gsub(/^(=+)\s*(.+)$/){|s| 
      sectionlevel = $1.length
      sectionname = $2
      sectionlabel="rdocsect"+@@sectionheadercounter.to_s
      @@sectionheaders.push [sectionlabel,sectionlevel,sectionname,filename]
      @@sectionheadercounter+= 1
      s+"\n<name>" + sectionlabel + "</name>\n"
    }
  end

@@filefortoc = []
  def process_toc(instring,filename)
    if instring =~ /:tableofcontents:/
      instring=instring.gsub(/:tableofcontents:/,"TOCTOCTOCTOC")
      @@filefortoc.push(filename)
    end
    instring
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
    unless tolatex_flag
      s = convert_link_to_rdoc_style(instring,dirname)
    else
      s = instring
    end
    tmpstring=prep_cp_string(s,dirname).split("\n");
    if tolatex_flag
      tmp2= Rdoctotex::convert_to_latex(tmpstring,dirname);
    else
      tmp2= find_and_process_tex_inlines(tmpstring,dirname);
      tmp2= find_and_process_tex_equations(tmp2,dirname);
      tmp2= find_and_process_figures(tmp2,dirname);
      tmp2= find_and_process_generic_tag(tmp2,dirname,"nosectionnumber",
					 "process_nosectionnumber")
      tmp2= process_tex_labels(tmp2,dirname);
      tmp2= process_toc(tmp2,infile);
      tmp2= process_section_headers(tmp2,infile)
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


  def prep_rb_special_comments(infile)
    begin
      ifile = open(infile, "r")
    rescue
      raise "#{infile} does not exist"
    end
    outfile = File.dirname(infile)+"/."+File.basename(infile)
    ofile = open(outfile,"w+")
    while s = ifile.gets
      case s
      when /\#(\d+)(.*)/  
	s = s.gsub(/\#(\d+)(.*)/,'')      
      when /(^|(\s)+)\#(\s*)\:segment /
	s = nil
      end
      ofile.print s if s
    end
    ifile.close
    ofile.close
  end

  def prep_rb_special_comments_for_partfiles(file)
    basename =File.dirname(file)+"/."+File.basename(file) 
    Dir[basename+"{+,-}*"].each{|f|
      newname =File.dirname(f)+"/."+File.basename(f) 
      prep_rb_special_comments(f)
      File.rename(newname,f)
    }
  end

  def prep_rb_hashes(infile)
    begin
      ifile = open(infile, "r")
    rescue
      raise "#{infile} does not exist"
    end
    hashnames=[]
    while s = ifile.gets
      if s  =~ /\#(\d+)(.*)/
	segment_name = $1
	s = s.gsub(/\#(\d+)(.*)/,'')
	print segment_name if $DEBUG
	outfile = File.dirname(infile)+"/."+File.basename(infile) +
	  "-" + segment_name
	if hashnames.index(outfile) 
	  openmode= "a+" 
	else
	  openmode = "w+"
	  hashnames.push(outfile)
	end
	open(outfile,openmode){|f| f.print s}
      end
    end
    ifile.close
  end

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
	method_name = "_divide_" if method_name == "/"
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
  
@@toppagefilename= nil
  def navigation_string(prev,nex,filename)
    uppath = "../"* File.dirname(filename).split('/').size
    prevtext = "Previous"
    nexttext = "Next"
    toctext  = "ToC"
    toptext  = ""
    if @@toppagefilename
      toptext = "<td> \n   <a href=#{uppath+@@toppagefilename}>Up</a>\n   </td>"
    end
    prevtext = "<a href=#{uppath+prev}>Previous</a>" if prev    
    nexttext = "<a href=#{uppath+nex}>Next</a>" if nex
    toctext = "<a href=#{uppath+@@filefortoc[0]}#TOC>ToC</a>" if @@filefortoc[0]
    s = <<END
<table border="0" width="70%">
<tr>
  <td>
      #{prevtext}
  </td>
  <td>
      #{toctext}
  </td>
  #{toptext}
  <td>
      #{nexttext}
  </td>
</tr>
</table>
END
  end

  def add_links(filename,prev,nex)
    navi = navigation_string(prev,nex,filename)
    instring = open(filename,"r"){|f| f.gets(nil)}.split("\n")
    ostring=""
    while s = instring.shift
      if s =~ /^<body(.)*>/
	ostring += s + "\n" + navi+ "\n"
      elsif s =~ /^<\/body/
	ostring +=   navi + "\n" + s + "\n"
      elsif  s =~ /<!-- banner header -->/
        # adding space after header table
	ostring +=    s + "\n <p>&ensp;</p> \n"
      elsif s =~ /^<table.*Information on file/
	# skip the filename block created by rdoc
	while s !~ /^<\/table>/
	  s= instring.shift
	end
      else
	ostring +=     s + "\n"
      end
    end
    open(filename,"w"){|f| f.puts(ostring)}
  end
  def add_navigation_links(rdochtmls)
    n=rdochtmls.size
    rdochtmls.each_index{|i|
      if i == 0 then
	prev = nil
      else
	prev = rdochtmls[i-1]
      end
      nex = rdochtmls[i+1]
      add_links(rdochtmls[i],prev,nex)
    }
  end
  def convert_cpfilename_to_rdoc_htmlfilename(name)
    'doc/files/'+name.gsub(/\./, '_')+ '.html'
  end
  def create_navigations_for_cp_files(args)
    cpfiles = args.select{|x| x =~/\.cp$/}
    cpfiles.collect!{|x|convert_cpfilename_to_rdoc_htmlfilename(x)}
    add_navigation_links(cpfiles)
  end

  
  def create_toc_string(filename)
    s = "<p><H1>Contents</H1><a name=TOC><p>"
    currentlevel=0
    dirlevel=File.dirname(filename).split('/').size

    uppath = "../"*dirlevel
    @@sectionheaders.each{|x|
      while x[1]>currentlevel
	s += "<ul>\n"
	currentlevel+= 1
      end
      while x[1]<currentlevel
	s+=  "</ul>\n"
	currentlevel -= 1
      end
      basename =File.dirname(x[3])+"/."+File.basename(x[3]) 
      fname =     uppath+basename.gsub(/\./, '_')+ '.html'
      s+= "<li> <a href=#{fname}##{x[0]}>#{x[2]}</a></li>\n"
    }
    while 0<currentlevel
      s+=  "</ul>\n"
      currentlevel -= 1
    end
    s
  end  
  def add_toc
    @@filefortoc.collect!{|x|
      s=create_toc_string(x)
      fname=convert_cpfilename_to_rdoc_htmlfilename(File.dirname(x)+
						    "/."+File.basename(x))
      instring=File.open(fname,"r").read.gsub(/TOCTOCTOCTOC/,s)
      f=File.open(fname,"w+")
      f.write(instring)
      f.close
      fname
    }
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
    if File.exist?(a)
      unless tolatex_flag 
	dot_a = File.dirname(a)+"/."+File.basename(a);
      else
	dot_a = File.dirname(a)+"/"+File.basename(a,".cp")+ ".tex"
      end
      prep_cp(a, dot_a, tolatex_flag)
      a = dot_a
      del_file_list.push(dot_a)
    else
      a = ""
    end
    a
  elsif a =~ /\.rb$/
    if File.exist?(a)
      prep_rb(a)
      prep_rb_defs(a)
      prep_rb_hashes(a)
      prep_rb_special_comments(a)
      prep_rb_special_comments_for_partfiles(a)
    else
      a = ""
    end
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

add_toc
create_navigations_for_cp_files(ARGV)
if del_flag
  del_file_list.each do |f|
    File.delete(f)
  end


end
# :segment end:
