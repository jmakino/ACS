class Vector < Array
#:segment start: binplus
  def +(a)
    sum = Vector.new                                   
    self.each_index{|k| sum[k] = self[k]+a[k]}
    sum
  end
#:segment end:
#:segment start: binmin
  def -(a)
    diff = Vector.new
    self.each_index{|k| diff[k] = self[k]-a[k]}
    diff
  end
#:segment end:
#:segment start: unmin
  def -@
    self.map{|x| -x}.to_v
  end
#:segment end:
#:segment start: unplus
  def +@
    self
  end
#:segment end:
#:segment start: binmul
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
#:segment end:
#:segment start: bindiv
  def /(a)
    if a.class == Vector
      raise
    else
      quotient = Vector.new           # scalar quotient
      self.each_index{|k| quotient[k] = self[k]/a}
    end
    quotient
  end
#:segment end:
end

class Array
  def to_v
    Vector[*self]
  end
end
