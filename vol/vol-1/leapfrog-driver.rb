#<i>driver for leapfrog integration of an N-body system</i>

require "nbody-leapfrog.rb"

NDIM = 3

def print_help
  print "usage: ", $0,
    " [-h (for help)] [-d step_size]\n",
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
  ["-t", "--total_duration", GetoptLong::REQUIRED_ARGUMENT],
  ["-x", "--extra_diagnostics", GetoptLong::NO_ARGUMENT])

def read_options(parser)
  dt = 0.01
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

  return dt, dt_dia, dt_out, dt_tot, init_out, x_flag
end

dt, dt_dia, dt_out, dt_tot, init_out, x_flag = read_options(parser)

STDERR.print "dt = ", dt, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_tot = ", dt_tot, "\n",
      "init_out = ", init_out, "\n",
      "x_flag = ", x_flag, "\n"

nb = Nbody.new
nb.simple_read

nb.initialize_energy
nb.evolve(dt, dt_dia, dt_out, dt_tot, init_out, x_flag)





