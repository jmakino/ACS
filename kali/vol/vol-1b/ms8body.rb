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

  def yo4(dt)
    d = [1.351207191959657, -1.702414383919315]
    leapfrog(dt*d[0])
    leapfrog(dt*d[1])
    leapfrog(dt*d[0])
  end

  def yo6(dt)
    d = [0.784513610477560e0, 0.235573213359357e0, -1.17767998417887e0,
         1.31518632068391e0]
    for i in 0..2 do leapfrog(dt*d[i]) end
    leapfrog(dt*d[3])
    for i in 0..2 do leapfrog(dt*d[2-i]) end
  end

  def yo8(dt)
    d = [0.104242620869991e1, 0.182020630970714e1, 0.157739928123617e0, 
         0.244002732616735e1, -0.716989419708120e-2, -0.244699182370524e1, 
         -0.161582374150097e1, -0.17808286265894516e1]
    for i in 0..6 do leapfrog(dt*d[i]) end
    leapfrog(dt*d[7])
    for i in 0..6 do leapfrog(dt*d[6-i]) end
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
      @a5 = acc
      yo6(dt)
    elsif @nsteps == 1
      @a4 = acc
      yo6(dt)
    elsif @nsteps == 2
      @a3 = acc
      yo6(dt)
    elsif @nsteps == 3
      @a2 = acc
      yo6(dt)
    elsif @nsteps == 4
      @a1 = acc
      yo6(dt)
    else
      a0 = acc
      j = (a0*137 - @a1*300 + @a2*300 - @a3*200 + @a4*75 - @a5*12)/60
      s = (a0*45 - @a1*154 + @a2*214 - @a3*156 + @a4*61 - @a5*10)/12
      c = (a0*17 - @a1*71 + @a2*118 - @a3*98 + @a4*41 - @a5*7)/4
      p = a0*3 - @a1*14 + @a2*26 - @a3*24 + @a4*11 - @a5*2
      x = a0 - @a1*5 + @a2*10 - @a3*10 + @a4*5 - @a5
      @pos += (vel+(a0+(j+(s+(c+p/6)/5)/4)/3)*dt/2)*dt
      @vel += (a0 +(j +(s+(c+(p+x/6)/5)/4)/3)/2)*dt
      @a5 = @a4
      @a4 = @a3
      @a3 = @a2
      @a2 = @a1
      @a1 = a0
    end
  end

  def ms8(dt)
    if @nsteps == 0
      @a7 = acc
      yo8(dt)
    elsif @nsteps == 1
      @a6 = acc
      yo8(dt)
    elsif @nsteps == 2
      @a5 = acc
      yo8(dt)
    elsif @nsteps == 3
      @a4 = acc
      yo8(dt)
    elsif @nsteps == 4
      @a3 = acc
      yo8(dt)
    elsif @nsteps == 5
      @a2 = acc
      yo8(dt)
    elsif @nsteps == 6
      @a1 = acc
      yo8(dt)
    else
      a0 = acc
      j = (a0*1089 - @a1*2940 + @a2*4410 - @a3*4900 + @a4*3675 - @a5*1764 + 
           @a6*490 - @a7*60)/420
      s = (a0*938 - @a1*4014 + @a2*7911 - @a3*9490 + @a4*7380 - @a5*3618 +
           @a6*1019 - @a7*126)/180
      c = (a0*967 - @a1*5104 + @a2*11787 - @a3*15560 + 
          @a4*12725 - @a5*6432 + @a6*1849 - @a7*232)/120
      p = (a0*56 - @a1*333 + @a2*852 - @a3*1219 + @a4*1056 - @a5*555 +
           @a6*164 - @a7*21)/6
      x = (a0*46 - @a1*295 + @a2*810 - @a3*1235 +
           @a4*1130 - @a5*621 + @a6*190 - @a7*25)/6
      y = a0*4 - @a1*27 + @a2*78 - @a3*125 + @a4*120 - @a5*69 + @a6*22 - @a7*3
      z = a0 - @a1*7 + @a2*21 - @a3*35 + @a4*35 - @a5*21 + @a6*7 - @a7
      @pos += (vel+(a0+(j+(s+(c+(p+(x+y/8)/7)/6)/5)/4)/3)*dt/2)*dt
      @vel += (a0 +(j +(s+(c+(p+(x+(y+z/8)/7)/6)/5)/4)/3)/2)*dt
      @a7 = @a6
      @a6 = @a5
      @a5 = @a4
      @a4 = @a3
      @a3 = @a2
      @a2 = @a1
      @a1 = a0
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
