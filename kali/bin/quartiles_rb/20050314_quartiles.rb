#!/usr/local/bin/ruby -w

require "acs"

class Body

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

end

class NBody

  attr_accessor :time, :body

  def initialize
    @body = []
  end

  def order_squared_radii
    a = []
    @body.each{|b| a.push b.pos*b.pos}
    a.sort!
  end    

  def quartiles
    a = order_squared_radii
    n = a.size
    r_1 = sqrt(a[(n/4.0).round - 1])                                         #1
    r_2 = sqrt(a[(n/2.0).round - 1])                                         #1
    r_3 = sqrt(a[(n*3/4.0).round - 1])                                       #1
    print "The values of the three quartiles for r(M) are:\n"
    print "  r(1/4) = "
    printf("%.4g\n", r_1)
    print "  r(1/2) = "
    printf("%.4g\n", r_2)
    print "  r(3/4) = "
    printf("%.4g\n", r_3)
  end

end

include Math

nb = ACS_IO.acs_read(NBody)
nb.quartiles
