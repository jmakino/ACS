require "acs"

class Body

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

end

class NBody

  attr_accessor :time, :body

  def initialize(n = 0)
    @body = []
    for i in 0...n
      @body[i] = Body.new
    end
  end

  def adjust_center_of_mass
    vel_com = pos_com = @body[0].pos*0     # null vectors of the correct length
    @body.each do |b|
      pos_com += b.pos*b.mass
      vel_com += b.vel*b.mass
    end
    @body.each do |b|
      b.pos -= pos_com
      b.vel -= vel_com
    end
  end

end

include Math

def frand(low, high)
  low + rand * (high - low)
end

def spherical(r)
  vector = Vector.new
  theta = acos(frand(-1, 1))
  phi = frand(0, 2*PI)
  vector[0] = r * sin( theta ) * cos( phi )
  vector[1] = r * sin( theta ) * sin( phi )
  vector[2] = r * cos( theta )
  vector
end  

def mkplummer(n, seed)
  if seed == 0
    srand
  else
    srand seed
  end
  scalefactor = 16.0 / (3.0 * PI)
  nb = NBody.new(n)
  cumulative_mass_min = 0
  cumulative_mass_max = 1.0/n
  nb.body.each do |b|
    b.mass = 1.0/n
    cumulative_mass = frand(cumulative_mass_min, cumulative_mass_max)
    cumulative_mass_min = cumulative_mass_max
    cumulative_mass_max += 1.0/n
    radius = 1.0 / sqrt( cumulative_mass ** (-2.0/3.0) - 1.0)
    b.pos = spherical(radius) / scalefactor
    x = 0.0
    y = 0.1
    while y > x*x*(1.0-x*x)**3.5
      x = frand(0,1)
      y = frand(0,0.1)
    end
    velocity = x * sqrt(2.0) * ( 1.0 + radius*radius)**(-0.25)
    b.vel = spherical(velocity) * sqrt(scalefactor)
  end
  nb.adjust_center_of_mass                                                   #1
  STDERR.print "             actual seed used\t: ", srand, "\n"
  nb.acs_write
end

options_text= <<-END

  Description: Plummer's Model Builder
  Long description:
    This program creates an N-body realization of Plummer's Model.
    (c) 2004, Piet Hut and Jun Makino; see ACS at www.artcompsi.org

    The algorithm used is described in Aarseth, S., Henon, M., & Wielen, R.,
    Astron. Astroph. 37, 183 (1974).


  Short name:		-n
  Long name:            --n_particles
  Value type:           int
  Default value:        1
  Variable name:        n
  Print name:           N
  Description:          Number of particles
  Long description:
    Number of particles in a realization of Plummer's Model.

    Each particle is drawn from the Plummer distribution, from successive
    shells, in a layered manner, starting from the center.  The radial
    distance from the center to the particles thus increases monotonically,
    upon creation.  Subsequently, the center of mass is shifted to the origin,
    which may disturb somewhat the monotonicity of the particle layering.

    Standard Units are used in which G = M = 1 and E = -1/4, where
      G is the gravitational constant
      M is the total mass of the N-body system
      E is the total energy of the N-body system


  Short name:           -s
  Long name:            --seed
  Value type:           int
  Default value:        0
  Description:          pseudorandom number seed given
  Print name:           
  Variable name:      seed
  Long description:
    Seed for the pseudorandom number generator.  If a seed is given with
    value zero, a preudorandom number is chosen as the value of the seed.
    The seed value used is echoed separately from the seed value given,
    to allow the possibility to repeat the creation of an N-body realization.

      Example:

        |gravity> ruby mkplummer1.rb -n 42 -s 0
        . . .
        pseudorandom number seed given	: 0
                     actual seed used	: 1087616341
        . . .
        |gravity> ruby mkplummer1.rb -n 42 -s 1087616341
        . . .
        pseudorandom number seed given	: 1087616341
                     actual seed used	: 1087616341
        . . .


  END

c = parse_command_line(options_text)

mkplummer(c.n, c.seed)
