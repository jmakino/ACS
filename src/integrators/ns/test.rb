require 'pairwize'
say_hello
hello2 2, 2.5
hello1 1
hello1 1.0


class Body

  NDIM = 3

  attr_accessor :mass, :pos, :vel, :acc, :jerk,
                :old_pos, :old_vel, :old_acc, :old_jerk, :cbody

  def initialize(mass = 0, pos = [0.0,0.0,0.0], vel = [0.0,0.0,0.0])
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

x = Body.new
y = Body.new
x.mass = 1
y.mass = 1
x.pos = [1.0,1.0,0.0]
y.pos = [-1.0,-1.0,0.0]
x.clear_acc_and_jerk
y.clear_acc_and_jerk
x.clear_epot
x.reset_coll_time_q
print "pos = ";p x.pos
print "acci = ";p x.acc

pairwize_force(x, y, Body)
p x.mass
print "pos = ";p x.pos
print "acci = ";p x.acc
print "accj = ";p y.acc
p x.acc


x.clear_acc_and_jerk
y.clear_acc_and_jerk

x.cbody.set_pos_vel_and_mass(x);
y.cbody.set_pos_vel_and_mass(y);

x.cbody.set_acc_jerk(y);
y.cbody.set_acc_jerk(y);

p x.acc

x.cbody.pairwize_force(y.cbody);

x.cbody.get_acc_jerk(x);
y.cbody.get_acc_jerk(y);

p x.acc
y=x
p y.acc
x.clear_acc_and_jerk
p y.acc
cb = x.cbody
x.acc=[1,2,3]
p x.acc
x.cbody.get_acc_jerk(x);
p x.cbody
p cb

class Cbody
  def set_xx(a)
    @xx = a
  end
  def get_xx
    @xx
  end
end

cb.set_xx(2)
p cb.get_xx
x.clear_acc_and_jerk
p x.acc
x.cbody.get_acc_jerk(x);
p x.acc
  
class Cbody
  attr_accessor :xx
end
p cb.xx





