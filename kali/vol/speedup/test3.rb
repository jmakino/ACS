require "vector.rb"

include Math

NDIM = 3
EPS = 0.000001

n = gets.to_i

r = Array.new(n)
r.each_index do |i|
  v = Vector.new(NDIM)
  (0...NDIM).each{|k| v[k] = (i * NDIM + k) * EPS}
  r[i] = v
end

sum = 0
r.each_index do |i|
  (i+1...n).each do |j|
    rji = r[j]-r[i]
    sum += rji*rji
  end
end

print "n = #{n} ; sum = #{sum}\n"
