require "rknbody1.rb"

include Math

dt = 0.01          # time step
dt_dia = 0.01      # diagnostics printing interval
dt_out = 0.01      # output interval
dt_end = 0.01      # duration of the integration
method = "rk4"     # integration method

STDERR.print "dt = ", dt, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_end = ", dt_end, "\n",
      "method = ", method, "\n"

nb = Nbody.new
nb.simple_read
nb.evolve(method, dt, dt_dia, dt_out, dt_end)
