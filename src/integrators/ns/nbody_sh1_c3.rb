#!/usr/bin/ruby
#
# nbody_sh1_c3.rb
#
# predict and correct in C struct
#
#==============================================================================

#require 'profile'
require 'pairwize'
include Math

class Body

  NDIM = 3

  attr_accessor :mass, :pos, :vel, :acc, :jerk,
                :old_pos, :old_vel, :old_acc, :old_jerk, :cbody

  def initialize(mass = 0, pos = [0,0,0], vel = [0,0,0])
    @mass, @pos, @vel = mass, pos, vel
    @old_pos, @old_vel = [], []   # necessary to give the correct
    @old_acc, @old_jerk = [], []  # type to old_something
    @cbody = Cbody.new
  end

  def to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.join(", ") + "\n" +
    "   vel = " + @vel.join(", ") + "\n"
  end

  def pp            # pretty print
    print to_s
  end

  def diag_to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.join(", ") + "\n" +
    "   vel = " + @vel.join(", ") + "\n" +
    "   acc = " + @acc.join(", ") + "\n" +
    "   jerk = " + @jerk.join(", ") + "\n"
  end

  def diag_pp            # pretty print
    STDERR.print diag_to_s
  end

  def simple_print
    printf("%22.15e", @mass)
    @pos.each do |x| printf("%23.15e", x) end
    @vel.each do |x| printf("%23.15e", x) end
    print "\n"
  end

  def simple_read
    s = gets
    a = s.split
    a.collect! { |x| x.to_f }
    @mass = a[0]
    @pos = a[1..NDIM]
    @vel = a[(NDIM+1)..(NDIM*2)]
  end

  def get_time
    @@time
  end

  def set_time(time)
    @@time = time
  end

  def inc_time(dt)
    @@time += dt
  end

  def clear_acc_and_jerk
    @acc = [0,0,0]
    @jerk = [0,0,0]
  end

  def clear_ekin
    @@ekin = 0
  end

  def get_ekin
    @@ekin
  end

  def clear_epot
    @@epot = 0
  end

  def get_epot
    @@epot
  end

  VERY_LARGE_NUMBER = 1e300

  def reset_coll_time_q
    @@coll_time_q = VERY_LARGE_NUMBER
  end

  def get_collision_time
    sqrt(sqrt(@@coll_time_q))
  end

  def add_kin
    for k in 0...NDIM
      @@ekin += 0.5 * @mass * @vel[k] * @vel[k]
    end
  end

  def update_old_coordinates
    for k in 0...NDIM
      @old_pos[k] = @pos[k]
      @old_vel[k] = @vel[k]
      @old_acc[k] = @acc[k]
      @old_jerk[k] = @jerk[k]
    end
  end

  def predict_step(dt)
    for k in 0...NDIM
      @pos[k] = @pos[k] + @vel[k]*dt +
	            @acc[k]*dt*dt/2 + @jerk[k]*dt*dt*dt/6
      @vel[k] = @vel[k] + @acc[k]*dt +
	            @jerk[k]*dt*dt/2
    end
  end

  def correct_step(dt)
    for k in 0...NDIM
      @vel[k] = @old_vel[k] +
	            (@old_acc[k] + @acc[k])*dt/2 +
	            (@old_jerk[k] - @jerk[k])*dt*dt/12
      @pos[k] = @old_pos[k] + 
                    (@old_vel[k] + @vel[k])*dt/2 +
	            (@old_acc[k] - @acc[k])*dt*dt/12
    end
  end

  def pairwise_acc_jerk_pot_coll(other)
    rji = Array.new(NDIM)
    vji = Array.new(NDIM)
    for k in 0...NDIM
      rji[k] = other.pos[k] - @pos[k]
      vji[k] = other.vel[k] - @vel[k]
    end
    r2 = v2 = rv_r2 = 0.0
    for k in 0...NDIM
      r2 += rji[k] * rji[k]
      v2 += vji[k] * vji[k]
      rv_r2 += rji[k] * vji[k]
    end
    rv_r2 = rv_r2 / r2
    r = sqrt(r2)
    r3 = r * r2

    @@epot -= @mass * other.mass / r

    da = Array.new(NDIM)
    dj = Array.new(NDIM)
    for k in 0...NDIM
      da[k] = rji[k] / r3
      dj[k] = (vji[k] - 3 * rv_r2 * rji[k]) / r3
    end
    for k in 0...NDIM
      @acc[k] += other.mass * da[k]
      other.acc[k] -= @mass * da[k]
      @jerk[k] += other.mass * dj[k]
      other.jerk[k] -= @mass * dj[k]
    end

    coll_est_q = (r2*r2) / (v2*v2)
    if @@coll_time_q > coll_est_q
      @@coll_time_q = coll_est_q
    end
    da2 = 0
    for k in 0...NDIM
      da2 += da[k] * da[k]
    end
    mij = @mass + other.mass
    da2 = da2 * mij * mij
    coll_est_q = r2/da2;
    if @@coll_time_q > coll_est_q
      @@coll_time_q = coll_est_q;
    end
  end
