#<i> Leapfrog implements the leapfrog (or Verlet) algorithm </i>

module Leapfrog

  include Math

  def predict_step(dt)
    for k in 0...NDIM
      @vel[k] += @acc[k]*0.5*dt
      @pos[k] += @vel[k]*dt
    end
  end

  def correct_step(dt)
    for k in 0...NDIM
      @vel[k] += @acc[k]*0.5*dt
    end
  end

  def pairwise_acc_pot(other)
    rji = Array.new(NDIM)
    for k in 0...NDIM
      rji[k] = other.pos[k] - @pos[k]
    end
    r2 = 0.0
    for k in 0...NDIM
      r2 += rji[k] * rji[k]
    end
    r = sqrt(r2)
    r3 = r * r2

    dpot = -@mass * other.mass / r

    da = Array.new(NDIM)
    for k in 0...NDIM
      da[k] = rji[k] / r3
    end
    for k in 0...NDIM
      self.acc[k] += other.mass * da[k]
      other.acc[k] -= self.mass * da[k]
    end

    return dpot
  end

end
