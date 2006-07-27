class Vector < Array
  def plus(a)
    sum = Vector.new
    self.each_index{|k| sum[k] = self[k]+a[k]}
    return sum
  end
end
