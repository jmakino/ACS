# Here is a stand-alone version of a simple leapfrog integration scheme,
# for an N-body system containing point masses.
#
# <i>I/O data format:</i>
# N::            number of particles
# time::         time 
# mass::         mass of particle #1
# position::     x y z : vector components of position of particle #1
# velocity::     vx vy vz : vector components of velocity of particle #1
# mass::         mass of particle #2
# ...::          ...
#
# <i>usage:</i>
#
#  ruby leap.rb [-h (for help)] [-s softening_length] [-d step_size]
#               [-e diagnostics_interval] [-o output_interval]
#               [-t total_duration] [-i (start output at t = 0)]
#               [-x (extra debugging diagnostics)]

# number of spatial dimensions:

NDIM = 3

#<i>Body models a single particle in an N-body system</i>

class Body

  attr_accessor :mass, :pos, :vel      # mass, position, velocity

  def initialize(mass = 0, pos = [0,0,0], vel = [0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.join(", ") + "\n" +
    "   vel = " + @vel.join(", ") + "\n"
  end

  def pp               # pretty print
    print to_s
  end

  def write
    printf("%22.15e\n", @mass)
    @pos.each do |x| printf("%23.15e", x) end
    print "\n"
    @vel.each do |x| printf("%23.15e", x) end
    print "\n"
  end

  def read
    @mass = gets.to_f
    s = gets
    a = s.split
    a.collect! { |x| x.to_f }
    @pos = a[0...NDIM]
    s = gets
    a = s.split
    a.collect! { |x| x.to_f }
    @vel = a[0...NDIM]
  end

end

#<i> Nbody models an N-body system </i>

class Nbody

  attr_accessor :time, :body       # time, array of bodies

  def n
    @body.size
  end

  def initialize(n=0, time = 0.0)
    @body = [Body.new]
    for i in 0...n
      @body[i] = Body.new
    end
    @time = time
  end

  def pp                                # pretty print
    print "     N = ", @body.size, "\n"
    print "  time = ", @time, "\n"
    @body.each do |b| b.pp end
  end

  def write
    print @body.size, "\n"
    printf("%22.15e\n", @time)
    @body.each do |b| b.write end
  end

  def read
    n = gets.to_i
    @time = gets.to_f
    for i in 0...n
      @body[i] = Body.new
      @body[i].read
    end
  end

end

#<i>extends Body class to provide energy diagnostics</i>

class Body

  def single_kin                 # returns kinetic energy for a single particle
    ekin = 0
    for k in 0...NDIM
      ekin += 0.5 * @mass * @vel[k] * @vel[k]
    end
    return ekin
  end

  def pairwise_pot(other, eps)   # returns potential energy for a particle pair
    rji = Array.new(NDIM)        # `eps' is the softening parameter
    for k in 0...NDIM
      rji[k] = other.pos[k] - @pos[k]
    end
    r2 = eps*eps
    for k in 0...NDIM
      r2 += rji[k] * rji[k]
    end
    r = sqrt(r2)
    return -@mass * other.mass / r
  end

end

#<i>extends Nbody class to provide energy diagnostics</i>

class Nbody

  attr_accessor :einit, :ekin, :epot       # initial, kinetic, potential energy

  def clear_ekin
    @ekin = 0
  end

  def clear_epot
    @epot = 0
  end

  def update_ekin
    clear_ekin
    @body.each do |b|
      @ekin += b.single_kin
    end
  end

  def update_epot(eps)
    clear_epot
    i = 0
    while (i < @body.size) 
      j = i+1
      while (j < @body.size)
	@epot += @body[i].pairwise_pot(@body[j], eps)
	j += 1
      end
      i += 1
    end
  end

  def initialize_energy(eps)
    update_ekin
    update_epot(eps)
    @einit = @ekin + @epot
  end

  def report_energy
    etot = @ekin+@epot
    STDERR.print "  E_kin = ", @ekin, " , E_pot = ", @epot,
      " , E_tot = ", etot, "\n                ",
      "absolute energy error: E_tot - E_init = ", etot-@einit,
      "\n                ",
      "relative energy error: (E_tot - E_init) / E_init = ",
               (etot - @einit) / @einit, "\n"
  end

end

#<i>extends Body class to implement the leapfrog algorithm</i>

class Body

  include Math

  attr_accessor :acc, :pot         # acceleration, potential

  def clear_acc
    @acc = [0.0,0.0,0.0]
  end

  def leapfrog_to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.join(", ") + "\n" +
    "   vel = " + @vel.join(", ") + "\n" +
    "   acc = " + @acc.join(", ") + "\n"
  end

  def leapfrog_pp            # pretty print
    print leapfrog_to_s
  end

  def predict_step(dt)
    for k in 0...NDIM
      @vel[k] += @acc[k]*0.5*dt     # mid-point velocity
      @pos[k] += @vel[k]*dt         # new position `leapfrogs' over mid-point
    end
  end

  def correct_step(dt)
    for k in 0...NDIM
      @vel[k] += @acc[k]*0.5*dt     # new velocity determined from acceleration
    end                             # at new position
  end

  def pairwise_acc(other, eps)      # add pairwise acceleration to the total
    rji = Array.new(NDIM)           # acceleration that is being accumulated
    for k in 0...NDIM               # in each of the 2 particles: self & other
      rji[k] = other.pos[k] - @pos[k]
    end
    r2 = eps*eps
    for k in 0...NDIM
      r2 += rji[k] * rji[k]
    end
    r = sqrt(r2)
    r3 = r * r2
    da = Array.new(NDIM)
    for k in 0...NDIM
      da[k] = rji[k] / r3
    end
    for k in 0...NDIM
      self.acc[k] += other.mass * da[k]
      other.acc[k] -= self.mass * da[k]
    end
  end

end

#<i>extends Nbody class in order to include Leapfrog module</i>

class Nbody

  def leapfrog_pp                         # pretty print
    print "     N = ", @body.size, "\n"
    print "  time = ", @time, "\n"
    i = 0
    @body.each do |b|
      i += 1
      STDERR.print "  internal data for particle ", i, " :\n"
      STDERR.print b.leapfrog_to_s
    end
  end

  def write_diagnostics(eps, nsteps, x_flag)
    STDERR.print "at time t = ", @time, " , after ", nsteps, " steps :\n"
    update_ekin
    update_epot(eps)
    report_energy
    if x_flag
      STDERR.print "  for debugging purposes, here is the internal data ",
	"representation:\n"
      leapfrog_pp
    end
  end

  def predict_step(dt)
    @body.each do |b| b.predict_step(dt) end
  end
  
  def get_acc(eps)
    @body.each do |b| b.clear_acc end
    i = 0
    while (i < @body.size) 
      j = i+1
      while (j < @body.size)
	@body[i].pairwise_acc(@body[j], eps)
	j += 1
      end
      i += 1
    end
  end
  
  def correct_step(dt)
    @body.each do |b| b.correct_step(dt) end
  end
  
  def inc_time(dt)
    @time += dt
  end
  
  def evolve_step(eps, dt)        # take one leapfrog step for the whole system
    predict_step(dt)
    get_acc(eps)
    correct_step(dt)
    inc_time(dt)
  end
  
  def evolve(eps, dt, dt_dia, dt_out, dt_tot, init_out, x_flag)

    STDERR.print "Starting a leapfrog integration for a ", @body.size,
                 "-body system,\n  from time t = ", @time, 
                 " with time step dt = ", dt,
                 "  until time ", @time + dt_tot,
                 " ,\n  with diagnostics output interval dt_dia = ", dt_dia,
                 ",\n  and snapshot output interval dt_out = ", dt_out, ".\n"
                 # eps: epsilon, the softening length
                 # init_out: if true, a full output initially, before evolving
                 # x_flag: if true, provide extra diagnostics

    initialize_energy(eps)
    get_acc(eps)
    nsteps = 0
    write_diagnostics(eps, nsteps, x_flag)

    write if init_out

    t_dia = @time + dt_dia
    t_out = @time + dt_out
    t_end = @time + dt_tot

    loop do
      while @time < t_dia and @time < t_out and @time < t_end
	evolve_step(eps, dt)
	nsteps += 1
      end
      if @time >= t_dia
	write_diagnostics(eps, nsteps, x_flag)
	t_dia += dt_dia
      end
      if @time >= t_out
	write
	t_out += dt_out
      end
      if @time >= t_end
	break;
      end
    end
  end

end

#<i>driver for leapfrog integration of an N-body system</i>

def print_help
  print "usage: ", $0,
    " [-h (for help)] [-s softening_length] [-d step_size]\n",
    "         [-e diagnostics_interval] [-o output_interval]\n",
    "         [-t total_duration] [-i (start output at t = 0)]\n",
    "         [-x (extra debugging diagnostics)]\n"
end

require "getoptlong"

parser = GetoptLong.new
parser.set_options(
  ["-d", "--step_size", GetoptLong::REQUIRED_ARGUMENT],
  ["-e", "--diagnostics_interval", GetoptLong::REQUIRED_ARGUMENT],
  ["-h", "--help", GetoptLong::NO_ARGUMENT],
  ["-i", "--initial_output", GetoptLong::NO_ARGUMENT],
  ["-o", "--output_interval", GetoptLong::REQUIRED_ARGUMENT],
  ["-s", "--softening_length", GetoptLong::REQUIRED_ARGUMENT],
  ["-t", "--total_duration", GetoptLong::REQUIRED_ARGUMENT],
  ["-x", "--extra_diagnostics", GetoptLong::NO_ARGUMENT])

def read_options(parser)
  dt = 0.01
  dt_dia = 1
  dt_out = 1
  dt_tot = 10
  eps = 0
  init_out = false
  x_flag = false

  loop do
    begin
      opt, arg = parser.get
      break if not opt

      case opt
      when "-d"
	dt = arg.to_f
      when "-e"
	dt_dia = arg.to_f
      when "-h"
	print_help
        exit         # exit after providing help
      when "-i"
	init_out = true
      when "-o"
	dt_out = arg.to_f
      when "-s"
	eps = arg.to_f
      when "-t"
	dt_tot = arg.to_f
      when "-x"
	x_flag = true
      end

    rescue => err
      print_help
      exit           # exit if option unknown
    end

  end

  return eps, dt, dt_dia, dt_out, dt_tot, init_out, x_flag
end

eps, dt, dt_dia, dt_out, dt_tot, init_out, x_flag = read_options(parser)

STDERR.print "eps = ", eps, "\n",
      "dt = ", dt, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_tot = ", dt_tot, "\n",
      "init_out = ", init_out, "\n",
      "x_flag = ", x_flag, "\n"

nb = Nbody.new
nb.read

nb.evolve(eps, dt, dt_dia, dt_out, dt_tot, init_out, x_flag)
