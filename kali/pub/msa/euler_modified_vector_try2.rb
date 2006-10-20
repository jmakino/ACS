require "vector.rb"
include Math

def print_pos_vel(r,v)
  r.each{|x| print(x, "  ")}
  v.each{|x| print(x, "  ")}
  print "\n"
end

r = [1, 0, 0].to_v
v = [0, 0.5, 0].to_v
dt = 0.01

print_pos_vel(r,v)
1000.times{
  r02 = r*r
  r03 = r02 * sqrt(r02)
  a = -r/r03
  r1 = r + v*dt
  v1 = v + a*dt
  r12 = r1*r1
  r13 = r12 * sqrt(r12)
  a1 = -r1/r13
  r2 = r1 + v1*dt
  v2 = v1 + a1*dt
  r = 0.5 * ( r + r2 )
  v = 0.5 * ( v + v2 )
  print_pos_vel(r,v)
}
