include Math

N = 1000000

a = 1.0

def new_a(old_a)
  return sqrt( (old_a * (old_a + 1))/(old_a + 0.001*old_a) )
end

N.times{a = new_a(a)}

print "a = #{a}\n"
