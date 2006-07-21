include Math

r = [1, 0, 0]
v = [0, 0.5, 0]
dt = 0.01

r.each{|x| print(x, "  ")}
v.each{|x| print(x, "  ")}
print "\n"

1000.times{
  r2 = 0
  r.each{|x| r2 += x*x}
  r3 = r2 * sqrt(r2)
  a = r.map{|x| -x/r3}
  r1 = r.map{|x| x}
  r1.each_index{|k| r1[k] = r[k] + v[k]*dt}
  v1 = r.map{|x| x}
  v1.each_index{|k| v1[k] = v[k] + a[k]*dt}
  r = r1.map{|x| x}
  v = v1.map{|x| x}
  r.each{|x| print(x, "  ")}
  v.each{|x| print(x, "  ")}
  print "\n"
}
