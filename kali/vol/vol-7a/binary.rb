require "nbody.rb"

class Binary < Body

  def initialize(body1, body2, init_time = 0)
    @init_time = init_time
    @m1 = body1.mass
    @m2 = body2.mass
    @mass = @m1 + @m2
    @reduced_mass = ( @m1 * @m2 ) / ( @mass )
    @pos = (@m1*body1.pos + @m2*body2.pos)/@mass
    @vel = (@m1*body1.vel + @m2*body2.vel)/@mass
    @rel_pos = body2.pos - body1.pos
    @rel_vel = body2.vel - body1.vel
  end

  def rel_kinetic_energy
    0.5 * @reduced_mass * @rel_vel * @rel_vel
  end

  def rel_potential_energy
    -( @m1 * @m2 / sqrt( @rel_pos * @rel_pos ) )
  end

  def rel_energy
    rel_kinetic_energy + rel_potential_energy
  end

  def angular_momentum_squared
    r_cross_v = @rel_pos.cross(@rel_vel)
    @reduced_mass**2 * r_cross_v * r_cross_v
  end

  def semi_major_axis
    -( @m1 * @m2 ) / ( 2 * rel_energy )
  end

  def eccentricity
    e_sq = 1 - angular_momentum_squared /
                 ( @reduced_mass * @m1 * @m2 * semi_major_axis )
    e_sq = 0.0 if e_sq < 0.0  # to avoid round-off to slightly negative numbers
    sqrt(e_sq)
  end

  def period
    2*PI/sqrt( @mass / semi_major_axis**3 )
  end

  def most_recent_return_time(time)
    p = period
    phase = (time - @init_time)/p
    phase = phase - phase.floor
    time - phase*p
  end

  def dissolve
    b1 = Body.new
    b2 = Body.new
    b1.mass = @m1
    b2.mass = @m2
    b1.pos = @pos - (@m2/@mass)*@rel_pos
    b2.pos = @pos + (@m1/@mass)*@rel_pos
    b1.vel = @vel - (@m2/@mass)*@rel_vel
    b2.vel = @vel + (@m1/@mass)*@rel_vel
    [b1, b2]
  end

  def deep_copy
    Marshal.load(Marshal.dump(self))
  end

end
