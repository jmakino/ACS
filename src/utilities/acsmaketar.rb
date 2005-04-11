#
# acsmaketar.rb
#
# version 2005/1/29
#
# This will make two tarballs,
#
# acssrcs.tgz and acsall.tgz
#
# acssrcs.tgz containes all files under 
# SVN
#
# In addition to them, acsall.tgz will contain
#
# -- all "doc" files
# -- all "some regular expression" files (see the source...)
#    in the same directory as doc directory
#    (to get volume0.ps and volume0.pdf etc)
# -- all files (except under .svn) under msa
#
# 
$LOAD_PATH.push(ENV["ACSROOT"]+"/src/utilities")
require "clop.rb"

optionstext= <<-END

Short name: -l
Long name:  --local_only
Value type:  bool
Default value: nil
Description: if set, only the local archive file is made
Variable name: localonly
Long description:
When this option is set, #{$0} does create a new release,
but does not upload the release to the web server.


Short name: -w
Long name:  --web_only
Value type:  bool
Default value: nil
Description: if set, web update is run, from existing release
Variable name: webonly
Long description:
When this option is set, #{$0} does not create a new release,
but still uploads the existing release to the web server.


Short name: -r
Long name:  --release_name
Value type:  string
Default value:NONE
Description: New release name
Variable name: releasename
Long description:
New release name (specify "1.0" for version 1.0). If not specified,
#{$0} requests input from standard input.


END

clop= Clop.new(optionstext,ARGV,true)
p $webonly
p $localonly
p $releasename


#
# Information on where to install the web pages
# 
installhost="grape.astron.s.u-tokyo.ac.jp"
installuname="acs"
installdir="WWW/artcompsci"
$toplevelname="acs/"
#
# filenames will be acs_(version).tgz acs_lite_(version).tgz
# 
# They will be placed at versions directory
#
#
#
# release file format:
#
# Date: XXX
# person: YYY
# release: ZZZ
storedir = "rel"
releaselog = "RELEASES"



load "src/utilities/svn-recursive-print-files.rb"

def doc_directories(dirname,excludeexpression)
  return [] if dirname =~ excludeexpression 
  docs = []
  childdirs = Dir.entries(dirname)
  childdirs =childdirs.collect{|x| 
    x=nil if File.ftype(dirname+"/"+x) != "directory" or x == "." or  x == ".." 
    x
  }
  childdirs.each{|x|
    docs += doc_directories(dirname+"/"+x,excludeexpression ) if x
  }
  docname = dirname+"/doc"
  if File.exist?(docname) and  File.ftype(docname) == "directory"
    docs.push(docname)
  end
 docs
end


def all_files(dirname,excludeexp)
  files = []
  excludefiles = []
  childdirs = Dir.entries(dirname)
  childdirs =childdirs.collect{|x| 
    if x =~ excludeexp
      excludefiles.push(dirname+"/"+x) 
      x=nil
    else
      x=nil if File.ftype(dirname+"/"+x) != "directory" or x == "." or  x == ".."
    end
    x
  }
  childdirs.each{|x|
    if x
      f=all_files(dirname+"/"+x,excludeexp) 
      files+=f[0]
      excludefiles+= f[1]
    end
  }
  Dir.entries(dirname).each{|x| 
    fname = dirname+"/"+x
    files.push(fname) if File.ftype(fname) == "file"
  }
  [files,excludefiles]
end


def add_files(dirlist,fileexpressions)
  newfiles=[]
  dirlist.each{|x|
    dirname = File.dirname(x)
    savedir = Dir.pwd
    Dir.chdir(dirname)
    fileexpressions.each{|y|
      newfiles += Dir.glob(y).collect{|z| dirname+"/"+z}
    }
    Dir.chdir(savedir)
  }
  newfiles
end

unless $webonly

  STDERR.print "Creating the file list....."
  svnlist = svn_files(".",/(^\.\/msa$)|(^\.\/not$)/)
  
  STDERR.print "svn files finished ..."
  doclist = doc_directories(".", /.\/not$/)
  doclist += add_files(doclist, ["v*.ps.gz",  "v*.pdf", ".imgs"])
  #doclist.each{|x| print x,"\n"}
  STDERR.print "doc files finished ..."
  
  msafiles,excludefiles = all_files("msa", /^(\.svn|web_old)$/)
  STDERR.print "msa files finished ..."
  p svnlist[0]
  [svnlist,doclist,excludefiles].each{|x|x.map!{|y| $toplevelname+y}
  }
  p svnlist[0]
  open("tmp.tarfilelist","w"){
    |f| f.print(svnlist.join("\n"))
  }
  open("tmp.tarfilelist2","w"){
    |f| f.print((svnlist+doclist+[$toplevelname+"msa"]).join("\n"))
  }
  open("tmp.tarfilelist3","w"){
    |f| f.print((excludefiles).join("\n"))
  }
  STDERR.print "file list  finished\n"
  
end

releasefile=storedir+"/"+releaselog
if File.exist?(releasefile)
  print "\nLast release:\n"
  system "tail -3 #{releasefile}"
else
  print "\nNo release file exist\n"
end
ans = ""
newversion=$releasename
if newversion=="NONE"
  begin
    print "Enter new version:"
    x = STDIN.gets().chomp
    print "\nNew version: #{x}\n\nAre you sure to make this version?(y/q/n)"
    ans = STDIN.gets()
    newversion=x
  end  while ans !~ /(y)|(q)/
  if ans == "q"
    STDERR.print "Abort: version #{newversion} is not created\n"
    exit
  end
end

tarfilename= storedir+"/acs-#{newversion}.tgz"
tarlitefilename= storedir+"/acs-lite-#{newversion}.tgz"

topd=$toplevelname

unless $webonly
  STDERR.print "Creating  #{tarfilename}...."
  system "cd ../; tar czf #{topd}#{tarfilename} -X #{topd}tmp.tarfilelist3 `cat #{topd}tmp.tarfilelist2`"
  STDERR.print "finished\n"
  
  STDERR.print "Creating  #{tarlitefilename}...."
  system "cd ../; tar czf  #{topd}#{tarlitefilename} `cat #{topd}tmp.tarfilelist`"
  STDERR.print "finished\n"
  system "rm tmp.tarfilelist*"
  system "echo '' >> #{releasefile}"
  system "(echo -n Date: ;date) >> #{releasefile}"
  system "(echo -n Person: ;whoami) >> #{releasefile}"
  open(releasefile,"a"){|f| f.print "Release: #{newversion}\n"}
end

unless $localonly
  sendcommand="rsync -e ssh -avprog #{storedir} #{installuname}@#{installhost}:#{installdir}"
  extractcommand="ssh -l #{installuname} #{installhost} \"cd #{installdir} ; tar xzf #{tarfilename}\""
  
  STDERR.print <<-END
  Do you want to update the files on the ACS WEB SERVER now?
  You do not need to do this rigt now. If you want to do so later, 
  you can do, at $ACSROOT
  #{sendcommand}
  and
  #{extractcommand}
  END
  print "(y/n)?"
  ans = STDIN.gets().chomp
  if ans == "y"
    system sendcommand
    system extractcommand
  end
end
