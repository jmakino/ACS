#
# backupnonsvnfiles.rb
#
# version 2005/2/28
#
#
$LOAD_PATH.push(ENV["ACSROOT"]+"/src/utilities")
require "clop.rb"

optionstext= <<-END

Short name: -s
Long name:  --source_dir
Value type:  string
Default value: none
Description: Name of the source directory
Variable name: sdir
Long description:
The name of the source directory, under which the files are copied.


Short name: -t
Long name:  --target_dir
Value type:  string
Default value: none
Description: Name of the target directory
Variable name: tdir
Long description:
The name of the target directory, to which the files are copied.

Short name: -d
Long name:  --copy_dot_files
Value type:  bool
Default value: nil
Description: if set, files with name start with "."  are also copied
Variable name: copy_dot_files
Long description:
If set, all files including the files with name starting with "." are copied.
Deafault is not to copy these dot files.


END

clop= Clop.new(optionstext,ARGV,true)

require "svn-recursive-print-files.rb"

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

def  checkandcreatedir(dirname)
  return if FileTest.exist?(dirname) and  FileTest.directory?(dirname)
  unless FileTest.exist?(dirname)
    upname = File.dirname(dirname)
    if upname != "." 
      checkandcreatedir(upname)  unless FileTest.exist?(upname)
    end
    print "Making dir #{dirname}\n"
    Dir.mkdir(dirname)
  else
    raise "#{dirname} is not a directory" 
  end
end




def copyfile (filename, sourcedir, targetdir)
  targetfile=filename.sub(sourcedir, targetdir)
  raise "Name substitution failed for #{filename}" if filename == targetfile
  checkandcreatedir(File.dirname(targetfile))
  print "copying  #{filename} to #{targetfile}\n"
  system "cp -p #{filename} #{targetfile}"
end


STDERR.print "Creating the file list....."
svnlist = svn_files($sdir)
STDERR.print "svn files finished ..."
if $copy_dot_files
  exclude = /^$/
else
  exlude = /^\.\w+/
end
allfiles=all_files($sdir,exlude)
filestocopy=(allfiles[0]-allfiles[1])-svnlist
filestocopy.each{|fname|
  copyfile(fname, $sdir, $tdir)
}

