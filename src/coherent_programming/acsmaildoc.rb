#!/usr/bin/ruby
=begin
  acsmaildoc.rb
=end

require "time.rb"

$docroot = "/usr2/acs/acs/not/Mail"
$maildir = "/home/not/Mail/inbox"
$seqfile = $docroot + "/seq"

module Acsmaildoc

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
	s = " "+s if s[0] == ">" # to handle quoted texts
	outfile.print s
      end
    end
    infile.close
    outfile.close if inheader == 0
  end

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
      print "Now runs update-lab etc"
      bindir = ENV["ACSBIN"]
      system("csh -f #{bindir}/update-lab")
      system("cd #{$docroot} ; svn commit --message Mails-added")
    end
  end
end

include Acsmaildoc
acsmaildoc
