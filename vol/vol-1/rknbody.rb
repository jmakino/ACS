require "rknvector.rb"

class Body

  attr_accessor :mass, :pos, :vel, :nb

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def acc
    a = @pos*0                              # null vector of the correct length
    @nb.b.each do |b|
      unless b == self
        r = b.pos - @pos
        r2 = r*r
        r3 = r2*sqrt(r2)
        a += r*(b.mass/r3)
      end
    end
    a
  end    

  def ekin                        # kinetic energy
    0.5*(@vel*@vel)
  end

  def epot                        # potential energy
    p = 0
    @nb.b.each do |b|
      unless b == self
        r = b.pos - @pos
        r2 = r*r
        r3 = r2*sqrt(r2)
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

class Nbody

  attr_accessor :time, :b

  def initialize(n=0, time = 0)
    @time = time
    @b = []
    for i in 0...n
      @b[i] = Body.new
      @b[i].nb = self
    end
  end

  def evolve(integration_method, dt, dt_dia, dt_out, dt_end)
    nsteps = 0
    @e0 = ekin + epot                      # initial total energy
    write_diagnostics(nsteps)

    t_dia = dt_dia - 0.5*dt
    t_out = dt_out - 0.5*dt
    t_end = dt_end - 0.5*dt

    while @time < t_end
      send(integration_method,dt)
      @time += dt
      nsteps += 1
      if @time >= t_dia
	write_diagnostics(nsteps)
	t_dia += dt_dia
      end
      if @time >= t_out
	simple_print
	t_out += dt_out
      end
    end
  end

  def forward(dt)
    @b.each{|b| b.vel += b.acc*dt}
    @b.each{|b| b.pos += b.vel*dt}
  end

  def leapfrog(dt)
    @b.each{|b| b.vel += b.acc*0.5*dt}
    @b.each{|b| b.pos += b.vel*dt}
    @b.each{|b| b.vel += b.acc*0.5*dt}
  end

  def rk2(dt)
    old_pos = []
    @b.each_index{|i| old_pos[i] = @b[i].pos}
    half_vel = []
    @b.each_index{|i| half_vel[i] = @b[i].vel + @b[i].acc*0.5*dt}
    @b.each{|b| b.pos += b.vel*0.5*dt}
    @b.each{|b| b.vel += b.acc*dt}
    @b.each_index{|i| @b[i].pos = old_pos[i] + half_vel[i]*dt}
  end

  def rk4(dt)
    old_pos = []
    @b.each_index{|i| old_pos[i] = @b[i].pos}
    a0 = []
    @b.each_index{|i| a0[i] = @b[i].acc}
    @b.each_index{|i| @b[i].pos =
                             old_pos[i] + @b[i].vel*0.5*dt + a0[i]*0.125*dt*dt}
    a1 = []
    @b.each_index{|i| a1[i] = @b[i].acc}
    @b.each_index{|i| @b[i].pos = old_pos[i] + @b[i].vel*dt + a1[i]*0.5*dt*dt}
    a2 = []
    @b.each_index{|i| a2[i] = @b[i].acc}
    @b.each_index{|i| @b[i].pos =
                     old_pos[i] + @b[i].vel*dt + (a0[i]+a1[i]*2)*(1/6.0)*dt*dt}
    @b.each_index{|i| @b[i].vel += (a0[i]+a1[i]*4+a2[i])*(1/6.0)*dt}
  end

  def ekin                        # kinetic energy
    e = 0
    @b.each{|b| e += b.ekin}
    e
  end

  def epot                        # potential energy
    e = 0
    @b.each{|b| e += b.epot}
    e/2                           # pairwise potentials were counted twice
  end

  def write_diagnostics(nsteps)
    etot = ekin + epot
    STDERR.print <<END
at time t = #{sprintf("%g", time)}:
  E_kin = #{sprintf("%.3g", ekin)} ,\
 E_pot =  #{sprintf("%.3g", epot)},\
 E_tot = #{sprintf("%.3g", etot)}
             E_tot - E_init = #{sprintf("%.3g", etot - @e0)}
  (E_tot - E_init) / E_init = #{sprintf("%.3g", (etot - @e0)/@e0 )}
END
  end

  def pp                                # pretty print
    print "     N = ", @b.size, "\n"
    print "  time = ", @time, "\n"
    @b.each{|b| b.pp}
  end

  def simple_print
    print @b.size, "\n"
    printf("%24.16e\n", @time)
    @b.each{|b| b.simple_print}
  end

  def simple_read
    n = gets.to_i
    @time = gets.to_f
    for i in 0...n
      @b[i] = Body.new
      @b[i].nb = self
      @b[i].simple_read
    end
  end

end
