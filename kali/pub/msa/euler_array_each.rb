include Math

r = [1, 0, 0]
v = [0, 0.5, 0]
dt = 0.01

r.each{|x| print(x, "  ")}               #1
v.each{|x| print(x, "  ")}
print "\n"

1000.times{
  r2 = 0
  r.each{|x| r2 += x*x}
  r3 = r2 * sqrt(r2)
  a = r.map{|x| -x/r3}                   #2
  r.each_index{|k| r[k] += v[k]*dt}      #3
  v.each_index{|k| v[k] += a[k]*dt}
  r.each{|x| print(x, "  ")}
  v.each{|x| print(x, "  ")}
  print "\n"
}
