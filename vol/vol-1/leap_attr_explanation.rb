# leap_attr_explanation.rb: 
#
# An explanation of the meaning of +attr_accessor+ for ACS, Volume 1.

class Body

NDIM = 3           # number of spatial dimensions

#:segment start: attr

  attr_accessor :mass

#:segment end:

#:segment start: meaning

  def mass
    @mass
  end

  def mass=(m)
    @mass = m
  end

#:segment end:

end

#:segment start: use1

the_mass = Body.mass

#:segment end:

#:segment start: use2

Body.mass = new_mass

#:segment end:



