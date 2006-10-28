require "vector.rb"
include Math

def acc(r)
  r2 = r*r
  r3 = r2 * sqrt(r2)
  -r/r3
end

def print_pos_vel(r,v)
  r.each{|x| print(x, "  ")}
  v.each{|x| print(x, "  ")}
  print "\n"
end

def print_energy(r,v,e0)
  ekin = 0.5*v*v
  epot = -1/sqrt(r*r)
  etot = ekin + epot
  STDERR.print "  E_kin = ", sprintf("%.3g, ", ekin)
  STDERR.print "E_pot = ", sprintf("%.3g; ", epot)
  STDERR.print "E_tot = ", sprintf("%.3g\n", etot)
  STDERR.print "            E_tot - E_init = ", sprintf("%.3g, ", etot-e0)
  STDERR.print "(E_tot - E_init) / E_init = ", sprintf("%.3g\n", (etot-e0)/e0)
end

r = [1, 0, 0].to_v
v = [0, 0.5, 0].to_v
e0 = 0.5*v*v - 1/sqrt(r*r)
t = 0
STDERR.print "time step =\n"
dt = gets.to_f
STDERR.print "final time =\n"
t_end = gets.to_f
print_pos_vel(r,v)
print_energy(r,v,e0)

a = acc(r)
while t < t_end - 0.5*dt
  v += 0.5*a*dt
  r += v*dt
  a = acc(r)
  v += 0.5*a*dt
  t += dt
  print_pos_vel(r,v)
end
print_energy(r,v,e0)
