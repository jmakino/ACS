require "old_vector.rb"

class Body

  attr_accessor :mass, :pos, :vel, :acc

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
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
    @pos.each{|x| printf("%24.16e", x)}; print "\n"
    @vel.each{|x| printf("%24.16e", x)}; print "\n"
  end

  def simple_read
    @mass = gets.to_f
#:segment start: pos_f_v
    @pos = gets.split.map{|x| x.to_f}.to_v
#:segment end:
    @vel = gets.split.map{|x| x.to_f}.to_v
  end

end
