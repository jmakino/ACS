#
# report1.rb          2004-8-29          Piet Hut and Michele Trenti
#
# We first determine the center of mass of an N-body system, then
# we determine the escapers, and recompute the c.o.m. of the bound
# particles.  We keep iterating this procedure, until the identity
# of the escapers no longer changes.

require "vector.rb"

class Body

  attr_accessor :mass, :pos, :vel, :escaper_flag, :new_escaper_flag

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def ekin                        # kinetic energy
    0.5*@mass*(@vel*@vel)
  end

  def epot(body_array)                  # potential energy
    p = 0
    body_array.each do |b|
      unless b == self or b.escaper_flag
        r = b.pos - @pos
        p += -@mass*b.mass/sqrt(r*r)
      end
    end
    p
  end

  def simple_read
    @mass = gets.to_f
    @pos = gets.split.map{|x| x.to_f}.to_v
    @vel = gets.split.map{|x| x.to_f}.to_v
  end

  def simple_print
    printf("%24.16e\n", @mass)
    @pos.each{|x| printf("%24.16e", x)}; print "\n"
    @vel.each{|x| printf("%24.16e", x)}; print "\n"
  end

end

class Nbody

  attr_accessor :time, :body, :com

  def initialize
    @body = []
  end

  def ekin                        # kinetic energy
    e = 0
    @body.each{|b| e += b.ekin}
    e
  end

  def epot                        # potential energy
    e = 0
    @body.each{|b| e += b.epot(@body)}
    e/2                           # pairwise potentials were counted twice
  end

  def simple_read
    n = gets.to_i
    @time = gets.to_f
    for i in 0...n
      @body[i] = Body.new
      @body[i].simple_read
    end
  end

  def center_of_mass             # only of non-escapers
    @com = Body.new
    @com.mass = 0
    @com.pos = @com.vel = @body[0].pos*0    # null vector of the correct length
    @body.each do |b|
      unless b.escaper_flag
        @com.mass += b.mass
        @com.pos += b.pos * b.mass
        @com.vel += b.vel * b.mass
      end
    end
    if @com.mass > 0
      @com.pos /= @com.mass
      @com.vel /= @com.mass
    else
      STDERR.print "warning: center_of_mass: "
      STDERR.print "@com.mass = ", @com.mass, " which is <= 0;\n"
      STDERR.print "  (returning a body with zero position and velocity)\n"
    end
      return @com
  end

  def recenter(new_pos, new_vel)
    @body.each do |b|
      b.pos -= new_pos
      b.vel -= new_vel
    end
  end

  def set_escaper_flags_to_false
    @body.each{|b| b.escaper_flag = false}
  end

  def set_new_escaper_flags_to_false
    @body.each{|b| b.new_escaper_flag = false}
  end

  def find_new_escapers?
    set_new_escaper_flags_to_false
    @body.each do |b|
      b.new_escaper_flag = true if b.ekin + b.epot(@body) > 0
    end
    change = change_in_escapers?
    @body.each{|b| b.escaper_flag = b.new_escaper_flag}
    return change
  end

  def change_in_escapers?
    change = false
    @body.each do |b|
      change = true unless b.escaper_flag == b.new_escaper_flag
    end
    return change
  end

  def number_of_escapers
    n = 0
    @body.each do |b|
      n += 1 if b.escaper_flag
    end
    return n
  end

  def find_escapers
    set_escaper_flags_to_false
    change = true
    while change
      center_of_mass
      recenter(com.pos, com.vel)
      change = find_new_escapers?
      write_report
    end
  end

  def write_report
    print "This is a ", body.size, "-body system,\n"
    print "containing ", body.size - number_of_escapers,
          " bound particles and ", number_of_escapers, " escapers\n"
  end

end

include Math

nb = Nbody.new
nb.simple_read
#nb.write_report
nb.find_escapers