end

def get_snapshot
  s = gets
  n = s.to_i
  s = gets
  nb = [Body.new]
  nb[0].set_time(s.to_f)   # type set here; in principle it could be
                           # single, double, or quadruple precision,
                           # and it is better to keep the cllas definitions
                           # the same in all cases
  i = 0
  while i < n
    nb[i] = Body.new
    nb[i].simple_read
    i += 1
  end
  return nb
end

def put_snapshot(nb)
  print nb.size, "\n"
  print nb[0].get_time, "\n"
  nb.each do |b| b.simple_print end
end

def pp_snapshot(nb)
  print "N = ", nb.size, "\n"
  print "time = ", nb[0].get_time, "\n"
  nb.each do |b| b.pp end
end  

def diag_pp_snapshot(nb)
  STDERR.print "N = ", nb.size, "\n"
  STDERR.print "time = ", nb[0].get_time, "\n"
  i = 0
  nb.each do |b|
    i += 1
    STDERR.print "internal data for particle ", i, " :\n"
    b.diag_pp
  end
end  

def print_help
  print "usage: ", $0,
    " [-h (for help)] [-d step_size_control_parameter]\n",
    "         [-e diagnostics_interval] [-o output_interval]\n",
    "         [-t total_duration] [-i (start output at t = 0)]\n",
    "         [-x (extra debugging diagnostics)]\n"
end

require "getoptlong"

parser = GetoptLong.new
parser.set_options(
  ["-d", "--step_size_control_parameter", GetoptLong::REQUIRED_ARGUMENT],
  ["-e", "--diagnostics_interval", GetoptLong::REQUIRED_ARGUMENT],
  ["-h", "--help", GetoptLong::NO_ARGUMENT],
  ["-i", "--initial_output", GetoptLong::NO_ARGUMENT],
  ["-o", "--output_interval", GetoptLong::REQUIRED_ARGUMENT],
  ["-t", "--total_duration", GetoptLong::REQUIRED_ARGUMENT],
  ["-x", "--extra_diagnostics", GetoptLong::NO_ARGUMENT])

def read_options(parser)
  dt_param = 0.03
  dt_dia = 1
  dt_out = 1
  dt_tot = 10
  init_out = false
  x_flag = false

  loop do
    begin
      opt, arg = parser.get
      break if not opt

      case opt
      when "-d"
	dt_param = arg.to_f
      when "-e"
	dt_dia = arg.to_f
      when "-h"
	print_help
        exit         # exit after providing help
      when "-i"
	init_out = true
      when "-o"
	dt_out = arg.to_f
      when "-t"
	dt_tot = arg.to_f
      when "-x"
	x_flag = true
      end

    rescue => err
      print_help
      exit           # exit if option unknown
    end

  end

  return dt_param, dt_dia, dt_out, dt_tot, init_out, x_flag
end

def write_diagnostics(nb, nsteps, einit, init_flag, x_flag)
  nb[0].clear_ekin
  nb.each do |b| b.add_kin end
  ekin = nb[0].get_ekin
  epot = nb[0].get_epot
  etot = ekin + epot
  einit = etot if init_flag
  STDERR.print "at time t = ", nb[0].get_time, " , after ", nsteps,
               " steps :\n  E_kin = ", ekin, " , E_pot = ", epot,
               " , E_tot = ", etot, "\n                ",
               "absolute energy error: E_tot - E_init = ", etot - einit,
               "\n                ",
               "relative energy error: (E_tot - E_init) / E_init = ",
               (etot - einit) / einit, "\n"
  if x_flag
    STDERR.print "  for debugging purposes, here is the internal data ",
                 "representation:\n"
    diag_pp_snapshot(nb)
  end
end

