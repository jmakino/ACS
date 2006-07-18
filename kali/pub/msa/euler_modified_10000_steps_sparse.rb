include Math

x = 1
y = 0
z = 0
vx = 0
vy = 0.5
vz = 0
dt = 0.001

print(x, "  ", y, "  ", z, "  ")
print(vx, "  ", vy, "  ", vz, "\n")

10000.times{|i|
  r2 = x*x + y*y + z*z
  r3 = r2 * sqrt(r2)
  ax = - x / r3
  ay = - y / r3
  az = - z / r3
  x1 = x + vx*dt
  y1 = y + vy*dt
  z1 = z + vz*dt
  vx1 = vx + ax*dt
  vy1 = vy + ay*dt
  vz1 = vz + az*dt
  r12 = x1*x1 + y1*y1 + z1*z1
  r13 = r12 * sqrt(r12)
  ax1 = - x1 / r13
  ay1 = - y1 / r13
  az1 = - z1 / r13
  x2 = x1 + vx1*dt
  y2 = y1 + vy1*dt
  z2 = z1 + vz1*dt
  vx2 = vx1 + ax1*dt
  vy2 = vy1 + ay1*dt
  vz2 = vz1 + az1*dt
  x = 0.5 * (x + x2)
  y = 0.5 * (y + y2)
  z = 0.5 * (z + z2)
  vx = 0.5 * (vx + vx2)
  vy = 0.5 * (vy + vy2)
  vz = 0.5 * (vz + vz2)
  if i%10 == 9
    print(x, "  ", y, "  ", z, "  ")
    print(vx, "  ", vy, "  ", vz, "\n")
  end
}
