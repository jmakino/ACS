require "nbody1.rb"
require "clop.rb"

options_definition_string = <<-END

  Description:		The simplest ACS N-body code
  Long description:
    This is the simplest N-body code provided in the ACS environment
    (c) 2004, Piet Hut and Jun Makino; see ACS at www.artcompsi.org
    It offers a choice of integrators, for constant shared time steps.


  Short name: 		-m
  Long name:		--integration_method
  Value type:		string
  Default value:	rk4
  Variable name:	method
  Print name:		                    # blank: suppresses glob. var. name
  Description:		Integration method
  Long description:
    There are a variety of integration methods available, including:

      Forward Euler:		forward
      Leapfrog:			leapfrog
      2nd-order Runge Kutta:	rk2
      4th-order Runge Kutta:	rk4


  Short name: 		-d
  Long name:		--step_size
  Value type:		float
  Default value:	0.001
  Variable name:	dt
  Description:		Integration time step
  Long description:
    In this code, the integration time step is held constant,
    and shared among all particles in the N-body system.


  Short name: 		-e
  Long name:		--diagnostics_interval
  Value type:		float
  Default value:	1
  Variable name:	dt_dia
  Description:		Diagnostics output interval
  Long description:
    The time interval between successive diagnostics output.
    The diagnostics include the kinetic and potential energy,
    and the absolute and relative drift of total energy, since
    the beginning of the integration.
        These diagnostics appear on the standard error stream.
    For more diagnostics, try option "-x" or "--extra_diagnostics".


  Short name: 		-o
  Long name:		--output_interval
  Value type:		float
  Default value:	1
  Variable name:	dt_out
  Description:		Snapshot output interval
  Long description:
    The time interval between output of a complete snapshot
    A snapshot of an N-body system contains the values of the
    mass, position, and velocity for each of the N particles.

        This information appears on the standard output stream,
    currently in the following simple format (only numbers):

      N:            number of particles
      time:         time 
      mass:         mass of particle #1
      position:     x y z : vector components of position of particle #1
      velocity:     vx vy vz : vector components of velocity of particle #1
      mass:         mass of particle #2
      ...:          ...

    Example:

       2
       0
       0.5
      7.3406783488452532e-02  2.1167291484119417e+00 -1.4097856092768946e+00
      3.1815484836541341e-02  2.7360312082526089e-01  2.4960049959942499e-02
       0.5
     -7.3406783488452421e-02 -2.1167291484119413e+00  1.4097856092768946e+00
     -3.1815484836541369e-02 -2.7360312082526095e-01 -2.4960049959942499e-02


  Short name: 		-t
  Long name:		--total_duration
  Value type:		float
  Default value:	10
  Variable name:	dt_end
  Description:		Duration of the integration
  Long description:
    This option allows specification of the time interval, after which
    integration will be halted.


  Short name: 		-s
  Long name:		--softening_length
  Value type:		float
  Default value: 	0
  Variable name: 	eps
  Description:		Softening length
  Long description:
    This option sets the softening length used to calculate the force
    between two particles.  The calculation scheme comforms to standard
    Plummer softening, where rs2=r**2+eps**2 is used in place of r**2.


  Short name:		-i
  Long name:  		--initial_output
  Value type:  		bool
  Variable name:	init_out
  Description:		Extra diagnostics
  Long description:
    The following extra diagnostics will be printed:

      acceleration (for all integrators)
      jerk (for the Hermite integrator)


  Short name:		-x
  Long name:  		--extra_diagnostics
  Value type:  		bool
  Variable name:	x_flag
  Description:		Extra diagnostics
  Long description:
    The following extra diagnostics will be printed:

      acceleration (for all integrators)
      jerk (for the Hermite integrator)


  END

parse_command_line(options_definition_string)

include Math

nb = Nbody.new
nb.simple_read
nb.evolve($method, $eps, $dt, $dt_dia, $dt_out, $dt_end, $init_out, $x_flag)
