include Math

def print_pos_vel(r,v)
  r.each{|x| print(x, "  ")}
  v.each{|x| print(x, "  ")}
  print "\n"
end

r = [1, 0, 0]
v = [0, 0.5, 0]
dt = 0.0001

100000.times{
  r2 = 0
  r.each{|x| r2 += x*x}
  r3 = r2 * sqrt(r2)
  a = r.map{|x| -x/r3}
  r.each_index{|k| r[k] += v[k]*dt}
  v.each_index{|k| v[k] += a[k]*dt}
}
print_pos_vel(r,v)
