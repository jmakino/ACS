require "vector.rb"
require "clop.rb"
require "block_time.rb"

class Body

  attr_accessor :time, :next_time

  def autonomous_step(ba, dt_param)
    take_one_step(ba, @next_time, dt_param)
  end

  def forced_step(ba, t, dt_param)
    take_one_step(ba, t, dt_param)
  end

  def take_one_step(ba, t, dt_param)
    ba.each do |b|
      b.predict_step(t)
    end
    correct_step(ba, t, dt_param)
  end

  def predict_step(t)
    if t > @next_time
      STDERR.print "predict_step: t = ", t, " > @next_time = "
      STDERR.print @next_time, "\n"
      exit
    end
    dt = t - @time
    @pred_pos = @pos + @vel*dt + @acc*(dt*dt/2.0) + @jerk*(dt*dt*dt/6.0)
    @pred_vel = @vel + @acc*dt + @jerk*(dt*dt/2.0)
  end

  def correct_step(ba, t, dt_param)
    dt = t - @time
    new_jerk = get_jerk(ba)
    new_acc = get_acc(ba)
    new_vel = @vel + (@acc + new_acc)*(dt/2.0) +        # first compute new_vel
                      (@jerk - new_jerk)*(dt*dt/12.0)   # since new_vel is used
    new_pos = @pos + (@vel + new_vel)*(dt/2.0) +        # to compute new_pos
                      (@acc - new_acc)*(dt*dt/12.0)
    @pos = new_pos
    @vel = new_vel
    @acc = new_acc
    @jerk = new_jerk
    @pred_pos = @pos
    @pred_vel = @vel
    @time = t
    @next_time = @time + find_block_time_step(ba, dt_param)
  end  

  def collision_time_scale(body_array)
    time_scale_sq = 1e30                 # square of time scale value
    body_array.each do |b|
      unless b == self
        r = b.pred_pos - @pred_pos    # NOTE: we use only predicted pos & vel
	v = b.pred_vel - @pred_vel
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
    sqrt(time_scale_sq)                  # time scale value
  end

  def find_block_time_step(ba, dt_param)
    dt_estimate = (collision_time_scale(ba)) * dt_param
    dt_block = 1.0                         # maximum value: 2^(-k), with k >= 0
    while dt_block > dt_estimate
      dt_block /= 2.0
      if dt_block < 1e-14             # too small for double precision accuracy
        STDERR.print "find_block_time_step: dt_block = ", dt_block
        STDERR.print " < 1e-14\n"
        exit
      end
    end
    dt_block
  end

end

class Nbody

  attr_accessor :time

  def startup(dt_param)
    @e0 = ekin + epot                      # initial total energy
    @body.each do |b|
      b.pred_pos = b.pos             # first compute pred_pos & pred_vel
      b.pred_vel = b.vel
    end
    @body.each do |b|
      b.acc = b.get_acc(@body)       # acc & jerk use pred_pos & pred_vel
      b.jerk = b.get_jerk(@body)
    end
    @body.each do |b|
      b.time = @time
      b.next_time = @time + b.find_block_time_step(@body, dt_param)
     end
  end

  def evolve(dt_param, delta_dia, delta_out, delta_end, init_out, x_flag)
    nsteps = 0
    startup(dt_param)
    write_diagnostics(nsteps, x_flag)
    t_dia = @time + delta_dia
    t_out = @time + delta_out
    t_end = @time + delta_end
    simple_print if init_out
    while @time < t_end
      np = find_next_particle
      @time = np.next_time
      if (@time < t_end)
        np.autonomous_step(@body, dt_param)
        nsteps += 1
      end
      if @time >= t_dia
        sync(t_dia, dt_param)
        nsteps += @body.size
        write_diagnostics(nsteps, x_flag)
        t_dia += delta_dia
      end
      if @time >= t_out
        sync(t_out, dt_param)      # we are now syncing twice, if t_dia = t_out
        nsteps += @body.size
        simple_print
        t_out += delta_out
      end
    end
  end

  def find_next_particle
    next_time = 1e30
    next_particle = nil
    @body.each do |b|
      if next_time > b.next_time
        next_time = b.next_time
        next_particle = b
      end
    end
    next_particle
  end

  def sync(t, dt_param)
    @body.each{|b| b.forced_step(@body, t, dt_param)}
    @time = t
  end

end

options_text= <<-END

  Description: Individual Time Step Hermite Code
  Long description:
    This program evolves an N-body code with a fourth-order Hermite Scheme,
    using individual time steps.  The only allowed time steps are powers of
    two, with a maximum value for the time steps of unity.  In other words,
    dt = 2^(-k) with k >= 0.  This time, we also require lineup of smaller
    time steps at the boundaries of larger time steps.  This means that for
    each particle k is only allowed to increase when the total time t is an
    even multiple of the time step dt, i.e. t/2^(k) is even, or t/2^(1-k)
    is an integer.

    (c) 2004, Piet Hut, Jun Makino, Murat Kaplan; see ACS at www.artcompsi.org


  Short name: 		-d
  Long name:		--step_size_control
  Value type:		float
  Default value:	0.01
  Global variable:	dt_param
  Description:		Parameter to determine time step size
  Long description:
    This option sets the step size control parameter dt_param << 1.  Before
    each new time step, we first calculate the time scale t_scale on which
    changes are expected to happen, such as close encounters or significant
    changes in velocity.  The new shared time step is then given as the
    product t_scale * dt_param << t_scale.


  Short name: 		-e
  Long name:		--diagnostics_interval
  Value type:		float
  Default value:	1
  Global variable:	dt_dia
  Description:		Interval between diagnostics output
  Long description:
    This option sets the time interval between diagnostics output,
    which will appear on the standard error channel.


  Short name: 		-o
  Long name:		--output_interval
  Value type:		float
  Default value:	1
  Global variable:	dt_out
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
  Global variable:	dt_end
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
  Global variable:	init_out
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
      jerk (for the Hermite integrator)


  END

parse_command_line(options_text)

include Math

nb = Nbody.new
nb.simple_read
nb.evolve($dt_param, $dt_dia, $dt_out, $dt_end, $init_out, $x_flag)
