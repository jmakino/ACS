# mbody.rb: message passing toy model extension of rkbody.rb
# Douglas and Piet, 2004/4/8-9.

require "vector.rb"

class Body

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = Vector[], vel = Vector[])
    @mass, @pos, @vel = mass, pos, vel
    @history = Array.new
    @history[0] = Array.new
    @history[0][0] = 0                        # initial time
    @history[0][1] = Array.new                # [r,v,a]
    @history[0][1][0] = pos
    @history[0][1][1] = vel
  end

  def propagator(integration_method, eta, t_end)
    unless @history[0][1][2]
      e_init
      @history[0][1][2] = acc
      @history[1] = Array.new
      @history[1][0] = @history[0][0] + time_step(eta)
      @history[1][1] = Array.new
    end
    nsteps = 0
    while (t_end > @history.last[0])
      dt = @history.last[0] - @history[@history.size-2][0]
      send(integration_method,dt)
      @history.last[1] = [pos, vel, acc]
      @history[@history.size] = Array.new
      @history[@history.size-1][0] = @history[@history.size-2][0] +
                                     time_step(eta)
      @history[@history.size-1][1] = Array.new
      nsteps += 1
    end
    extrapolate(t_end)
    write_diagnostics(nsteps, t_end)
    dump_history
    simple_print
  end

  def acc
    r2 = @pos*@pos
    r3 = r2*sqrt(r2)
    @pos*(-@mass/r3)
  end    

  def time_step(eta)
    r2 = pos*pos
    v2 = vel*vel
    a = acc
    a2 = a*a
    collision_time = sqrt(r2/v2)
    free_fall_time = sqrt(sqrt(r2/a2))
    [collision_time, free_fall_time].min*eta
  end

  def extrapolate(t)
    dt = t - @history[@history.size-2][0]
    @pos += vel*dt + acc*0.5*dt*dt
    @vel += acc*dt
  end

  def forward(dt)
    @pos += @vel*dt
    @vel += acc*dt
  end

  def leapfrog(dt)
    @vel += acc*0.5*dt
    @pos += @vel*dt
    @vel += acc*0.5*dt
  end

  def rk2(dt)
    old_pos = pos
    half_vel = vel + acc*0.5*dt
    @pos += vel*0.5*dt
    @vel += acc*dt
    @pos = old_pos + half_vel*dt
  end

  def rk4(dt)
    old_pos = pos
    a0 = acc
    @pos = old_pos + vel*0.5*dt + a0*0.125*dt*dt
    a1 = acc
    @pos = old_pos + vel*dt + a1*0.5*dt*dt
    a2 = acc
    @pos = old_pos + vel*dt + (a0+a1*2)*(1/6.0)*dt*dt
    @vel = vel + (a0+a1*4+a2)*(1/6.0)*dt
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

  def simple_print
    printf("%24.16e\n", @mass)
    @pos.each{|x| printf("%24.16e", x)}; print "\n"
    @vel.each{|x| printf("%24.16e", x)}; print "\n"
  end

  def simple_read
    @mass = gets.to_f
    @pos = gets.split.map{|x| x.to_f}.to_v
    @vel = gets.split.map{|x| x.to_f}.to_v
    @history[0][1][0] = pos
    @history[0][1][1] = vel
  end

end
