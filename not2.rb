#!/usr/bin/ruby
# not2.rb
#
# test program to start writing conversion from .not to various formats
#
session_time = "-1";
session_number = "-1";

print <<END_HTML_HEADER
<HTML>
  <HEAD><TITLE> #{Time.now} </TITLE></HEAD>
<BODY>
<pre>
END_HTML_HEADER

main_index_file_name = "index.html"
main_index_file = open(main_index_file_name, "w+");
singlesession_source_file = nil;
multisession_source_file = nil;
while str=gets
  if (str =~ /\\header/)==0
    
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
    current_time=array[0];  current_session_number=array[1];

# single session file= current_time/current_session_number.html
# monthly file= current_time.html
# name tag= session_number
    secondary_index_file_name = current_time+main_index_file_name;
    multisession_source_file_name =  current_time+ ".html";
    singlesession_source_file_name =  current_time + "/" + current_session_number +  ".html";
    
    if current_time != session_time
      if session_time != "-1"
	secondary_index_file.close;
	multisession_source_file.close;
      end
      session_time=current_time;
      secondary_index_file = open(secondary_index_file_name, "w+");
      multisession_source_file=open(multisession_source_file_name, "w+");
      
      main_index_file.print <<-END_MAININDEX
      <a href= #{multisession_source_file_name}>
	#{month}  #{year} </a>
	<a href= #{secondary_index_file_name}>
    Table
    </a><br>
    END_MAININDEX

    end;

    if session_number != "-1"
      singlesession_source_file.print "</pre>"
      singlesession_source_file.close;
    end
    session_number = current_session_number;
    singlesession_source_file=open(singlesession_source_file_name, "w+");
    print "single session file opened \n";

      secondary_index_file.print <<-END_SECONDINDEX
      <a href= #{multisession_source_file_name}##{session_number}>
	#{month} #{day} #{day_of_week} #{year} #{time} Session #{session_number} </a>/
	<a href= #{singlesession_source_file_name}>
    single page
    </a><br>
    END_SECONDINDEX
    multisession_source_file.print "</pre>" unless session_number  == "001";
    multisession_source_file.print <<-END_MULTISOURCE

      <a name=#{session_number}>

<H2> 	#{month} #{day} #{day_of_week} #{year} #{time} Session #{session_number} </H2>
	<a href= #{singlesession_source_file_name}>
    single page
    </a>
<pre>
END_MULTISOURCE
singlesession_source_file.print <<-END_SINGLESOURCE

      <a name=#{session_number}>

<H2> 	#{month} #{day} #{day_of_week} #{year} #{time} Session #{session_number} </H2>
<pre>
END_SINGLESOURCE
    else
      singlesession_source_file.print
      multisession_source_file.print
    end  

  end;


print <<END_HTML_FOOTER
</pre>
</BODY>
</HTML>
END_HTML_FOOTER
