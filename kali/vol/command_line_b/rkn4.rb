require "../command_line/clop.rb"

require "vector.rb"

class Body

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def calc(softening_parameter, body_array, time_step, s)
    ba  = body_array
    dt = time_step
    eps = softening_parameter
    eval(s)
  end

  def acc(body_array, eps)
    a = @pos*0                              # null vector of the correct length
    body_array.each do |b|
      unless b == self
        r = b.pos - @pos
        r2 = r*r + eps*eps
        r3 = r2*sqrt(r2)
        a += r*(b.mass/r3)
      end
    end
    a
  end    

  def ekin                         # kinetic energy
    0.5*@mass*(@vel*@vel)
  end

  def epot(body_array, eps)        # potential energy
    p = 0
    body_array.each do |b|
      unless b == self
        r = b.pos - @pos
        p += -@mass*b.mass/sqrt(r*r + eps*eps)
      end
    end
    p
  end

  def to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.join(", ") + "\n" +
    "   vel = " + @vel.join(", ") + "\n"
  end

  def pp                           # pretty print
    print to_s
  end

  def ppx(body_array, eps)         # pretty print, with extra information (acc)
    STDERR.print to_s + "   acc = " + acc(body_array, eps).join(", ") + "\n"
  end

  def simple_print
    printf("%24.16e\n", @mass)
    @pos.each{|x| printf("%24.16e", x)}; print "\n"
    @vel.each{|x| printf("%24.16e", x)}; print "\n"
  end

  def simple_read
    @mass = gets.to_f
    @pos = gets.split.map{|x| x.to_f}.to_v
    @vel = gets.split.map{|x| x.to_f}.to_v
  end

end

class NBody

  attr_accessor :time, :body

  def initialize
    @body = []
  end

  def evolve(integration_method, eps, dt, dt_dia, dt_out, dt_end,
             init_out, x_flag)
    @dt = dt
    @eps = eps
    @nsteps = 0
    e_init
    write_diagnostics(x_flag)
    t_dia = dt_dia - 0.5*dt
    t_out = dt_out - 0.5*dt
    t_end = dt_end - 0.5*dt

    simple_print if init_out

    while @time < t_end
      send(integration_method)
      @time += @dt
      @nsteps += 1
      if @time >= t_dia
        write_diagnostics(x_flag)
        t_dia += dt_dia
      end
      if @time >= t_out
        simple_print
        t_out += dt_out
      end
    end
  end

  def calc(s)
    @body.each{|b| b.calc(@eps, @body, @dt, s)}
  end

  def forward
    calc(" @old_acc = acc(ba,eps) ")
    calc(" @pos += @vel*dt ")
    calc(" @vel += @old_acc*dt ")
  end

  def leapfrog
    calc(" @vel += acc(ba,eps)*0.5*dt ")
    calc(" @pos += @vel*dt ")
    calc(" @vel += acc(ba,eps)*0.5*dt ")
  end

  def rk2
    calc(" @old_pos = @pos ")
    calc(" @half_vel = @vel + acc(ba,eps)*0.5*dt ")
    calc(" @pos += @vel*0.5*dt ")
    calc(" @vel += acc(ba,eps)*dt ")
    calc(" @pos = @old_pos + @half_vel*dt ")
  end

  def rk4
    calc(" @old_pos = @pos ")
    calc(" @a0 = acc(ba,eps) ")
    calc(" @pos = @old_pos + @vel*0.5*dt + @a0*0.125*dt*dt ")
    calc(" @a1 = acc(ba,eps) ")
    calc(" @pos = @old_pos + @vel*dt + @a1*0.5*dt*dt ")
    calc(" @a2 = acc(ba,eps) ")
    calc(" @pos = @old_pos + @vel*dt + (@a0+@a1*2)*(1/6.0)*dt*dt ")
    calc(" @vel += (@a0+@a1*4+@a2)*(1/6.0)*dt ")
  end

  def ekin                        # kinetic energy
    e = 0
    @body.each{|b| e += b.ekin}
    e
  end

  def epot                        # potential energy
    e = 0
    @body.each{|b| e += b.epot(@body, @eps)}
    e/2                           # pairwise potentials were counted twice
  end

  def e_init                      # initial total energy
    @e0 = ekin + epot
  end

  def write_diagnostics(x_flag)
    etot = ekin + epot
    STDERR.print <<END
