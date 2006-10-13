include Math

x = 1
y = 0
z = 0
vx = 0
vy = 0.5
vz = 0
dt = 0.00001

m1 = 0.6                                                        #4
m2 = 1 - m1                                                     #4

def print2(m1,m2,x,y,z,vx,vy,vz)                                #1
  mfrac1 = m1/(m1+m2)
  mfrac2 = m2/(m1+m2)
  print(-mfrac2*x, "  ", -mfrac2*y, "  ", -mfrac2*z, "  ")      #1
  print(-mfrac2*vx, "  ", -mfrac2*vy, "  ", -mfrac2*vz, "\n")   #1
  print(mfrac1*x, "  ", mfrac1*y, "  ", mfrac1*z, "  ")         #1
  print(mfrac1*vx, "  ", mfrac1*vy, "  ", mfrac1*vz, "\n")      #1
end                                                             #1

print2(m1,m2,x,y,z,vx,vy,vz)                                    #2
1000000.times{|i|                                               #2
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
  print2(m1,m2,x,y,z,vx,vy,vz) if i%1000 == 999                 #3
}
