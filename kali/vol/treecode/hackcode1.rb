require "../command_line/clop.rb"

def get_self_other_acc(myself, other, eps)
  return @pos*0 if myself == other
  rji = myself.pos - other.pos
  r2 = eps * eps + rji * rji
  myself.mass * rji / (r2 * sqrt(r2))
end

class Body

  attr_accessor :mass, :pos, :vel, :acc

  def pairwise_acc(other, eps)
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
    get_self_other_acc(self, other, eps)
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

class NBody

  attr_accessor :time, :body, :rootnode

  def initialize(n=0, time = 0.0)
    @body = [Body.new]
    for i in 0...n
      @body[i] = Body.new
    end
    @time = time
  end

  def evolve(tol, eps, dt, dt_dia, dt_out, dt_end, init_out, x_flag)
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
    @body.each{|b|b.acc = b.pos*0}
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
    maketree
    @rootnode.center_of_mass
#    @rootnode.pp(0)
    @body.each{|b| b.acc = @rootnode.get_node_acc(b, @tol, @eps)}
  end

  def ekin                        # kinetic energy
    e = 0
    @body.each{|b| e += b.ekin}
    e
  end

  def epot                        # potential energy
    e = 0
    @body.each{|b| e += b.epot(@body, @eps)}
    e/2                           # pairwise potentials were counted twice
  end

  def e_init                      # initial total energy
    @e0 = ekin + epot
  end

  def write_diagnostics(x_flag)
    etot = ekin + epot
    STDERR.print <<END
at time t = #{sprintf("%g", time)}, after #{@nsteps} steps :
  E_kin = #{sprintf("%.3g", ekin)} ,\
 E_pot =  #{sprintf("%.3g", epot)} ,\
 E_tot = #{sprintf("%.3g", etot)}
             E_tot - E_init = #{sprintf("%.3g", etot - @e0)}
  (E_tot - E_init) / E_init = #{sprintf("%.3g", (etot - @e0)/@e0 )}
END
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
    @body.each{|b| b.ppx(@body, @eps)}
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

  attr_accessor :mass, :pos

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
    @mass = 0
    @pos = [0, 0, 0].to_v
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
    if 2 * @size > tol * sqrt(distance*distance)
      acc = @pos*0
      @child.each{|c| acc += c.get_node_acc(b, tol, eps) if c}
      acc
    else
      get_self_other_acc(self, b, eps)
    end
  end

end

options_text = <<-END

  Description: First very simple version of Barnes-Hut tree code
  Long description:
    First very simple version of Barnes-Hut tree code

    (c) 2005, Piet Hut and Jun Makino; see ACS at www.artcompsi.org

    example:
    ruby #{$0} -t 1 < cube1.in


  Short name: 		-T
  Long name:		--opening_tolerance
  Value type:		float
  Default value: 	0.5
  Global variable: 	tol
  Description:		Opening tolerance
  Long description:
    This option sets the tolerance value that governs the maximum size
    of a tree cell that can remain closed; cells (nodes) with a size
    large than the product of tolerance and distance to that cell will
    be opened, and acceleration to its children will be computed.


  Short name: 		-s
  Long name:		--softening_length
  Value type:		float
  Default value: 	0.0
  Global variable: 	eps
  Description:		Softening length
  Long description:
    This option sets the softening length used to calculate the force
    between two particles.  The calculation scheme comforms to standard
    Plummer softening, where rs2=r**2+eps**2 is used in place of r**2.


  Short name: 		-c
  Long name:		--step_size
  Value type:		float
  Default value:	0.001
  Global variable:	dt
  Description:		Time step size
  Long description:
    This option sets the size of the time step, which is constant and
    shared by all particles.  It is wise to use option -s to specify a
    softening length that is significantly larger than the time step size.


  Short name: 		-d
  Long name:		--diagnostics_interval
  Value type:		float
  Default value:	1
  Global variable:	dt_dia
  Description:		Interval between diagnostics output
  Long description:
    The time interval between successive diagnostics output.
    The diagnostics include the kinetic and potential energy,
    and the absolute and relative drift of total energy, since
    the beginning of the integration.
        These diagnostics appear on the standard error stream.
    For more diagnostics, try option "-x" or "--extra_diagnostics".


  Short name: 		-o
  Long name:		--output_interval
  Value type:		float
  Default value:	1
  Global variable:	dt_out
  Description:		Time interval between snapshot output
  Long description:
    The time interval between output of a complete snapshot
    A snapshot of an N-body system contains the values of the
    mass, position, and velocity for each of the N particles.

        This information appears on the standard output stream,
    currently in the following simple format (only numbers):

      N:            number of particles
      time:         time 
      mass:         mass of particle #1
      position:     x y z : vector components of position of particle #1
      velocity:     vx vy vz : vector components of velocity of particle #1
      mass:         mass of particle #2
      ...:          ...

    Example:

       2
       0
       0.5
      7.3406783488452532e-02  2.1167291484119417e+00 -1.4097856092768946e+00
      3.1815484836541341e-02  2.7360312082526089e-01  2.4960049959942499e-02
       0.5
     -7.3406783488452421e-02 -2.1167291484119413e+00  1.4097856092768946e+00
     -3.1815484836541369e-02 -2.7360312082526095e-01 -2.4960049959942499e-02


  Short name: 		-t
  Long name:		--duration
  Value type:		float
  Default value:	10
  Global variable:	dt_end
  Description:		Duration of the integration
  Long description:
    This option sets the duration t of the integration, the time period
    after which the integration will halt.  If the initial snapshot is
    marked to be at time t_init, the integration will halt at time
    t_final = t_init + t.


  Short name:		-i
  Long name:  		--init_out
  Value type:  		bool
  Global variable: 	init_out
  Description:		Output the initial snapshot
  Long description:
    If this flag is set to true, the initial snapshot will be output
    on the standard output channel, before integration is started.


  Short name:		-x
  Long name:  		--extra_diagnostics
  Value type:  		bool
  Global variable:	x_flag
  Description:		Extra diagnostics
  Long description:
    If this flag is set to true, the following extra diagnostics
    will be printed: 

      acceleration (for all integrators)


  END

parse_command_line(options_text)

include Math

nb = NBody.new
nb.read
#p nb
nb.evolve($tol, $eps, $dt, $dt_dia, $dt_out, $dt_end, $init_out, $x_flag)





## ##nb = Nbody.new
## ##nb.read
## ##nb.maketree
## ##nb.rootnode.center_of_mass
## ##nb.rootnode.pp
## ###nb.body[0].pos += [10, 0, 0].to_v
## ##nb.rootnode.check_body_in_cell
