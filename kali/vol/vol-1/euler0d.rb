require "body.rb"

include Math

#:segment start: barebones
dt = 0.01           # time step
dt_dia = 2.5        # diagnostics printing interval
dt_out = 20         # output interval
dt_end = 10         # duration of the integration
#:segment end:

STDERR.print "dt = ", dt, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_end = ", dt_end, "\n"

b = Body.new
b.simple_read
b.evolve(dt, dt_dia, dt_out, dt_end)
