require "nbody.rb"

class Body

  attr_accessor :escaper_flag, :new_escaper_flag, :density

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

  def find_density(body_array, k)              # based on k-th nearest neighbor
    rk = find_distance(body_array, k)          # note: for equal masses only
    volume = ((4*PI)/3)*rk**3
    @density = (k-1)*(mass/volume)
  end

  def find_distance(body_array, k)        # distance to k-th nearest neighbor
    distance_sq = []
    body_array.each_index do |i|
      r = body_array[i].pos - @pos
      distance_sq[i] = r*r
    end
    distance_sq.sort!
    sqrt(distance_sq[k])
  end

end

class Nbody

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

  def find_density(k)                        # based on k-th nearest neighbor
    @body.each{|b| b.find_density(@body, k)}
  end

  def find_highest_density(k)                  # based on k-th nearest neighbor
    find_density(k)
    highest_density = 0
    body_with_highest_density = nil
    @body.each do |b|
      if b.density > highest_density
        highest_density = b.density
        body_with_highest_density = b
      end
    end
    body_with_highest_density
  end

  def center_of_density(k)             # only of non-escapers
    @cod = Body.new
    @cod.mass = 0
    @cod.pos = @cod.vel = @body[0].pos*0    # null vector of the correct length
    @body.each do |b|
      unless b.escaper_flag
        rho = b.find_density(@body, k)
        @cod.mass += rho                    # pseudo-mass, really, defined
        @cod.pos += b.pos * rho             #   in terms of density !
        @cod.vel += b.vel * rho
      end
    end
    if @cod.mass > 0
      @cod.pos /= @cod.mass
      @cod.vel /= @cod.mass
    else
      STDERR.print "warning: center_of_density: "
      STDERR.print "@cod.mass = ", @cod.mass, " which is <= 0;\n"
      STDERR.print "  (returning a body with zero position and velocity)\n"
    end
      @cod
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
    change
  end

  def change_in_escapers?
    change = false
    @body.each do |b|
      change = true unless b.escaper_flag == b.new_escaper_flag
    end
    change
  end

  def number_of_escapers
    n = 0
    @body.each do |b|
      n += 1 if b.escaper_flag
    end
    n
  end

  def find_escapers(k)
    set_escaper_flags_to_false
    change = true
    while change
      center_of_density(k)
      recenter(@cod.pos, @cod.vel)
      change = find_new_escapers?
    end
  end

  def report_escapers(clop)
    find_escapers(clop.n_neighbor)
    STDERR.printf("  time = %8.3f :    ", @time) if @time   # to be improved <==
    ne = number_of_escapers
    STDERR.print body.size - ne, " bound particles and ", ne, " escaper"
    if ne == 1
      STDERR.print "\n"
    else
      STDERR.print "s\n"
    end
#    STDERR.printf("At time = %8.3f :\n", @time) if @time   # to be improved <==
#    STDERR.print "  This is a ", body.size, "-body system,\n"
#    STDERR.print "  containing ", body.size - number_of_escapers,
#          " bound particles and ", number_of_escapers, " escapers\n"
  end

end

options_text= <<-END

  Description: Find and report escaping stars
  Long description:
    This program accepts Nbody snapshots, and returns information about the
    escpaing stars (gravitationally unboud to the rest of the system) on the
    stderr output channel.  Escapers are determined by iteration: after
    excluding escapers from consideration, the binding energy of each
    remaining star to the rest of the remaining system is calculated, to
    determine whether there are any other escapers.  In each iteration, the
    restframe is determined by the position and motion of the density center,
    defined as the density weighted average position and velocity of all
    particles.

    NOTE: FOR NOW, THIS PROGRAM ONLY WORKS FOR EQUAL-MASS STARS (soon to be
    extended to general masses).

    This program also echoes each original snapshot on the stdout output 
    channel, so that it so that it will be available for another diagnostics
    program.

    (c) 2004, Piet Hut, Jun Makino, Michelle Trenti;
    see ACS at www.artcompsi.org

    example:
               ruby mkplummer.rb -n 3 | ruby world1.rb | ruby #{$0}


  Short name: 		-k
  Long name:            --density_neighbor_number
  Value type:           int
  Default value:        6
  Description:          Neighbor # for density calculation
  Variable name:        n_neighbor
  Long description:
    This option allows the user to set the number of neighbors used to
    calculate the density around each particle.  These density values are
    then used to determine the rest frame for the center of density, in which
    the escapers are being calculated.


  END

clop = parse_command_line(options_text)

nb = ACS_IO.acs_read
while nb
  raise "class #{nb.class} is not Nbody" unless nb.class == Nbody
  nb.report_escapers(clop)
  nb.acs_write
  nb = ACS_IO.acs_read
end
