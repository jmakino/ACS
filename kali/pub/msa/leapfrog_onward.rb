require "vector.rb"
include Math

def print_pos_vel(r,v)
  r.each{|x| print(x, "  ")}
  v.each{|x| print(x, "  ")}
  print "\n"
end

def acc(r)
  r2 = r*r
  r3 = r2 * sqrt(r2)
  -r/r3
end

r = [1, 0, 0].to_v
v = [0, 0.5, 0].to_v
dt = 0.01
print_pos_vel(r,v)

a = acc(r)

10000.times{
  v += 0.5*a*dt
  r += v*dt
  a = acc(r)
  v += 0.5*a*dt
}

v = -v                  #1
10000.times{
  v += 0.5*a*dt
  r += v*dt
  a = acc(r)
  v += 0.5*a*dt
  print_pos_vel(r,v)
}

