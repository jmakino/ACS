#!/usr/local/bin/ruby -w

require "nbody.rb"

options_text= <<-END

  Description: Takes an N-body system, and gives each body a unique ID
  Long description:
    This program accepts an N-body system, and assigns a number to each
    body consecutively, as an instance variable @body_id which takes on
    integer values.

    (c) 2005, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:
               kali #{$0} -n 0


  Short name: 		-n
  Long name:            --starting_number
  Value type:           int
  Default value:        1
  Description:          value of @body_id for 1st body
  Variable name:        n
  Long description:
    This option allows the user to start numbering the particles at a
    value different from the default value (0, say, or 10, or whatever).


  END

c = parse_command_line(options_text)

class Body
  attr_accessor :body_id
end

nb = ACS_IO.acs_read(NBody)
i = c.n
nb.body.each do |b|
  b.body_id = i
  i += 1
end
nb.acs_write($stdout, false, c.precision)
