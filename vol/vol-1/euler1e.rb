class Body

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = [0,0,0], vel = [0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.join(", ") + "\n" +
    "   vel = " + @vel.join(", ") + "\n"
  end

  def pp               # pretty print
    print to_s
  end

  def simple_print
    printf("%24.16e\n", @mass)
    @pos.each { |x| printf("%24.16e", x) } ; print "\n"
    @vel.each { |x| printf("%24.16e", x) } ; print "\n"
  end

  def simple_read
    @mass = gets.to_f
    @pos = gets.split.map { |x| x.to_f }
    @vel = gets.split.map { |x| x.to_f }
  end

end

include Math

#:segment start: barebones
dt = 0.001            # time step size
ns = 10000             # number of time steps
#:segment end:

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
