#!/usr/bin/ruby
#
# nbody_sh1.rb
#
#==============================================================================

class Body

  def initialize(mass = 0, pos = [0,0,0], vel = [0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def to_s
    "  mass = " + @mass.to_s + "\n" +
      "   pos = " + @pos.join(", ") + "\n" +
      "   vel = " + @vel.join(", ") + "\n"
  end

  def pp            # pretty print
    print to_s
  end

  def simple_print
    printf("%22.15e", @mass)
    @pos.each do |x| printf("%23.15e", x) end
    @vel.each do |x| printf("%23.15e", x) end
    print "\n"
  end

  def simple_read
    s = gets
    a = s.split
    @mass = a[0]
    @pos = a[1..3]
    @vel = a[4..6]
  end

  def get_time
    @@time
  end

  def set_time(time)
    @@time = time
  end

end

def get_snapshot
  s = gets
  n = s.to_i
  s = gets
  nb = [Body.new]
  nb[0].set_time(s.to_f)
  i = 0
  while i < n
    nb[i] = Body.new
    nb[i].simple_read
    i += 1
  end
  return nb
end

def put_snapshot(nb)
  print nb.size, "\n"
  print nb[0].get_time, "\n"
  nb.each do |b| b.simple_print end
end

def pp_snapshot(nb)
  print "N = ", nb.size, "\n"
  print "time = ", nb[0].get_time, "\n"
  nb.each do |b| b.pp end
end  

def print_help
  print "usage: ", $0,
    " [-h (for help)] [-d step_size_control_parameter]\n",
    "         [-e diagnostics_interval] [-o output_interval]\n",
    "         [-t total_duration] [-i (start output at t = 0)]\n",
    "         [-x (extra debugging diagnostics)]\n"
end

require "getoptlong"

parser = GetoptLong.new
parser.set_options(
  ["-d", "--step_size_control_parameter", GetoptLong::REQUIRED_ARGUMENT],
  ["-e", "--diagnostics_interval", GetoptLong::REQUIRED_ARGUMENT],
  ["-h", "--help", GetoptLong::NO_ARGUMENT],
  ["-i", "--initial_output", GetoptLong::NO_ARGUMENT],
  ["-o", "--output_interval", GetoptLong::REQUIRED_ARGUMENT],
  ["-t", "--total_duration", GetoptLong::REQUIRED_ARGUMENT],
  ["-x", "--extra_diagnostics", GetoptLong::NO_ARGUMENT])

def read_options(parser)
  dt_param = 0.03
  dt_dia = 1
  dt_out = 1
  dt_tot = 10
  init_out = false
  x_flag = false

  loop do
    begin
      opt, arg = parser.get
      break if not opt

      case opt
      when "-d"
	dt_param = arg.to_f
      when "-e"
	dt_dia = arg.to_f
      when "-h"
	print_help
        exit         # exit after providing help
      when "-i"
	init_out = true
      when "-o"
	dt_out = arg.to_f
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

  return dt_param, dt_dia, dt_out, dt_tot, init_out, x_flag
end

def evolve(nb, dt_param, dt_dia, dt_out, dt_tot, init_out, x_flag)
  STDERR.print "Starting a Hermite integration for a ", nb.size,
               "-body system,\n  from time t = ", nb[0].get_time, 
               " with time step control parameter dt_param = ", dt_param,
               "  until time ", nb[0].get_time + dt_tot,
               " ,\n  with diagnostics output interval dt_dia = ", dt_dia,
               ",\n  and snapshot output interval dt_out = ", dt_out, ".\n"
end

dt_param, dt_dia, dt_out, dt_tot, init_out, x_flag = read_options(parser)

STDERR.print "dt_param = ", dt_param, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_tot = ", dt_tot, "\n",
      "init_out = ", init_out, "\n",
      "x_flag = ", x_flag, "\n"

nb = get_snapshot

#put_snapshot(nb)
#pp_snapshot(nb)

evolve(nb, dt_param, dt_dia, dt_out, dt_tot, init_out, x_flag)



#b = Body.new(1.0, [1.0, 0.0, 0.0], [0.0, 0.1, 0.2])
#b.pp
#b.simple_print
#b = Body.new
#b.simple_read
#b.pp
#b.simple_print
