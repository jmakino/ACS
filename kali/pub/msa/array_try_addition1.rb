class Array
  def plus(a)
    sum = []
    self.each_index{|k| sum[k] = self[k]+a[k]}
    return sum
  end
end
