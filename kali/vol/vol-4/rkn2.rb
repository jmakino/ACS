require "rknbody.rb"                                                         #1
require "clop.rb"

include Math

options_definition_string = <<-END

  Short name: 		-m
  Long name:		--integration_method
  Value type:		string
  Default value:	rk4
  Global variable:	int_method
  Print name:		
  Description:		Integration method
  Long description:
    x
    x


  Short name: 		-
  Long name:		--
  Value type:		float
  Default value:	1
  Global variable:	x
  Print name:		x
  Description:		xx
  Long description:
    x
    x


  Short name: 		-
  Long name:		--
  Value type:		float
  Default value:	1
  Global variable:	x
  Print name:		x
  Description:		xx
  Long description:
    x
    x


  Short name: 		-
  Long name:		--
  Value type:		float
  Default value:	1
  Global variable:	x
  Print name:		x
  Description:		xx
  Long description:
    x
    x




  Short name: 		-t
  Long name:		--end_time
  Value type:		float
  Default value:	10
  Global variable:	tend
  Description:		Time to stop integration
  Long description:
    This option gives the time to stop integration.


  Short name: 		-s
  Long name:		--softening_length
  Value type:		float
  Default value: 	0.0
  Global variable: 	eps
  Description:		Softening length
  Long description:
    This option sets the softening length used to calculate the force
    between two particles.  The calculation scheme comforms to standard
    Plummer softening, where rs2=r**2+eps**2 is used in place of r**2.


  Short name:		-x
  Long name:  		--extra_diagnostics
  Value type:  		bool
  Global variable:	xdiag
  Description:		Extra diagnostics
  Long description:
    The following extra diagnostics will be printed:

      acceleration (for all integrators)
      jerk (for the Hermite integrator)


  END

parse_command_line(options_definition_string)
nb = Nbody.new
nb.simple_read
nb.evolve($method, $eps, $dt, $dt_dia, $dt_out, $dt_end, $init_out, $x_flag)
