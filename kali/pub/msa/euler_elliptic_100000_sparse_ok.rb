include Math

x = 1
y = 0
z = 0
vx = 0
vy = 0.5
vz = 0
dt = 0.00001

print(x, "  ", y, "  ", z, "  ")
print(vx, "  ", vy, "  ", vz, "\n")

1000000.times{|i|
  r2 = x*x + y*y + z*z
  r3 = r2 * sqrt(r2)
  ax = - x / r3
  ay = - y / r3
  az = - z / r3
  x += vx*dt
  y += vy*dt
  z += vz*dt
  vx += ax*dt
  vy += ay*dt
  vz += az*dt
  if i%1000 == 999
    print(x, "  ", y, "  ", z, "  ")
    print(vx, "  ", vy, "  ", vz, "\n")
  end
}
