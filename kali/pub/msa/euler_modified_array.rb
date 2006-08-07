include Math

def print_pos_vel(r,v)
  r.each{|x| print(x, "  ")}
  v.each{|x| print(x, "  ")}
  print "\n"
end

r = [1, 0, 0]
v = [0, 0.5, 0]
dt = 0.01

print_pos_vel(r,v)
1000.times{
  r2 = 0
  r.each{|x| r2 += x*x}
  r3 = r2 * sqrt(r2)
  a = r.map{|x| -x/r3}
  r1 = []                                              #1
  r.each_index{|k| r1[k] = r[k] + v[k]*dt}             #1
  v1 = []
  v.each_index{|k| v1[k] = v[k] + a[k]*dt}
  r12 = 0
  r1.each{|x| r12 += x*x}
  r13 = r12 * sqrt(r12)
  a1 = r1.map{|x| -x/r13}
  r2 = []
  r1.each_index{|k| r2[k] = r1[k] + v1[k]*dt}
  v2 = []
  v1.each_index{|k| v2[k] = v1[k] + a1[k]*dt}
  r.each_index{|k| r[k] = 0.5 * ( r[k] + r2[k] )}
  v.each_index{|k| v[k] = 0.5 * ( v[k] + v2[k] )}
  print_pos_vel(r,v)
}
