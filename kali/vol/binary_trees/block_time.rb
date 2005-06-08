class Block_time

  attr_reader :time

  def initialize(n = 0)
    @time = n
  end

  @@time_unit_exponent = 0

  def Block_time.scale_time_unit(factor)
    delta_time_unit_exponent = frexp(factor)[1]-1
    @@time_unit_exponent += delta_time_unit_exponent
    fraction = 2**(-delta_time_unit_exponent)
    ObjectSpace.each_object(Block_time){|x| x.rescale(fraction)}
  end

  def rescale(fraction)
    if fraction >= 1
      @time *= fraction
    else
      @time /= (1/fraction).to_i
    end
  end

  def to_f
    @time * 2**@@time_unit_exponent
  end

  def set(f)
    if @@time_unit_exponent < 0
      @time = (f * 2**(-@@time_unit_exponent)).floor
    else
      @time = (f / 2**@@time_unit_exponent).floor
    end
    self
  end

  def Block_time.to_b(f)
    Block_time.new.set(f)
  end

  def +(a)
    if a.class == Block_time
      Block_time.new(@time + a.time)
    else
      self.to_f + a
    end
  end

  def -(a)
    if a.class == Block_time
      Block_time.new(@time - a.time)
    else
      self.to_f - a
    end
  end

  def *(a)
    self.to_f * a
  end

  def /(a)
    self.to_f / a
  end

  def -@
    Block_time.new(-@time)
  end

  def +@
    self
  end

end

class Float
  alias :original_mult :*
  def *(a)
    if a.class == Block_time
      a*self
    else
      original_mult(a)
    end
  end
end

