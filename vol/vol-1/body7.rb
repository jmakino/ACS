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
    @pos.each do |x| printf("%24.16e", x) end
    print "\n"
    @vel.each do |x| printf("%24.16e", x) end
    print "\n"
  end

  def simple_read
    @mass = gets.to_f
    s = gets
    a = s.split
    a.collect! { |x| x.to_f }
    @pos = a[0...NDIM]
    s = gets
    a = s.split
    a.collect! { |x| x.to_f }
    @vel = a[0...NDIM]
  end

end
