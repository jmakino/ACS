#!/usr/local/bin/ruby -w

require "nbody.rb"

options_text= <<-END

  Description: 6N-dimensional phase space distance between two N-body systems
  Long description:
    This program accepts two N-body systems, and computes the distance between
    them in the 2DN-dimensional phase space of all positions and velocities,
    if they are given in D spatial dimensions.  The values of the masses of
    the particles are not used, and each particle is given equal weight.  In
    order to compare corresponding particles in the two systems, the body_id
    instance variable of each body is used as an identifier.

    (c) 2005, Piet Hut, Jun Makino; see ACS at www.artcompsi.org


  Short name: 		-r
  Long name:            --positions_only
  Value type:           bool
  Description:          Using only positions, not velocities
  Variable name:        position_only_flag
  Long description:
    This option allows the user to compute the distance between two N-body
    systems using only particle information; velocity information is not used.


  Short name:           -p
  Long name:            --precision
  Value type:           int
  Default value:        2
  Description:          Floating point precision
  Variable name:        precision
  Long description:
    The precision with which the distance between the two N-body systems
    is printed. 


  END

c = parse_command_line(options_text)

class Body
  attr_accessor :body_id
end

nb1 = ACS_IO.acs_read(NBody)
nb2 = ACS_IO.acs_read(NBody)
nb = nb1 - nb2
d = nb.body[0].pos.size
n = nb.body.size
print "#{2*d}N-dim. phase space dist. for two #{n}-body systems:  "
if c.position_only_flag == false
  printf(" %.#{c.precision}e\n", nb.abs)
else
  printf(" %.#{c.precision}e\n", nb.abs_pos)
end
