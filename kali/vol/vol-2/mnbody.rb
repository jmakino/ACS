# mnbody.rb: message passing toy model N-body extension of rkbody.rb
# Douglas and Piet, 2004/4/8-9.

require "mnhistory.rb"

class Body

  attr_accessor :mass, :nb, :ekin_init, :epot_init

  def initialize(time = 0, mass = 0, pos = Vector[], vel = Vector[])
    @hist = History.new
    @hist.set_first_time(time)
    @mass, @pos, @vel = mass, pos, vel
    @hist.set_last_rndot(0, @pos)
    @hist.set_last_rndot(1, @vel)
    @init_flag = true                # if true, needs initialization
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

  def finish_init
    @init_flag = false
    t_init = @hist.latest
    @hist.set_last_rndot(2, calculate_acc(t_init))
    @ekin_init = ekin(t_init)
    @epot_init = epot(t_init)
  end

  def pos(time)
    finish_init if @init_flag
    rndot(0, time)
  end

  def vel(time)
    finish_init if @init_flag
    rndot(1, time)
  end    

  def acc(time)
    finish_init if @init_flag
    rndot(2, time)
  end    

  def set_last_pos(x)
    @hist.set_last_rndot(0, x)
  end

  def set_last_vel(x)
    @hist.set_last_rndot(1, x)
  end

  def set_last_acc(x)
    @hist.set_last_rndot(2, x)
  end

  def integrate
    if @hist.predicting?
      propagate
    end
    if @hist.arrived?
      time = @hist.latest
      @hist.set_last_rndot(2, calculate_acc(time))
      @hist.extend(time + calculate_timestep(time), @nb.history_depth)
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

  def ekin(time)                        # kinetic energy
    vel(time)*vel(time) * 0.5 * @mass
  end

  def pot_from(other, time)
    rji = other.pos(time) - pos(time)
    r2 = rji * rji
    -other.mass/sqrt(r2)
  end

  def epot(time)                        # potential energy
    pot = 0
    @nb.body.each do |b|
      unless b==self
        pot += pot_from(b, time)
      end
    end
    pot
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
    set_last_pos(pos(t) + vel(t)*dt)
    set_last_vel(vel(t) + acc(t)*dt)
  end

  def leapfrog(t, dt)
    half_vel = vel(t) + acc(t)*0.5*dt
    set_last_pos(pos(t) + half_vel*dt)
    set_last_vel(half_vel + acc(t+dt)*0.5*dt)
  end

  def print_history
    @hist.pp
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

  attr_accessor :integration_method, :eta, :history_depth, :time, :body

  def n
    @body.size
  end

  def initialize(integration_method = "rk2", eta = 0.01,
                 history_depth = 10, n=0, time = 0.0)
    @integration_method = integration_method
    @eta = eta
    @history_depth = history_depth
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

  def print_history
    @body.each do |b|
      print "history for body ", b.id, " : \n"
      b.print_history
    end
  end

  def write_diagnostics(time)
    ekin = epot = ekin_init = epot_init = 0
    @body.each do |b|
      ekin += b.ekin(time)
      epot += b.epot(time)
      ekin_init += b.ekin_init
      epot_init += b.epot_init
    end
    etot = ekin + 0.5 * epot
    etot_init = ekin_init + 0.5 * epot_init
    STDERR.print <<END
at time t = #{sprintf("%g", time)}:
  E_kin = #{sprintf("%.3g", ekin)} ,\
 E_pot =  #{sprintf("%.3g", epot)},\
 E_tot = #{sprintf("%.3g", etot)}
             E_tot - E_init = #{sprintf("%.3g", etot - etot_init)}
  (E_tot - E_init) / E_init = #{sprintf("%.3g", (etot - etot_init)/etot_init )}
END
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
