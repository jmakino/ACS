struct Vector
  property :x, :y, :z
  def initialize(x : Float64,  y : Float64,  z : Float64)
    @x=x
    @y=y
    @z=z
  end
  def initialize(n : Int,  x : Float64)
    @x=x
    @y=x
    @z=x
  end
  def initialize
    @x=0.0
    @y=0.0
    @z=0.0
  end
  def +(a)
    Vector.new(@x+a.x, @y+a.y, @z+a.z)
  end
  def -(a)
    Vector.new(@x-a.x, @y-a.y, @z-a.z)
  end

  def -
    Vector.new(-@x, -@y, -@z)
  end
  def +
      self
  end

  def *(a : Vector)
    # inner product
    @x*a.x+ @y*a.y+ @z*a.z
  end
  def *(a : Float)
    Vector.new(@x*a, @y*a, @z*a)
  end
  def /(a)
    if a.class == Vector
      raise "division by 0"
    else
      Vector.new(@x/a, @y/a, @z/a)
    end
  end
  def cross(other)                   # outer product
    Vector.new(@y*other.z - @z*other.y,
               @z*other.x - @x*other.z,
               @x*other.y - @y*other.x)
  end
  def to_a
    [@x, @y, @z]
  end

  macro method_missing(call)
    to_a.{{call}}
  end
end

class Object
  macro alias_method(new, old)
    def {{new}}(*args, **kwargs)
      {{old}}(*args, **kwargs)
    end
  end
end

struct Struct
  macro alias_method(new, old)
    def {{new}}(*args, **kwargs)
      {{old}}(*args, **kwargs)
    end
  end
end

class Array
  def to_v
    Vector.new(self[0],self[1],self[2])
  end
end

struct Float64
  def *(a : Vector)
      a*self
  end
end
struct Float32
  def *(a : Vector)
      a*self
  end
end


# class Integer
#    alias_method :original_mult, :*
#   def *(a)
#     if a.class == Vector
#       a*self
#     else
#       original_mult(a)
#     end
#   end
# end
