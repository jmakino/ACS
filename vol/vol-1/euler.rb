# A simple forward-Euler integrator, for an 2-body system.
#
# <i>I/O data format:</i>
# mass::         sum of masses of the two particles
# position::     x y z : vector components of relative position
# velocity::     vx vy vz : vector components of relative velocity
#
# <i>usage:</i>
#
#  ruby euler.rb [-h (for help)] [-d step_size]
#                [-e diagnostics_interval] [-o output_interval]
#                [-t total_duration]

include Math

class Body

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = [0,0,0], vel = [0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def evolve_step(dt)
    r2 = 0
    pos.each {|p| r2 += p*p}
    r3 = r2 * sqrt(r2)
    acc = pos.map { |x| -x/r3 }
    pos.each_index { |k| pos[k] += vel[k] * dt }
    vel.each_index { |k| vel[k] += acc[k] * dt }
  end

  def ekin
    ek = 0
    vel.each {|v| ek += v*v}
    ek *= 0.5
  end

  def epot
    r2 = 0
    pos.each {|p| r2 += p*p}
    r = sqrt(r2)
    ep = -1/r
  end

  def e_init
    e0 = ekin() + epot()
  end

  def write_diagnostics(nsteps, time)
    STDERR.print "after time t = ", time, " , after ", nsteps, " steps :\n"
    etot = ekin() + epot()
    STDERR.print "  E_kin = ", ekin(), " , E_pot = ", epot(),
      " , E_tot = ", etot, "\n                ",
      "absolute energy error: E_tot - E_init = ", etot-e0,
      "\n                ",
      "relative energy error: (E_tot - E_init) / E_init = ",
               (etot - e0) / e0, "\n"
  end

  def to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.join(", ") + "\n" +
    "   vel = " + @vel.join(", ") + "\n"
  end

  def pp               # pretty print
    print to_s
  end

  def simple_print
    printf("%24.16e\n", @mass)
    @pos.each { |x| printf("%24.16e", x) } ; print "\n"
    @vel.each { |x| printf("%24.16e", x) } ; print "\n"
  end

  def simple_read
    @mass = gets.to_f
    @pos = gets.split.map { |x| x.to_f }
    @vel = gets.split.map { |x| x.to_f }
  end

end

#<i>driver for forward-Euler 2-body integrator</i>

def print_help
  print "usage: ", $0,
    " [-h (for help)] [-d step_size] [-e diagnostics_interval] \n",
    "         [-o output_interval] [-t total_duration]\n"
end

require "getoptlong"

parser = GetoptLong.new
parser.set_options(
  ["-d", "--step_size", GetoptLong::REQUIRED_ARGUMENT],
  ["-e", "--diagnostics_interval", GetoptLong::REQUIRED_ARGUMENT],
  ["-h", "--help", GetoptLong::NO_ARGUMENT],
  ["-o", "--output_interval", GetoptLong::REQUIRED_ARGUMENT],
  ["-t", "--total_duration", GetoptLong::REQUIRED_ARGUMENT])

def read_options(parser)
  dt = 0.01
  dt_dia = 1
  dt_out = 1
  dt_tot = 10

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
      when "-o"
	dt_out = arg.to_f
      when "-t"
	dt_tot = arg.to_f
      end

    rescue => err
      print_help
      exit           # exit if option unknown
    end

  end

  return dt, dt_dia, dt_out, dt_tot
end

def evolve(b, dt, dt_dia, dt_out, dt_tot)

  STDERR.print "Starting a forward Euler integration for a ",
               "2-body system,\n",
               " with time step dt = ", dt,
               "  for a duration of ", dt_tot,
               " ,\n  with diagnostics output interval dt_dia = ", dt_dia,
               ",\n  and snapshot output interval dt_out = ", dt_out, ".\n"

  time = 0
  nsteps = 0
  b.e_init()
  write_diagnostics(nsteps, time)

  t_dia = dt_dia
  t_out = dt_out
  t_end = dt_tot

  loop do
    while time < t_dia and time < t_out and time < t_end
	evolve_step(dt)
        time += dt
	nsteps += 1
    end
    if time >= t_dia
	write_diagnostics(nsteps, time)
	t_dia += dt_dia
    end
    if time >= t_out
	write
	t_out += dt_out
    end
    if time >= t_end
	break;
    end
  end
end

dt, dt_dia, dt_out, dt_tot = read_options(parser)

STDERR.print "dt = ", dt, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_tot = ", dt_tot, "\n"

b = Body.new
b.simple_read
evolve(b, dt, dt_dia, dt_out, dt_tot)



