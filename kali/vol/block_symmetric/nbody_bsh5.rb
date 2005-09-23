#!/usr/local/bin/ruby -w

require "nbody.rb"

class Body

  def calc(body_array, time_step, s)
    ba  = body_array
    dt = time_step
    eval(s)
  end

  def collision_time_scale(body_array)
    time_scale_sq = VERY_LARGE_NUMBER
    body_array.each do |b|
      unless b == self
        r = b.pos - @pos                                                     #2
	v = b.vel - @vel                                                     #2
	r2 = r*r
	v2 = v*v
        estimate_sq = r2 / v2            # [distance]^2/[velocity]^2 = [time]^2
        if time_scale_sq > estimate_sq
          time_scale_sq = estimate_sq
	end
	a = (@mass + b.mass)/r2
        estimate_sq = sqrt(r2)/a         # [distance]/[acceleration] = [time]^2
	if time_scale_sq > estimate_sq
          time_scale_sq = estimate_sq
	end
      end
    end
    sqrt(time_scale_sq)
  end

  def acc(body_array)
    a = @pos*0                              # null vector of the correct length
    body_array.each do |b|
      unless b == self
        r = b.pos - @pos
        r2 = r*r
        r3 = r2*sqrt(r2)
        a += r*(b.mass/r3)
      end
    end
    a
  end    

  def jerk(body_array)
    j = @pos*0                              # null vector of the correct length
    body_array.each do |b|
      unless b == self
        r = b.pos - @pos
        r2 = r*r
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

  def epot(body_array)             # potential energy
    p = 0
    body_array.each do |b|
      unless b == self
        r = b.pos - @pos
        p += -@mass*b.mass/sqrt(r*r)
      end
    end
    p
  end

end

class NBody

  def collision_time_scale
    time_scale = VERY_LARGE_NUMBER
    @body.each do |b|
      indiv_time_scale = b.collision_time_scale(@body)
      if time_scale > indiv_time_scale
        time_scale = indiv_time_scale
      end
    end
    time_scale
  end

  def force_power_2(x)
    newvalue = 1
    while newvalue > x
      newvalue *= 0.5
    end
    newvalue
  end
    
  def evolve(c)
    @nsteps = 0
    @e0 = ekin + epot
    write_diagnostics
    t_dia = @time + c.dt_dia                                                 #1
    t_out = @time + c.dt_out                                                 #1
    t_end = @time + c.dt_end                                                 #1
    acs_write if c.init_out_flag
    istep = 0
    logfile = (c.logfilename != "nofile") ? open(c.logfilename, "w+"):nil
    nsymmax = c.nsym

    oldndt = c.dt_param * collision_time_scale
    if c.blockstep
      olddt = force_power_2(oldndt)
    else
      olddt = oldndt
    end
    while @time < t_end
# at this point, we should have
#       oldndt: old natural dt
#       olddt:  actual previos timestep taken
      @dt = olddt
