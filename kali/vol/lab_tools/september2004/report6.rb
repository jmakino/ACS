#
# report6.rb          2004-9-15          Piet Hut and Michele Trenti
#
# simpler version of report5.rb, to output the density radius.  Together
# with the MSA code nbody_sh1.C, this can be used as, e.g.:
# 
#   ruby mkplummer3.rb -n32 | nbody_sh1 -i -o 2 -t 100 -e 2 | ruby report6.rb
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
    rk = find_distance(body_array, k)          # note: for equal masses only
    volume = ((4*PI)/3)*rk**3
    @density = (k-1)*(mass/volume)
  end

  def find_distance(body_array, k)          # distance to k-th nearest neighbor
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

  attr_accessor :time, :body, :index, :com, :cod, :eps, :r_c, :n_c,
                :energy_array, :epsilon_array, :energy_smoothing_number, :zeta

  def initialize
    @body = []
  end

  def simple_read
    n = gets.to_i
    @time = gets.to_f
    for i in 0...n
      @body[i] = Body.new
      @body[i].simple_read
    end
  end

##-----------------------------------------------------------------------------
## These methods lay the groundwork, before computing the run of the potential:
##-----------------------------------------------------------------------------

  def setup           # perhaps take out setup, and replace by error checking ?
    cube_root_of_k = 2     # default
    k = first_guess_for_k(cube_root_of_k)
    setup_helper(k)
    k = second_guess_for_k(cube_root_of_k)
    setup_helper(k)
    smoothing_length
  end

  def setup_helper(k)
    density(k)
    find_escapers
    radius
    core_radius
    core_number
  end

  def first_guess_for_k(cube_root_of_k)
    k = cube_root_of_k**3
    k = (0.5 * @body.size).to_i if @body.size < 2*k
    if k < 2
      k = 2             # since the density for a body is proportional to (k-1)
      if @body.size < k + 1
        STDERR.print "setup: N = ", @body.size, " < k + 1 = ", k, " + 1\n"
        exit
      end
    end
    return k
  end

  def second_guess_for_k(cube_root_of_k)
    if @n_c < cube_root_of_k**3
      k = @n_c
    else
      k = cube_root_of_k**2 * @n_c**(1/3.0)   # hence the name cube_root_of_k !
    end
    k = 2 if k < 2      # since the density for a body is proportional to (k-1)
    return k
  end

  def density(k)                               # based on k-th nearest neighbor
    @body.each{|b| b.find_density(@body, k)}
  end

  def find_escapers
    set_escaper_flags_to_false
    change = true
    while change
      center_of_density
      recenter(cod.pos, cod.vel)
      change = find_new_escapers?
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

  def center_of_density             # only of non-escapers
    @cod = Body.new
    @cod.mass = 0
    @cod.pos = @cod.vel = @body[0].pos*0    # null vector of the correct length
    @body.each do |b|
      unless b.escaper_flag
        rho = b.density
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

  def radius
    @body.each{|b| b.radius = sqrt(b.pos * b.pos)}
  end

  def core_radius
    @r_c = 0
    tot_density = 0
    @body.each do |b|
      @r_c += b.radius * b.density
      tot_density += b.density
    end
    @r_c = @r_c / tot_density
  end

  def core_number
    @n_c = 0
    @body.each do |b|
      @n_c += 1 if b.radius < @r_c
    end
  end

  def smoothing_length
    @eps = @r_c / @n_c**(1.0/3.0)
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

##-----------------------------------------------------------------------------
## This method is very general, should go into a math package:
##-----------------------------------------------------------------------------

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

