# mnbody_driver.rb: driver for mnbody.rb, which is a
# message passing toy model N-body extension of rkbody.rb
# Douglas and Piet, 2004/4/8-9.

require "mnbody.rb"

include Math

eta = 0.0001          # accuracy parameter
#dt_dia = 1           # diagnostics printing interval
#dt_out = 1           # output interval
t_end = 2.1088        # time of termination of the integration
#method = "forward"   # integration method
method = "leapfrog"  # integration method
#method = "rk2"       # integration method
#method = "rk4"       # integration method
history_depth = 5    # number of previous {pos,vel,acc,...} remembered

STDERR.print "eta = ", eta, "\n",
      "dt_end = ", t_end, "\n",
      "method = ", method, "\n"

nb = Nbody.new(method, eta, history_depth)
nb.read
nb.write(t_end)
nb.write_diagnostics(t_end)

# print "\n" ; nb.print_history
