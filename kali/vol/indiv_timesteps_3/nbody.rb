$acsdate="20050304"
require "acs"

class Body

# naming convention for instance variables in class Body:
#
# @mass    : mass 
# @pos     : position
# @vel     : velocity
# @idn     : identification number of a body, typically used when
#            numbering the bodies consecutively within an N-body system

end

class NBody

  ACS_OUTPUT_NAME = self.to_s

  attr_accessor :time, :body             # an array with elements of class Body

  def initialize
    @body = []
    @time = 0.0
  end

  def -(other)
    if other.class != self.class
      raise "other.class.name = #{other.class.name} != #{self.class.name}"
    end
    if (n = other.body.size) != body.size
      raise "other.body.size = #{other.body.size} != #{body.size}"
    end
    nb = NBody.new
    body.each_index do |i|
      ob = other.body.find{|oi| oi.body_id == body[i].body_id}
      if ob == nil
        raise "body_id = #{body[i].body_id} not found in other N-body system"
      end
      nb.body[i] = Body.new
      nb.body[i].pos = body[i].pos - ob.pos
      nb.body[i].vel = body[i].vel - ob.vel
    end
    nb
  end

  def abs
    a = 0
    body.each{ |b| a += b.pos*b.pos + b.vel*b.vel }
    sqrt a
  end

end
