require "vector.rb"
require "clop.rb"
require "acsio.rb"

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

  def initialize
    @body = []
  end

  def simple_print
    print @body.size, "\n"
    printf("%24.16e\n", @time)
    @body.each{|b| b.simple_print}
  end

end

options_text= <<-END

  Description: Conversion from ACS data format to simple N-body format
  Long description:

    This program takes an N-body snapshot in the standard ACS format:

                  ACS
                    Nbody 
                      Array body
                        Body body[0]
                          Float mass
                               m1
                          Vector pos
                               r1_x  r1_y  r1_z
                          Vector vel
                               v1_x  v1_y  v1_z
                        Body body[1]
                        ...
                        Body body[n]
                          Float mass
                               m1
                          Vector pos
                               r1_x  r1_y  r1_z
                          Vector vel
                               v1_x  v1_y  v1_z
                      Float time
                           t
                  SCA

    The output is given in the following simple format, where n is the
    number of particles; t is the time; and for each body i there is a
    mass mi, position ri and velocity vi, divided over separate lines as:

                  n
                  t
                  m1 r1_x r1_y r1_z v1_x v1_y v1_z
                  m2 r2_x r2_y r2_z v2_x v2_y v2_z
                  ...
                  mn rn_x rn_y rn_z vn_x vn_y vn_z

    (c) 2004, Piet Hut and Jun Makino; see ACS at www.artcompsi.org

    example:
    ruby #{$0} < cube1_acs.in


  Short name:		-m
  Long name:  		--integration_method
  Value type:  		string
  Default value:	hermite
  Variable name:	method
  Description:		Integration method
  Long description:
    This option receives a string, containing the name of the integration
    method that will be used.  Example: "-m hermite" .


  Short name: 		-s
  Long name:		--softening_length
  Value type:		float
  Default value: 	0.0
  Variable name: 	eps
  Description:		Softening length
  Long description:
    This option sets the softening length used to calculate the force
    between two particles.  The calculation scheme comforms to standard
    Plummer softening, where rs2=r**2+eps**2 is used in place of r**2.


  Short name: 		-d
  Long name:		--step_size
  Value type:		float
  Default value:	0.01
  Variable name:	dt
  Description:		Time step size
  Long description:
    This option sets the size of the time step, which is constant and
    shared by all particles.  It is wise to use option -s to specify
    a softening length that is at least as large as the time step size.


  Short name: 		-e
  Long name:		--diagnostics_interval
  Value type:		float
  Default value:	1
  Variable name:	dt_dia
  Description:		Interval between diagnostics output
  Long description:
    This option sets the time interval between diagnostics output,
    which will appear on the standard error channel.


  Short name: 		-o
  Long name:		--output_interval
  Value type:		float
  Default value:	1
  Variable name:	dt_out
  Description:		Time interval between snapshot output
  Long description:
    This option sets the time interval between output of a snapshot
    of the whole N-body system, which which will appear on the
    standard output channel.

    The snapshot contains the mass, position, and velocity values
    for all particles in an N-body system.

    The program expects input of a single snapshot of an N-body
    system, in the following format: the number of particles in the
    snapshot n; the time t; mass mi, position ri and velocity vi for
    each particle i, with position and velocity given through their
    three Cartesian coordinates, divided over separate lines as
    follows:

                  n
                  t
                  m1 r1_x r1_y r1_z v1_x v1_y v1_z
                  m2 r2_x r2_y r2_z v2_x v2_y v2_z
                  ...
                  mn rn_x rn_y rn_z vn_x vn_y vn_z

    Output of each snapshot is written according to the same format.


  Short name: 		-t
  Long name:		--duration
  Value type:		float
  Default value:	10
  Variable name:	dt_end
  Print name:		t
  Description:		Duration of the integration
  Long description:
    This option sets the duration t of the integration, the time period
    after which the integration will halt.  If the initial snapshot is
    marked to be at time t_init, the integration will halt at time
    t_final = t_init + t.


  Short name:		-i
  Long name:  		--init_out
  Value type:  		bool
  Variable name:	init_out
  Description:		Output the initial snapshot
  Long description:
    If this flag is set to true, the initial snapshot will be output
    on the standard output channel, before integration is started.


  Short name:		-x
  Long name:  		--extra_diagnostics
  Value type:  		bool
  Variable name:	x_flag
  Description:		Extra diagnostics
  Long description:
    If this flag is set to true, the following extra diagnostics
    will be printed: 

      acceleration (for all integrators)
      jerk (for the Hermite integrator)


  END

parse_command_line(options_text, true)

include Math

nb = Nbody.new
nb.simple_read
nb.evolve($method, $eps, $dt, $dt_dia, $dt_out, $dt_end, $init_out, $x_flag)
