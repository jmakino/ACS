#!/usr/local/bin/ruby -w

require "nbody.rb"

class WorldPoint < Body

  attr_accessor :acc, :jerk, :snap, :crackle,
                :time, :next_time, :nsteps,
                :minstep, :maxstep,
                :body_id

  def forward_setup(time)
    @time = @next_time = time
    @acc = @pos*0
    @nsteps = 0
    @minstep = VERY_LARGE_NUMBER
    @maxstep = 0
  end

  def leapfrog_setup(time)
    @time = @next_time = time
    @acc = @pos*0
  end

  def hermite_setup(time)
    @time = @next_time = time
    @acc = @pos*0
    @jerk = @pos*0
  end

  def ms4_setup(time)
    @time = @next_time = time
    @acc = @pos*0
    @jerk = @pos*0
    @snap = @pos*0
    @crackle = @pos*0
    @pop = @pos*0
  end

  def forward_startup(acc, timescale, dt_param, dt_max)
    @acc = acc
    dt = timescale * dt_param
    dt = dt_max if dt > dt_max
    @next_time = @time + dt
  end    

  def leapfrog_startup(acc, timescale, dt_param, dt_max)
    @acc = acc
    dt = timescale * dt_param
    dt = dt_max if dt > dt_max
    @next_time = @time + dt
  end    

  def hermite_startup(acc, jerk, timescale, dt_param, dt_max)
    @acc = acc
    @jerk = jerk
    dt = timescale * dt_param
    dt = dt_max if dt > dt_max
    @next_time = @time + dt
  end    

  def ms4_startup_first_round(acc, jerk, timescale, dt_param, dt_max)
    @acc = acc
    @jerk = jerk
    dt = timescale * dt_param
    dt = dt_max if dt > dt_max
    @next_time = @time + dt
  end    

  def ms4_startup_second_round(snap, crackle)
    @snap = snap
    @crackle = crackle
  end    

  def forward_correct(old_point, acc, timescale, dt_param, dt_max)
    @acc = acc
    dt = timescale * dt_param
    dt = dt_max if dt > dt_max
    @next_time = @time + dt
    dt = @time - old_point.time
    @vel = old_point.vel + (old_point.acc)*dt
    @pos = old_point.pos + (old_point.vel)*dt
    admin(old_point.time)
    self
  end

  def leapfrog_correct(old_point, acc, timescale, dt_param, dt_max)
    @acc = acc
    dt = timescale * dt_param
    dt = dt_max if dt > dt_max
    @next_time = @time + dt
    dt = @time - old_point.time
    @vel = old_point.vel + (1/2.0)*(old_point.acc + @acc)*dt
    @pos = old_point.pos + (1/2.0)*(old_point.vel + @vel)*dt
    admin(old_point.time)
    self
  end

  def hermite_correct(old_point, acc, jerk, timescale, dt_param, dt_max)
    @acc = acc
    @jerk = jerk
    dt = timescale * dt_param
    dt = dt_max if dt > dt_max
    @next_time = @time + dt
    dt = @time - old_point.time
    @vel = old_point.vel + (1/2.0)*(old_point.acc + @acc)*dt +
                           (1/12.0)*(old_point.jerk - @jerk)*dt**2
    @pos = old_point.pos + (1/2.0)*(old_point.vel + @vel)*dt +
                           (1/12.0)*(old_point.acc - @acc)*dt**2
    admin(old_point.time)
    self
  end

  def ms4_correct(wa, acc, timescale, dt_param, dt_max)
    d = [[acc]]
    t = [@time]
    dt = [0]
    wa.each do |wp|
      d[0].push(wp.acc)
      d.push([])
      t.push(wp.time)
      dt.push(@time - wp.time)
    end
    for k in (1..wa.size)
      for i in (0..wa.size-k)
        d[k][i] = (d[k-1][i] - d[k-1][i+1])/(t[i]-t[i+k])
      end
    end
    @pop = 24*d[4][0]
    @crackle = 6*(d[3][0] + d[4][0]*(dt[1] + dt[2] + dt[3]))
    @snap = 2*(d[2][0] + d[3][0]*(dt[1]+dt[2]) +
                         d[4][0]*(dt[1]*dt[2] + dt[2]*dt[3] + dt[3]*dt[1]))
    @jerk = d[1][0] + dt[1]*( d[2][0] + dt[2]*( d[3][0] + dt[3]*d[4][0] ) )
    @acc = acc
    dt = wa[0].time - @time
    @vel = wa[0].vel - ( @acc*dt + (1/2.0)*@jerk*dt**2 + (1/6.0)*@snap*dt**3 +
                         (1/24.0)*@crackle*dt**4 + (1/120.0)*@pop*dt**5 )
    @pos = wa[0].pos - ( @vel*dt + (1/2.0)*@acc*dt**2 + (1/6.0)*@jerk*dt**3 +
                         (1/24.0)*@snap*dt**4 + (1/120.0)*@crackle*dt**5 +
                         (1/720.0)*@pop*dt**6 )
    dt = timescale * dt_param
    dt = dt_max if dt > dt_max
    @next_time = @time + dt
    admin(wa[0].time)
    self
  end

  def admin(old_time)
    dt = @time - old_time
    @maxstep = dt if @maxstep < dt
    @minstep = dt if @minstep > dt
    @nsteps = @nsteps + 1
  end

  def extrapolate(t, method)
    eval("#{method}_extrapolate(t)")
  end

  def interpolate(other, t, method)
    eval("#{method}_interpolate(other, t)")
  end

  def forward_extrapolate(t)
    if t > @next_time
      raise "t = " + t.to_s + " > @next_time = " + @next_time.to_s + "\n"
    end
    wp = WorldPoint.new
    wp.minstep = @minstep
    wp.maxstep = @maxstep
    wp.nsteps = @nsteps
    wp.mass = @mass
    wp.time = t
    dt = t - @time
    wp.pos = @pos + @vel*dt
    wp.vel = @vel + @acc*dt
    wp
  end

  def leapfrog_extrapolate(t)
    if t > @next_time
      raise "t = " + t.to_s + " > @next_time = " + @next_time.to_s + "\n"
    end
    wp = WorldPoint.new
    wp.minstep = @minstep
    wp.maxstep = @maxstep
    wp.nsteps = @nsteps
    wp.mass = @mass
    wp.time = t
    dt = t - @time
    wp.pos = @pos + @vel*dt + (1/2.0)*@acc*dt**2
    wp.vel = @vel + @acc*dt
    wp
  end

  def hermite_extrapolate(t)
    if t > @next_time
      raise "t = " + t.to_s + " > @next_time = " + @next_time.to_s + "\n"
    end
    wp = WorldPoint.new
    wp.minstep = @minstep
    wp.maxstep = @maxstep
    wp.nsteps = @nsteps
    wp.mass = @mass
    wp.time = t
    dt = t - @time
    wp.pos = @pos + @vel*dt + (1/2.0)*@acc*dt**2 + (1/6.0)*@jerk*dt**3
    wp.vel = @vel + @acc*dt + (1/2.0)*@jerk*dt**2
    wp
  end

  def ms4_full_extrapolate(t)
    if t > @next_time
      raise "t = " + t.to_s + " > @next_time = " + @next_time.to_s + "\n"
    end
    wp = WorldPoint.new
    wp.minstep = @minstep
    wp.maxstep = @maxstep
    wp.nsteps = @nsteps
    wp.mass = @mass
    wp.time = t
    dt = t - @time
    wp.pos = @pos + @vel*dt + (1/2.0)*@acc*dt**2 + (1/6.0)*@jerk*dt**3 +
             (1/24.0)*@snap*dt**4 + (1/120.0)*@crackle*dt**5
    wp.vel = @vel + @acc*dt + (1/2.0)*@jerk*dt**2 + (1/6.0)*@snap*dt**3 +
             (1/24.0)*@crackle*dt**4
    wp.acc = @acc + @jerk*dt + (1/2.0)*@snap*dt**2 + (1/6.0)*@crackle*dt**3
    wp.jerk = @jerk + @snap*dt + (1/2.0)*@crackle*dt**2
    wp.snap = @snap + @crackle*dt
    wp.crackle = @crackle
    wp
  end

  def ms4_extrapolate(t)
    if t > @next_time
      raise "t = " + t.to_s + " > @next_time = " + @next_time.to_s + "\n"
    end
    wp = WorldPoint.new
    wp.minstep = @minstep
    wp.maxstep = @maxstep
    wp.nsteps = @nsteps
    wp.mass = @mass
    wp.time = t
    dt = t - @time
    wp.pos = @pos + @vel*dt + (1/2.0)*@acc*dt**2 + (1/6.0)*@jerk*dt**3
    wp.vel = @vel + @acc*dt + (1/2.0)*@jerk*dt**2
    wp.acc = @acc + @jerk*dt
    wp.jerk = @jerk
    wp
  end

  def forward_interpolate(other, t)
    wp = WorldPoint.new
    wp.minstep = @minstep
    wp.maxstep = @maxstep
    wp.nsteps = @nsteps
    wp.mass = @mass
    wp.time = t
    dt = other.time - @time
    dt = t - @time
    wp.pos = @pos + @vel*dt
    wp.vel = @vel + @acc*dt
    wp
  end

  def leapfrog_interpolate(other, t)
    wp = WorldPoint.new
    wp.minstep = @minstep
    wp.maxstep = @maxstep
    wp.nsteps = @nsteps
    wp.mass = @mass
    wp.time = t
    dt = other.time - @time
    jerk = (other.acc - @acc)/dt
    dt = t - @time
    wp.pos = @pos + @vel*dt + (1/2.0)*@acc*dt**2 + (1/6.0)*jerk*dt**3
    wp.vel = @vel + @acc*dt + (1/2.0)*jerk*dt**2
    wp
  end

  def hermite_interpolate(other, t)
    wp = WorldPoint.new
    wp.minstep = @minstep
    wp.maxstep = @maxstep
    wp.nsteps = @nsteps
    wp.mass = @mass
    wp.time = t
    dt = other.time - @time
    snap = (-6*(@acc - other.acc) - 2*(2*@jerk + other.jerk)*dt)/dt**2
    crackle = (12*(@acc - other.acc) + 6*(@jerk + other.jerk)*dt)/dt**3
    dt = t - @time
    wp.pos = @pos + @vel*dt + (1/2.0)*@acc*dt**2 + (1/6.0)*@jerk*dt**3 +
             (1/24.0)*snap*dt**4 + (1/144.0)*crackle*dt**5
    wp.vel = @vel + @acc*dt + (1/2.0)*@jerk*dt**2 + (1/6.0)*snap*dt**3 + 
             (1/24.0)*crackle*dt**4
    wp
  end

  def ms4_interpolate(other, t)
    wp = WorldPoint.new
    wp.minstep = @minstep
    wp.maxstep = @maxstep
    wp.nsteps = @nsteps
    wp.mass = @mass
    wp.time = t
    dt = other.time - @time
    dt = t - @time
    wp.pos = @pos + @vel*dt + (1/2.0)*@acc*dt**2 + (1/6.0)*@jerk*dt**3 +
             (1/24.0)*@snap*dt**4 + (1/120.0)*@crackle*dt**5 +
             (1/720.0)*@pop*dt**6
    wp.vel = @vel + @acc*dt + (1/2.0)*@jerk*dt**2 + (1/6.0)*@snap*dt**3 + 
             (1/24.0)*@crackle*dt**4 + (1/120.0)*@pop*dt**5
    wp
  end

  def kinetic_energy
    0.5*@mass*@vel*@vel
  end

  def potential_energy(body_array)
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