def get_acc_jerk_pot_coll(nb)
  nb.each do |b| b.clear_acc_and_jerk end
  nb[0].clear_epot
  nb[0].reset_coll_time_q

  for i in 0...nb.size
    nb[i].cbody.set_pos_vel_and_mass(nb[i]);
    nb[i].cbody.set_acc_jerk(nb[i]);
  end
  nb[0].cbody.set_epot_and_collq(Body)

  i = 0
  while (i < nb.size) 
    j = i+1
    while (j < nb.size)
#      nb[i].pairwise_acc_jerk_pot_coll(nb[j])
#      pairwize_force(nb[i],nb[j],Body)
      nb[i].cbody.pairwize_force(nb[j].cbody)
      j += 1
    end
    i += 1
  end

  for i in 0...nb.size
    nb[i].cbody.get_acc_jerk(nb[i]);
  end
  nb[0].cbody.get_epot_and_collq(Body)

end

def get_acc_jerk_pot_coll_in_c(nb)
  nb.each do |b| b.cbody.clear_acc_and_jerk end
  nb[0].clear_epot
  nb[0].reset_coll_time_q
  nb[0].cbody.set_epot_and_collq(Body)

  i = 0
  while (i < nb.size) 
    j = i+1
    while (j < nb.size)
      nb[i].cbody.pairwize_force(nb[j].cbody)
      j += 1
    end
    i += 1
  end

  nb[0].cbody.get_epot_and_collq(Body)

end

def predict_step(nb, dt)
  nb.each do |b| b.predict_step(dt) end
end

def correct_step(nb, dt)
  nb.each do |b| b.correct_step(dt) end
end

def evolve_step_old(nb, dt)
  nb.each do |b| b.update_old_coordinates end
  predict_step(nb, dt)
  get_acc_jerk_pot_coll(nb)
  correct_step(nb, dt)
  nb[0].inc_time(dt)
end

def evolve_step(nb, dt)
  nb.each do |b| b.cbody.copy_to_c(b) end
  nb[0].cbody.set_dt(dt)
  nb.each do |b| b.cbody.predict end
  get_acc_jerk_pot_coll_in_c(nb)
  nb.each do |b| b.cbody.correct end
  nb.each do |b| b.cbody.copy_from_c(b) end
  nb[0].inc_time(dt)
end

def evolve(nb, dt_param, dt_dia, dt_out, dt_tot, init_out, x_flag)

  STDERR.print "Starting a Hermite integration for a ", nb.size,
               "-body system,\n  from time t = ", nb[0].get_time, 
               " with time step control parameter dt_param = ", dt_param,
               "  until time ", nb[0].get_time + dt_tot,
               " ,\n  with diagnostics output interval dt_dia = ", dt_dia,
               ",\n  and snapshot output interval dt_out = ", dt_out, ".\n"

  get_acc_jerk_pot_coll(nb)

  nsteps = 0

  einit = 0.0    # undefined before first diagnostics call
  write_diagnostics(nb, nsteps, einit, true, x_flag)
  einit = nb[0].get_ekin + nb[0].get_epot

  put_snapshot(nb) if init_out

  t = nb[0].get_time
  t_dia = t + dt_dia
  t_out = t + dt_out
  t_end = t + dt_tot

  loop do
    while t < t_dia and t < t_out and t < t_end
      dt = dt_param * nb[0].get_collision_time
      evolve_step(nb, dt)
      t = nb[0].get_time
      nsteps += 1
    end
    if t >= t_dia
      write_diagnostics(nb, nsteps, einit, false, x_flag)
      t_dia += dt_dia
    end
    if t >= t_out
      put_snapshot(nb)
      t_out += dt_out
    end
    if t >= t_end
      break;
    end
  end
end

dt_param, dt_dia, dt_out, dt_tot, init_out, x_flag = read_options(parser)

STDERR.print "dt_param = ", dt_param, "\n",
      "dt_dia = ", dt_dia, "\n",
      "dt_out = ", dt_out, "\n",
      "dt_tot = ", dt_tot, "\n",
      "init_out = ", init_out, "\n",
      "x_flag = ", x_flag, "\n"

nb = get_snapshot

#put_snapshot(nb)
#pp_snapshot(nb)

evolve(nb, dt_param, dt_dia, dt_out, dt_tot, init_out, x_flag)



#b = Body.new(1.0, [1.0, 0.0, 0.0], [0.0, 0.1, 0.2])
#b.pp
#b.simple_print
#b = Body.new
#b.pp
