#!/usr/bin/ruby
#
# vector2.rb
#
# a toy version of a three-dimensional vector class
# implemented with an array
#
#==============================================================================

#  note: for now, NDIM = 3 is understood

class Vector

  attr_accessor :components

  def initialize(components = [0,0,0])
    @components = components
  end

  def set(components = [0,0,0])
    @components[0] = components[0]
    @components[1] = components[1]
    @components[2] = components[2]
  end

  def clear
    self.set
  end

  def +(other)
##
## this didn't work, and made w point to self:
##
##print "entering +\n"
##print "self = " ; self.ppn
##print "other = " ; other.ppn
##    w = self.dup
##print "w = " ; w.ppn
##    w.components[0] += other.components[0]
##    w.components[1] += other.components[1]
##    w.components[2] += other.components[2]
##print "w = " ; w.ppn
##print "self = " ; self.ppn
##print "other = " ; other.ppn
##print "exiting +\n"
##    return w
##
    w = Vector.new
    w.components[0] = @components[0] + other.components[0]
    w.components[1] = @components[1] + other.components[1]
    w.components[2] = @components[2] + other.components[2]
    return w
  end

  def -(other)
    w = Vector.new
    w.components[0] = @components[0] - other.components[0]
    w.components[1] = @components[1] - other.components[1]
    w.components[2] = @components[2] - other.components[2]
    return w
  end

  def dot(other)                            # inner product of vectors
    s = @components[0] * other.components[0] + @components[1] * other.components[1] + @components[2] * other.components[2]
  end

  def *(other)                              # multiplication vector * scalar
    w = Vector.new
    w.components[0] = @components[0] * other
    w.components[1] = @components[1] * other
    w.components[2] = @components[2] * other
    return w
  end

  def /(other)                              # division vector / scalar
    w = Vector.new
    w.components[0] = @components[0] / other
    w.components[1] = @components[1] / other
    w.components[2] = @components[2] / other
    return w
  end

  def to_f
    @components[0] = components[0].to_f
    @components[1] = components[1].to_f
    @components[2] = components[2].to_f
  end

  def to_s
    "[#{@components[0]}, #{@components[1]}, #{@components[2]}]"
  end

  def pp            # pretty print
    print to_s
  end

  def full_print
    printf("%23.15e%23.15e%23.15e\n", @components[0], @components[1], @components[2])
  end

  def ppn            # pretty print with new line
    print to_s
    print "\n"
  end

  def read
    s = gets
    a = s.split
    @components[0] = a[0]
    @components[1] = a[1]
    @components[2] = a[2]
  end
end

#def pp(c)
#  print c, " = "
#  c.pp     # how to get back to the variable name ???
#end

print "vector a : "
a = Vector.new
a.read
a.ppn
a.full_print
print "vector b : "
b = Vector.new
b.read
b.ppn
b.full_print
c = a + b
c.ppn
print "a = " ; a.ppn
print "b = " ; b.ppn
print "c = " ; c.ppn
a.to_f
b.to_f
c = a + b
c.ppn
print "a = " ; a.ppn
print "b = " ; b.ppn
print "c = " ; c.ppn
