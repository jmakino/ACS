#!/usr/local/bin/ruby -w

require "acs.rb"

options_text= <<-END

  Description: Returns the last n chunks of ACS data
  Long description:
    This program accepts ACS data, in the form of a series of chunks, each
    chunk starting with "ACS" on the first line, and ending with "SCA" on
    the last line.  Like the Unix program `tail', it returns the last n chunks.

    (c) 2004, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:
               ruby #{$0} -n 5


  Short name: 		-n
  Long name:            --number
  Value type:           int
  Default value:        1
  Description:          Number of last ACS data chunks returned
  Variable name:        n
  Long description:
    This option allows the user to choose the number of ACS chunks that
    will be returned by this program, after a series of ACS chunks have been
    read in.


  END

clop = parse_command_line(options_text)

chunk = []
a = ACS_IO.acs_read
while a
  chunk.push(a)
  chunk.shift if chunk.length > clop.n
  a = ACS_IO.acs_read
end
while chunk[0]
  chunk.shift.acs_write
end
