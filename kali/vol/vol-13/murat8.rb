require "vector.rb"
require "clop.rb"
require "block_time.rb"

$n_longer = $n_shorter = $n_halve = 0

class Body

  attr_accessor :mass, :pos, :vel, :acc, :jerk,
                :pred_pos, :pred_vel,
                :time, :next_time, :new_dt, :old_dt

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
    @new_dt = 1.to_b
  end

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
      STDERR.print "predict_step: t = ", t.to_f, " > @next_time = "
      STDERR.print @next_time.to_f, "\n"
      exit
    end
    dt = (t - @time).to_f
    @pred_pos = @pos + @vel*dt + @acc*(dt*dt/2.0) + @jerk*(dt*dt*dt/6.0)
    @pred_vel = @vel + @acc*dt + @jerk*(dt*dt/2.0)
  end

  def correct_step(ba, t, dt_param)
    dt = (t - @time).to_f	
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
    @next_time = find_next_time(ba, dt_param)
    @old_dt = @new_dt
    @new_dt = @next_time - @time
    $n_longer += 1 if @new_dt > @old_dt
    $n_shorter += 1 if @new_dt < @old_dt
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

  def find_next_time(ba, dt_param)
    dt_estimate = ( collision_time_scale(ba) * dt_param ).to_b
    if dt_estimate.int > 0
      dt_block = 1.to_b
    else
      dt_block = dt_estimate.block
    end
#STDERR.print "dt_block = ", dt_block.to_s, " ; @time = ", @time.to_s, "\n"
    while not dt_block.commensurable?(@time)
      dt_block.halve
      $n_halve += 1
#STDERR.print ">> dt_block = ", dt_block.to_s, " ; @time = ", @time.to_s, "\n"
    end
    if dt_block == 0.to_b
      STDERR.print "find_next_time: dt_block = 0\n"
      exit
    end
    return @time + dt_block
  end

  def get_acc(body_array)
    a = @pos*0                              # null vector of the correct length
    body_array.each do |b|
      unless b == self
        r = b.pred_pos - @pred_pos    # NOTE: we use only predicted pos
        r2 = r*r
        r3 = r2*sqrt(r2)
        a += r*(b.mass/r3)
      end
    end
    a
  end    

  def get_jerk(body_array)
    j = @pos*0                              # null vector of the correct length
    body_array.each do |b|
      unless b == self
        r = b.pred_pos - @pred_pos    # NOTE: we use only predicted pos & vel
        r2 = r*r
        r3 = r2*sqrt(r2)
        v = b.pred_vel - @pred_vel
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

  def to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.join(", ") + "\n" +
    "   vel = " + @vel.join(", ") + "\n"
  end

  def pp                           # pretty print
    print to_s
  end

  def ppx(body_array)              # pretty print, with extra information (acc)
    STDERR.print to_s
    STDERR.print "   acc = " + acc(body_array).join(", ") + "\n"
    STDERR.print "   jerk = " + jerk(body_array).join(", ") + "\n"
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
      b.time = @time.copy            # deep copy
      b.next_time = b.find_next_time(@body, dt_param)
      b.old_dt = b.new_dt
      b.new_dt = b.next_time - @time
     end
  end

  def evolve(dt_param, delta_dia, delta_out, delta_end, init_out, x_flag)
    nsteps = 0
    startup(dt_param)
    write_diagnostics(nsteps, x_flag)
    t_dia = @time + delta_dia.to_b
    t_out = @time + delta_out.to_b
    t_end = @time + delta_end.to_b
    simple_print if init_out
    while @time < t_end
      np = find_next_particle
      @time = np.next_time
      if @time < t_end
        np.autonomous_step(@body, dt_param)
        nsteps += 1
#STDERR.print "                                nsteps = ", nsteps, " \n"
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
    next_time = 1e9.to_b          # this will still fit inside a 32-bit integer
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

  def write_diagnostics(nsteps, x_flag)
    etot = ekin + epot
    STDERR.print <<END
at time t = #{sprintf("%g", time.to_f)}, after #{nsteps} steps :
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
    @body.each{|b| b.ppx(@body)}
  end

  def simple_print
    print @body.size, "\n"
    printf("%24.16e\n", @time.to_f)
    @body.each{|b| b.simple_print}
  end

  def simple_read
    n = gets.to_i
    @time = gets.to_b
    for i in 0...n
      @body[i] = Body.new
      @body[i].simple_read
    end
  end

end

options_text= <<-END

  Description: Individual Time Step Hermite Code
  Long description:
    This program evolves an N-body code with a fourth-order Hermite Scheme,
    using individual time steps.  The only allowed time steps are powers of
    two, with a maximum value for the time steps of unity.  In other words,
    dt = 2^(-k) with k >= 0.
         The difference with the previous version is that we print information
    about the number of times that particles have made their time step longer
    or shorter, and also the number of times that they had to halve their
    time step because of incommensurability.
         Comparing murat8.rb and murat6.rb, we find that for -d0.01, the
    standard value, murat8.rb takes 0.1% more time steps than murat6.rb.
    For -d0.1, however, murat8.rb takes 1% more time steps than murat6.rb.

    (c) 2004, Piet Hut, Jun Makino, Murat Kaplan; see ACS at www.artcompsi.org

    example:
    ruby mkplummer3.rb -n 5 | ruby murat7.rb -t 1


  Short name: 		-d
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
nb.evolve($dt_param, $dt_dia, $dt_out, $dt_end, $init_out, $x_flag)
STDERR.print "$n_shorter = ", $n_shorter, " ; $n_longer ", $n_longer
STDERR.print " ; $n_halve = ", $n_halve, "\n"
