include Math

class Body

  attr_accessor :mass, :pos, :vel, :ek, :ep, :e0

  def initialize(mass = 0, pos = [0,0,0], vel = [0,0,0])
    @mass, @pos, @vel = mass, pos, vel
    @ek = @ep = @e0 = 0
  end

  def evolve_step(dt)
    r2 = 0
    pos.each {|p| r2 += p*p}
    r3 = r2 * sqrt(r2)
    acc = pos.map { |x| -b.mass * x/r3 }
    pos.each_index { |k| pos[k] += vel[k] * dt }
    vel.each_index { |k| vel[k] += acc[k] * dt }
  end

  def ekin
    @ek = 0
    vel.each {|v| @ek += v*v}
    @ek *= 0.5
  end

  def epot
    r2 = 0
    pos.each {|p| r2 += p*p}
    r = sqrt(r2)
    @ep = -1/r
  end

  def e_init
    @e0 = ekin() + epot()
  end

  def write_diagnostics(nsteps, time)
    STDERR.print "\nat time t = "
    STDERR.printf("%g", time)
    STDERR.print " , after ", nsteps, " steps :\n"
    etot = ekin() + epot()
    STDERR.print "  E_kin = "
    STDERR.printf("%.3g", ekin())
    STDERR.print " , E_pot = "
    STDERR.printf("%.3g", epot())
    STDERR.print " , E_tot = "
    STDERR.printf("%.3g", etot)
    STDERR.print "\n             ",
      "E_tot - E_init = "
    STDERR.printf("%.3g", etot-@e0)
    STDERR.print "\n  ",
      "(E_tot - E_init) / E_init = "
    STDERR.printf("%.3g", (etot - @e0) / @e0 )
    STDERR.print "\n"
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
    @pos.each { |x| printf("%24.16e", x) } ; print "\n"
    @vel.each { |x| printf("%24.16e", x) } ; print "\n"
  end

  def simple_read
    @mass = gets.to_f
    @pos = gets.split.map { |x| x.to_f }
    @vel = gets.split.map { |x| x.to_f }
  end

end

def evolve(b, dt, dt_dia, dt_out, dt_tot)

  time = 0
  nsteps = 0
  b.e_init()
  b.write_diagnostics(nsteps, time)

  t_dia = dt_dia
  t_out = dt_out
  t_end = dt_tot

  loop do
    while time < t_dia and time < t_out and time < t_end
	b.evolve_step(dt)
        time += dt
	nsteps += 1
    end
    if time >= t_dia
	b.write_diagnostics(nsteps, time)
	t_dia += dt_dia
    end
    if time >= t_out
	b.simple_print
	t_out += dt_out
    end
    if time >= t_end
	break;
    end
  end
end

dt = 0.0001
dt_dia = 1
dt_out = 1
dt_tot = 1

STDERR.print "dt = ", dt, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_tot = ", dt_tot, "\n"

b = Body.new
b.simple_read
evolve(b, dt, dt_dia, dt_out, dt_tot)
