require "acs.rb"

class Body

  ACS_IO_NAME = self.to_s

  attr_accessor :mass, :pos, :vel

  def initialize
    @mass = 0.0
    @pos = Vector.new
    @vel = Vector.new
  end

end

class Nbody

  ACS_IO_NAME = self.to_s

  attr_accessor :body

  def initialize
    @body = []
  end

end
