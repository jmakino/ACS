#!/usr/bin/ruby
=begin
  acsmaildoc.rb
=end

require "time.rb"

$docroot = "/usr2/acs/acs/not/Mail"
$maildir = "/home/not/Mail/inbox"
$seqfile = $docroot + "/seq"
module Acsmaildoc
  def prep_cp(infile, outfile)
    begin
      ifile = open(infile, "r")
    rescue
      raise "#{infile} does not exist"
    end
    ofile = open(outfile, "w+")
    while s = ifile.gets
      s.gsub!(/<p>/, '<i>[Jun: ')
      s.gsub!(/<\/p>/, ' -- Piet] </i>')
      s.gsub!(/<j>/, '<i>[Piet ')
      s.gsub!(/<\/j>/, ' -- Jun] </i>')
      if s =~ /:in.*code:/ and s.index("\":inccode:\"")==nil
	s.sub!(/:in.*code:/, ':include:')
	ofile.print "---\n"
	ofile.print s
	ofile.print "---\n"
      else
	ofile.print s
      end
    end
    ifile.close
    ofile.close
  end

  # :segment start: prep_rb
  def process_mailfile(seqnum)
    infile = open(seqname = $maildir + "/" + seqnum.to_s, "r")
    inheader = 1
    while s=infile.gets
      if inheader == 1
	s.chomp!
	if s =~ /^Date:/
	  time = Time.parse(s[5,s.length])
	  year = time.year
	  month = time.month
	  print "time for file ", seqnum, " = ",  time, "\n"
	end
	if s =~ /^From:/ 
	  from = s
	end
	if s.empty? 
	  inheader = 0
	  dirname = $docroot
	  for subdirname in [year.to_s, month.to_s]
	    dirname = dirname + "/" + subdirname
	    Dir.mkdir(dirname) unless File.exist?(dirname)
	  end
	  print "open file ", dirname+   "/" + seqnum.to_s, "\n";
	  outfile=open(dirname+ "/"+ seqnum.to_s, "w+")
	  outfile.print "Date: ", time, "\n\n"
	  outfile.print from, "\n\n"
	end
      else
	outfile.print s
      end
    end
    infile.close
    outfile.close if inheader == 0
  end
  # :segment end: prep_rb
  def acsmaildoc()
    begin
      ifile = open($seqfile, "r")
      seqnum = ifile.gets.to_i
      ifile.close
    rescue
      ofile = open($seqfile, "w+")
      ofile.print "1"
      ofile.close
      retry
    end
    print "current seq = ", seqnum, "\n";
    updated = 0
    while  File.exist?(seqname = $maildir + "/" + seqnum.to_s) do
      process_mailfile(seqnum)
      seqnum += 1
      updated = 1
    end
    ofile = open($seqfile, "w+")
    ofile.print seqnum, "\n"
    ofile.close

    if updated == 1 then
      system("update-lab")
      system("cd #{$docroot} ; svn commit --message Mails-added")
    end
  end
end

include Acsmaildoc
acsmaildoc
