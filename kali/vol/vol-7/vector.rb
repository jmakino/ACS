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
  def *(a)
    if a.class == Vector              # inner product
      product = 0
      self.each_index{|k| product += self[k]*a[k]}
    else
      product = Vector.new            # scalar product
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
  def to_s(name = nil, precision = 16, indentation = 0)
    s = " " * indentation
    s += name + " = " if name
    s += self.map{|x| sprintf(" %#{precision+8}.#{precision}e", x)}.join
  end
end

class Array
  def to_v
    Vector[*self]
  end
end

class Float
  def to_s(name = nil, precision = 16, indentation = 0)
    s = " " * indentation
    s += name + " = " if name
    s += sprintf(" %#{precision+8}.#{precision}e", self)
  end
end
