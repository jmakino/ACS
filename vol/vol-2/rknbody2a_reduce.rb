require "rknbody2.rb"

include Math

nb = Nbody.new
nb.simple_read

relative_position = nb.body[0].pos - nb.body[1].pos
relative_velocity = nb.body[0].vel - nb.body[1].vel

print "relative_position = [", relative_position[0], ", ",
      relative_position[1], "]\n"
print "relative_velocity = [", relative_velocity[0], ", ",
      relative_velocity[1], "]\n"
