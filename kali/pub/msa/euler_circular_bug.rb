include Math

x = 1
y = 0
z = 0
vx = 0
vy = 1
vz = 0
dt = 0.01

print(x, "  ", y, "  ", z, "  ")
print(vx, "  ", vy, "  ", vz, "\n")

1000.times{                                     #1
  r2 = x*x + y*y + z*z
  r3 = r2 * sqrt(r2)
  ax = - x / r3
  ay = - y / r3
  az = - z / r3
  x += vx*dt
  y += vy*dt
  z += vz*dt
  vx += ax*dt                                   #2
  vy += ax*dt                                   #2
  vz += az*dt                                   #2
  print(x, "  ", y, "  ", z, "  ")
  print(vx, "  ", vy, "  ", vz, "\n")
}                                               #1
