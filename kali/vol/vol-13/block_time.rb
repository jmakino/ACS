class block_time

  TIME_ARRAY_MAX_LENGTH = 45     # 2**-45 = 2.8e-14 , okay for double precision

  attr_accessor :time_int, :time_array

  def initialize(time = 0)
    set_time(time)
  end

  set_time(t)
     @time_int = t.floor
     

end
