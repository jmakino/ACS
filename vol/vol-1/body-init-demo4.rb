#<i>Demo for +initialize+ method</i>

require "body.rb"

c = Body.new(1, [0.5, 0, 0], [0, 0.7, 0])
#:segment start: demo
p c.mass
p c.pos[0]
c.vel[1] = 0.8
p c.vel
#:segment end:
