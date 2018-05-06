#require "./clop.cr"
require "./vector.cr"


def get_self_other_acc(myself, other, eps)
  return Vector.new(3,0.0) if myself == other
  rji = myself.pos - other.pos
  r2 = eps * eps + rji * rji
  myself.mass * rji / (r2 * sqrt(r2))
end

struct Nil
  def to_i
    exit
  end
  def to_f
    exit
  end
  def split
    exit
  end
end  

class Body

  property :mass, :pos, :vel, :acc

  def initialize
    @pos = Vector.new
    @vel = Vector.new
    @acc = Vector.new
    @mass = 0.0
  end

  def get_other_acc( other, eps)
    return Vector.new(3,0.0) if self == other
#    print "get other acc ", @pos, " ", other.pos, " ", other.mass, "\n"

  rji = @pos  - other.pos 
  r2 = eps * eps + rji * rji
  @mass * rji / (r2 * sqrt(r2))
end

  def pairwise_acc(other, eps : Float)
    rji = other.pos - @pos
    r2 = eps * eps + rji * rji
    r = sqrt(r2)
    r3 = r * r2
    da = rji / r3
    self.acc += other.mass * da
    other.acc -= self.mass * da
  end

  def ekin                         # kinetic energy
    0.5*@mass*(@vel*@vel)
  end

  def epot(body_array, eps)        # potential energy
    p = 0
    body_array.each do |b|
      unless b == self
        r = b.pos - @pos
        p += -@mass*b.mass/sqrt(r*r + eps*eps)
      end
    end
    p
  end

  def get_node_acc(other, tol, eps)
#    get_self_other_acc(self, other, eps)
    get_other_acc(other, eps)
  end

  def to_s
    "mass = " + @mass.to_s +
    "   pos = " + @pos.join(", ") +
    "   vel = " + @vel.join(", ")
  end

  def pp(indent = 0)               # pretty print
    print " "*indent + to_s + "\n"
  end

  def ppx(body_array, eps)         # pretty print, with extra information (acc)
    STDERR.print to_s + "   acc = " + @acc.join(", ") + "\n"
  end
  def ppx
    STDERR.print to_s + "   acc = " + @acc.join(", ") + "\n"
  end

  def write
    printf("%22.15e", @mass)
    @pos.each do |x| printf("%23.15e", x) end
    @vel.each do |x| printf("%23.15e", x) end
    print "\n"
  end

  def read
    a = gets.split.map{|x| x.to_f}
#    p a
    @mass = a[0]
    @pos = a[1..3].to_v
    @vel = a[4..6].to_v
  end

  def loadtree(b)
    exit
  end

  def center_of_mass
    @pos
  end

end

struct Nil
    def loadtree(b)
    end
    def center_of_mass
      Vector.new(3,0.0)
    end
end

class NBody

  property :time, :body, :rootnode



  def initialize(n=0, time = 0.0)
    @body = [Body.new]
    n.times{|i|       @body[i] = Body.new}
    @time = time
    @eps = 0.0
    @e0=0.0
    @dt=0.015625
    @rootnode = Node.new([0.0,0.0,0.0].to_v, 1.0)
    @tol = 0.0
    @nsteps = 0
  end

  def evolve(tol : Float64,
      eps : Float64,
        dt : Float64, dt_dia : Float64,
	  dt_out : Float64, dt_end  : Float64,
	    init_out, x_flag)
    @dt = dt
    @tol = tol
    @eps = eps
    @nsteps = 0
    get_acc
    e_init
    write_diagnostics(x_flag)
    t_dia = dt_dia - 0.5*dt
    t_out = dt_out - 0.5*dt
    t_end = dt_end - 0.5*dt

    write if init_out

    while @time < t_end
      leapfrog
      @time += @dt
      @nsteps += 1
      if @time >= t_dia
        write_diagnostics(x_flag)
        t_dia += dt_dia
      end
      if @time >= t_out
        write
        t_out += dt_out
      end
    end
  end

  def leapfrog
    @body.each do |b|
      b.vel += b.acc*0.5*@dt
      b.pos += b.vel*@dt
    end
#    get_acc
    get_tree_acc
    @body.each do |b|
      b.vel += b.acc*0.5*@dt
    end
  end

  def get_acc
    @body.each{|b|b.acc = b.pos*0.0}
    i = 0
    while (i < @body.size) 
      j = i+1
      while (j < @body.size)
	@body[i].pairwise_acc(@body[j], @eps)
	j += 1
      end
      i += 1
    end
  end

  def get_tree_acc
#    p "maketree start"
    maketree
#    p "maketree end"
    @rootnode.center_of_mass
#    @rootnode.pp(0)

    i=0
    @body.each{|b|
#      print "calculating force on body #{i}\n"
      b.acc = @rootnode.get_node_acc(b, @tol, @eps)
      i+=1
    }
  end

  def ekin                        # kinetic energy
    e = 0.0
    @body.each{|b| e += b.ekin}
    e
  end

  def epot                        # potential energy
    e = 0.0
    @body.each{|b| e += b.epot(@body, @eps)}
    e/2                           # pairwise potentials were counted twice
  end

  def e_init                      # initial total energy
    @e0 = ekin + epot
  end
  
  def write_diagnostics(x_flag)
    etot = ekin + epot
    STDERR.print <<-EOF
