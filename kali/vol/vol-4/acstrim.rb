require "vector.rb"
require "clop.rb"
require "acsio.rb"

options_text= <<-END

  Description: Trims a stream of ACS data, returning only in in n chunks
  Long description:
    This program accepts ACS data, in the form of a series of chunks, each
    chunk starting with "ACS" on the first line, and ending with "SCA" on
    the last line.  It passes on only one out of n chunks, where the value
    of n can be specified by the user

    (c) 2004, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:
               ruby #{$0} -n 10


  Short name: 		-n
  Long name:            --number
  Value type:           int
  Default value:        1
  Description:          One over the fraction of ACS data chunks passed on
  Variable name:        n
  Long description:
    This option allows the user to choose the fraction of ACS chunks that
    are passed on.  The first chunk is always echoed, and after that only
    the nth chunk is echoed on the output stream.

  END

c = parse_command_line(options_text)

chunk = []
i = 0
a = ACS_IO.acs_read
while a
  if i == 0
    a.acs_write
    i = c.n
  end
  i -= 1
  a = ACS_IO.acs_read
end
