require "vector.rb"

class Body

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def evolve(integration_method, dt, dt_dia, dt_out, dt_end)
    time = 0
    nsteps = 0
    e_init
    write_diagnostics(nsteps, time)

    t_dia = dt_dia - 0.5*dt
    t_out = dt_out - 0.5*dt
    t_end = dt_end - 0.5*dt

    while time < t_end
      send(integration_method,dt)
      time += dt
      nsteps += 1
      if time >= t_dia
        write_diagnostics(nsteps, time)
        t_dia += dt_dia
      end
      if time >= t_out
        simple_print
        t_out += dt_out
      end
    end
  end

  def acc
    r2 = @pos*@pos
    r3 = r2*sqrt(r2)
    @pos*(-@mass/r3)
  end    

  def jerk
    r2 = @pos*@pos
    r3 = r2*sqrt(r2)
    (@vel+@pos*(-3*(@pos*@vel)/r2))*(-@mass/r3)
  end

  def forward(dt)
    old_acc = acc
    @pos += @vel*dt
    @vel += old_acc*dt
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
    @pos = old_pos + vel*dt + (a0+a1*2)*(1/6.0)*dt*dt                       #1
    @vel = vel + (a0+a1*4+a2)*(1/6.0)*dt                                    #1
  end

  def yo6(dt)
    d = [0.784513610477560e0, 0.235573213359357e0, -1.17767998417887e0,
         1.31518632068391e0]
    for i in 0..2 do leapfrog(dt*d[i]) end
    leapfrog(dt*d[3])
    for i in 0..2 do leapfrog(dt*d[2-i]) end
  end

  def hermite(dt)
    old_pos = @pos
    old_vel = @vel
    old_acc = acc
    old_jerk = jerk
    @pos += @vel*dt + old_acc*(dt*dt/2.0) + old_jerk*(dt*dt*dt/6.0)
    @vel += old_acc*dt + old_jerk*(dt*dt/2.0)
    @vel = old_vel + (old_acc + acc)*(dt/2.0) + (old_jerk - jerk)*(dt*dt/12.0)
    @pos = old_pos + (old_vel + vel)*(dt/2.0) + (old_acc - acc)*(dt*dt/12.0)
  end

  def ekin                        # kinetic energy
    0.5*(@vel*@vel)               # per unit of reduced mass
  end

  def epot                        # potential energy
    -@mass/sqrt(@pos*@pos)        # per unit of reduced mass
  end

  def e_init                      # initial total energy
    @e0 = ekin + epot             # per unit of reduced mass
  end

  def write_diagnostics(nsteps, time)
    etot = ekin + epot
    STDERR.print <<END
at time t = #{sprintf("%g", time)}, after #{nsteps} steps :
  E_kin = #{sprintf("%.3g", ekin)} ,\
 E_pot =  #{sprintf("%.3g", epot)} ,\
 E_tot = #{sprintf("%.3g", etot)}
             E_tot - E_init = #{sprintf("%.3g", etot-@e0)}
  (E_tot - E_init) / E_init =#{sprintf("%.3g", (etot - @e0) / @e0 )}
END
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
  end

end
