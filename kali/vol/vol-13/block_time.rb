class Block_time

  include Comparable

  TIME_ARRAY_MAX_LENGTH = 45     # 2**-45 = 2.8e-14 , okay for double precision

  attr_accessor :time_int, :time_array

  def initialize(time = 0)
    @time_array = []
    set_time(time)
  end

  def set_time(t)
    @time_int = t.floor
    rest = t - @time_int                          # 0 <= remainder < 1
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
    number
  end

  def <=>(a)
    if a.class == Block_time
      return -1 if @time_int < a.time_int
      return 1 if @time_int > a.time_int
      for i in 0...TIME_ARRAY_MAX_LENGTH
        return -1 if time_array[i] < a.time_array[i]
        return 1 if time_array[i] > a.time_array[i]
      end
      return 0
    end
    if a.class == Float or a.class == Fixnum
      a_block = Block_time.new(a)
      return self <=> a_block
    end
  end

  def +(a)
    if a.class == Block_time
      sum = Block_time.new
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
      a_block = Block_time.new(a)
      return self + a_block
    end
  end

  def -(a)
    if a.class == Block_time
      return self + (-a)
    end
    if a.class == Float or a.class == Fixnum
      a_block = Block_time.new(a)
      return self - a_block
    end
  end

#
# well, why does this not work ???
#
#  def =(a)
#    new_a = Block_time.new
#    new_a.time_int = a.time_int
#    a.time_array.each_index do |i|
#      new_a.time_array[i] = a.time_array[i]
#    end
#  end    

  def copy                               # this is a deep copy (or deep clone)
    Marshal.load(Marshal.dump(self))
  end

  def -@
    minus_self = self.copy
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
    self
  end

  def contract
    while @time_array[@time_array.size - 1] == 0
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
# t = t.int + t.block + t.rest  :  integer part, leading binary part, rest
#

  def int
    t = self.copy
    t.time_array = []
    t.expand
    t
  end

  def block
    t = self.copy
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
    t = self.copy
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

  def to_s
    contract
    s = "{" + @time_int.to_s + ", [" + @time_array.join(", ") + "]}\n"
    expand
    s
  end

end

class Float

  def to_b
    Block_time.new(self)
  end

end

class Fixnum

  def to_b
    Block_time.new(self)
  end

end

class String

  def to_b
    Block_time.new(self.to_f)
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

end