class WorldLine

  attr_accessor  :worldpoint, :method

  def initialize
    @worldpoint = []
  end

  def setup(time)
    eval("#{@method}_setup(time)")
  end

  def startup_done?(era, dt_max)
    eval("#{@method}_startup_done?(era, dt_max)")
  end

  def grow(era, dt_max)
    new_point = eval("take_#{@method}_step(era, dt_max)")
    @worldpoint.push(new_point)
  end

  def forward_setup(time)
    @worldpoint[0].forward_setup(time)
  end

  def leapfrog_setup(time)
    @worldpoint[0].leapfrog_setup(time)
  end

  def hermite_setup(time)
    @worldpoint[0].hermite_setup(time)
  end

  def ms4_setup(time)
    @worldpoint[0].ms4_setup(time)
  end

  def ms4_number_of_steps
    4
  end

  def forward_startup_done?(era, dt_max)
    wp = @worldpoint[0]
    acc = era.acc(self, wp)
    timescale = era.timescale(self, wp)
    wp.forward_startup(acc, timescale, @dt_param, dt_max)
    true
  end

  def leapfrog_startup_done?(era, dt_max)
    wp = @worldpoint[0]
    acc = era.acc(self, wp)
    timescale = era.timescale(self, wp)
    wp.leapfrog_startup(acc, timescale, @dt_param, dt_max)
    true
  end

  def hermite_startup_done?(era, dt_max)
    wp = @worldpoint[0]
    acc, jerk = era.acc_and_jerk(self, wp)
    timescale = era.timescale(self, wp)
    wp.hermite_startup(acc, jerk, timescale, @dt_param, dt_max)
    true
  end

  def ms4_startup_done?(era, dt_max)
    wp = @worldpoint[0]
    if not defined? @startup_cycle
      @startup_cycle = 1
      acc, jerk = era.acc_and_jerk(self, wp)
      timescale = era.timescale(self, wp)
      wp.ms4_startup_first_round(acc, jerk, timescale, @dt_param, dt_max)
      return false
    elsif @startup_cycle == 1
      @startup_cycle += 1
      snap, crackle = era.snap_and_crackle(self, wp)
      wp.ms4_startup_second_round(snap, crackle)
      time = wp.time
      dt = wp.next_time - time
      for i in (1..3)
        @worldpoint.unshift(wp.ms4_full_extrapolate(time - i*dt))
      end
      return true
    else
      raise "@startup_cycle = #{@startup_cycle} != 1"
    end
  end

  def take_forward_step(era, dt_max)
    new_point = forward_predict
    acc = era.acc(self, new_point)
    timescale = era.timescale(self, new_point)
    new_point.forward_correct(@worldpoint.last, acc,
                              timescale, @dt_param, dt_max)
  end

  def take_leapfrog_step(era, dt_max)
    new_point = leapfrog_predict
    acc = era.acc(self, new_point)
    timescale = era.timescale(self, new_point)
    new_point.leapfrog_correct(@worldpoint.last, acc,
                              timescale, @dt_param, dt_max)
  end

  def take_hermite_step(era, dt_max)
    new_point = hermite_predict
    acc, jerk = era.acc_and_jerk(self, new_point)
    timescale = era.timescale(self, new_point)
    new_point.hermite_correct(@worldpoint.last, acc, jerk,
                              timescale, @dt_param, dt_max)
  end

  def take_ms4_step(era, dt_max)
    new_point = ms4_predict
    acc = era.acc(self, new_point)
    timescale = era.timescale(self, new_point)
    k = @worldpoint.size
    wp_array = @worldpoint[k-4...k].reverse
    new_point.ms4_correct(wp_array, acc, timescale, @dt_param, dt_max)
  end

  def forward_predict
    wp = @worldpoint.last
    wp.forward_extrapolate(wp.next_time)
  end

  def leapfrog_predict
    wp = @worldpoint.last
    wp.leapfrog_extrapolate(wp.next_time)
  end

  def hermite_predict
    wp = @worldpoint.last
    wp.hermite_extrapolate(wp.next_time)
  end

  def ms4_predict
    wp = @worldpoint.last
    wp.ms4_extrapolate(wp.next_time)
  end

  def valid_extrapolation?(time)
    unless @worldpoint.last.time <= time and time <= @worldpoint.last.next_time
      raise "#{time} not in [#{@worldpoint.last.time}, #{@worldpoint.last.next_time}]"
    end
  end

  def valid_interpolation?(time)
    unless @worldpoint[0].time <= time and time <= @worldpoint.last.time
      raise "#{time} not in [#{@worldpoint[0].time}, #{@worldpoint.last.time}]"
    end
  end

  def take_snapshot_of_worldline(time)
    if time >= @worldpoint.last.time
      valid_extrapolation?(time)
      wp = @worldpoint.last.extrapolate(time, @method)
      wp.body_id = @body_id
      wp
    else
      valid_interpolation?(time)
      @worldpoint.each_index do |i|
        if @worldpoint[i].time > time
          wp = @worldpoint[i-1].interpolate(@worldpoint[i], time, @method)
          wp.body_id = @body_id
          return wp
        end
      end
    end
  end

  def next_worldline(time)
    valid_interpolation?(time)
    i = @worldpoint.size
    loop do
      i -= 1
      if @worldpoint[i].time <= time
        wl = self.clone
        if i-@number_of_steps+1 < 0
          raise "i-@number_of_steps+1 = #{i-@number_of_steps+1} < 0"
        end
        wl.worldpoint = @worldpoint[i-(@number_of_steps-1)...@worldpoint.size]
        return wl
      end
    end
  end

  def setup_from_single_worldpoint(b, method, dt_param, time)
    @worldpoint[0] = b.to_worldpoint
    @body_id = @worldpoint[0].body_id
    @method = method
    @dt_param = dt_param
    if eval("defined? #{@method}_number_of_steps")
      eval("@number_of_steps = #{@method}_number_of_steps")
    else
      @number_of_steps = 1
    end
    setup(time)
  end

