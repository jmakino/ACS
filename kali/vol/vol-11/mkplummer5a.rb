require "vector.rb"
$LOAD_PATH.push(ENV["ACSROOT"]+"/src/utilities")
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
  scalefactor = 16.0 / (3.0 * PI)
  nb = Nbody.new(n)
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
  adjust_center_of_mass(nb)                                                  #1
  STDERR.print "seed used = ", srand, "\n"
  nb.simple_print
end

def adjust_center_of_mass(nb)
  vel_com = pos_com = nb.body[0].pos*0     # null vectors of the correct length
  nb.body.each do |b|
    pos_com += b.pos*b.mass
    vel_com += b.vel*b.mass
  end
  nb.body.each do |b|
    b.pos -= pos_com
    b.vel -= vel_com
  end
end

optionstext= <<-END
Short name: -n
Long name:  --particle_number
Value type:  int
Default value: 1
Description: Number of particles
Global variable: nparticles
Long description:
Number of particles created


Short name: -s
Long name:  --seed
Value type:  int
Default value: 0
Description: seed
Global variable: seed
Long description:
Seed for the pseudorandom number generator.
END

Clop.new(optionstext,ARGV)

STDERR.print "N = ", $nparticles, "\n",
      "seed given = ", $seed, "\n"

mkplummer($nparticles,$seed)
