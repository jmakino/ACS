class Array
  def +(a)
    sum = []
    self.each_index{|k| sum[k] = self[k]+a[k]}
    sum
  end
end
