class Vector < Array
  def +(a)
    sum=Vector.new
    self.each_index{|k| sum[k]=self[k]+a[k]}
    sum
  end
  def *(a)
    if a.class == Vector              # inner product
      product=0
      self.each_index{|k| product += self[k]*a[k]}
    else
      product=Vector.new             # scalar product
      self.each_index{|k| product[k]=self[k]*a}
    end
    product
  end
end

class Array
  def to_v
    Vector[*self]
  end
end
