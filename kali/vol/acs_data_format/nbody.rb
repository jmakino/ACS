require "acs"

class Body

  attr_accessor :mass, :pos, :vel

end

class NBody

  attr_accessor :time, :body

  def initialize
    @body = []
    @time = 0.0
  end

end
