#!/usr/bin/ruby
# register-note.rb
#
# 
def addpreceedingzeros(n,s="")
zeros="0" * (n - s.length);
zeros  + s;
end

sequence_file_name = "sequence.dat";
sequence_string_length=3;
global_not_file_name = "all.not";

print "Year", Time.now.year
print "Month", Time.now.month, "\n";
print "day", Time.now.day, "\n";

year =  addpreceedingzeros(4,Time.now.year.to_s);
month = addpreceedingzeros(2,Time.now.month.to_s);
day =   addpreceedingzeros(2,Time.now.day.to_s);
dirname = year + "-" +  month;
mask = 0777 ^ File.umask;
p dirname
seqname = dirname + "/"  + sequence_file_name + Time.now.day.to_s;
unless File.exist?(dirname)
   print "creating the new directory", dirname, "\n";
   Dir.mkdir(dirname, mask);
end

if  File.exist?(seqname)
   seqfile = open(seqname, "r");
   count = seqfile.gets.to_i;
   seqfile.close;
else
   count=0
end
sequence_number = count + 1;
seqfile = open(seqname, "w+");
seqfile.print sequence_number;
seqfile.close;
STDERR.print "New sequence count = ", count, "\n";

# create the header line for the new labnote entry
#
# header line consists of:
# Date, Name of the person, sequence number 
#
# sequence number here includes year, month and zeros preceeding
# the real number, like 2003-11-13-001
#
header = Time.now;
name = `finger -l #username | head -1`.split($/);
realname = name[0];
realname = realname[realname.index("Name:")+6..999];
sequence_string = addpreceedingzeros(sequence_string_length,
                 sequence_number.to_s)
short_sequence_string=  day + "-" + sequence_string;
long_sequence_string= dirname + "-" + short_sequence_string;

header = "\\header " + header.to_s + " "+ realname+ \
   " Session No. = "+ long_sequence_string;

sequence_lab_file_name = seqname = dirname + "/"  + short_sequence_string;
STDERR.print "seq file name = ", sequence_lab_file_name, "\n";
STDERR.print "global = ", global_not_file_name, "\n";
 
STDERR.print header,"\n\n";

all_file = open(global_not_file_name, "a+");
sequence_file = open(sequence_lab_file_name, "w+");
STDERR.print header,"\n\n";
all_file.print header,"\n\n";
sequence_file.print header,"\n\n";


while gets
   all_file.print 
   sequence_file.print 
end
all_file.print "\n\n";

