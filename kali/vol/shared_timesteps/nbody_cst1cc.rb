#!/usr/local/bin/ruby -w

require "nbody.rb"

class Body

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

  attr_accessor :a1

  def gacc(body_array, eps)
    ga = @pos*0                             # null vector of the correct length
    body_array.each do |b|
      unless b == self
        r = b.pos - @pos
        r2 = r*r + eps*eps
        r3 = r2*sqrt(r2)
        a = b.a1 - @a1
        ga += 2*(b.mass/r3)*(a - 3*((r*a)/r2)*r)
      end
    end
    ga
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

end

class NBody

  def evolve(integration_method, t0, alpha, eps, dt, dt_dia, dt_out, dt_end,
             init_out)
    $alpha = alpha
    $t0 = t0
    @dt = dt
    @eps = eps
    @nsteps = 0
    e_init
    write_diagnostics
    t_dia = @time + dt_dia - 0.5*dt                                          #1
    t_out = @time + dt_out - 0.5*dt                                          #1
    t_end = @time + dt_end - 0.5*dt                                          #1

    acs_write if init_out

    while @time < t_end
      send(integration_method)
      @time += @dt
      @nsteps += 1
      if @time >= t_dia
        write_diagnostics
        t_dia += dt_dia
      end
      if @time >= t_out
        acs_write
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

  def cc
    calc(" @vel += (1/6.0)*acc(ba,eps)*dt ")
    calc(" @pos += 0.5*@vel*dt ")
    calc(" @a1 = acc(ba,eps) ")
    calc(" @a = @a1 + (1/48.0)*gacc(ba,eps)*dt**2 ")
    calc(" @vel += (2/3.0)*@a*dt ")
    calc(" @pos += 0.5*@vel*dt ")
    calc(" @vel += (1/6.0)*acc(ba,eps)*dt ")
  end

  def cct
    calc(" @t0 = $t0 ")
    calc(" @alpha = $alpha ")
    calc(" @v1 = 1.0/(6*(1-2*@t0)**2) ")
    calc(" @t1 = 0.5 - @t0 ")
    calc(" @v2 = 1 - 2*@v1 ")
    calc(" @u0 = (1/12.0)*(1 + (@v1-1)/(1-2*@t0)) ")
    calc(" @pos += @t0*@vel*dt ")
    calc(" @a1 = acc(ba,eps) ")
    calc(" @a = @v1*@a1 + 0.5*@alpha*@u0*gacc(ba,eps)*dt**2 ")
    calc(" @vel += @a*dt ")
    calc(" @pos += @t1*@vel*dt ")
    calc(" @a1 = acc(ba,eps) ")
    calc(" @a = @v2*@a1 + (1-@alpha)*@u0*gacc(ba,eps)*dt**2 ")
    calc(" @vel += @a*dt ")
    calc(" @pos += @t1*@vel*dt ")
    calc(" @a1 = acc(ba,eps) ")
    calc(" @a = @v1*@a1 + 0.5*@alpha*@u0*gacc(ba,eps)*dt**2 ")
    calc(" @vel += @a*dt ")
    calc(" @pos += @t0*@vel*dt ")
  end

  def yo2
    leapfrog
  end

  def yo4
    d = [1.351207191959657, -1.702414383919315]
    old_dt = @dt
    @dt = old_dt * d[0]
    leapfrog
    @dt = old_dt * d[1]
    leapfrog
    @dt = old_dt * d[0]
    leapfrog
    @dt = old_dt
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

  def yo8
    d = [0.104242620869991e1, 0.182020630970714e1, 0.157739928123617e0, 
         0.244002732616735e1, -0.716989419708120e-2, -0.244699182370524e1, 
         -0.161582374150097e1, -0.17808286265894516e1]
    old_dt = @dt
    for i in 0..6
      @dt = old_dt * d[i]
      leapfrog
    end
    @dt = old_dt * d[7]
    leapfrog
    for i in 0..6
      @dt = old_dt * d[6-i]
      leapfrog
    end
    @dt = old_dt
  end

  def ms2
    if @nsteps == 0
      calc(" @prev_acc = acc(ba,eps) ")
      rk2
    else
      calc(" @old_acc = acc(ba,eps) ")
      calc(" @jdt = @old_acc - @prev_acc ")
      calc(" @pos += @vel*dt + @old_acc*0.5*dt*dt ")
      calc(" @vel += @old_acc*dt + @jdt*0.5*dt")
      calc(" @prev_acc = @old_acc ")
    end
  end

  def ms4
    if @nsteps == 0
      calc(" @ap3 = acc(ba,eps) ")
      rk4
    elsif @nsteps == 1
      calc(" @ap2 = acc(ba,eps) ")
      rk4
    elsif @nsteps == 2
      calc(" @ap1 = acc(ba,eps) ")
      rk4
    else
      calc(" @ap0 = acc(ba,eps) ")
      calc(" @jdt = @ap0*(11.0/6.0) - @ap1*3 + @ap2*1.5 - @ap3/3.0 ")
      calc(" @sdt2 = @ap0*2 - @ap1*5 + @ap2*4 - @ap3 ")
      calc(" @cdt3 = @ap0 - @ap1*3 + @ap2*3 - @ap3 ")
      calc(" @pos += (@vel+(@ap0+ (@jdt+@sdt2/4)/3)*dt/2)*dt ")
      calc(" @vel += (@ap0+(@jdt+(@sdt2+@cdt3/4)/3)/2)*dt ")
      calc(" @ap3 = @ap2 ")
      calc(" @ap2 = @ap1 ")
      calc(" @ap1 = @ap0 ")
    end
  end

  def ms6
    if @nsteps == 0
      calc(" @a5 = acc(ba,eps) ")
      yo6
    elsif @nsteps == 1
      calc(" @a4 = acc(ba,eps) ")
      yo6
    elsif @nsteps == 2
      calc(" @a3 = acc(ba,eps) ")
      yo6
    elsif @nsteps == 3
      calc(" @a2 = acc(ba,eps) ")
      yo6
    elsif @nsteps == 4
      calc(" @a1 = acc(ba,eps) ")
      yo6
    else
      calc(" @a0 = acc(ba,eps) ")
      calc(" @j=(@a0*137 - @a1*300 + @a2*300 - @a3*200 + @a4*75 - @a5*12)/60 ")
      calc(" @s =(@a0*45 - @a1*154 + @a2*214 - @a3*156 + @a4*61 - @a5*10)/12 ")
      calc(" @c = (@a0*17 - @a1*71 + @a2*118 - @a3*98 + @a4*41 - @a5*7)/4 ")
      calc(" @p = @a0*3 - @a1*14 + @a2*26 - @a3*24 + @a4*11 - @a5*2 ")
      calc(" @x = @a0 - @a1*5 + @a2*10 - @a3*10 + @a4*5 - @a5 ")
      calc(" @pos += (@vel+(@a0+(@j+(@s+(@c+@p/6)/5)/4)/3)*dt/2)*dt ")
      calc(" @vel += (@a0 +(@j +(@s+(@c+(@p+@x/6)/5)/4)/3)/2)*dt ")
      calc(" @a5 = @a4 ")
      calc(" @a4 = @a3 ")
      calc(" @a3 = @a2 ")
      calc(" @a2 = @a1 ")
      calc(" @a1 = @a0 ")
    end
  end

  def ms8
    if @nsteps == 0
      calc(" @a7 = acc(ba,eps) ")
      yo8
    elsif @nsteps == 1
      calc(" @a6 = acc(ba,eps) ")
      yo8
    elsif @nsteps == 2
      calc(" @a5 = acc(ba,eps) ")
      yo8
    elsif @nsteps == 3
      calc(" @a4 = acc(ba,eps) ")
      yo8
    elsif @nsteps == 4
      calc(" @a3 = acc(ba,eps) ")
      yo8
    elsif @nsteps == 5
      calc(" @a2 = acc(ba,eps) ")
      yo8
    elsif @nsteps == 6
      calc(" @a1 = acc(ba,eps) ")
      yo8
    else
      calc(" @a0 = acc(ba,eps) ")
      calc(" @j = (@a0*1089 - @a1*2940 + @a2*4410 - @a3*4900 +
                   @a4*3675 - @a5*1764 + @a6*490 - @a7*60)/420 ")
      calc(" @s = (@a0*938 - @a1*4014 + @a2*7911 - @a3*9490 +
                   @a4*7380 - @a5*3618 + @a6*1019 - @a7*126)/180 ")
      calc(" @c = (@a0*967 - @a1*5104 + @a2*11787 - @a3*15560 + 
                   @a4*12725 - @a5*6432 + @a6*1849 - @a7*232)/120 ")
      calc(" @p = (@a0*56 - @a1*333 + @a2*852 - @a3*1219 +
                   @a4*1056 - @a5*555 + @a6*164 - @a7*21)/6 ")
      calc(" @x = (@a0*46 - @a1*295 + @a2*810 - @a3*1235 +
                   @a4*1130 - @a5*621 + @a6*190 - @a7*25)/6 ")
      calc(" @y = @a0*4 - @a1*27 + @a2*78 - @a3*125 + @a4*120 - @a5*69 +
                  @a6*22 - @a7*3 ")
      calc(" @z = @a0 - @a1*7 + @a2*21 - @a3*35 + @a4*35 - @a5*21 +
                  @a6*7 - @a7 ")
      calc(" @pos +=
               (@vel+(@a0+(@j+(@s+(@c+(@p+(@x+@y/8)/7)/6)/5)/4)/3)*dt/2)*dt ")
      calc(" @vel +=
               (@a0 +(@j +(@s+(@c+(@p+(@x+(@y+@z/8)/7)/6)/5)/4)/3)/2)*dt ")
      calc(" @a7 = @a6 ")
      calc(" @a6 = @a5 ")
      calc(" @a5 = @a4 ")
      calc(" @a4 = @a3 ")
      calc(" @a3 = @a2 ")
      calc(" @a2 = @a1 ")
      calc(" @a1 = @a0 ")
    end
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

  def write_diagnostics
    etot = ekin + epot
    STDERR.print <<END
at time t = #{sprintf("%g", time)}, after #{@nsteps} steps :
  E_kin = #{sprintf("%.3g", ekin)} ,\
 E_pot =  #{sprintf("%.3g", epot)} ,\
 E_tot = #{sprintf("%.3g", etot)}
             E_tot - E_init = #{sprintf("%.3g", etot - @e0)}
  (E_tot - E_init) / E_init = #{sprintf("%.3g", (etot - @e0)/@e0 )}
END
  end

end

options_text = <<-END

  Description: Constant Time Step Code
  Long description:
    This program evolves an N-body code with a fourth-order Hermite Scheme,
    or various other schemes such as forward Euler, leapfrog, or Runge-Kutta,
    using constant time steps, shared by all particles, where the size of
    of the time step is prescribed beforehand.  The program includes the
    option to provide softening for the potential.  This is essential for
    a constant time step code; the alternative, instead of softening, would
    be to use a variable time step algorithm.

    (c) 2005, Piet Hut and Jun Makino; see ACS at www.artcompsi.org

    example:
    kali mkplummer.rb -n 4 -s 1 | kali #{$0} -t 1 > /dev/null


  Short name:		-g
  Long name:  		--integration_method
  Value type:  		string
  Default value:	hermite
  Variable name:	method
  Description:		Integration method
  Long description:
    This option receives a string, containing the name of the integration
    method that will be used.  Example: "-g hermite" .


  Long name:  		--t0
  Value type:  		float
  Default value:	0.0
  Variable name:	t0
  Description:		Integration method parameter
  Long description:
    This option sets the value of a free parameter `t0', for use in
    an integration algorithm that allows such a free parameter


  Long name:  		--alpha
  Value type:  		float
  Default value:	0.0
  Variable name:	alpha
  Description:		Integration method parameter
  Long description:
    This option sets the value of a free parameter `alpha', for use in
    an integration algorithm that allows such a free parameter


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


  Short name: 		-c
  Long name:		--step_size
  Value type:		float
  Default value:	0.001
  Variable name:	dt
  Description:		Time step size
  Long description:
    This option sets the size of the time step, which is constant and
    shared by all particles.  It is wise to use option -s to specify a
    softening length that is significantly larger than the time step size.


  Short name: 		-d
  Long name:		--diagnostics_interval
  Value type:		float
  Default value:	1
  Variable name:	dt_dia
  Description:		Interval between diagnostics output
  Long description:
    The time interval between successive diagnostics output.
    The diagnostics include the kinetic and potential energy,
    and the absolute and relative drift of total energy, since
    the beginning of the integration.
        These diagnostics appear on the standard error stream.


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
    for all particles in an N-body system, in ACS format.


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


  END

parse_command_line(options_text, true)

nb = ACS_IO.acs_read(NBody)
nb.evolve($method, $t0, $alpha, $eps, $dt, $dt_dia, $dt_out, $dt_end,
          $init_out)
