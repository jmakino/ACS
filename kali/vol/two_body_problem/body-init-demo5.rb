#<i>Demo for +initialize+ method</i>

require "body.rb"

c = Body.new(1, [0.5, 0, 0], [0, 0.7, 0])
#:segment start: demo
c.vel = [0, 0.8, 0]
p c.vel
#:segment end:
