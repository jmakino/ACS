  def evolve(dt_param, delta_dia, delta_out, delta_end, init_out, x_flag)
    nsteps = 0
    startup(dt_param)
    write_diagnostics(nsteps, x_flag)
    t_dia = @time + delta_dia
    t_out = @time + delta_out
    t_end = @time + delta_end
    simple_print if init_out
    while @time < t_end
      nsteps += do_x(t_end)
      if @time >= t_dia
        t_dia += do_y(t, t_dia, dt_param, nsteps, x_flag)
        nsteps += @body.size
      end
      if @time >= t_out
        nsteps += do_z(t, t_dia, dt_param, nsteps, x_flag)
      end
    end
  end

  def do_x(t, dt_param)
    nstep = 0
    np = find_next_particle
    @time = np.next_time
    if (@time < t)
      np.autonomous_step(@body, dt_param)
      nstep = 1
    end
    nstep
  end

  def do_y(t, t_dia, dt_param, nsteps, x_flag)
    sync(t_dia, dt_param)
    write_diagnostics(nsteps, x_flag)
    t_dia += delta_dia
    @body.size
  end

  def do_z(t, t_out, dt_param)
    sync(t_out, dt_param)
    simple_print
    t_out += delta_out
    @body.size
  end
