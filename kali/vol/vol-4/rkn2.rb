require "rknbody.rb"                                                         #1

def print_help
  print "usage: ", $0,
    " [-h (for help)] [-s softening_length] [-d step_size]\n",
    "         [-e diagnostics_interval] [-o output_interval]\n",
    "         [-t total_duration] [-i (start output at t = 0)]\n",
    "         [-x (extra debugging diagnostics)]\n", 
    "         [-m integration_method]\n"
end

require "getoptlong"                                                         #5

parser = GetoptLong.new
parser.set_options(
  ["-d", "--step_size", GetoptLong::REQUIRED_ARGUMENT],
  ["-e", "--diagnostics_interval", GetoptLong::REQUIRED_ARGUMENT],
  ["-h", "--help", GetoptLong::NO_ARGUMENT],
  ["-i", "--initial_output", GetoptLong::NO_ARGUMENT],
  ["-m", "--integration_method", GetoptLong::REQUIRED_ARGUMENT],
  ["-o", "--output_interval", GetoptLong::REQUIRED_ARGUMENT],
  ["-s", "--softening_length", GetoptLong::REQUIRED_ARGUMENT],
  ["-t", "--total_duration", GetoptLong::REQUIRED_ARGUMENT],
  ["-x", "--extra_diagnostics", GetoptLong::NO_ARGUMENT])

def read_options(parser)
  dt = 0.001
  dt_dia = 1
  dt_out = 1
  dt_end = 10
  eps = 0
  init_out = false
  x_flag = false
  method = "rk4"

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
      when "-m"
	method = arg
      when "-o"
	dt_out = arg.to_f
      when "-s"
	eps = arg.to_f
      when "-t"
	dt_end = arg.to_f
      when "-x"
	x_flag = true
      end

    rescue => err
      print_help
      exit           # exit if option unknown
    end

  end

  return eps, dt, dt_dia, dt_out, dt_end, init_out, x_flag, method
end

eps, dt, dt_dia, dt_out, dt_end, init_out, x_flag, method =                  #4
   read_options(parser)                                                      #4

STDERR.print "eps = ", eps, "\n",                                            #3
      "dt = ", dt, "\n",                                                     #3
      "dt_dia = ", dt_dia, "\n",                                             #3
      "dt_out = ", dt_out, "\n",                                             #3
      "dt_end = ", dt_end, "\n",                                             #3
      "init_out = ", init_out, "\n",                                         #3
      "x_flag = ", x_flag, "\n",                                             #3
      "method = ", method, "\n"                                              #3

include Math

nb = Nbody.new                                                               #2
nb.simple_read                                                               #2
nb.evolve(method, eps, dt, dt_dia, dt_out, dt_end, init_out, x_flag)         #2
