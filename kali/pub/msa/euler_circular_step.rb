include Math                              #5

x = 1                                     #1
y = 0                                     #1
z = 0                                     #1
vx = 0                                    #1
vy = 1                                    #1
vz = 0                                    #1
dt = 0.1                                  #2

print(x, "  ", y, "  ", z, "  ")          #7
print(vx, "  ", vy, "  ", vz, "\n")       #7

r = sqrt(x*x + y*y + z*z)                 #4
r3 = r*r*r                                #6
ax = - x / r3                             #6
ay = - y / r3                             #6
az = - z / r3                             #6
x += vx*dt                                #3
y += vy*dt                                #3
z += vz*dt                                #3
vx += ax*dt                               #3
vy += ay*dt                               #3
vz += az*dt                               #3

print(x, "  ", y, "  ", z, "  ")
print(vx, "  ", vy, "  ", vz, "\n")