#      @dt *= 2 if @time % (2*@dt) == 0
      @oldbody = @body.deep_copy
      newndt = 0
      c.nsym.times{|i|
        @body = @oldbody.deep_copy
        send(c.method)
        newndt = c.dt_param * collision_time_scale
        if ( i < c.nsym - 1)
          if c.blockstep
            @dt = force_power_2((oldndt+newndt)*0.5)
            @dt *= 0.5 while @time % (@dt) != 0
          else
            @dt = (oldndt+newndt)*0.5
          end
        end
      }
      oldndt = newndt
      olddt = @dt
      @time += @dt
      @nsteps += 1
      istep += 1
      if logfile and istep == c.energyloginterval
        istep = 0
        write_energy_log(logfile)
      end
      if @time >= t_dia
        write_diagnostics
        t_dia += c.dt_dia
      end
      if @time >= t_out - 1.0/VERY_LARGE_NUMBER
        acs_write
        t_out += c.dt_out
      end
    end
  end

  def calc(s)
    @body.each{|b| b.calc(@body, @dt, s)}
  end

  def forward
    calc(" @old_acc = acc(ba) ")
    calc(" @pos += @vel*dt ")
    calc(" @vel += @old_acc*dt ")
  end

  def leapfrog
    calc(" @vel += acc(ba)*0.5*dt ")
    calc(" @pos += @vel*dt ")
    calc(" @vel += acc(ba)*0.5*dt ")
  end

  def rk2
    calc(" @old_pos = @pos ")
    calc(" @half_vel = @vel + acc(ba)*0.5*dt ")
    calc(" @pos += @vel*0.5*dt ")
    calc(" @vel += acc(ba)*dt ")
    calc(" @pos = @old_pos + @half_vel*dt ")
  end

  def rk4
    calc(" @old_pos = @pos ")
    calc(" @a0 = acc(ba) ")
    calc(" @pos = @old_pos + @vel*0.5*dt + @a0*0.125*dt*dt ")
    calc(" @a1 = acc(ba) ")
    calc(" @pos = @old_pos + @vel*dt + @a1*0.5*dt*dt ")
    calc(" @a2 = acc(ba) ")
    calc(" @pos = @old_pos + @vel*dt + (@a0+@a1*2)*(1/6.0)*dt*dt ")
    calc(" @vel += (@a0+@a1*4+@a2)*(1/6.0)*dt ")
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

  def hermite
    calc(" @old_pos = @pos ")
    calc(" @old_vel = @vel ")
    calc(" @old_acc = acc(ba) ")
    calc(" @old_jerk = jerk(ba) ")
    calc(" @pos += @vel*dt + @old_acc*(dt*dt/2.0) + @old_jerk*(dt*dt*dt/6.0) ")
    calc(" @vel += @old_acc*dt + @old_jerk*(dt*dt/2.0) ")
    calc(" @vel = @old_vel + (@old_acc + acc(ba))*(dt/2.0) +
                      (@old_jerk - jerk(ba))*(dt*dt/12.0) ")
    calc(" @pos = @old_pos + (@old_vel + vel)*(dt/2.0) +
                      (@old_acc - acc(ba))*(dt*dt/12.0) ")
  end

  def ekin                        # kinetic energy
    e = 0
    @body.each{|b| e += b.ekin}
    e
  end

  def epot                        # potential energy
    e = 0
    @body.each{|b| e += b.epot(@body)}
    e/2                           # pairwise potentials were counted twice
  end

  def write_diagnostics
    etot = ekin + epot
    STDERR.print <<END
at time t = #{sprintf("%g", @time)}, after #{@nsteps} steps :
  E_kin = #{sprintf("%.3g", ekin)} ,\
 E_pot =  #{sprintf("%.3g", epot)} ,\
 E_tot = #{sprintf("%.3g", etot)}
             E_tot - E_init = #{sprintf("%.3g", etot - @e0)}
  (E_tot - E_init) / E_init = #{sprintf("%.3g", (etot - @e0)/@e0 )}
END
  end
  def write_energy_log(file)
    etot = ekin + epot
    file.print " #{sprintf("%g", @time)}  #{sprintf("%.3g", etot)}" 
    file.print "  #{sprintf("%.3g", etot - @e0)}  #{sprintf("%.3g", (etot - @e0)/@e0 )}\n"
  end

end

options_text = <<-END

  Description: Shared Time Step Code
  Long description:
    This program evolves an N-body code with a fourth-order Hermite Scheme,
    or various other schemes such as forward Euler, leapfrog, or Runge-Kutta,
    using variable time steps, shared by all particles, where the size of
    of the time step is determined adaptively.

    (c) 2005, Piet Hut and Jun Makino; see ACS at www.artcompsi.org

    example:
    kali mkplummer.rb -n 4 -s 1 | kali #{$0} -t 1 > /dev/null


  Short name:		-l
  Long name:  		--energy_log_file
  Value type:  		string
  Default value:	nofile
  Variable name:	logfilename
  Description:		Name of the detailed log file
  Long description:
    This option specfies the name of the log file to which
    energy error data is printed


  Short name:		-k
  Long name:  		--energy_log_interval
  Value type:  		int
  Default value:	10
  Variable name:	energyloginterval
  Description:		interval for energy log output
  Long description:
    This option specfies the energy log interval (once every x timesteps, 
    where x is the value of this option)


  Short name:		-s
  Long name:  		--symmetric_iteraction
  Value type:  		int
  Default value:	1
  Variable name:	nsym
  Description:		Iteration count for symmetrization
  Long description:
    This option specfies the iteration count for implicit timestep 
    symmetrization


  Short name:		-g
  Long name:  		--integration_method
  Value type:  		string
  Default value:	leapfrog
  Variable name:	method
  Description:		Integration method
  Long description:
    This option receives a string, containing the name of the integration
    method that will be used.  Example: "-g hermite" .


  Short name: 		-c
  Long name:		--step_size_control
  Value type:		float
  Default value:	0.01
  Variable name:	dt_param
  Description:		Parameter to determine time step size
  Long description:
    This option sets the step size control parameter dt_param << 1.  Before
    each new time step, we first calculate the time scale t_scale on which
    changes are expected to happen, such as close encounters or significant
    changes in velocity.  The new shared time step is then given as the
    product t_scale * dt_param << t_scale.


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
  Variable name:	init_out_flag
  Description:		Output the initial snapshot
  Long description:
    If this flag is set to true, the initial snapshot will be output
    on the standard output channel, before integration is started.



  Short name:		-b
  Long name:  		--blockstep
  Value type:  		bool
  Variable name:	blockstep
  Description:		blockstep flag
  Long description:
    If this flag is set to true, we use some blockstep algorithm.


  END

clop = parse_command_line(options_text)

nb = ACS_IO.acs_read(NBody)
nb.evolve(clop)
