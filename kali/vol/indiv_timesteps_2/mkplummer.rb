#!/usr/local/bin/ruby -w

require "acs.rb"

require "acs"

class Body

  attr_accessor :mass, :pos, :vel

  def ekin                         # kinetic energy
    0.5*@mass*(@vel*@vel)
  end

  def epot(body_array)             # potential energy
    p = 0
    body_array.each do |b|
      unless b == self
        r = b.pos - @pos
        p += -@mass*b.mass/sqrt(r*r)
      end
    end
    p
  end

end

class NBody

  attr_accessor :time, :body

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

  def adjust_units
    alpha = -epot / 0.5
    beta = ekin / 0.25
    @body.each do |b|
      b.pos *= alpha
      b.vel /= sqrt(beta)
    end
  end

end

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

def mkplummer(c)
  if c.seed == 0
    srand
  else
    srand c.seed
  end
  nb = NBody.new
  c.n.times do
    b = plummer_sample
    b.mass = 1.0/c.n
    nb.body.push(b)
  end
  nb.adjust_center_of_mass
  nb.adjust_units
  STDERR.print "             actual seed used\t: ", srand, "\n"
  nb.acs_write($stdout, false, c.precision, c.add_indent)
end

def plummer_sample
  b = Body.new
  scalefactor = 16.0 / (3.0 * PI)
  radius = 1.0 / sqrt( rand ** (-2.0/3.0) - 1.0)
  b.pos = spherical(radius) / scalefactor
  x = 0.0
  y = 0.1
  while y > x*x*(1.0-x*x)**3.5
    x = frand(0,1)
    y = frand(0,0.1)
  end
  velocity = x * sqrt(2.0) * ( 1.0 + radius*radius)**(-0.25)
  b.vel = spherical(velocity) * sqrt(scalefactor)
  b
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

    Each particles is drawn at random from the Plummer distribution,
    and therefore there are no correlations between the particles.

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


  Short name:           -p
  Long name:            --precision
  Value type:           int
  Default value:        16
  Description:          Floating point precision
  Variable name:        precision
  Long description:
    The precision with which floating point numbers are printed in the output.
    The default precision is comparable to double precision accuracy.


  Long name:            --indentation
  Value type:           int
  Default value:        2
  Description:          Incremental indentation
  Variable name:        add_indent
  Long description:
    This option allows the user to set the incremental indentation, i.e.
    the number of white spaces added in front of the output of data, for
    each level that the data are removed from the top level.

    Starting at zero indentation at the level of the top ACS structure,
    one set of incremental indentation is added for each level down,
    from ACS to DSS, from DSS to World, and so on.


  END

mkplummer(parse_command_line(options_text, true))
