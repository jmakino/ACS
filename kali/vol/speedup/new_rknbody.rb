require "new_vector.rb"
require "speedup1a/pairwise"
class Body

  attr_accessor :mass, :pos, :vel, :acc

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def calc(time_step, s)
    dt = time_step
    eval(s)
  end

  def ekin                         # kinetic energy
    0.5*@mass*(@vel*@vel)
  end

  def epot(body_array, eps)        # potential energy
    p = 0
    body_array.each do |b|
      unless b == self
        r = b.pos - @pos
        p += -@mass*b.mass/sqrt(r*r + eps*eps)
      end
    end
    p
  end

  def to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.join(", ") + "\n" +
    "   vel = " + @vel.join(", ") + "\n"
  end

  def pp                           # pretty print
    print to_s
  end

  def ppx(body_array, eps)         # pretty print, with extra information (acc)
    STDERR.print to_s + "   acc = " + acc(body_array, eps).join(", ") + "\n"
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
    @init_flag = true
  end

  def acc_ruby
    @body.each{|bi| bi.acc = bi.pos*0}
    @body.each_index do |i|
      bi = @body[i]
      (i+1...@body.size).each do |j|
	bj = @body[j]
	r = bj.pos - bi.pos
	r2 = r*r + @eps*@eps
	r3 = r2*sqrt(r2)
	bj.acc -= r*(bi.mass/r3)
	bi.acc += r*(bj.mass/r3)
      end
    end
  end    
  def acc_c
    ndim = @body[0].pos.size
    @body.each{|bi| bi.acc = bi.pos*0}
    @body.each_index do |i|
      bi = @body[i]
      (i+1...@body.size).each do |j|
	bj = @body[j]
	pairwise_acceleration(bi.acc,bj.acc,bi.pos,bj.pos,bi.mass,bj.mass,@eps,
			      ndim)
      end
    end
  end    
  
  def acc
    acc_ruby
  end
  def evolve(integration_method, eps, dt, dt_dia, dt_out, dt_end,
             init_out, x_flag)
    @dt = dt                                                                 #1
    @eps = eps                                                               #1
    nsteps = 0
    e_init
    write_diagnostics(nsteps, x_flag)
    t_dia = dt_dia - 0.5*dt
    t_out = dt_out - 0.5*dt
    t_end = dt_end - 0.5*dt

    simple_print if init_out

    while @time < t_end
      send(integration_method)
      @time += dt
      nsteps += 1
      if @time >= t_dia
        write_diagnostics(nsteps, x_flag)
        t_dia += dt_dia
      end
      if @time >= t_out
        simple_print
        t_out += dt_out
      end
    end
  end

  def calc(s)
    @body.each{|b| b.calc(@dt, s)}
  end

  def forward
    acc
    calc(" @old_acc = @acc ")
    calc(" @pos += @vel*dt ")
    calc(" @vel += @old_acc*dt ")
  end

  def leapfrog
    if @init_flag
      acc
      @init_flag = false
    end
    calc(" @vel += @acc*0.5*dt ")
    calc(" @pos += @vel*dt ")
    acc
    calc(" @vel += @acc*0.5*dt ")
  end

  def rk2
    calc(" @old_pos = @pos ")
    acc
    calc(" @half_vel = @vel + @acc*0.5*dt ")
    calc(" @pos += @vel*0.5*dt ")
    acc
    calc(" @vel += @acc*dt ")
    calc(" @pos = @old_pos + @half_vel*dt ")
  end

  def rk4
    calc(" @old_pos = @pos ")
    acc
    calc(" @a0 = @acc ")
    calc(" @pos = @old_pos + @vel*0.5*dt + @a0*0.125*dt*dt ")
    acc
    calc(" @a1 = @acc ")
    calc(" @pos = @old_pos + @vel*dt + @a1*0.5*dt*dt ")
    acc
    calc(" @a2 = @acc ")
    calc(" @pos = @old_pos + @vel*dt + (@a0+@a1*2)*(1/6.0)*dt*dt ")
    calc(" @vel += (@a0+@a1*4+@a2)*(1/6.0)*dt ")
  end

  def ekin                        # kinetic energy
    e = 0
    @body.each{|b| e += b.ekin}
    e
  end

  def epot                        # potential energy
    e = 0
    @body.each{|b| e += b.epot(@body, @eps)}
    e/2                           # pairwise potentials were counted twice
  end

  def e_init                      # initial total energy
    @e0 = ekin + epot
  end

  def write_diagnostics(nsteps, x_flag)
    etot = ekin + epot
    STDERR.print <<END
at time t = #{sprintf("%g", time)}, after #{nsteps} steps :
  E_kin = #{sprintf("%.3g", ekin)} ,\
 E_pot =  #{sprintf("%.3g", epot)} ,\
 E_tot = #{sprintf("%.3g", etot)}
             E_tot - E_init = #{sprintf("%.3g", etot - @e0)}
  (E_tot - E_init) / E_init = #{sprintf("%.3g", (etot - @e0)/@e0 )}
END
    if x_flag
      STDERR.print "  for debugging purposes, here is the internal data ",
                   "representation:\n"
      ppx
    end
  end

  def pp                           # pretty print
    print "     N = ", @body.size, "\n"
    print "  time = ", @time, "\n"
    @body.each{|b| b.pp}
  end

  def ppx                          # pretty print, with extra information (acc)
    print "     N = ", @body.size, "\n"
    print "  time = ", @time, "\n"
    @body.each{|b| b.ppx(@body, @eps)}
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
