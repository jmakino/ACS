#
# svn-recursive-print-files.rb
#
# print all files (not directories) under svn version control, 
# by recursively walk through all directories under svn.
# Jun Makino 2004-5-30
#
require 'rexml/document'

def svn_files(dirname,excluderegexp=nil)
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
	if excluderegexp
	  if dirname+"/"+id =~ excluderegexp
	    STDERR.print "Directory  #{dirname+"/"+id} skipped\n" if $DEBUG
	    go = nil
	  end
	end
	files += svn_files(newname,excluderegexp) if go
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


