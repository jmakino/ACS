require "vector.rb"
include Math

def print_pos_vel(r,v)
  r.each{|x| print(x, "  ")}
  v.each{|x| print(x, "  ")}
  print "\n"
end

def step_pos_vel(r,v,dt)
  r2 = r*r
  r3 = r2 * sqrt(r2)
  a = -r/r3
  [r + v*dt, v + a*dt]
end

r = [1, 0, 0].to_v
v = [0, 0.5, 0].to_v
dt = 0.01
print_pos_vel(r,v)

#:segment start: loop
1000.times{
  r1, v1 = step_pos_vel(r,v,dt)
  r2, v2 = step_pos_vel(r1,v1,dt)
  r = 0.5 * ( r + r2 )
  v = 0.5 * ( v + v2 )
  print_pos_vel(r,v)
}
#:segment end:
