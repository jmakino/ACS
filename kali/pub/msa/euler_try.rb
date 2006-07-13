include Math                                #7

x = 1                                       #1
y = 0                                       #1
z = 0                                       #1
vx = 0                                      #1
vy = 0.5                                    #1
vz = 0                                      #1
dt = 0.01                                   #2

print(x, "  ", y, "  ", z, "  ")
print(vx, "  ", vy, "  ", vz, "\n")

1000.times{                                 #3
  r2 = x*x + y*y + z*z                      #6
  r3 = r2 * sqrt(r2)                        #8
  ax = - x / r3                             #8
  ay = - y / r3                             #8
  az = - z / r3                             #8
  x += vx*dt                                #4
  y += vy*dt                                #4
  z += vz*dt                                #4
  vx += ax*dt                               #5
  vy += ax*dt                               #5
  vz += az*dt                               #5
  print(x, "  ", y, "  ", z, "  ")          #9
  print(vx, "  ", vy, "  ", vz, "\n")       #9
}                                           #3
