require "rk6body.rb"

include Math

#:segment start: barebones
dt = 0.1          # time step
dt_dia = 0.5          # diagnostics printing interval
dt_out = 0.5        # output interval
dt_end = 0.5         # duration of the integration
method = "rk6"       # integration method
#:segment end:

STDERR.print "dt = ", dt, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_end = ", dt_end, "\n",
      "method = ", method, "\n"

b = Body.new
b.simple_read
b.evolve(method, dt, dt_dia, dt_out, dt_end)
