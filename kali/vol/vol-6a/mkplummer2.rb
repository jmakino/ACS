require "vector.rb"
require "clop.rb"

class Body

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def simple_print
    printf("%24.16e\n", @mass)
    @pos.each{|x| printf("%24.16e", x)}; print "\n"
    @vel.each{|x| printf("%24.16e", x)}; print "\n"
  end

end

class Nbody

  attr_accessor :time, :body

  def initialize(n = 0, time = 0)
    @time = time
    @body = []
    for i in 0...n
      @body[i] = Body.new
    end
  end

  def simple_print
    print @body.size, "\n"
    printf("%24.16e\n", @time)
    @body.each{|b| b.simple_print}
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
  nb = Nbody.new(n)
  nb.body.each do |b|                                                        #1
    b.mass = 1.0/n                                                           #1
    radius = 1.0 / sqrt( rand ** (-2.0/3.0) - 1.0)                           #1
    b.pos = spherical(radius)                                                #1
    x = 0.0                                                                  #1
    y = 0.1                                                                  #1
    while y > x*x*(1.0-x*x)**3.5                                             #1
      x = frand(0,1)                                                         #1
      y = frand(0,0.1)                                                       #1
    end                                                                      #1
    velocity = x * sqrt(2.0) * ( 1.0 + radius*radius)**(-0.25)               #1
    b.vel = spherical(velocity)                                              #1
  end                                                                        #1
  STDERR.print "             actual seed used\t: ", srand, "\n"
  nb.simple_print
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
  Variable name:      n_particles
  Print name:           N
  Description:          Number of particles
  Long description:
    Number of particles in a realization of Plummer's Model.

    Each particles is drawn at random from the Plummer distribution,
    and therefore there are no correlations between the particles.

    Physical Units are used in which G = M = a = 1, where
      G is the gravitational constant
      M is the total mass of the N-body system
      a is the structural length, with potential U(r) = GM/(r^2 + a^2)^{1/2}


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

parse_command_line(options_text, true)

mkplummer($n_particles, $seed)
