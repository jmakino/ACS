# mnbody.rb: message passing toy model N-body extension of rkbody.rb
# Douglas and Piet, 2004/4/8-9.

require "mnvector.rb"

class Body

  attr_accessor :mass, :nb

  def initialize(mass = 0, pos = Vector[], vel = Vector[])
    @mass, @pos, @vel = mass, pos, vel
    @history = Array.new
    @history[0] = Array.new
    @history[0][0] = 0                        # initial time
    @history[0][1] = Array.new                # [r,v,a]
    @history[0][1][0] = @pos
    @history[0][1][1] = @vel
    @init_flag = true             # if true, needs history initialization
  end

  def pos(time)
    propagator(time)
    @pos
  end    

  def vel(time)
    propagator(time)
    @vel
  end    

  def propagator(time)
#    print "entering propagator for body ", self.id, " at time ", time, "\n"
#print "   with @history = "
#p @history
#print "(@history[0][0] != time) = (", @history[0][0], " != ", time, ") = ",
# (@history[0][0] != time), "\n"
    if (@history[0][0] != time)
#print "entering if statement -- if (@history[0][0] != time)\n"
      if @init_flag
        @init_flag = false
#print "entering: @history[0][0] = ",  @history[0][0], "\n"
#        e_init
        @history[0][1][2] = acc(@history[0][0])
        @history[1] = Array.new
        @history[1][0] = @history[0][0] + time_step(@history[0][0])
#print "  after setting timestep for particle ", self.id, " ; history = "
#p @history
       @history[1][1] = Array.new
      end
      nsteps = 0
#p @history
##print "id = ", self.id, " ; time = ", time, "\n"
##p @history
      while (time > @history.last[0])
#print "id = ", self.id, " ; nsteps = ", nsteps, " ; time = ", time, "\n"
       dt = @history.last[0] - @history[@history.size-2][0]
        send(@nb.integration_method, @history[@history.size-2][0], dt)
#print "never!\n"
###print "id = ", self.id, " ; nsteps = ", nsteps, " ; time = ", time, "\n"
        @history.last[1] = [@pos, @vel, acc(@history.last[0])]
        tmp = @history.last[0] +
              time_step(@history.last[0])
        @history[@history.size] = Array.new
        @history.last[0] = tmp
        @history.last[1] = Array.new
        nsteps += 1
#print "-->nsteps = ", nsteps, "\n"
      end
      extrapolate(time)
#      write_diagnostics(nsteps, time)
#      dump_history
#      simple_print
    end
  end

  def acc(time)
#print "entering acc\n" 
    if (@history[(@history.size) -1][0] == time) and @history[(@history.size) -1][1][2]
      @history[(@history.size) -1][1][2]
    else
      if @history[(@history.size) -2][0] == time and @history[(@history.size) -2][1][2]
        @history[(@history.size) -2][1][2]
      else
        a = Vector.new(@pos.size, 0.0)
        @nb.body.each do |b|
          unless b==self
            a += acc_by(b, time)
          end
        end
        a
      end
    end
  end

  def acc_by(other, time)
#print "entering acc_by for particles ", self.id, " and ", other.id, "time = ", time, "\n"
    rji = other.pos(time) - pos(time)
    r2 = rji * rji
    r3 = r2*sqrt(r2)
    rji * (other.mass / r3)
  end

#  def acc
#    r2 = @pos*@pos
#    r3 = r2*sqrt(r2)
#    @pos*(-@mass/r3)
#  end    

  def time_step(time)
#print "entering time_step for particles ", self.id, " at time = ", time, "\n"
    position = pos(time)
    velocity = vel(time)
    acceleration = acc(time)
#print "acceleration = ", acceleration, "\n"
    r2 = position * position
    v2 = velocity * velocity
    a2 = acceleration * acceleration
    collision_time = sqrt(r2/v2)
    free_fall_time = sqrt(sqrt(r2/a2))
    [collision_time, free_fall_time].min*@nb.eta
  end

  def extrapolate(t)
#print "entering extrapolate for particle ", self.id, " at time = ", t, "\n"
    dt = t - @history[@history.size-2][0]
    @pos += @vel*dt + acc(@history[@history.size-2][0])*0.5*dt*dt
    @vel += acc(@history[@history.size-2][0])*dt
  end

  def forward(t, dt)
#print "entering forward\n"
    acceleration = acc(t)
    @pos += @vel*dt
    @vel += acceleration * dt
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

  def ekin                        # kinetic energy
    @ek = 0.5*(@vel*@vel)         # per unit of reduced mass
  end

  def epot                        # potential energy
    @ep = -@mass/sqrt(@pos*@pos)  # per unit of reduced mass
  end

  def e_init                      # initial total energy
    @e0 = ekin + epot             # per unit of reduced mass
  end

  def write_diagnostics(nsteps, time)
    etot = ekin + epot
    STDERR.print <<END
at time t = #{sprintf("%g", time)}, after #{nsteps} steps :
  E_kin = #{sprintf("%.3g", ekin)} ,\
 E_pot =  #{sprintf("%.3g", epot)},\
 E_tot = #{sprintf("%.3g", etot)}
             E_tot - E_init = #{sprintf("%.3g", etot-@e0)}
  (E_tot - E_init) / E_init =#{sprintf("%.3g", (etot - @e0) / @e0 )}
END
  end

  def dump_history
    p @history
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
###p @history
  end

  def simple_read
    @mass = gets.to_f
    @pos = gets.split.map{|x| x.to_f}.to_v
    @vel = gets.split.map{|x| x.to_f}.to_v
    @history[0][1][0] = @pos
    @history[0][1][1] = @vel
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

  def write(time)
    print @body.size, "\n"
    printf("%24.16e\n", time)
    @body.each do |b| b.write(time) end
  end

  def simple_read
    n = gets.to_i
    @time = gets.to_f
    for i in 0...n
      @body[i] = Body.new
      @body[i].nb = self
      @body[i].simple_read
    end
  end

end
