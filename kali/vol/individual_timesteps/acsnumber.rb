#!/usr/local/bin/ruby -w

require "nbody.rb"

options_text= <<-END

  Description: Takes an N-body system, and numbers all particles
  Long description:
    This program accepts an N-body system, and assigns a number to each
    body, as an instance variable @body_id which takes on integer values.

    (c) 2005, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:
               ruby #{$0} -n 0


  Short name: 		-n
  Long name:            --starting_number
  Value type:           int
  Default value:        1
  Description:          The value of @body_id for the first particle
  Variable name:        n
  Long description:
    This option allows the user to start numbering the particles at a
    value different from the default value (0, say, or 10, or whatever).


  Short name:           -p
  Long name:            --precision
  Value type:           int
  Default value:        16
  Description:          Floating point precision
  Variable name:        precision
  Long description:
    The precision with which floating point numbers are printed in the output.
    The default precision is comparable to double precision accuracy.


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
