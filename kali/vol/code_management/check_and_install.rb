#!/usr/bin/env ruby
#
# check_and_install.rb
#
# command to check if target is doubly defined or not 
# and put it if it is okay.
#
# Usage: check_and_install.rb NAME_OF_ENV_VAR_FOR_INSTALL_DIR [files ...]
#
# Example: check_and_install.rb ACSSCRIPTS check_and_install.rb

def addzeros(n,s="")
zeros="0" * (n - s.length);
zeros  + s;
end




def name_with_date(name)

  addzeros(4,Time.now.year.to_s)+ addzeros(2,Time.now.month.to_s)+
    addzeros(2,Time.now.day.to_s)+"_"+name
end
def store_file_with_date(sourcename, dirname)
  subdirname = (sourcename.gsub(/\./,"_"))
  subdirpath = dirname+"/"+subdirname
  Dir.mkdir(subdirpath) unless File.exist?(subdirpath)
  targetname=name_with_date(sourcename)
  system "/bin/cp -p #{sourcename} #{subdirpath}/#{targetname}"
  system "svn add -N #{subdirpath}"
  system "svn add -N #{subdirpath}/#{targetname}"
end  

def check_and_install(sourcename, dirname)
  checkdirname= dirname+"/./.sourcenames"
  checkfilename= checkdirname+"/"+sourcename
  fullsourcename=File.expand_path(sourcename)
  if File.exist?(checkfilename)
    oldsource= open(checkfilename,"r"){|f| f.gets}.chomp
    if oldsource != fullsourcename
      raise "Target file comes from #{oldsource}, different from #{fullsourcename}"
    end
  end
  system "diff -q  #{sourcename} #{dirname}"
  if $? != 0
    STDERR.print "Installing file #{sourcename} to  #{dirname}" 
    system "/bin/cp -p #{sourcename} #{dirname}" 
    store_file_with_date(sourcename,dirname)
    STDERR.print " ... finished\n" 
  end
  Dir.mkdir(checkdirname) unless File.exist?(checkdirname)
  unless File.exist?(checkfilename)
    open(checkfilename,"w"){|f| f.print fullsourcename} 
  end
end

dirvarname = ARGV.shift
raise "#{$0}:Env var #{dirvarname} not defined" unless ENV.key?(dirvarname)
dirname= ENV[dirvarname]
raise "#{$0}:#{dirname} does not exist" unless File.exist?(dirname)
raise "#{$0}:#{dirname} is not a directory" unless File.stat(dirname).ftype == "directory"

while sourcename = ARGV.shift
  check_and_install(sourcename, dirname)
end
