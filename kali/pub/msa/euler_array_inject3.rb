include Math

def print_pos_vel(r,v)
  [r,v,"\n"].flatten.each{|x| print("  ", x)}
end

r,v,dt = [[1, 0, 0], [0, 0.5, 0], 0.01]

print_pos_vel(r,v)
1000.times{
  r2 = r.inject(0){|sum, x| sum + x*x}
  r3 = r2 * sqrt(r2)
  a = r.map{|x| -x/r3}
  r.each_index{|k| r[k] += v[k]*dt ; v[k] += a[k]*dt}
  print_pos_vel(r,v)
}
