#!/usr/bin/ruby
# not2.rb
#
# convers the mother .not file to HTML files with ponters and all fancy
# stuffs
#
# usage: ruby not2.rb mother_not_file_name
#
# example: ruby not2.rb all.not
#
session_time = "-1";
session_number = "-1";


main_index_file_name = "index.html"
main_index_file = open(main_index_file_name, "w+");
$singlesession_source_file = nil;
$multisession_source_file = nil;
$backward_pointer_file_name = "backaward_pointers.dat";

def printboth(s)
  $singlesession_source_file.print s
  $multisession_source_file.print s
end

def addpreceedingzeros(n,s="")
zeros="0" * (n - s.length);
zeros  + s;
end
def removepreceedingzeros(s)
s = s[1..(s.length)] while s[0..0]== "0"
s
end

def normalize_ref_tag(s)
  element=s.split("-");
  year=element[0];
  month=addpreceedingzeros(2,element[1]);
  day=addpreceedingzeros(2,element[2]);
  seq=addpreceedingzeros(3,element[3]);
  tag = year + "-" + month + "-" + day + "-" + seq
  if  element.length > 4
    section_number = element[4];
    p section_number
    last = (section_number.length) -1
    section_number += "." if section_number[last..last] != "."
    tag += "-" + section_number;
  end
  tag
end

def denormalize_ref_tag(s)
  element=s.split("-");
  year=element[0];
  month=removepreceedingzeros(element[1]);
  day=removepreceedingzeros(element[2]);
  seq=removepreceedingzeros(element[3]);
  tag = year + "-" + month + "-" + day + "-" + seq
  if  element.length > 4
    section_number = element[4];
    tag += "-" + section_number;
  end
  tag
end


def make_ref_tag(s,mode)
  element=s.split("-");
  year=element[0];
  month=element[1];
  day=element[2];
  seq=element[3];
  if mode == "single"
    tag = "../"+ year + "-" + month + "/" + day + "-" + seq + ".html"
  else
    tag = year + "-" + month + ".html#"+ day + "-" + seq
  end
  if  element.length > 4
    if mode == "single"
      tag += "#"+element[4]
    else
      tag += "-"+element[4]
    end
  end
  tag;
end

$backref_source_count={0=>0}
$backref_source={0=>0}
def readin_reference_pairs
  backward_pointer_file=open($backward_pointer_file_name,"r") 
  while s=backward_pointer_file.gets
    ss=s.split
    source=ss[0];
    target=ss[1];
    if $backref_source_count[target] == nil
      $backref_source_count[target] =1
    else
      $backref_source_count[target] +=1
    end
    $backref_source[target+ " " + $backref_source_count[target].to_s]=source
  end
  backward_pointer_file.close
  p $backref_source_count
  p $backref_source
end

$backward_pointer_file = nil;
def process_reference(s,scurrent)
  mask = 0777 ^ File.umask;
  if $backward_pointer_file== nil
    $backward_pointer_file=open($backward_pointer_file_name,"w+") 
  end
    print "Enter process_reference ",  s, "\n";
  while (ref_pos = s.index("\\ref")) != nil
    print "In process_reference ",  s, " ",s.length,"\n";
    printboth(s[0..(ref_pos-1)]) unless ref_pos < 1
    ok = 0
    if ref_pos == 0
      ok=1
    elsif s[ref_pos-1..ref_pos-1] != "\\"
      ok=1
    end
    s=s[(ref_pos+4)..(s.length)];
    if ok == 1 
      refid = s.split[0];
      s = s[(refid.length)..(s.length)];
      p refid
      p ref_pos
      #
      # now we need to make:
      #
      # ../2003-11/11-001.html#x.x.x for single source
      # 2003-11.html#11-001-x.x.x for multi source
      nref = normalize_ref_tag(refid)
      dref = denormalize_ref_tag(refid)
      p nref
      p dref
      $backward_pointer_file.print scurrent, " ", nref, "\n"
      $singlesession_source_file.print "<a href=",make_ref_tag(nref,"single"),\
      ">", dref, "</a>"
      $multisession_source_file.print "<a href=",make_ref_tag(nref,"multi"),\
      ">", dref, "</a>"
    else
      STDERR.print "escaped ref ", s, "\n";
      printboth("\\ref ")
    end
  end
end


def add_backref(slabel)
  if $backref_source_count[slabel] != nil
    printboth "See: "
    i=1
    while i <= $backref_source_count[slabel]
      blabel= $backref_source[slabel + " " + i.to_s]
      dlabel=denormalize_ref_tag(blabel)
      $singlesession_source_file.print "<a href=",make_ref_tag(blabel,"single"),\
      ">", dlabel, "</a> "
      $multisession_source_file.print "<a href=",make_ref_tag(blabel,"multi"),\
      ">", dlabel, "</a> "
      i+=1
    end
  end
end


#create session list 
session_list = `ls ????-??/??-???`.split($/);
session_id={session_list[0]=>0}
section_header="";
i=0;
while  i<session_list.length 
   session_id[session_list[i]]=i;
   i+= 1;
end
session_list.each{|s|
   i = session_id[s];
}

#create backward_pointer list
readin_reference_pairs;

