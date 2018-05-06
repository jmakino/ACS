class Vector < Array(Float64)
  def +(a)
    sum = Vector.new(3,0.0)
    self.each_index{|k| sum[k] = self[k]+a[k]}
    sum
  end
  def -(a)
    diff = Vector.new(3,0.0)
    self.each_index{|k| diff[k] = self[k]-a[k]}
    diff
  end
  def -
    self.map{|x| -x}.to_v
  end
  def +
    self
  end
  def *(a : Vector)
     # inner product
    product = 0
    self.each_index{|k| product += self[k]*a[k]}
    product
  end
  def *(a : Float)
    product = Vector.new(3,0.0)           # scalar product
    self.each_index{|k| product[k] = self[k]*a}
    product
  end
  def /(a)
    if a.class == Vector
      raise "division by 0"
    else
      quotient = Vector.new(3,0.0)           # scalar quotient
      self.each_index{|k| quotient[k] = self[k]/a}
    end
    quotient
  end
  def cross(other)                   # outer product
    if other.size == 3
      result = Vector.new(3,0.0)
      result[0] = self[1]*other[2] - self[2]*other[1]
      result[1] = self[2]*other[0] - self[0]*other[2]
      result[2] = self[0]*other[1] - self[1]*other[0]
      result
    elsif other.size == 2
      self[0]*other[1] - self[1]*other[0]
    elsif other.size == 1
      0
    else
      raise "dimension = #{other.size} not supported"
    end
  end
end

class Object
  macro alias_method(new, old)
    def {{new}}(*args, **kwargs)
      {{old}}(*args, **kwargs)
    end
  end
end

struct Struct
  macro alias_method(new, old)
    def {{new}}(*args, **kwargs)
      {{old}}(*args, **kwargs)
    end
  end
end

class Array
  def to_v
    result = Vector.new(3,0.0)
    self.each_index{|k| result[k] = self[k]}
    result
  end
end

struct Float64
  def *(a : Vector)
      a*self
  end
end
struct Float32
  def *(a : Vector)
      a*self
  end
end


# class Integer
#    alias_method :original_mult, :*
#   def *(a)
#     if a.class == Vector
#       a*self
#     else
#       original_mult(a)
#     end
#   end
# end
