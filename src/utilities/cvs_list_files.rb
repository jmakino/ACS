
def cvs_files(dirname)
  p dirname
  files=[]
  entry = dirname + "/CVS/Entries"
  unless File.exist?(entry) 
    return []
  end
  s = open(entry,"r"){|f| f.gets("")}.split("\n")
  s.each{|x|
    a=x.split("/")
    if a[0] =~ /^D$/ 
      if a[1]
	print "descending to ", a[1], "\n"
	files.push(dirname+ "/"+a[1])
	files += cvs_files(dirname+"/"+a[1])
      end
    else
      files.push(dirname+ "/"+a[1])
#      p dirname+ "/"+a[1]
    end
  }
  files
end

x= cvs_files(ARGV[0])
p "end of cvs_files"
x.each{|y| 
  print "svn add -N "+y+"\n"
  system("svn add -N "+y+"\n")
}
