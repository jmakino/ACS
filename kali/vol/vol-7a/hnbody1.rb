require "vector.rb"
require "clop.rb"

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

  def jerk(body_array, eps)
    j = @pos*0                              # null vector of the correct length
    body_array.each do |b|
      unless b == self
        r = b.pos - @pos
        r2 = r*r + eps*eps
        r3 = r2*sqrt(r2)
        v = b.vel - @vel
        j += (v-r*(3*(r*v)/r2))*(b.mass/r3)
      end
    end
    j
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

  def ppx(body_array, eps)   # pretty print, with extra information (acc, jerk)
    STDERR.print to_s + "   acc = " + acc(body_array, eps).join(", ") + "\n"
    STDERR.print to_s + "   jerk = " + jerk(body_array, eps).join(", ") + "\n"
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

class Nbody

  attr_accessor :time, :body

  def initialize
    @body = []
  end

  def evolve(integration_method, eps, dt, dt_dia, dt_out, dt_end,
             init_out, x_flag)
    @dt = dt                                                                 #1
    @eps = eps                                                               #1
    nsteps = 0
    e_init
    write_diagnostics(nsteps, x_flag)
    t_dia = dt_dia - 0.5*dt
    t_out = dt_out - 0.5*dt
    t_end = dt_end - 0.5*dt

    simple_print if init_out

    while @time < t_end
      send(integration_method)
      @time += dt
      nsteps += 1
      if @time >= t_dia
        write_diagnostics(nsteps, x_flag)
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

  def yo6
    d = [0.784513610477560e0, 0.235573213359357e0, -1.17767998417887e0,
         1.31518632068391e0]
    old_dt = @dt
    for i in 0..2
      @dt = old_dt * d[i]
      leapfrog
    end
    @dt = old_dt * d[3]
    leapfrog
    for i in 0..2
      @dt = old_dt * d[2-i]
      leapfrog
    end
    @dt = old_dt
  end

  def hermite
    calc(" @old_pos = @pos ")
    calc(" @old_vel = @vel ")
    calc(" @old_acc = acc(ba,eps) ")
    calc(" @old_jerk = jerk(ba,eps) ")
    calc(" @pos += @vel*dt + @old_acc*(dt*dt/2.0) + @old_jerk*(dt*dt*dt/6.0) ")
    calc(" @vel += @old_acc*dt + @old_jerk*(dt*dt/2.0) ")
    calc(" @vel = @old_vel + (@old_acc + acc(ba,eps))*(dt/2.0) +
                      (@old_jerk - jerk(ba,eps))*(dt*dt/12.0) ")
    calc(" @pos = @old_pos + (@old_vel + vel)*(dt/2.0) +
                      (@old_acc - acc(ba,eps))*(dt*dt/12.0) ")
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

  def write_diagnostics(nsteps, x_flag)
    etot = ekin + epot
    STDERR.print <<END
at time t = #{sprintf("%g", time)}, after #{nsteps} steps :
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

options_text= <<-END

  Description: Constant Time Step Hermite Code
  Long description:
    This program evolves an N-body code with a fourth-order Hermite Scheme,
    or various other schemes such as forward Euler, leapfrog, or Runge-Kutta,
    using constant time steps, shared by all particles, where the size of
    of the time step is prescribed beforehand.  The program includes the
    option to provide softening for the potential.  This is essential for
    a constant time step code; the alternative, instead of softening, would
    be to use a variable time step algorithm.
    (c) 2004, Piet Hut and Jun Makino; see ACS at www.artcompsi.org


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
