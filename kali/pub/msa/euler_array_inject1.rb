include Math

def print_pos_vel(r,v)
  [r,v].flatten.each{|x| print(x, "  ")}
  print "\n"
end

r,v = [[1, 0, 0], [0, 0.5, 0]]
dt = 0.01

print_pos_vel(r,v)
1000.times{
  r2 = r.inject(0){|sum, x| sum + x*x}
  r3 = r2 * sqrt(r2)
  a = r.map{|x| -x/r3}
  r.each_index{|k| r[k] += v[k]*dt ; v[k] += a[k]*dt}
  print_pos_vel(r,v)
}
