#require "clop.rb"


class NBody
  
#  attr_accessor :time, :body, :rootnode
  
  
  def readx
    n = gets.to_i
    @time = gets.to_f
    n.times{|i|
      @body[i] = Body.new
      @body[i].read
    }
  end

end
