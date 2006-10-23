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
  r0 = r
  v0 = v

  r02 = r0*r0
  r03 = r02 * sqrt(r02)
  a0 = -r0/r03
  r1 = r0 + v0*dt
  v1 = v0 + a0*dt

  r12 = r1*r1
  r13 = r12 * sqrt(r12)
  a1 = -r1/r13
  r2 = r1 + v1*dt
  v2 = v1 + a1*dt

  r = 0.5 * ( r0 + r2 )
  v = 0.5 * ( v0 + v2 )
  print_pos_vel(r,v)
}
