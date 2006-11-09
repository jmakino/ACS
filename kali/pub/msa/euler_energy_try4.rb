require "vector.rb"
include Math

def energies(r,v)
  ekin = 0.5*v*v
  epot = -1/sqrt(r*r)
  [ekin, epot, ekin+epot]
end

def print_pos_vel_energy(r,v,e0)
  r.each{|x| printf("%.5g  ", x)}
  v.each{|x| printf("%.5g  ", x)}
  etot = energies(r,v).last
  print (etot-e0)/e0
  print "\n"
end

def print_diagnostics(t,r,v,e0)
  ekin, epot, etot = energies(r,v)
  STDERR.print "  t = ", sprintf("%.3g, ", t)
  STDERR.print "  E_kin = ", sprintf("%.3g, ", ekin)
  STDERR.print "E_pot = ", sprintf("%.3g; ", epot)
  STDERR.print "E_tot = ", sprintf("%.3g\n", etot)
  STDERR.print "            E_tot - E_init = ", sprintf("%.3g, ", etot-e0)
  STDERR.print "(E_tot - E_init) / E_init = ", sprintf("%.3g\n", (etot-e0)/e0)
end

r = [1, 0, 0].to_v
v = [0, 0.5, 0].to_v
e0 = energies(r,v).last
t = 0
STDERR.print "time step = ?\n"
dt = gets.to_f
STDERR.print "final time = ?\n"
t_end = gets.to_f
print_pos_vel_energy(r,v,e0)
print_diagnostics(t,r,v,e0)

while t < t_end
  r2 = r*r
  r3 = r2 * sqrt(r2)
  a = -r/r3
  r += v*dt
  v += a*dt
  t += dt
  print_pos_vel_energy(r,v,e0)
end
print_diagnostics(t,r,v,e0)
