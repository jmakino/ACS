#
# for now, no tools for dealing with numbers with different scale factors
#

class Block_time

  include Comparable

  TIME_ARRAY_MAX_LENGTH = 50     # 2**-50 = 9e-16 , okay for double precision

  attr_accessor :scale_factor, :time_int, :time_array

  def initialize(time = 0, scale_factor = 1.0)
    @time_array = []
    set_time(time, scale_factor)
  end

  def set_time(time, factor = 1)
    @scale_factor = factor
    tau = time/factor.to_f
    @time_int = tau.floor
    rest = tau - @time_int                          # 0 <= remainder < 1
    for i in 0...TIME_ARRAY_MAX_LENGTH
      rest *= 2
      @time_array[i] = rest.floor
      rest = rest - rest.floor
    end
  end

  def to_f
    rest = 1.0
    number = @time_int
    for i in 0...@time_array.size
      rest = rest * 0.5
      number += rest * @time_array[i]	
    end
    number*@scale_factor
  end

  def <=>(a)
    if a.class == Block_time
      raise if @scale_factor != a.scale_factor
      return -1 if @time_int < a.time_int
      return 1 if @time_int > a.time_int
      for i in 0...TIME_ARRAY_MAX_LENGTH
        return -1 if time_array[i] < a.time_array[i]
        return 1 if time_array[i] > a.time_array[i]
      end
      return 0
    end
    if a.class == Float or a.class == Fixnum
      return self.to_f <=> a
    end
  end

  def +(a)
    if a.class == Block_time
      raise if @scale_factor != a.scale_factor
      sum = Block_time.new
      sum.scale_factor = @scale_factor
      i = TIME_ARRAY_MAX_LENGTH - 1
      carry = 0
      while i >= 0
        total = @time_array[i] + a.time_array[i] + carry
        case total
          when 0
            sum.time_array[i] = 0
            carry = 0
          when 1
            sum.time_array[i] = 1
            carry = 0
          when 2
            sum.time_array[i] = 0
            carry = 1
          when 3
            sum.time_array[i] = 1
            carry = 1
        end
        i -= 1
      end
      sum.time_int = @time_int + a.time_int + carry
      return sum
    end
    if a.class == Float or a.class == Fixnum
      a_block = Block_time.new(a, @scale_factor)
      return self + a_block
    end
  end

  def -(a)
    if a.class == Block_time
      return self + (-a)
    end
    if a.class == Float or a.class == Fixnum
      a_block = Block_time.new(a, @scale_factor)
      return self - a_block
    end
  end

#
# well, why does this not work ???
#
#  def =(a)
#    @scale_factor = a.scale_factor
#    @time_int = a.time_int
#    @time_array.each_index do |i|
#      @time_array[i] = a.time_array[i]
#    end
#  end

  def clone                                               # this is a deep copy
    a = Block_time.new
    a.scale_factor = @scale_factor
    a.time_int = @time_int
    @time_array.each_index do |i|
      a.time_array[i] = @time_array[i]
    end
    a
  end

  def -@
    minus_self = self.clone
    minus_self.contract
    arr = minus_self.time_array
    for i in 0..(arr.size-2)
      if arr[i] == 1
        arr[i] = 0
      else
        arr[i] = 1
      end
    end
    minus_self.time_int = -minus_self.time_int
    minus_self.time_int -= 1 if arr.size > 0
    minus_self.expand
    minus_self
  end

  def +@
    self.clone
  end

  def contract
    while @time_array.last == 0
      break unless @time_array.pop
    end
    self
  end

  def expand
    i = @time_array.size
    while i < TIME_ARRAY_MAX_LENGTH
      @time_array[i] = 0
      i += 1
    end
    self
  end

#
# t = t.int + t.bit + t.rest      :      integer part, leading binary bit, rest
#

  def int
    t = self.clone
    t.time_array = []
    t.expand
    t
  end

  def bit
    t = self.clone
    t.time_int = 0
    n = t.time_array.size
    for i in 0...n
      if t.time_array[i] == 1
        t.time_array[(i+1)...n] = nil
        t.expand
        return t
      end
    end
    t.time_array = []
    t.expand
    return t
  end

  def rest
    t = self.clone
    t.time_int = 0
    n = t.time_array.size
    for i in 0...n
      if t.time_array[i] == 1
        t.time_array[i] = 0
        return t
      end
    end
    return t
  end

  def commensurable?(t)
    raise if @scale_factor != t.scale_factor
    answer = self.contract.time_array.size >= t.contract.time_array.size
    expand
    t.expand
    answer
  end

  def halve
    k = @time_array.size
    while k > 1
      k -= 1
      @time_array[k] = @time_array[k-1]
    end
    i = @time_int
    if 2*(i/2) == i          # i even
      @time_int = i/2
      @time_array[0] = 0
    else                     # i odd
      @time_int = (i/2.0).floor
      @time_array[0] = 1
    end
    self
  end

  def to_s
    contract
    s = "{" + @scale_factor.to_s + ", " + @time_int.to_s +
        ", [" + @time_array.join(", ") + "]}\n"
    expand
    s
  end

end

class Float

  def to_b(scale_factor = 1)
    Block_time.new(self, scale_factor)
  end

end

class Fixnum

  def to_b(scale_factor = 1)
    Block_time.new(self, scale_factor)
  end

end

class String

  def to_b(scale_factor = 1)
    Block_time.new(self.to_f, scale_factor)
  end

end

if __FILE__ == $0

a = Block_time.new
b = Block_time.new
a.set_time(3.14)
b.set_time(2.04)
p (a+b).to_f
p (-a).to_f
p (-b).to_f
p (a-b).to_f
p -(a-b).to_f
p -((a-b).to_f)
p (-(a-b)).to_f
p 9.87654321.to_b.to_f
print 2.75.to_b.contract.to_s
p 2.75.to_b.contract.to_f
print 2.75.to_b(2).contract.to_s
p 2.75.to_b(2).contract.to_f
print 2.75.to_b(0.25).contract.to_s
p 2.75.to_b(0.25).contract.to_f
print 2.75.to_b(3).contract.to_s
p 2.75.to_b(3).contract.to_f

end
