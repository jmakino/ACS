require "acs.rb"

class Worldpoint

  attr_accessor :mass, :pos, :vel, :acc, :jerk, :time

  def read
    @mass = gets.to_f
    @pos = gets.split.map{|x| x.to_f}.to_v
    @vel = gets.split.map{|x| x.to_f}.to_v
  end

  def write
    printf("%24.16e\n", @mass)
    @pos.each{|x| printf("%24.16e", x)}; print "\n"
    @vel.each{|x| printf("%24.16e", x)}; print "\n"
  end

end

class Worldline

  attr_accessor  :worldpoint

  def initialize
    @worldpoint = []
  end

#  def read                               # for self-documenting data
#  end

  def read_worldpoint
    @worldpoint[0] = Worldpoint.new
    @worldpoint[0].read
  end

  def write_worldpoint
    @worldpoint.last.write
  end

  def write
    print "    Begin World Line\n"               # for now; 
    @worldpoint.each do |wp|
      print "    at time ", wp.time, " :\n"
      wp.write
    end
    print "    End World Line\n"                 # for now; 
  end

end

class Worldera

  attr_accessor  :start_time, :end_time  # end of container, not necessarily of
                                         # of the contents, i.e. the worldlines
  def initialize
    @worldline = []
    @nsteps = 0
  end

  def startup(dt_param)
  end

  def find_next_worldline
  end

  def evolve(c, t_dia, t_out, t_end)
    startup(c.dt_param) if not @worldline[0].worldpoint[0].acc
    @end_time = @start_time + c.dt_era
    time = @start_time
    while time < t_end and time < @end_time
      nwl = find_next_worldline
      time = nwl.next_time
      if (time < t_end)
        nwl.extend(@wordline, c.dt_param)
        @nsteps += 1
      end
      if time >= t_dia
        write_diagnostics(t_dia, c.x_flag)
        t_dia += c.dt_dia
      end
      if @time >= t_out
        sync(t_out, c.dt_param)    # we are now syncing twice, if t_dia = t_out
        nsteps += @body.size
        simple_print
        t_out += c.dt_out
      end
    end
  end

#  def read          # for self-documenting data, either Worldera or Snapshot
#  end

  def read_snapshot
    n = gets.to_i
    @start_time = gets.to_f
    for i in 0...n
      @worldline[i] = Worldline.new
      @worldline[i].read_worldpoint         # for now; "read" w. self-doc. data
      @worldline[i].worldpoint[0].time = @start_time
    end
  end

  def write_snapshot
    print @worldline.size, "\n"
    printf("%24.16e\n", @worldline[0].worldpoint.last.time)
    @worldline.each do |wl|
      wl.write_worldpoint
    end
  end

  def write
    print "  Begin Era\n"               # for now; 
    @worldline.each do |wl|
      wl.write
    end
    print "  End Era\n"                 # for now; 
  end

end

class World

  def evolve(c)
    time = @era.start_time
    @era.write_diagnostics(time, c.x_flag)
    t_dia = time + c.dt_dia
    t_out = time + c.dt_out
    t_end = time + c.dt_end
    @era.write_snapshot if c.init_out
    while @era.start_time < t_end
      @new_era = @era.evolve(c, t_dia, t_out, t_end)
      @old_era = @era
      @era = @new_era
    end

  def read_snapshot
    @era = Worldera.new
    @era.read_snapshot
  end

  def write_snapshot
    @era.write_snapshot
  end

  def write
    print "Begin World\n"               # for now; 
    @era.write
    print "End World\n"               # for now; 
  end

end

#class Worldsnapshot
#end

options_text= <<-END

  Description: Individual Time Step Hermite Code
  Long description:
    This program evolves an N-body code with a fourth-order Hermite Scheme,
    using individual time steps.
    (c) 2004, Piet Hut, Jun Makino, Murat Kaplan; see ACS at www.artcompsi.org

    example:
    ruby mkplummer3.rb -n 5 | ruby #{$0} -t 1


  Short name: 		-s
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
  Long name:		--era_length
  Value type:		float
  Default value:	0.01
  Variable name:	dt_era
  Description:		Interval between diagnostics output
  Long description:
    This option sets the time interval between diagnostics output,
    which will appear on the standard error channel.


  Short name: 		-d
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
  Long name:		--time_period
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

clop = parse_command_line(options_text, true)

w = World.new
w.read_snapshot                            # for now; "read" w. self-doc. data
w.write_snapshot
#w.write
w.evolve(clop)
