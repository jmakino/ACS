#
#  murat0.rb   040913    Version of rknbody8.rb, with adaptive time step
#

require "vector.rb"

class Body

  @@coll_time = 0

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def calc(body_array, time_step, s)
    ba  = body_array
    dt = time_step*get_time
    #    printf "body'de %f\n",dt
    eval(s)
  end

  def new_dt(body_array,time_st)
    @@coll_time_sq=1e30
    body_array.each do |b|
      unless b == self
        r = b.pos - @pos
	v = b.vel - @vel
	r2 = r*r
	v2 = v*v
        coll_est_q=(r2*r2)/(v2*v2)
	if @@coll_time_sq > coll_est_q
          @@coll_time_sq = coll_est_q
	end
	r3 = r2 *sqrt(r2)
	rv = r*v/r2
	da = r/r3
	da2 = da*da*b.mass*b.mass
        coll_est_q=r2/da2
	if @@coll_time_sq > coll_est_q
          @@coll_time_sq = coll_est_q
	end
      end
    end
    #@@dtn = time_st*@@dtn
    @@coll_time = sqrt(sqrt(@@coll_time_sq))
    @@coll_time
  end
  #	printf("r2 = %.3g \n",r2)
  #	printf("v2 = %.3g \n",v2)
  #coll_time = sqrt(coll_time_sq)
  #printf("dt ilk = %.3g \n",sqrt(coll_time_sq))
  #	b.dtn = dt*b.dtn
  #	printf("coll_est_sq= %.3g \n",coll_est_sq)
  def get_time
    @@coll_time
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

  def ekin                         # kinetic energy
    0.5*@mass*(@vel*@vel)
  end

  def epot(body_array)             # potential energy
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

  def pp                           # pretty print
    print to_s
  end

  def ppx(body_array)              # pretty print, with extra information (acc)
    STDERR.print to_s + "   acc = " + acc(body_array).join(", ") + "\n"
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

  def evolve(integration_method, dt, dt_dia, dt_out, dt_end, init_out, x_flag)
    nsteps = 0
    e_init
    write_diagnostics(nsteps, x_flag)
    @dt = dt
    dtilk = @dt
#printf "YENÝ DT: %f\n",@dt
    t_dia = dt_dia - 0.5*dt
    t_out = dt_out - 0.5*dt
    t_end = dt_end - 0.5*dt
    simple_print if init_out                                                 #1
    while @time < t_end
    calc(" dt = new_dt(ba,dt) ")
      send(integration_method)                                               #2
#printf "YENÝ DT: %f\n",body[0].get_time
	#dt=dtilk*body[0].get_time
#printf "YENÝÝÝÝ: %f\n",dt
	#@time += dt
	dtn=body[0].get_time
	@time += dtilk*dtn
printf "evolve'de time : %f\n",@time
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
    @body.each{|b| b.calc(@body, @dt, s)}
  end

  def forward
    calc(" @old_acc = acc(ba) ")
    calc(" @pos += @vel*dt ")
    calc(" @vel += @old_acc*dt ")
  end

  def leapfrog
    #calc(" dt =dt*new_dt(dt) ")
    calc(" @vel += acc(ba)*0.5*dt ")
    calc(" @pos += @vel*dt ")
    calc(" @vel += acc(ba)*0.5*dt ")
  end

  def rk2
    calc(" @old_pos = @pos ")
    calc(" @half_vel = @vel + acc(ba)*0.5*dt ")
    calc(" @pos += @vel*0.5*dt ")
    calc(" @vel += acc(ba)*dt ")
    calc(" @pos = @old_pos + @half_vel*dt ")
  end

  def rk4
    calc(" @old_pos = @pos ")
    calc(" @a0 = acc(ba) ")
    calc(" @pos = @old_pos + @vel*0.5*dt + @a0*0.125*dt*dt ")
    calc(" @a1 = acc(ba) ")
    calc(" @pos = @old_pos + @vel*dt + @a1*0.5*dt*dt ")
    calc(" @a2 = acc(ba) ")
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
    @body.each{|b| e += b.epot(@body)}
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
    @body.each{|b| b.ppx(@body)}
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
