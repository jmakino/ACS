require "vector.rb"
include Math

r = [1, 0, 0].to_v
v = [0, 0.5, 0].to_v
dt = 0.01

r.each{|x| print(x, "  ")}
v.each{|x| print(x, "  ")}
print "\n"

1000.times{
  r2 = r*r
  r3 = r2 * sqrt(r2)
  a = -r/r3
  r1 = r + v*dt
  v1 = v + a*dt
  r12 = r1*r1
  r13 = r12 * sqrt(r12)
  a1 = -r1/r13
  r2 = r1 + v1*dt
  v2 = v1 + a1*dt
  r = ( r + r2 ) / 2.0
  v = ( v + v2 ) / 2.0
  r.each{|x| print(x, "  ")}
  v.each{|x| print(x, "  ")}
  print "\n"
}
