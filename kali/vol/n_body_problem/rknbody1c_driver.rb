require "rknbody1.rb"

include Math

dt = 0.0001          # time step
dt_dia = 1           # diagnostics printing interval
dt_out = 1           # output interval
dt_end = 1           # duration of the integration
method = "forward"   # integration method

STDERR.print "dt = ", dt, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_end = ", dt_end, "\n",
      "method = ", method, "\n"

nb = Nbody.new
nb.simple_read
nb.evolve(method, dt, dt_dia, dt_out, dt_end)
