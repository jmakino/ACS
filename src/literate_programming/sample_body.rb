class Body

  NDIM = 3

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

  def simple_print
    printf("%22.15e", @mass)
    @pos.each do |x| printf("%23.15e", x) end
    @vel.each do |x| printf("%23.15e", x) end
    print "\n"
  end

  def simple_read
    s = gets
    a = s.split
    a.collect! { |x| x.to_f }
    @mass = a[0]
    @pos = a[1..NDIM]
    @vel = a[(NDIM+1)..(NDIM*2)]
  end

end
