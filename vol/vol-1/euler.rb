require "body.rb"

include Math

dt = 0.01
dt_dia = 1
dt_out = 1
dt_end = 1

STDERR.print "dt = ", dt, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_end = ", dt_end, "\n"

b = Body.new
b.simple_read
b.evolve(dt, dt_dia, dt_out, dt_end)
