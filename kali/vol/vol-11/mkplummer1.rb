require "vector.rb"

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

  attr_accessor :body

  def initialize(n=0)
    @body = []
    for i in 0...n
      @body[i] = Body.new
    end
  end

  def simple_print
    print @body.size, "\n"
    printf("%24.16e\n", 0.0)
    @body.each{|b| b.simple_print}
  end

end

include Math

def frand(low, high)
  low + rand * (high - low)
end

def mkplummer(n, seed)
  if seed == 0
    srand
  else
    srand seed
  end
  scalefactor = 16.0 / (3.0 * PI)
  inv_scalefactor = 1.0 / scalefactor
  sqrt_scalefactor = sqrt( scalefactor )
  nb = Nbody.new(n)
  nb.body.each do |b|
    b.mass = 1.0/n
    radius = 1.0 / sqrt( rand ** (-2.0/3.0) - 1.0)
    theta = acos(frand(-1, 1))
    phi = frand(0, 2*PI)
    b.pos[0] = radius * sin( theta ) * cos( phi )
    b.pos[1] = radius * sin( theta ) * sin( phi )
    b.pos[2] = radius * cos( theta )
    b.pos *= inv_scalefactor
    x = 0.0
    y = 0.1
    while y > x*x*(1.0-x*x)**3.5
      x = frand(0,1)
      y = frand(0,0.1)
    end
    velocity = x * sqrt(2.0) * ( 1.0 + radius*radius)**(-1.0/4.0)
    theta = acos(frand(-1, 1))
    phi = frand(0, 2*PI)
    b.vel[0] = velocity * sin( theta ) * cos( phi )
    b.vel[1] = velocity * sin( theta ) * sin( phi )
    b.vel[2] = velocity * cos( theta )
    b.vel *= sqrt_scalefactor
  end
  STDERR.print "seed used = ", srand, "\n"
  nb.simple_print
end

def print_help
  print "usage: ", $0,
    " [-h (for help)] [-n particle_number] [-s seed]\n"
end

require "getoptlong"

parser = GetoptLong.new
parser.set_options(
  ["-n", "--particle_number", GetoptLong::REQUIRED_ARGUMENT],
  ["-s", "--seed", GetoptLong::REQUIRED_ARGUMENT])

def read_options(parser)
  n = 1
  s = 0

  loop do
    begin
      opt, arg = parser.get
      break if not opt

      case opt
      when "-n"
	n = arg.to_i
      when "-s"
	s = arg.to_i
      end

    rescue => err
      print_help
      exit           # exit if option unknown
    end

  end

  return n, s
end

n, s = read_options(parser)

STDERR.print "N = ", n, "\n",
      "seed given = ", s, "\n"

mkplummer(n, s)
