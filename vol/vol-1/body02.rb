##  body02.rb

##<i> Body models a single particle in an N-body system </i>

class Body

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = [0,0,0], vel = [0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.to_s + "\n" +
    "   vel = " + @vel.to_s + "\n"
  end

end
