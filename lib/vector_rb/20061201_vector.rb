class Vector < Array
  def +(a)
    sum = Vector.new
    self.each_index{|k| sum[k] = self[k]+a[k]}
    sum
  end
  def -(a)
    diff = Vector.new
    self.each_index{|k| diff[k] = self[k]-a[k]}
    diff
  end
  def -@
    self.map{|x| -x}.to_v
  end
  def +@
    self
  end
  def *(a)
    if a.class == Vector              # inner product
      product = 0
      self.each_index{|k| product += self[k]*a[k]}
    else
      product = Vector.new           # scalar product
      self.each_index{|k| product[k] = self[k]*a}
    end
    product
  end
  def /(a)
    if a.class == Vector
      raise
    else
      quotient = Vector.new           # scalar quotient
      self.each_index{|k| quotient[k] = self[k]/a}
    end
    quotient
  end
  def cross(other)                   # outer product
    if other.size == 3
      result = Vector.new
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

class Array
  def to_v
    Vector[*self]
  end
end

class Float
  alias :original_mult :*
  def *(a)
    if a.class == Vector
      a*self
    else
      original_mult(a)
    end
  end
end

class Fixnum
  alias :original_mult :*
  def *(a)
    if a.class == Vector
      a*self
    else
      original_mult(a)
    end
  end
end