##-----------------------------------------------------------------------------
## These methods compute the run of the potential:
##-----------------------------------------------------------------------------

  def soft_kernel(r)                    # softening kernel
    if r < @eps
      raux = r / @eps
      pot = (1.0/@eps)*(1.0+0.5*(1.0-raux**2)+(21.0/16.0)*(1.0-raux**2)**2)
    else
      pot = 1.0 / r
    end
  end

  def soft_potential(location)              # softened potential
    p = 0
    @body.each do |b|
      unless b == b.escaper_flag
        r = b.pos - location
        p += - b.mass * soft_kernel(sqrt(r*r))
      end
    end
    p 
  end

  def central_potential
    location = @body[0].pos*0               # null vector of the correct length
    soft_potential(location)
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
    raux = @body[r_index[0]].radius
    r_pot_offset[0] = -@body[r_index[0]].mass * soft_kernel(raux)
    for i in 1...r_index.size
      raux = @body[r_index[i]].radius
      r_pot_offset[i] =
        r_pot_offset[i-1] - @body[r_index[i]].mass * soft_kernel(raux)
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

  def make_index
    r = []
    @body.each_index do |i|
      r[i] = @body[i].radius
    end
    @index = make_indirect_address_array(r)
  end

  def phi(r)                          # smoothed with K_0 Dehnen kernel
    make_index             # this is necessary before anything else can be done
    k = next_outer_shell_index(r)
    if k == 0
      kepler_part = 0 
    else
      kepler_part = - cumulative_mass[k-1] * soft_kernel(r)
    end
    if k == @body.size
      offset_part = 0
    else
      offset_part = potential_offset[k]
    end
    kepler_part + offset_part
  end

##-----------------------------------------------------------------------------
## these methods compute the distribution function:
##-----------------------------------------------------------------------------

  def setup_f_of_e_helpers(zeta)
    @zeta = zeta
    @energy_smoothing_number = 4                # default
    setup_energy_array
    setup_epsilon_array
  end

  def setup_energy_array
    @energy_array = []
    @body.each_index do |i|
      @energy_array[i] = @body[i].ekin/@body[i].mass + phi(@body[i].radius)
    end
    @energy_array.sort!
  end

  def setup_epsilon_array
    @epsilon_array = []
    @energy_array.each_index do |i|
      @epsilon_array[i] = find_eta(i)
      @epsilon_array[i] = @zeta if @epsilon_array[i] < @zeta
    end
  end

  def find_eta(i)
    k = @energy_smoothing_number
    j_low = i - k
    j_low = 0 if j_low
    j_high = i + k
    j_high = @energy_array.size - 1 if j_high > @energy_array.size - 1
    a = []
    for j in j_low..j_high
      a[j-j_low] = (@energy_array[i] - @energy_array[j]).abs
    end
    a.sort!
    a[k]
  end

  def n_of_E(energy)
    low = lowest_index(energy)
    high = highest_index(energy)
    result = 0
    for i in low..high
      result += n_of_E_contribution(i, energy)
    end
    result
  end
  
  def lowest_index(energy)
    unless energy < 0
      STDERR.print "lowest_index: energy = ", energy, " >= 0\n"
      exit
    end
    i = @energy_array.size - 1;
    while @energy_array[i] > energy
      i -= 1
      break if i == 0
    end
    low_index = i - @energy_smoothing_number
    low_index = 0 if low_index < 0
    while (energy - @energy_array[low_index] < @zeta and low_index > 0)
      low_index -= 1
    end
    low_index
  end

  def highest_index(energy)
    unless energy < 0
      STDERR.print "highest_index: energy = ", energy, " >= 0\n"
      exit
    end
    max_index = @energy_array.size - 1
    while @energy_array[max_index] >= 0
      max_index -= 1
    end
    i = 0
    while @energy_array[i] < energy
      i += 1
      break if i == max_index
    end
    high_index = i + @energy_smoothing_number
    high_index = max_index if high_index > max_index
    while @energy_array[high_index] - energy < @zeta and high_index < max_index
      high_index += 1
    end
    high_index    
  end

  def energy_kernel(x)             # normalized to have surface area unity
    x = x.abs
    return 0 if x >= 1
    if x <= 0.5
      return 1 - 2*x**2
    else
      return 2*(1-x)**2
    end
  end

  def n_of_E_contribution(i, energy)
    eps = @epsilon_array[i]
    x = (energy - @energy_array[i]) / eps
    energy_kernel(x) / eps
  end

##-----------------------------------------------------------------------------
## remaining methods: 
##-----------------------------------------------------------------------------

  def write_report
    print "This is a ", body.size, "-body system,\n"
    print "containing ", body.size - number_of_escapers,
          " bound particles and ", number_of_escapers, " escapers\n"
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

  def find_highest_density                     # based on k-th nearest neighbor
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

end

include Math

while true
  nb = Nbody.new
  nb.simple_read
  nb.setup
  nb.write_report
  print "r_c = ", nb.r_c, "\n"
  print "n_c = ", nb.n_c, "\n"
end
