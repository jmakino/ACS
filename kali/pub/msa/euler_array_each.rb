include Math

r = [1, 0, 0]
v = [0, 0.5, 0]
dt = 0.01

1000.times{
  r2 = 0
  r.each{|x| r2 += x*x}
  r3 = r2 * sqrt(r2)
  acc = r.map{|x| -x/r3}
  3.times{|k| r[k] += v[k]*dt ; v[k] += acc[k]*dt}
  r.each{|x| print(x, "  ")}
  v.each{|x| print(x, "  ")}
  print "\n"
}
