#!/usr/bin/ruby
# not2.rb
#
# test program to start writing conversion from .not to various formats
#
session_time = "-1";
session_number = -1;

print <<END_HTML_HEADER
<HTML>
  <HEAD><TITLE> #{Time.now} </TITLE></HEAD>
<BODY>
<pre>
END_HTML_HEADER

while str=gets
  if (str =~ /\\header/)==0
    
    print "header found ",  str;
    entry_name = str[9..999];
    current_session = str.split(" ")[-1];
    print current_session, "\n"
    array=current_session.split("-");
    current_time=array[0];  current_session_number=array[1];

# single session file= current_time/current_session_number.html
# monthly file= current_time-current_session_number.html
# name tag= session_number
    
    if current_time != session_time
      
    end
    
  end;
   print str
end


print <<END_HTML_FOOTER
</pre>
</BODY>
</HTML>
END_HTML_FOOTER
