require "ms2body.rb"

include Math

#:segment start: barebones
dt = 0.001           # time step
dt_dia = 0.1         # diagnostics printing interval
dt_out = 0.1         # output interval
dt_end = 0.1         # duration of the integration
method = "ms2"       # integration method
#:segment end:

STDERR.print "dt = ", dt, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_end = ", dt_end, "\n",
      "method = ", method, "\n"

b = Body.new
b.simple_read
b.evolve(method, dt, dt_dia, dt_out, dt_end)
