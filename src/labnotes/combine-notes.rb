#!/usr/bin/ruby
#
# combine-notes.rb
#
# this script combines the notes files (second arguments and ...)
# and create a single RDOC format file
# 
# usage: ruby combine-notes.rb  combined_file_name input_notes...
#
# example: ruby combine-notes.rb combined/2003-11-all */2003/11/*
# 

#require "parsedate.rb"
#include ParseDate
require "time.rb"

class FileWithTime
  attr_accessor :time, :name
  def initialize(t=0,fn="")
    @time = t
    @name = fn
  end
  def <=> (other)
    self.time <=> other.time
  end
end


def check_time_stamp(filename)
  timestamp_filename = File.dirname(filename)+"/."+File.basename(filename);
  unless FileTest.exists? timestamp_filename
    modtime = File.mtime(filename)
    print modtime, "\n"
    unless timestamp_file = open(timestamp_filename,"w+")
      raise "Failed to create output file #{timestamp_filename}"
    end
    timestamp_file.print modtime, "\n"
    timestamp_file.close
  end
  unless timestamp_file = open(timestamp_filename,"r")
    raise "Failed to open timestamp file #{timestamp_filename}"
  end
  s = timestamp_file.gets
  timestamp_file.close
  filewithtime = FileWithTime.new
  filewithtime.time = Time.parse(s)
  filewithtime.name = filename
  filewithtime
end

def printnote(filewithtime, outputfile)
  unless infile = open(filewithtime.name,"r")
    raise "Failed to open note file #{filewithtime.name}"
  end
  name = filewithtime.name
  contributer_name = $contributer_list[name[0...(name.index("/"))]]
  p contributer_name
  outputfile.print "\n---\n<em>#{filewithtime.time} -- </em>" +
    "#{contributer_name}\n---\n"
  
  while s = infile.gets
    outputfile.print s
  end
end


$contributer_list = {}
peoplefile = "people"
load peoplefile if FileTest.exists? peoplefile




if not output_file = open(ARGV[0],"w+")
  raise "Failed to create output file #{output_file}"
end

ARGV.shift

filewithtime = Array.new(ARGV.size)
for i in 0...ARGV.size
  filewithtime[i] = check_time_stamp(ARGV[i])
end
p filewithtime
sorted = filewithtime.sort
sorted.each do |x| 
  print x.time," ", x.name, "\n"
  printnote(x,output_file)
end


