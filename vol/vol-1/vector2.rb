class Vector < Array
  def +(a)
    sum = []
    self.each_index{|k| sum[k] = self[k]+a[k]}
    sum
  end
  def *(a)
    if a.class == Vector              # inner product
      iproduct = 0
      self.each_index{|k| iproduct += self[k]*a[k]}
      iproduct
    else
      sproduct = []                   # scalar product
      self.each_index{|k| sproduct[k] = self[k]*a}
      sproduct
    end
  end
end
