#
# svn-recursive-print-files.rb
#
# print all files (not directories) under svn version control, 
# by recursively walk through all directories under svn.
# Jun Makino 2004-5-30
#
require 'rexml/document'

def svn_files(dirname,expcluderegexp=nil)
  STDERR.print "Entering  #{dirname}\n"
  files=[]
  entry = dirname + "/.svn/entries"
  unless File.exist?(entry) 
    return []
  end
  doc = nil
  File.open(entry) {|f|  doc = REXML::Document.new(f.read) }
  doc.elements.each("wc-entries"){|x| 
    x.elements.each("entry"){|y| 
      id = y.attributes["name"]
      kind =y.attributes["kind"]
      if kind =~ /dir/ and id != ""
	newname = dirname+"/"+id
	go = true
	p id
	p kind
	if excluderegexp
	  go = nil if id =~ excluderegexp
	end
	files += svn_files(newname,expcluderegexp) if go
      end
      if kind =~ /file/ 
	files.push(dirname+"/"+id)
      end
    }
  }
  files
end

if __FILE__ == $0
  print "test code output\n"
  print svn_files(ARGV[0]).join("\n"),"\n"
end


