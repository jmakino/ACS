include Comparable

class Block_time

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
    contract
  end

  def get_time
    # should return a floating point value of the time
  end

  def contract
    while @time_array[@time_array.size - 1] == 0
      break unless @time_array.pop
    end
  end

  def expand
    i = @time_array.size
    while i < TIME_ARRAY_MAX_LENGTH
      @time_array[i] = 0
      i += 1
    end
  end

  def <=>(a)
    if a.class == Block_time
      return -1 if @time_int < a.time_int
      return 1 if @time_int > a.time_int
      self_size = @time_array.size
      other_size = a.time_array.size
      for i in 0...TIME_ARRAY_MAX_LENGTH
        if self_size <= i 
          if other_size <= i
            return 0
          else
            if a.time_array[i] == 1
              return -1
            end
          end
        end
        if other_size <= i
          if @time_array[i] == 1
            return 1
          end
        end
      end
    end
    if a.class == Float or a.class == Fixnum
      a_block = Block_time.new(a)
      return self <=> a_block
    end
  end

  def +(a)
    if a.class == Block_time
      sum = Block_time.new
      expand
      a.expand
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
      sum.contract
      return sum
    end
    if a.class == Float or a.class == Fixnum
      a_block = Block_time.new(a)
      return self + a_block
    end
  end

  def to_s
    "{" + @time_int.to_s + ", [" + @time_array.join(", ") + "]}\n"
  end

end