at time t = #{sprintf("%g", time)}, after #{@nsteps} steps :
  E_kin = #{sprintf("%.3g", ekin)} ,\
 E_pot =  #{sprintf("%.3g", epot)} ,\
 E_tot = #{sprintf("%.3g", etot)}
             E_tot - E_init = #{sprintf("%.3g", etot - @e0)}
  (E_tot - E_init) / E_init = #{sprintf("%.3g", (etot - @e0)/@e0 )}
END
    if x_flag
      STDERR.print "  for debugging purposes, here is the internal data ",
                   "representation:\n"
      ppx
    end
  end

  def pp                           # pretty print
    print "     N = ", @body.size, "\n"
    print "  time = ", @time, "\n"
    @body.each{|b| b.pp}
  end

  def ppx                          # pretty print, with extra information (acc)
    print "     N = ", @body.size, "\n"
    print "  time = ", @time, "\n"
    @body.each{|b| b.ppx(@body, @eps)}
  end

  def simple_print
    print @body.size, "\n"
    printf("%24.16e\n", @time)
    @body.each{|b| b.simple_print}
  end

  def simple_read
    n = gets.to_i
    @time = gets.to_f
    for i in 0...n
      @body[i] = Body.new
      @body[i].simple_read
    end
  end

end

options_text = <<-END

  Description: Constant Time Step Code
  Long description:
    This program evolves an N-body code with a fourth-order Runge-Kutta Scheme,
    or schemes such as forward Euler, leapfrog, or second-order Runge-Kutta,
    using constant time steps, shared by all particles, where the size of
    of the time step is prescribed beforehand.  The program includes the
    option to provide softening for the potential.  This is essential for
    a constant time step code; the alternative, instead of softening, would
    be to use a variable time step algorithm.

    (c) 2005, Piet Hut and Jun Makino; see ACS at www.artcompsi.org

    example:
    ruby #{$0} -t 1 < cube1.in


  Short name:		-g
  Long name:  		--integration_method
  Value type:		string
  Default value:	rk4
  Global variable:	method
  Description:		Integration method
  Long description:
    There are a variety of integration methods available, including:

      Forward Euler:		forward
      Leapfrog:			leapfrog
      2nd-order Runge Kutta:	rk2
      4th-order Runge Kutta:	rk4


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


  Short name: 		-c
  Long name:		--step_size
  Value type:		float
  Default value:	0.001
  Global variable:	dt
  Description:		Time step size
  Long description:
    This option sets the size of the time step, which is constant and
    shared by all particles.  It is wise to use option -s to specify a
    softening length that is significantly larger than the time step size.


  Short name: 		-d
  Long name:		--diagnostics_interval
  Value type:		float
  Default value:	1
  Global variable:	dt_dia
  Description:		Interval between diagnostics output
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
  Global variable:	dt_out
  Description:		Time interval between snapshot output
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
  Long name:		--duration
  Value type:		float
  Default value:	10
  Global variable:	dt_end
  Description:		Duration of the integration
  Long description:
    This option sets the duration t of the integration, the time period
    after which the integration will halt.  If the initial snapshot is
    marked to be at time t_init, the integration will halt at time
    t_final = t_init + t.


  Short name:		-i
  Long name:  		--init_out
  Value type:  		bool
  Global variable: 	init_out
  Description:		Output the initial snapshot
  Long description:
    If this flag is set to true, the initial snapshot will be output
    on the standard output channel, before integration is started.


  Short name:		-x
  Long name:  		--extra_diagnostics
  Value type:  		bool
  Global variable:	x_flag
  Description:		Extra diagnostics
  Long description:
    If this flag is set to true, the following extra diagnostics
    will be printed: 

      acceleration (for all integrators)


  END

parse_command_line(options_text)

include Math

nb = NBody.new
nb.simple_read
nb.evolve($method, $eps, $dt, $dt_dia, $dt_out, $dt_end, $init_out, $x_flag)
