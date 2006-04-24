include Math

r,v = [[1, 0, 0], [0, 0.5, 0]]
dt = 0.01

[r,v].flatten.each{|x| print(x, "  ")}

1000.times{
  r2 = r.inject(0){|sum, x| sum + x*x}
  r3 = r2 * sqrt(r2)
  acc = r.map{|x| -x/r3}
  3.times{|k| r[k] += v[k]*dt ; v[k] += acc[k]*dt}
  [r,v].flatten.each{|x| print(x, "  ")}
  print "\n"
}