end

class WorldEra

  attr_accessor  :start_time, :end_time, :worldline

  def initialize
    @worldline = []
  end

  def acc(wl, wp)
    take_snapshot_except(wl, wp.time).get_acc(wp.pos)
  end

  def acc_and_jerk(wl, wp)
    take_snapshot_except(wl, wp.time).get_acc_and_jerk(wp.pos, wp.vel)
  end

  def snap_and_crackle(wl, wp)
    take_snapshot_except(wl, wp.time).get_snap_and_crackle(wp.pos, wp.vel,
                                                           wp.acc, wp.jerk)
  end

  def timescale(wl, wp)
    take_snapshot_except(wl, wp.time).collision_time_scale(wp.mass,
                                                           wp.pos, wp.vel)
  end

  def startup_and_report_energy(dt_max)
    list = @worldline.clone
    while list.size > 0
      new_list = []
      list.each do |wl|
        new_list.push(wl) unless wl.startup_done?(self, dt_max)
      end
      list = new_list
    end
    take_snapshot(@start_time).total_energy
  end

  def shortest_extrapolated_worldline
    t = VERY_LARGE_NUMBER
    wl = nil
    @worldline.each do |w|
      if t > w.worldpoint.last.next_time
        t = w.worldpoint.last.next_time
        wl = w
      end
    end
    wl
  end

  def shortest_interpolated_worldline
    t = VERY_LARGE_NUMBER
    wl = nil
    @worldline.each do |w|
      if t > w.worldpoint.last.time
        t = w.worldpoint.last.time
        wl = w
      end
    end
    wl
  end

  def evolve(dt_era, dt_max, shared_flag)
    nsteps = 0
    while shortest_interpolated_worldline.worldpoint.last.time < @end_time
      unless shared_flag
        shortest_extrapolated_worldline.grow(self, dt_max)
        nsteps += 1
      else
        t = shortest_extrapolated_worldline.worldpoint.last.next_time
        @worldline.each do |w|
          w.worldpoint.last.next_time = t
          w.grow(self, dt_era)
          nsteps += 1
        end
      end
    end
    [next_era(dt_era), nsteps]
  end

  def next_era(dt_era)
    e = WorldEra.new
    e.start_time = @end_time
    e.end_time = @end_time + dt_era
    @worldline.each do |wl|
      e.worldline.push(wl.next_worldline(e.start_time))
    end
    e
  end

  def take_snapshot(time)
    take_snapshot_except(nil, time)
  end

  def take_snapshot_except(wl, time)
    ws = WorldSnapshot.new
    ws.time = time
    @worldline.each do |w|
      s = w.take_snapshot_of_worldline(time)
      ws.body.push(s) unless w == wl
    end
    ws
  end

  def write_diagnostics(t, nsteps, initial_energy, init_flag = false)
    STDERR.print "at time t = #{sprintf("%g", t)} "
    STDERR.print "(from interpolation after #{nsteps} steps "
    if init_flag
      STDERR.print "to time #{sprintf("%g", @start_time)}):\n"
    else
      STDERR.print "to time #{sprintf("%g", @end_time)}):\n"
    end
    take_snapshot(t).write_diagnostics(initial_energy)
  end

  def setup_from_snapshot(ss, method, dt_param, dt_era)
    @start_time = ss.time
    @end_time = @start_time + dt_era
    ss.body.each do |b|
      wl = WorldLine.new
      wl.setup_from_single_worldpoint(b, method, dt_param, ss.time)
      @worldline.push(wl)
    end
  end