at time t = #{sprintf("%g", time)}, after #{@nsteps} steps :
  e_kin = #{sprintf("%.3g", ekin)} ,\
 e_pot =  #{sprintf("%.3g", epot)} ,\
 e_tot = #{sprintf("%.3g", etot)}
             e_tot - e_init = #{sprintf("%.3g", etot - @e0)}
  (e_tot - e_init) / e_init = #{sprintf("%.3g", (etot - @e0)/@e0 )}
EOF
    
    if x_flag
      STDERR.print "  for debugging purposes, here is the internal data ",
                   "representation:\n"
      ppx
    end
  end

  def pp                                # pretty print
    print "     N = ", @body.size, "\n"
    print "  time = ", @time, "\n"
    @body.each do |b| b.pp end
  end

  def ppx                          # pretty print, with extra information (acc)
    print "     N = ", @body.size, "\n"
    print "  time = ", @time, "\n"
    @body.each{|b| b.ppx}
  end

  def write
    print @body.size, "\n"
    printf("%22.15e\n", @time)
    @body.each do |b| b.write end
  end

  def read
#    p "Nbody.read called"
    n = gets.to_i
    @time = gets.to_f
    @body=Array(Body).new(n,Body.new)
#    p @body
    n.times{|i|
      @body[i]=Body.new
      @body[i].read
      @body[i].ppx
    }
  end

  def makerootnode : Node
    r = @body.reduce(0){|oldmax, b| [oldmax, b.pos.map{|x| x.abs}.max].max}
    s = 1.0
    while r > s
        s *= 2
    end  
    Node.new([0.0, 0.0, 0.0].to_v, s)
  end
  def maketree
    @rootnode = self.makerootnode
    i=0
    @body.each do |b|
#      print "loading body #{i} #{b.pos}\n"
      @rootnode.loadtree(b)
      i+=1
    end

  end

end

class Node

  property :mass, :pos

  def initialize(center : Vector, size : Float64)
    @child = Array(Node|Body|Nil).new(8,nil)
#    @size=0.0
    @pos = Vector.new(3,0.0)
    @mass=0.0
    @center, @size = center, size
  end
  def get_other_acc( other, eps)
    return Vector.new(3,0.0) if self == other
#    print "get other acc ", @pos, " ", other.pos, " ", other.mass, "\n"

  rji = @pos  - other.pos 
  r2 = eps * eps + rji * rji
  @mass * rji / (r2 * sqrt(r2))
end

  def octant(pos)
    result = 0
    pos.each_index do |i| 
      result *= 2
      result += 1 if pos[i] > @center[i]
    end
    result
  end

  def loadtree(b : Node)
  end
  def loadtree(b : Nil)
  end
  def loadtree(b : Body)
#    print "loadtree for center,size= #{@center}, #{@size}\n"
    corner = octant(b.pos)
#    print "new octant=#{corner}\n"
    if @child[corner] == nil
      @child[corner] = b
#      p "inserted\n"
      return
    end
    if @child[corner].class == Body
      tmp_b = @child[corner]
      child_size = @size / 2.0
      @child[corner] = Node.new(@center + child_size*offset(corner),child_size)
      @child[corner].loadtree(tmp_b)
#      p "new cell made\n"
    end
#    p "recursive call"
    @child[corner].loadtree(b)
  end

  def offset(corner)
    r=[] of Float64
    3.times{ r.unshift( ((corner & 1)*2 - 1 )+0.0) ; corner>>=1 }
    r.to_v
  end

  def pp(indent = 0)
    print " "*indent+"node: center = #{@center.join(" ")} ; size = #{@size}\n"
    if @mass
      print " "*indent+"      mass = #{@mass}   pos = #{@pos.join(", ")}\n"
    end
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

  def center_of_mass
    @mass = 0.0
    @pos = [0.0, 0.0, 0.0].to_v
    @child.each do |c|
      c.center_of_mass if c.class == Node
      if c
        @mass += c.mass
        @pos += c.mass * c.pos
      end
    end
    @pos /= @mass
  end

  def get_node_acc(b, tol, eps)
    distance = b.pos - @pos
#    print b.pos, " ", @pos, " ", sqrt(distance*distance), " ", @size,"\n"
    if 2 * @size > tol * sqrt(distance*distance)
      acc = Vector.new(3,0.0)
      @child.each{|c| acc += c.get_node_acc(b, tol, eps) if c}
      acc
    else
      self.get_other_acc(b, eps)
    end
  end

end

include Math

print "enter hackcode\n"
nb = NBody.new
#print "afer new enter hackcode\n"
#p nb
nb.read
#nb.evolve(c.tol, c.eps, c.dt, c.dt_dia, c.dt_out, c.dt_end, c.init_out, c.x_flag)

nb.evolve(0.5, 0.05, 0.015625,0.5,0.5,0.5, false, true)





## ##nb = Nbody.new
## ##nb.read
## ##nb.maketree
## ##nb.rootnode.center_of_mass
## ##nb.rootnode.pp
## ###nb.body[0].pos += [10, 0, 0].to_v
## ##nb.rootnode.check_body_in_cell
