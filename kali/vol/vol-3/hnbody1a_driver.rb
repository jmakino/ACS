require "hnbody1.rb"

include Math

eps = 0
dt = 0.01            # time step
dt_dia = 2.1088      # diagnostics printing interval
dt_out = 2.1088      # output interval
dt_end = 2.1088      # duration of the integration
init_out = false     # initial output requested ?
x_flag = false       # extra diagnostics requested ?
method = "hermite"   # integration method

STDERR.print "eps = ", eps, "\n",
      "dt = ", dt, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_end = ", dt_end, "\n",
      "init_out = ", init_out, "\n",
      "x_flag = ", x_flag, "\n",
      "method = ", method, "\n"

nb = Nbody.new
nb.simple_read
nb.evolve(method, eps, dt, dt_dia, dt_out, dt_end, init_out, x_flag)
