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
  scalefactor = 16.0 / (3.0 * PI)                                            #1
  nb = Nbody.new(n)
  nb.body.each do |b|
    b.mass = 1.0/n
    radius = 1.0 / sqrt( rand ** (-2.0/3.0) - 1.0)
    b.pos = spherical(radius) / scalefactor                                  #2
    x = 0.0
    y = 0.1
    while y > x*x*(1.0-x*x)**3.5
      x = frand(0,1)
      y = frand(0,0.1)
    end
    velocity = x * sqrt(2.0) * ( 1.0 + radius*radius)**(-0.25)
    b.vel = spherical(velocity) * sqrt(scalefactor)                          #3
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
