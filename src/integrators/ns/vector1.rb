#!/usr/bin/ruby
#
# vector1.rb
#
# a toy version of a three-dimensional vector class
# implemented without an array
#
#==============================================================================

#  note: for now, NDIM = 3 is understood

class Vector

  attr_accessor :v1, :v2, :v3

  def initialize(components = [0,0,0])
    @v1 = components[0]
    @v2 = components[1]
    @v3 = components[2]
  end

  def set(components = [0,0,0])
    @v1 = components[0]
    @v2 = components[1]
    @v3 = components[2]    
  end

  def clear
    @v1 = @v2 = @v3 = 0
  end

  def +(other)
    w = self.dup
    w.v1 += other.v1
    w.v2 += other.v2
    w.v3 += other.v3
    return w
  end

  def -(other)
    w = self.dup
    w.v1 -= other.v1
    w.v2 -= other.v2
    w.v3 -= other.v3
    return w
  end

  def dot(other)                            # inner product of vectors
    s = @v1 * other.v1 + @v2 * other.v2 + @v3 * other.v3
  end

  def *(other)                              # multiplication vector * scalar
    w = self.dup
    w.v1 *= other
    w.v2 *= other
    w.v3 *= other
    return w
  end

  def /(other)                              # division vector / scalar
    w = self.dup
    w.v1 /= other
    w.v2 /= other
    w.v3 /= other
    return w
  end

  def to_f
    @v1 = v1.to_f
    @v2 = v2.to_f
    @v3 = v3.to_f
  end

  def to_s
    "[#{@v1}, #{@v2}, #{@v3}]"
  end

  def pp            # pretty print
    print to_s
  end

  def full_print
    printf("%23.15e%23.15e%23.15e\n", @v1, @v2, @v3)
  end

  def ppn            # pretty print with new line
    print to_s
    print "\n"
  end

  def read
    s = gets
    a = s.split
    @v1 = a[0]
    @v2 = a[1]
    @v3 = a[2]
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
