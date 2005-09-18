class Vector < Array
  def +(a)
    sum = Vector.new
    self.each_index{|k| sum[k] = self[k]+a[k]}
    sum
  end
  def -(a)
    diff = Vector.new
    self.each_index{|k| diff[k] = self[k]-a[k]}
    diff
  end
  def -@
    self.map{|x| -x}.to_v
  end
  def +@
    self
  end
  def *(a)
    if a.class == Vector              # inner product
      product = 0
      self.each_index{|k| product += self[k]*a[k]}
    else
      product = Vector.new           # scalar product
      self.each_index{|k| product[k] = self[k]*a}
    end
    product
  end
  def /(a)
    if a.class == Vector
      raise
    else
      quotient = Vector.new           # scalar quotient
      self.each_index{|k| quotient[k] = self[k]/a}
    end
    quotient
  end
  def cross(other)                   # outer product
    if other.size == 3
      result = Vector.new
      result[0] = self[1]*other[2] - self[2]*other[1]
      result[1] = self[2]*other[0] - self[0]*other[2]
      result[2] = self[0]*other[1] - self[1]*other[0]
      result
    elsif other.size == 2
      self[0]*other[1] - self[1]*other[0]
    elsif other.size == 1
      0
    else
      raise "dimension = #{other.size} not supported"
    end
  end
end

class Array
  def to_v
    Vector[*self]
  end
end

class Float
  alias :original_mult :*
  def *(a)
    if a.class == Vector
      a*self
    else
      original_mult(a)
    end
  end
end

class Fixnum
  alias :original_mult :*
  def *(a)
    if a.class == Vector
      a*self
    else
      original_mult(a)
    end
  end
end

class Body

  attr_accessor :mass, :pos, :vel      # mass, position, velocity

  def to_s
    "mass = " + @mass.to_s +
    "   pos = " + @pos.join(", ") +
    "   vel = " + @vel.join(", ") + "\n"
  end

  def pp(indent = 0)               # pretty print
    print " "*indent + to_s
  end

  def write
    printf("%22.15e", @mass)
    @pos.each do |x| printf("%23.15e", x) end
    @vel.each do |x| printf("%23.15e", x) end
    print "\n"
  end

  def read
    a = gets.split.collect{|x| x.to_f}
    @mass = a[0]
    @pos = a[1..3].to_v
    @vel = a[4..6].to_v
  end

end

class Nbody

  attr_accessor :time, :body, :rootnode

  def n
    @body.size
  end

  def initialize(n=0, time = 0.0)
    @body = [Body.new]
    for i in 0...n
      @body[i] = Body.new
    end
    @time = time
  end

  def pp                                # pretty print
    print "     N = ", @body.size, "\n"
    print "  time = ", @time, "\n"
    @body.each do |b| b.pp end
  end

  def write
    print @body.size, "\n"
    printf("%22.15e\n", @time)
    @body.each do |b| b.write end
  end

  def read
    n = gets.to_i
    @time = gets.to_f
    for i in 0...n
      @body[i] = Body.new
      @body[i].read
    end
  end

  def maketree
    @rootnode = makerootnode
    @body.each do |b|
      @rootnode.loadtree(b)
    end
  end

  def makerootnode
    r = @body.inject(0){|oldmax, b| [oldmax, b.pos.map{|x| x.abs}.max].max}
    s = 1
    s *= 2 while r > s
    Node.new([0.0, 0.0, 0.0], s)
  end

end

class Node

  def initialize(center, size)
    @center, @size = center.to_v, size
    @child = Array.new(8)
  end

  def octant(pos)
    result = 0
    pos.each_index do |i| 
      result *= 2
      result += 1 if pos[i] > @center[i]
    end
    result
  end

  def loadtree(b)
    corner = octant(b.pos)
    if @child[corner] == nil
      @child[corner] = b
      return
    end
    if @child[corner].class == Body
      tmp_b = @child[corner]
      child_size = @size / 2.0
      @child[corner] = Node.new(@center + child_size*offset(corner),child_size)
      @child[corner].loadtree(tmp_b)
    end
    @child[corner].loadtree(b)
  end

  def offset(corner)
    r=[]
    3.times{ r.unshift( (corner & 1)*2 - 1 ) ; corner>>=1 }
    r.to_v
  end

  def pp(indent = 0)
    print " "*indent+"node: center = #{@center.join(" ")} ; size = #{@size}\n"
    @child.each{|c| c.pp(indent + 2) if c}
  end

  def check_body_in_cell
    @child.each do |c|
      if c.class == Body
        (c.pos - @center).each do |x|
          raise("\nbody out of cell:\n#{c.to_s}\n") if x.abs > @size
        end
      elsif c.class == Node
        c.check_body_in_cell
      end
    end
  end

end

nb = Nbody.new
nb.read
nb.maketree
nb.rootnode.pp
#nb.body[0].pos += [10, 0, 0].to_v
nb.rootnode.check_body_in_cell