end

class World

  def evolve(c)
    while @era.start_time < @t_end
      @new_era, dn = @era.evolve(c.dt_era, @dt_max, c.shared_flag)
      @nsteps += dn
      @time = @era.end_time
      while @t_dia <= @era.end_time and @t_dia <= @t_end
        @era.write_diagnostics(@t_dia, @nsteps, @initial_energy)
        @t_dia += c.dt_dia
      end
      while @t_out <= @era.end_time and @t_out <= @t_end
        output(c)
        @t_out += c.dt_out
      end
      @old_era = @era
      @era = @new_era
    end
  end

  def output(c)
    if c.world_output_flag
      acs_write($stdout, false, c.precision, c.add_indent)
    else
      @era.take_snapshot(@t_out).acs_write($stdout, true,
                                           c.precision, c.add_indent)
    end
  end

  def setup_from_world(c)
    init_output(c)
    @t_out += c.dt_out
    @t_end += c.dt_end
    @dt_max = c.dt_era * c.dt_max_param
    @new_era = @era.next_era(c.dt_era)
    @old_era = @era
    @era = @new_era
  end

  def setup_from_snapshot(ss, c)
    @era = WorldEra.new
    @era.setup_from_snapshot(ss, c.integration_method, c.dt_param, c.dt_era)
    @nsteps = 0
    @dt_max = c.dt_era * c.dt_max_param
    @initial_energy = @era.startup_and_report_energy(@dt_max)
    @time = @era.start_time
    @t_out = @time
    @t_dia = @time
    @t_end = @time
    init_output(c)
    @t_out += c.dt_out
    @t_dia += c.dt_dia
    @t_end += c.dt_end
  end

  def init_output(c)
    @era.write_diagnostics(@time, @nsteps, @initial_energy, true)
    if c.init_out
      if c.world_output_flag
        acs_write($stdout, false, c.precision, c.add_indent)
      else
        @era.take_snapshot(@t_out).acs_write($stdout, true,
                                            c.precision, c.add_indent)
      end
    end
  end

  def World.admit(file, c)
    object = acs_read([self, WorldSnapshot], file)
    if object.class == self
      object.setup_from_world(c)
      return object
    elsif object.class == WorldSnapshot
      w = World.new
      w.setup_from_snapshot(object, c) if object.class == WorldSnapshot
      return w
    else
      raise "#{object.class} not recognized"
    end
  end

