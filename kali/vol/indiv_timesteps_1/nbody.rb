require "acs.rb"

class Body

  attr_accessor :mass, :pos, :vel

end

class NBody

  attr_accessor :time, :body

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
      if (id = body[i].body_id) == nil
        raise "body[#{i}].body_id == nil"
      end
      ob = other.body.find{|oi| oi.body_id == id}
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

  def abs_pos
    a = 0
    body.each{ |b| a += b.pos*b.pos }
    sqrt a
  end

  def abs_vel
    a = 0
    body.each{ |b| a += b.vel*b.vel }
    sqrt a
  end

end
