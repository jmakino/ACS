include Math

N = 1000000

a = 1.0

N.times{a = sqrt( (a * (a + 1))/(a + 0.001*a) )}

print "N = #{N} ; a = #{a}\n"
