require "vector.rb"

class Body

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def evolve(integration_method, dt, dt_dia, dt_out, dt_end)
    time = 0
    @nsteps = 0
    e_init
    write_diagnostics(time)

    t_dia = dt_dia - 0.5*dt
    t_out = dt_out - 0.5*dt
    t_end = dt_end - 0.5*dt

    while time < t_end
      send(integration_method,dt)
      time += dt
      @nsteps += 1
      if time >= t_dia
        write_diagnostics(time)
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

  def rk6(dt)
    d = [0.784513610477560e0, 0.235573213359357e0, -1.17767998417887e0,
         1.31518632068391e0]
    for i in 0..2 do leapfrog(dt*d[i]) end
    leapfrog(dt*d[3])
    for i in 0..2 do leapfrog(dt*d[2-i]) end
  end

  def ms2(dt)
    if @nsteps == 0
      @prev_acc = acc
      rk2(dt)
    else
      old_acc = acc
      jdt = old_acc - @prev_acc
      @pos += vel*dt + old_acc*0.5*dt*dt
      @vel += old_acc*dt + jdt*0.5*dt
      @prev_acc = old_acc
    end
  end

  def ms4(dt)
    if @nsteps == 0
      @ap3 = acc
      rk4(dt)
    elsif @nsteps == 1
      @ap2 = acc
      rk4(dt)
    elsif @nsteps == 2
      @ap1 = acc
      rk4(dt)
    else
      ap0 = acc
      jdt = ap0*(11.0/6.0) - @ap1*3 + @ap2*1.5 - @ap3/3.0
      sdt2 = ap0*2 - @ap1*5 + @ap2*4 - @ap3
      cdt3 = ap0 - @ap1*3 + @ap2*3 - @ap3
      @pos += (vel+(ap0+ (jdt+sdt2/4)/3)*dt/2)*dt
      @vel += (ap0+(jdt+(sdt2+cdt3/4)/3)/2)*dt
      @ap3 = @ap2
      @ap2 = @ap1
      @ap1 = ap0
    end
  end

  def ms6(dt)
    if @nsteps == 0
      @ap5 = acc
      rk6(dt)
    elsif @nsteps == 1
      @ap4 = acc
      rk6(dt)
    elsif @nsteps == 2
      @ap3 = acc
      rk6(dt)
    elsif @nsteps == 3
      @ap2 = acc
      rk6(dt)
    elsif @nsteps == 4
      @ap1 = acc
      rk6(dt)
    else
      ap0 = acc
      jdt = (ap0*137 - @ap1*300 + @ap2*300 - @ap3*200 + @ap4*75 - @ap5*12)/60
      sdt2 = (ap0*45 - @ap1*154 + @ap2*214 - @ap3*156 + @ap4*61 - @ap5*10)/12
      cdt3 = (ap0*17 - @ap1*71 + @ap2*118 - @ap3*98 + @ap4*41 - @ap5*7)/4
      pdt4 = ap0*3 - @ap1*14 + @ap2*26 - @ap3*24 + @ap4*11 - @ap5*2
      xdt5 = ap0 - @ap1*5 + @ap2*10 - @ap3*10 + @ap4*5 - @ap5
      @pos += (vel+(ap0+ (jdt+(sdt2+(cdt3+pdt4/6)/5)/4)/3)*dt/2)*dt
      @vel += (ap0+(jdt+(sdt2+(cdt3+(pdt4+xdt5/6)/5)/4)/3)/2)*dt
      @ap5 = @ap4
      @ap4 = @ap3
      @ap3 = @ap2
      @ap2 = @ap1
      @ap1 = ap0
    end
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

  def write_diagnostics(time)
    etot = ekin + epot
    STDERR.print <<END
at time t = #{sprintf("%g", time)}, after #{@nsteps} steps :
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
