#<i>extends Body class in order to provide pot/kin energy diagnostics</i>

require "body.rb"

class Body

  def single_kin
    ekin = 0
    for k in 0...NDIM
      ekin += 0.5 * @mass * @vel[k] * @vel[k]
    end
    return ekin
  end

  def pairwise_pot(other)
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
    return -@mass * other.mass / r
  end

end
