##  body04.rb

##<i> Body models a single particle in an N-body system </i>

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

  def pp            # pretty print
    print to_s
  end

end

b = Body.new(3, [0.1, 0.2, 0.3], [4, 5, 6])

b.pp
