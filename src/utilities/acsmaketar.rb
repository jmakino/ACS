#
# acsmaketar.rb
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

#
# Information on where to install the web pages
# 
installhost="grape.astron.s.u-tokyo.ac.jp"
installuname="acs"
installdir="WWW/tmp/artcompsci"



load "src/utilities/svn-recursive-print-files.rb"

def doc_directories(dirname)
  docs = []
  childdirs = Dir.entries(dirname)
  childdirs =childdirs.collect{|x| 
    x=nil if File.ftype(dirname+"/"+x) != "directory" or x == "." or  x == ".." 
    x
  }
  childdirs.each{|x|
    docs += doc_directories(dirname+"/"+x) if x
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


STDERR.print "Creating the file list....."
svnlist = svn_files(".",/(^\.\/msa$)/)

STDERR.print "svn files finished ..."
doclist = doc_directories(".")
doclist += add_files(doclist, ["v*.ps.gz",  "v*.pdf", ".imgs"])
#doclist.each{|x| print x,"\n"}
STDERR.print "doc files finished ..."

msafiles,excludefiles = all_files("msa", /(^\.svn$)/)
STDERR.print "msa files finished ..."

open("tmp.tarfilelist","w"){
  |f| f.print(svnlist.join("\n"))
}
open("tmp.tarfilelist2","w"){
  |f| f.print((svnlist+doclist+["msa"]).join("\n"))
}
open("tmp.tarfilelist3","w"){
  |f| f.print((excludefiles).join("\n"))
}
STDERR.print "file list  finished\n"

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
storedir = "versions"
releaselog = "RELEASES"

releasefile=storedir+"/"+releaselog
if File.exist?(releasefile)
  print "\nLast release:\n"
  system "tail -3 #{releasefile}"
else
  print "\nNo release file exist\n"
end
ans = ""
newversion=""
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

tarfilename= storedir+"/acs-#{newversion}.tgz"
tarlitefilename= storedir+"/acs-lite-#{newversion}.tgz"
STDERR.print "Creating  #{tarfilename}...."
system "tar czf #{tarfilename} -X tmp.tarfilelist3 `cat tmp.tarfilelist2`"
STDERR.print "finished\n"

STDERR.print "Creating  #{tarlitefilename}...."
system "tar czf  #{tarlitefilename} `cat tmp.tarfilelist`"
STDERR.print "finished\n"
system "rm tmp.tarfilelist*"
system "echo '' >> #{releasefile}"
system "(echo -n Date: ;date) >> #{releasefile}"
system "(echo -n Person: ;whoami) >> #{releasefile}"
open(releasefile,"a"){|f| f.print "Release: #{newversion}\n"}

sendcommand="rsync -e ssh -avprog versions #{installuname}@#{installhost}:#{installdir}"
extractcommand="ssh -l #{installuname} #{installhost} \"cd #{installdir} ; tar xzf #{tarfilename}\""

STDERR.print <<END
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