def make_relative_tag(s,i,session_id,session_list)
    current_index = session_id[s];
  "<a href=../"+ session_list[(current_index+i) % session_list.length] + ".html>" 
end
def make_relative_tag_in_multi(s,i,session_id,session_list)
    current_index = session_id[s];
  p current_index
    session_names = session_list[(current_index+i)%session_list.length].split('/')
       "<a href="+ session_names[0] + ".html#" + session_names[1]+ ">" 
end

prev_line = "";
in_preformatted = 0;
while str=gets
  if (str =~ /\\header/)==0
    
    section_header="";
    print "header found ",  str;
    splitted_header=str.split(" ");
    year = splitted_header[6];
    month = splitted_header[2];
    day = splitted_header[3];
    time = splitted_header[4];
    day_of_week = splitted_header[1];
    person_name = splitted_header[7..-5].join
    entry_name = str[9..999];
    p person_name;
    current_session = splitted_header[-1];
    print current_session, "\n"
    array=current_session.split("-");
    p array
    current_time=array[0].to_s + "-" + array[1].to_s;
    current_session_number=array[2].to_s + "-" + array[3].to_s;
    print "time, session = ", current_time, " ", current_session_number, "\n";

# single session file= current_time/current_session_number.html
# monthly file= current_time.html
# name tag= session_number
    secondary_index_file_name = current_time+main_index_file_name;
    $multisession_source_file_name =  current_time+ ".html";
    $singlesession_source_file_name =  current_time + "/" + current_session_number +  ".html";
    
    if current_time != session_time
      if session_time != "-1"
	secondary_index_file.close;
	$multisession_source_file.close;
      end
      session_time=current_time;
      secondary_index_file = open(secondary_index_file_name, "w+");
      $multisession_source_file=open($multisession_source_file_name, "w+");
      
      main_index_file.print <<-END_MAININDEX
      <a href= #{$multisession_source_file_name}>
	#{month}  #{year} </a>
	<a href= #{secondary_index_file_name}>
	Table
      </a><br>
    END_MAININDEX

    end;

    if session_number != "-1"
      $singlesession_source_file.close;
    end
    session_number = current_session_number;
    source_label = current_time + "-"+ current_session_number

    $singlesession_source_file=open($singlesession_source_file_name, "w+");
    print "single session file opened \n";

      secondary_index_file.print <<-END_SECONDINDEX
      <a href= #{$multisession_source_file_name}##{session_number}>
	#{month} #{day} #{day_of_week} #{year} #{time} Session #{session_number} </a>/
	<a href= #{$singlesession_source_file_name}>
    single page
    </a><br>
    END_SECONDINDEX
      p session_id, session_list, current_session_number
    $multisession_source_file.print <<-END_MULTISOURCE

      <a name=#{session_number}>

<H2> 	#{month} #{day} #{day_of_week} #{year} #{time} Session #{session_number} </H2>
	<a href= #{$singlesession_source_file_name}>
    single page</a> 
	#{make_relative_tag_in_multi(current_time + "/"                 + current_session_number,-1,session_id,session_list)} prev</a> 
	#{make_relative_tag_in_multi(current_time + "/"                 + current_session_number,1,session_id,session_list)} next</a>
END_MULTISOURCE




$singlesession_source_file.print <<-END_SINGLESOURCE
      <a name=#{session_number}>
<H2> 	#{month} #{day} #{day_of_week} #{year} #{time} Session #{session_number} </H2>
      <a href=../#{current_time}.html##{session_number}>monthly notes</a> 
	#{make_relative_tag(current_time + "/"                 + current_session_number,-1,session_id,session_list)} prev</a> 
	#{make_relative_tag(current_time + "/"                 + current_session_number,1,session_id,session_list)} next</a> 
END_SINGLESOURCE


# end of the header processing

    elsif str=~ /^\d{1,}\./
      # section header, starting with numbers
      STDERR.print "section header found: ", str, "\n";
      section_header = str.split[0];
      section_level = section_header.count('.');
      header_level = section_level+2;
      $singlesession_source_file.print <<-END_SINGLESOURCE
      <a name=#{section_header}>
	<H#{header_level}>
      #{str}
	</H#{header_level}>
     END_SINGLESOURCE
      $multisession_source_file.print <<-END_MULTISOURCE
      <a name=#{session_number}-#{section_header}>
	<H#{header_level}>
      #{str}
	</H#{header_level}>
     END_MULTISOURCE

      #process backward pointers
      source_label = current_time + "-"+ current_session_number
      source_label += "-" + section_header unless section_header == ""
      add_backref(source_label)


    elsif str=~ /\\begincode/
      printboth("<blockquote>");
      printboth("<pre>");
      in_preformatted=1;
    elsif str=~ /\\endcode/
      printboth("</pre>");
      printboth("</blockquote>");
      in_preformatted=0;
    elsif str=~ /\\ref/
      #string contains reference to a previous note
      print "Found referene ", str;
      s = str;
      process_reference(s,source_label)
    else
      #normal line. 
      if  prev_line == "\n"
	printboth("<p>\n") unless in_preformatted == 1;
      end
      printboth(str);
    end  
    prev_line = str;
  end;
