require "vbody1.rb"                                                          #1

include Math

dt = 0.01            # time step size
ns = 100             # number of time steps

b = Body.new
b.simple_read

ns.times do
  r2 = b.pos * b.pos
  r3 = r2 * sqrt(r2)
  acc = b.pos * (-b.mass / r3)
  b.pos += b.vel * dt
  b.vel += acc * dt
end

b.pp
