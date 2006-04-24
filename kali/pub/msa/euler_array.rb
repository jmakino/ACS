include Math

r = [1, 0, 0]
v = [0, 0.5, 0]
dt = 0.01

print(r[0], "  ", r[1], "  ", r[2], "  ")
print(v[0], "  ", v[1], "  ", v[2], "\n")

1000.times{
  r2 = r[0]*r[0] + r[1]*r[1] + r[2]*r[2]
  r3 = r2 * sqrt(r2)
  acc[0] = - r[0] / r3
  acc[1] = - r[1] / r3
  acc[2] = - r[2] / r3
  r[0] += v[0]*dt
  r[1] += v[1]*dt
  r[1] += v[2]*dt
  v[0] += acc[0]*dt
  v[1] += acc[1]*dt
  v[2] += acc[2]*dt
  print(r[0], "  ", r[1], "  ", r[2], "  ")
  print(v[0], "  ", v[1], "  ", v[2], "\n")
}
