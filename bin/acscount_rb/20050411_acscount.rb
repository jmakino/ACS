#!/usr/local/bin/ruby -w

require "acs"

options_text= <<-END

  Description: Counts the number of chunks of ACS data
  Long description:
    This program accepts ACS data, in the form of a series of chunks, each
    chunk starting with "ACS" on the first line, and ending with "SCA" on
    the last line.  It prints the total number of chunks.

    (c) 2005, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:
               kali #{$0} -n 5


  Short name: 		-n
  Long name:            --number
  Value type:           int
  Default value:        1
  Description:          NONSENSE!  ERASE THIS OPTION when clop is repaired
  Variable name:        n
  Long description:
    This option allows the user to choose the number of ACS chunks that
    will be returned by this program, after a series of ACS chunks have been
    read in.


  END

clop = parse_command_line(options_text)

n = 0
a = ACS_IO.acs_read
while a
  n += 1
  a = ACS_IO.acs_read
end
print "#{$0}: #{n} chunks of ACS data\n"
