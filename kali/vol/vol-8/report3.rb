#
# report3.rb          2004-9-1          Piet Hut and Michele Trenti
#
# In addition to what we have done for report2.rb, we now determine
# information about the potential.
# 
# 
# 
# 

require "vector.rb"

class Body

  attr_accessor :mass, :pos, :vel, :radius,
                :escaper_flag, :new_escaper_flag, :density

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

  def find_density(body_array, k)              # based on k-th nearest neighbor
    rk = find_distance_1(body_array, k)        # note: for equal masses only
    volume = ((4*PI)/3)*rk**3
    @density = (k-1)*(mass/volume)
  end

  def find_distance_1(body_array, k)        # distance to k-th nearest neighbor
    distance_sq = []
    body_array.each_index do |i|
      r = body_array[i].pos - @pos
      distance_sq[i] = r*r
    end
    distance_sq.sort!
    sqrt(distance_sq[k])
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

  attr_accessor :time, :body, :index, :com, :cod

  def initialize
    @body = []
  end

  def radius
    @body.each{|b| b.radius = sqrt(b.pos * b.pos)}
  end

  def ekin                        # total kinetic energy
    e = 0
    @body.each{|b| e += b.ekin}
    e
  end

  def epot                        # total potential energy
    e = 0
    @body.each{|b| e += b.epot(@body)}
    e/2                           # pairwise potentials were counted twice
  end

  def soft_kernel(r, eps)            # softening kernel
    if r < eps
      raux = r / eps
      pot = (1.0/eps)*(1.0+0.5*(1.0-raux**2)+(21.0/16.0)*(1.0-raux**2)**2)
    else
      pot = 1.0 / r
    end
  end

  def soft_potential(location, eps)         # softened potential
    p = 0
    @body.each do |b|
      unless b == b.escaper_flag
        r = b.pos - location
        p += - b.mass * soft_kernel(sqrt(r*r), eps)
      end
    end
    p 
  end

  def central_potential
    location = @body[0].pos*0               # null vector of the correct length
    eps = 1.0 / @body.size**(1.0/3.0)
    soft_potential(location, eps)
  end

  # produce an indirect addressing array ind_add[] which will allow traversing 
  # the array a[] in monotonically increasing order of the values of a[], i.e.:
  # for i < j, a[ind_add[i]] <= a[ind_add[j]] ; in other words, the new array
  # ordered_a[i] = a[ind_add[i]] is an ordered version of the array a[]
  #
  def make_indirect_address_array(a)     
    aux_a = []              
    a.each_index{|i| aux_a[i] = [i,a[i]] }
    aux_a.sort!{|x,y| x[1] <=> y[1]}
    ind_add = []
    aux_a.each_index{|i| ind_add[i] = aux_a[i][0]}
    ind_add
  end

  def make_index
    radius            # computes the radius for each particle
    r = []
    @body.each_index do |i|
      r[i] = @body[i].radius
    end
    @index = make_indirect_address_array(r)
  end

  def cumulative_mass                   # using the ordered index
    cum_mass = []
    cum_mass[0] = @body[@index[0]].mass
    for i in 1...@index.size
      cum_mass[i] = cum_mass[i-1] + @body[@index[i]].mass
    end
    cum_mass
  end

  # potential_offset[i] is the contribution to the potential at the position
  # of the ith particle (ordered in radius) stemming from all particles with
  # a distance to the center larger than the ith particle.
  # In other words, it is the difference between the potential at that point
  # and the energy per unit mass needed to escape from the gravitational
  # influence of only the cumulative mass at that point.
  #
  def potential_offset                          # unsmoothed for now
    r_pot_offset = []
    r_index = @index.reverse
    r_pot_offset[0] = -@body[r_index[0]].mass / @body[r_index[0]].radius
    for i in 1...r_index.size
      r_pot_offset[i] =
        r_pot_offset[i-1] - @body[r_index[i]].mass / @body[r_index[i]].radius
    end
    r_pot_offset.reverse
  end

  # the N shells representing the 3D-isotropized positions of the N particles,
  # are indexed from the inside out by index 0, 1, 2, . . . N-2, N-1.
  # The following method returns the index of the closest shell just outside
  # the radial position r.  If r is larger than the radius of the outermost
  # shell, the method returns the value N.
  # To sum up: there are N+1 possible return values, in the range
  # 0, 1, 2, . . . , N-1, N
  #
  def next_outer_shell_index(r)
    k = @body.size
    @index.each_index do |i|
      if r < @body[index[i]].radius
        k = i
        break
      end
    end
    k
  end

  def phi(r)                          # unsmoothed for now
    make_index             # this is necessary before anything else can be done
    k = next_outer_shell_index(r)
    if k == 0
      kepler_part = 0 
    else
      kepler_part = - cumulative_mass[k-1] / r
    end
    if k == @body.size
      offset_part = 0
    else
      offset_part = potential_offset[k]
    end
    kepler_part + offset_part
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
      @com
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
      recenter(cod.pos, cod.vel)
      change = find_new_escapers?
      write_report
    end
  end

  def write_report
    print "This is a ", body.size, "-body system,\n"
    print "containing ", body.size - number_of_escapers,
          " bound particles and ", number_of_escapers, " escapers\n"
  end

  def report_com_cod_etc_pos_distances(k)
    com_pos = center_of_mass.pos
    cod_pos = center_of_density(k).pos
    hid_pos = find_highest_density(k).pos
    print "|com_pos| = ", sqrt(com_pos * com_pos), "\n"
    print "|cod_pos| = ", sqrt(cod_pos * cod_pos), "\n"
    print "|hid_pos| = ", sqrt(hid_pos * hid_pos), "\n"
    dist = com_pos - cod_pos
    print "|com_pos - cod_pos| = ", sqrt(dist * dist), "\n"
    dist = hid_pos - cod_pos
    print "|hid_pos - cod_pos| = ", sqrt(dist * dist), "\n"
    dist = hid_pos - com_pos
    print "|hid_pos - com_pos| = ", sqrt(dist * dist), "\n"
  end

  def report_com_cod_etc_vel_distances(k)
    com_vel = center_of_mass.vel
    cod_vel = center_of_density(k).vel
    hid_vel = find_highest_density(k).vel
    print "|com_vel| = ", sqrt(com_vel * com_vel), "\n"
    print "|cod_vel| = ", sqrt(cod_vel * cod_vel), "\n"
    print "|hid_vel| = ", sqrt(hid_vel * hid_vel), "\n"
    dist = com_vel - cod_vel
    print "|com_vel - cod_vel| = ", sqrt(dist * dist), "\n"
    dist = hid_vel - cod_vel
    print "|hid_vel - cod_vel| = ", sqrt(dist * dist), "\n"
    dist = hid_vel - com_vel
    print "|hid_vel - com_vel| = ", sqrt(dist * dist), "\n"
  end

end

include Math

nb = Nbody.new
nb.simple_read
#nb.write_report
#k = 5
#nb.find_escapers(k)
#nb.report_com_cod_etc_pos_distances(k)
#p nb.central_potential
  r = 0.0001
  pot = nb.phi(r)
  print r, "  ", pot, "\n"
for i in 1..300
  r = i/100.0
  pot = nb.phi(r)
  print r, "  ", pot, "\n"
end
