#<i>extends Nbody class in order to include Leapfrog module</i>

require "nbody-energy.rb"
require "body-leapfrog.rb"
require "leapfrog.rb"

class Nbody

  def leapfrog_pp            # pretty print
    print "     N = ", @body.size, "\n"
    print "  time = ", @time, "\n"
    i = 0
    @body.each do |b|
      i += 1
      STDERR.print "  internal data for particle ", i, " :\n"
      STDERR.print b.leapfrog_to_s
    end
  end

  def write_diagnostics(nsteps, x_flag)
    STDERR.print "at time t = ", @time, " , after ", nsteps, " steps :\n"
    update_ekin        # a hack, really; should make this cleaner
    report_energy
    if x_flag
      STDERR.print "  for debugging purposes, here is the internal data ",
	"representation:\n"
      leapfrog_pp
    end
  end

  def predict_step(dt)
    @body.each do |b| b.predict_step(dt) end
  end
  
  def get_acc_pot
    @body.each do |b| b.clear_acc end
    clear_epot
    i = 0
    while (i < @body.size) 
      j = i+1
      while (j < @body.size)
	@epot += @body[i].pairwise_acc_pot(@body[j])
	j += 1
      end
      i += 1
    end
  end
  
  def correct_step(dt)
    @body.each do |b| b.correct_step(dt) end
  end
  
  def inc_time(dt)
    @time += dt
  end
  
  def evolve_step(dt)
    predict_step(dt)
    get_acc_pot
    correct_step(dt)
    inc_time(dt)
  end
  
  def evolve(dt, dt_dia, dt_out, dt_tot, init_out, x_flag)

    STDERR.print "Starting a leapfrog integration for a ", @body.size,
                 "-body system,\n  from time t = ", @time, 
                 " with time step dt = ", dt,
                 "  until time ", @time + dt_tot,
                 " ,\n  with diagnostics output interval dt_dia = ", dt_dia,
                 ",\n  and snapshot output interval dt_out = ", dt_out, ".\n"

    get_acc_pot

    nsteps = 0

    write_diagnostics(nsteps, x_flag)

    simple_print if init_out

    t_dia = @time + dt_dia
    t_out = @time + dt_out
    t_end = @time + dt_tot

    loop do
      while @time < t_dia and @time < t_out and @time < t_end
	evolve_step(dt)
	nsteps += 1
      end
      if @time >= t_dia
	write_diagnostics(nsteps, x_flag)
	t_dia += dt_dia
      end
      if @time >= t_out
	simple_print
	t_out += dt_out
      end
      if @time >= t_end
	break;
      end
    end
  end

end