end

class WorldSnapshot < NBody

  attr_accessor :time

  def initialize
    super
    @time = 0.0
  end

  def get_acc(pos)
    acc = pos*0                            # null vectors of the correct length
    @body.each do |b|
      r = b.pos - pos
      r2 = r*r
      r3 = r2*sqrt(r2)
      acc += b.mass*r/r3
    end
    acc
  end    

  def get_acc_and_jerk(pos, vel)
    acc = jerk = pos*0                  # null vectors of the correct length
    @body.each do |b|
      r = b.pos - pos
      r2 = r*r
      r3 = r2*sqrt(r2)
      v = b.vel - vel
      acc += b.mass*r/r3
      jerk += b.mass*(v-3*(r*v/r2)*r)/r3
    end
    [acc, jerk]
  end    

  def get_snap_and_crackle(pos, vel, acc, jerk)
    snap = crackle = pos*0                 # null vectors of the correct length
    @body.each do |b|
      r = b.pos - pos
      r2 = r*r
      r3 = r2*sqrt(r2)
      v = b.vel - vel
      a = b.acc - acc
      j = b.jerk - jerk
      c1 = r*v/r2
      c2 = (v*v + r*a)/r2 + c1*c1
      c3 = (3*v*a + r*j)/r2 + c1*(3*c2 - 4*c1*c1)
      d_acc = b.mass*r/r3
      d_jerk = b.mass*v/r3 - 3*c1*d_acc
      d_snap = b.mass*a/r3 - 6*c1*d_jerk - 3*c2*d_acc
      d_crackle = b.mass*j/r3 - 9*c1*d_snap - 9*c2*d_jerk - 3*c3*d_acc
      snap += d_snap
      crackle += d_crackle
    end
    [snap, crackle]
  end    

  def collision_time_scale(mass, pos, vel)
    time_scale_sq = VERY_LARGE_NUMBER              # square of time scale value
    @body.each do |b|
      r = b.pos - pos
      v = b.vel - vel
      r2 = r*r
      v2 = v*v + 1.0/VERY_LARGE_NUMBER          # always non-zero, for division
      estimate_sq = r2 / v2              # [distance]^2/[velocity]^2 = [time]^2
      if time_scale_sq > estimate_sq
        time_scale_sq = estimate_sq
      end
      a = (mass + b.mass)/r2
      estimate_sq = sqrt(r2)/a           # [distance]/[acceleration] = [time]^2
      if time_scale_sq > estimate_sq
        time_scale_sq = estimate_sq
      end
    end
    sqrt(time_scale_sq)                  # time scale value
  end

  def kinetic_energy
    e = 0
    @body.each{|b| e += b.kinetic_energy}
    e
  end

  def potential_energy
    e = 0
    @body.each{|b| e += b.potential_energy(@body)}
    e/2                                # pairwise potentials were counted twice
  end

  def total_energy
    kinetic_energy + potential_energy
  end

  def write_diagnostics(initial_energy)
    e0 = initial_energy
    ek = kinetic_energy
    ep = potential_energy
    etot = ek + ep
    STDERR.print <<-END
    E_kin = #{sprintf("%.3g", ek)} ,\
     E_pot =  #{sprintf("%.3g", ep)} ,\
      E_tot = #{sprintf("%.3g", etot)}
       E_tot - E_init = #{sprintf("%.3g", etot - e0)}
        (E_tot - E_init) / E_init = #{sprintf("%.3g", (etot - e0)/e0 )}
    END
  end

