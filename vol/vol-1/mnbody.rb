# mnbody.rb: message passing toy model N-body extension of rkbody.rb
# Douglas and Piet, 2004/4/8-9.

require "mnhistory.rb"

class Body

  attr_accessor :mass, :nb

  def initialize(time = 0, mass = 0, pos = Vector[], vel = Vector[])
    @hist = History.new
    @hist.set_first_time(time)
    @mass, @pos, @vel = mass, pos, vel
    @hist.set_last_rndot(0, @pos)
    @hist.set_last_rndot(1, @vel)
    @acc_initializing = false
  end

  def rndot(n, time)
    if (time < @hist.earliest)
      STDERR.print "error: time = ", time, " < @hist.earliest = "
      STDERR.print @hist.earliest, "\n"
      exit
    end
    while (time > @hist.latest)
      integrate
    end
    @hist.rndot(n, time)
  end

  def pos(time)
    unless @acc_initializing
      @acc_initializing = true
      @hist.set_last_rndot(2, calculate_acc(@hist.latest))
    end
    rndot(0, time)
  end

  def vel(time)
    unless @acc_initializing
      @acc_initializing = true
      @hist.set_last_rndot(2, calculate_acc(@hist.latest))
    end
    rndot(1, time)
  end    

  def acc(time)
    unless @acc_initializing
      @acc_initializing = true
      @hist.set_last_rndot(2, calculate_acc(@hist.latest))
    end
    rndot(2, time)
  end    

  def integrate
    if @hist.predicting?
      propagate
    end
    if @hist.arrived?
      time = @hist.latest
      @hist.set_last_rndot(2, calculate_acc(time))
      @hist.extend(time + calculate_timestep(time))
    else
      STDERR.print "error: integrate: history structure is `incomplete'\n"
      exit
    end
  end

  def calculate_acc(time)
    a = Vector.new(@hist.ndim, 0.0)      # vector with ndim components, all 0.0
    @nb.body.each do |b|
      unless b==self
        a += acc_by(b, time)
      end
    end
    a
  end

  def acc_by(other, time)
    rji = other.pos(time) - pos(time)
    r2 = rji * rji
    r3 = r2*sqrt(r2)
    rji * (other.mass / r3)
  end

  def calculate_timestep(time)
    very_large_number = 1e100
    timestep = very_large_number
    @nb.body.each do |b|
      unless b==self
        if timestep > collision_time_by(b, time)
          timestep = collision_time_by(b, time)
        end
      end
    end
    free_fall_time = very_large_number
    @nb.body.each do |b|
      unless b==self
        if timestep > free_fall_time_by(b, time)
          timestep = free_fall_time_by(b, time)
        end
      end
    end
    timestep*@nb.eta
  end

  def collision_time_by(other, time)
    delta_pos = other.pos(time) - pos(time)
    delta_vel = other.vel(time) - vel(time)
    r2 = delta_pos * delta_pos
    v2 = delta_vel * delta_vel
    sqrt(r2/v2)
  end

  def free_fall_time_by(other, time)
    delta_pos = other.pos(time) - pos(time)
    delta_acc = other.acc(time) - acc(time)
    r2 = delta_pos * delta_pos
    a2 = delta_acc * delta_acc
    sqrt(sqrt(r2/a2))
  end

  def propagate
    time = @hist.before_latest
    dt = @hist.latest - time
    send(@nb.integration_method, time, dt)
  end

  def forward(t, dt)
    @hist.set_last_rndot(0, pos(t) + vel(t)*dt)
    @hist.set_last_rndot(1, vel(t) + acc(t)*dt)
  end

  def leapfrog(t, dt)
    @vel += acc(t)*0.5*dt
    @pos += @vel*dt
    @vel += acc(t+dt)*0.5*dt
  end

  def rk2(t, dt)
    old_pos = @pos
    half_vel = @vel + acc(t)*0.5*dt
    @pos += @vel*0.5*dt
    @vel += acc(t+0.5*dt)*dt
    @pos = old_pos + half_vel*dt
  end

  def rk4(t, dt)
    old_pos = @pos
    a0 = acc(t)
    @pos = old_pos + @vel*0.5*dt + a0*0.125*dt*dt
    a1 = acc(t)
    @pos = old_pos + @vel*dt + a1*0.5*dt*dt
    a2 = acc(t)
    @pos = old_pos + @vel*dt + (a0+a1*2)*(1/6.0)*dt*dt
    @vel = @vel + (a0+a1*4+a2)*(1/6.0)*dt
  end

  def dump_history
    p @hist
  end

  def to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.join(", ") + "\n" +
    "   vel = " + @vel.join(", ") + "\n"
  end

  def pp               # pretty print
    print to_s
  end

  def write(time)
    printf("%24.16e\n", @mass)
    position = pos(time)
    position.each{|x| printf("%24.16e", x)}; print "\n"
    velocity = vel(time)
    velocity.each{|x| printf("%24.16e", x)}; print "\n"
  end

  def read
    @mass = gets.to_f
    @pos = gets.split.map{|x| x.to_f}.to_v
    @vel = gets.split.map{|x| x.to_f}.to_v
    @hist.set_last_rndot(0, @pos)
    @hist.set_last_rndot(1, @vel)
  end

end

class Nbody

  attr_accessor :integration_method, :eta, :time, :body

  def n
    @body.size
  end

  def initialize(integration_method = "rk2", eta = 0.01, n=0, time = 0.0)
    @integration_method = integration_method
    @eta = eta
    @body = [Body.new]
    for i in 0...n
      @body[i] = Body.new
      @body[i].nb = self
    end
    @time = time
  end

  def pp                                # pretty print
    print "     N = ", @body.size, "\n"
    print "  time = ", @time, "\n"
    @body.each do |b| b.pp end
  end

  def dump_history
    @body.each do |b|
      print "Full history for body ", b.id, " : \n"
      b.dump_history
    end
  end

  def write(time)
    print @body.size, "\n"
    printf("%24.16e\n", time)
    @body.each do |b| b.write(time) end
  end

  def read
    n = gets.to_i
    @time = gets.to_f
    for i in 0...n
      @body[i] = Body.new(@time)
      @body[i].nb = self
      @body[i].read
    end
  end

end
