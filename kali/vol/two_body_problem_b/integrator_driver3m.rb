require "yo8body.rb"

include Math

#:segment start: barebones
dt = 0.01            # time step
dt_dia = 0.2         # diagnostics printing interval
dt_out = 0.2         # output interval
dt_end = 0.2         # duration of the integration
method = "yo8"       # integration method
#:segment end:

STDERR.print "dt = ", dt, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_end = ", dt_end, "\n",
      "method = ", method, "\n"

b = Body.new
b.simple_read
b.evolve(method, dt, dt_dia, dt_out, dt_end)
