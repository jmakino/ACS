include Math

x = 1
y = 0
z = 0
vx = 0
vy = 0.5
vz = 0
dt = 0.01

print(x, "  ", y, "  ", z, "  ")
print(vx, "  ", vy, "  ", vz, "\n")

r2 = x*x + y*y + z*z
r3 = r2 * sqrt(r2)
ax = - x / r3
ay = - y / r3
az = - z / r3

1000.times{
  vx += 0.5*ax*dt
  vy += 0.5*ay*dt
  vz += 0.5*az*dt
  x += vx*dt
  y += vy*dt
  z += vz*dt
  r2 = x*x + y*y + z*z
  r3 = r2 * sqrt(r2)
  ax = - x / r3
  ay = - y / r3
  az = - z / r3
  vx += 0.5*ax*dt
  vy += 0.5*ay*dt
  vz += 0.5*az*dt
  print(x, "  ", y, "  ", z, "  ")
  print(vx, "  ", vy, "  ", vz, "\n")
}