end

class Body

  def to_worldpoint
    wp = WorldPoint.new
    wp.restore_contents(self)
  end

end

options_text= <<-END

  Description: Individual Time Step, Individual Integration Scheme Code
  Long description:
    This program evolves an N-body code with a fourth-order Hermite Scheme,
    using individual time steps.  Note that the program can be forced to let
    all particles share the same (variable) time step with the option -a.

    This is a test version, for the ACS data format

    (c) 2004, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:
    ruby mkplummer.rb -n 5 -s 1 | ruby #{$0} -t 1


  Short name: 		-g
  Long name:		--integration_method
  Value type:		string
  Default value:	hermite
  Variable name:	integration_method
  Description:		Choice of integration method
  Long description:
    This option chooses the integration method.  The user is expected to
    provide a string with the name of the method, for example "leapfrog",
    "hermite".


  Short name: 		-c
  Long name:		--step_size_control
  Value type:		float
  Default value:	0.01
  Variable name:	dt_param
  Description:		Determines the time step size
  Long description:
    This option sets the step size control parameter dt_param << 1.  Before
    each new time step, we first calculate the time scale t_scale on which
    changes are expected to happen, such as close encounters or significant
    changes in velocity.  The new time step is then given as the product
    t_scale * dt_param << t_scale.


  Short name: 		-e
  Long name:		--era_length
  Value type:		float
  Default value:	0.01
  Variable name:	dt_era
  Description:		Duration of an era
  Long description:
    This option sets the time interval between begin and end of an era,
    which is the period in time that contains a bundle of world lines,
    all of which are guaranteed to extend beyond the era boundaries with
    by at least one world point in either direction.  In other words, each
    world line has an earliest world point before the beginning of the era,
    and a latest world point past the end of the era.  This guarantees
    accurate interpolation at each time within an era.


  Short name: 		-m
  Long name:		--max_timestep_param
  Value type:		float
  Default value:	1
  Variable name:	dt_max_param
  Description:		Maximum time step (units dt_era)
  Long description:
    This option sets an upper limit to the size dt of a time step,
    as the product of the duration of an era and this parameter:
    dt <= dt_max = dt_era * dt_max_param .


  Short name: 		-d
  Long name:		--diagnostics_interval
  Value type:		float
  Default value:	1
  Variable name:	dt_dia
  Description:		Diagnostics output interval
  Long description:
    This option sets the time interval between diagnostics output,
    which will appear on the standard error channel.


  Short name: 		-o
  Long name:		--output_interval
  Value type:		float
  Default value:	1
  Variable name:	dt_out
  Description:		Snapshot output interval
  Long description:
    This option sets the time interval between output of a snapshot
    of the whole N-body system, which which will appear on the
    standard output channel.

    The snapshot contains the mass, position, and velocity values
    for all particles in an N-body system.


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


  Short name:		-r
  Long name:  		--world_output
  Value type:  		bool
  Variable name:	world_output_flag
  Description:		World output format, instead of snapshot
  Long description:
    If this flag is set to true, each output will take the form of a
    full world dump, instead of a snapshot (the default).  Reading in
    such an world again will allow a fully accurate restart of the
    integration,  since no information is lost in the process of writing
    out and reading in, in terms of world format.


  Short name:		-a
  Long name:  		--shared_timesteps
  Value type:  		bool
  Variable name:	shared_flag
  Description:		All particles share the same time step
  Long description:
    If this flag is set to true, all particles will march in lock step,
    all sharing the same time step.


  Short name:           -p
  Long name:            --precision
  Value type:           int
  Default value:        16
  Description:          Floating point precision
  Variable name:        precision
  Long description:
    The precision with which floating point numbers are printed in the output.
    The default precision is comparable to double precision accuracy.


  Long name:            --indentation
  Value type:           int
  Default value:        2
  Description:          Incremental indentation
  Variable name:        add_indent
  Long description:
    This option allows the user to set the incremental indentation, i.e.
    the number of white spaces added in front of the output of data, for
    each level that the data are removed from the top level.

    Starting at zero indentation at the level of the top ACS structure,
    one set of incremental indentation is added for each level down,
    from ACS to DSS, from DSS to World, and so on.


  END

clop = parse_command_line(options_text, true)

World.admit($stdin, clop).evolve(clop)
