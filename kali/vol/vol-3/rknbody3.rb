require "vector.rb"

class Body

  attr_accessor :mass, :pos, :vel                                            #1

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def acc(body_array)
    a = @pos*0                              # null vector of the correct length
    body_array.each do |b|
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
    0.5*@mass*(@vel*@vel)
  end

  def epot(body_array)                  # potential energy
    p = 0
    body_array.each do |b|
      unless b == self
        r = b.pos - @pos
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

  attr_accessor :time, :body

  def initialize
    @body = []
  end

  def evolve(integration_method, dt, dt_dia, dt_out, dt_end)
    nsteps = 0
    e_init
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
    old_acc = []
    @body.each_index{|i| old_acc[i] = @body[i].acc(@body)}
    @body.each{|b| b.pos += b.vel*dt}
    @body.each_index{|i| @body[i].vel += old_acc[i]*dt}
  end

  def leapfrog(dt)
    @body.each{|b| b.vel += b.acc(@body)*0.5*dt}
    @body.each{|b| b.pos += b.vel*dt}
    @body.each{|b| b.vel += b.acc(@body)*0.5*dt}
  end

  def rk2(dt)
    old_pos = []
    @body.each_index{|i| old_pos[i] = @body[i].pos}
    half_vel = []
    @body.each_index{|i|
      half_vel[i] = @body[i].vel + @body[i].acc(@body)*0.5*dt}
    @body.each{|b| b.pos += b.vel*0.5*dt}
    @body.each{|b| b.vel += b.acc(@body)*dt}
    @body.each_index{|i| @body[i].pos = old_pos[i] + half_vel[i]*dt}
  end

  def rk4(dt)
    old_pos = []
    @body.each_index{|i| old_pos[i] = @body[i].pos}
    a0 = []
    @body.each_index{|i| a0[i] = @body[i].acc(@body)}
    @body.each_index{|i|
      @body[i].pos = old_pos[i] + @body[i].vel*0.5*dt + a0[i]*0.125*dt*dt}
    a1 = []
    @body.each_index{|i| a1[i] = @body[i].acc(@body)}
    @body.each_index{|i|
      @body[i].pos = old_pos[i] + @body[i].vel*dt + a1[i]*0.5*dt*dt}
    a2 = []
    @body.each_index{|i| a2[i] = @body[i].acc(@body)}
    @body.each_index{|i|
      @body[i].pos = old_pos[i] + @body[i].vel*dt +
                     (a0[i]+a1[i]*2)*(1/6.0)*dt*dt}
    @body.each_index{|i| @body[i].vel += (a0[i]+a1[i]*4+a2[i])*(1/6.0)*dt}
  end

  def ekin                        # kinetic energy
    e = 0
    @body.each{|b| e += b.ekin}
    e
  end

  def epot                        # potential energy
    e = 0
    @body.each{|b| e += b.epot(@body)}
    e/2                           # pairwise potentials were counted twice
  end

  def e_init                      # initial total energy
    @e0 = ekin + epot
  end

  def write_diagnostics(nsteps)
    etot = ekin + epot
    STDERR.print <<END
at time t = #{sprintf("%g", time)}, after #{nsteps} steps :
  E_kin = #{sprintf("%.3g", ekin)} ,\
 E_pot =  #{sprintf("%.3g", epot)},\
 E_tot = #{sprintf("%.3g", etot)}
             E_tot - E_init = #{sprintf("%.3g", etot - @e0)}
  (E_tot - E_init) / E_init = #{sprintf("%.3g", (etot - @e0)/@e0 )}
END
  end

  def pp                                # pretty print
    print "     N = ", @body.size, "\n"
    print "  time = ", @time, "\n"
    @body.each{|b| b.pp}
  end

  def simple_print
    print @body.size, "\n"
    printf("%24.16e\n", @time)
    @body.each{|b| b.simple_print}
  end

  def simple_read
    n = gets.to_i
    @time = gets.to_f
    for i in 0...n
      @body[i] = Body.new
      @body[i].simple_read
    end
  end

end
