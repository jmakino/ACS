#<i> Body models a single particle in an N-body system </i>

#:segment start: barebones
class Body

  NDIM = 3

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = [0,0,0], vel = [0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end
#
#end
#:segment end:

#:segment start: to_s
  def to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.join(", ") + "\n" +
    "   vel = " + @vel.join(", ") + "\n"
  end
#:segment end:

#:segment start: pp
  def pp            # pretty print
    print to_s
  end
#:segment end:

#:segment start: simple-IO
  def simple_print
    printf("%22.15e\n", @mass)
    @pos.each do |x| printf("%23.15e", x) end
    print "\n"
    @vel.each do |x| printf("%23.15e", x) end
    print "\n"
  end

  def simple_read
    @mass = gets
    s = gets
    a = s.split
    @pos = a[0...NDIM]
    s = gets
    a = s.split
    @vel = a[0...NDIM]
  end
#:segment end:

end
