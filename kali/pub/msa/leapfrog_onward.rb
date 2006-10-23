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

10000.times{            #1
  v += 0.5*a*dt         #1
  r += v*dt             #1
  a = acc(r)            #1
  v += 0.5*a*dt         #1
}                       #1
                        #1
v = -v                  #1
10000.times{            #1
  v += 0.5*a*dt         #1
  r += v*dt             #1
  a = acc(r)            #1
  v += 0.5*a*dt         #1
  print_pos_vel(r,v)    #1
}                       #1

