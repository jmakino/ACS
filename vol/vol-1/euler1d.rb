require "body.rb"

include Math

dt = 0.01            # time step size
ns = 1000             # number of time steps

b = Body.new
b.simple_read

ns.times do
  r2 = 0
  b.pos.each {|p| r2 += p*p}
  r3 = r2 * sqrt(r2)
  acc = b.pos.map { |x| -b.mass * x/r3 }
  b.pos.each_index { |k| b.pos[k] += b.vel[k] * dt }
  b.vel.each_index { |k| b.vel[k] += acc[k] * dt }
end

b.pp
