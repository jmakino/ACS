require "vector.rb"
require "clop.rb"

class Body

  TAG = "particle"

  attr_accessor :mass, :pos, :vel, :acc, :type

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass.to_f, pos.to_v, vel.to_v
    @type = nil
  end

  def to_s(precision = 16, base_indentation = 0, additional_indentation = 2)
    subtag = if @type then " "+@type else "" end
    indent = base_indentation + additional_indentation
    return " " * base_indentation + "begin " + TAG + subtag + "\n" +
      mass.to_s("mass", precision, indent) + "\n" +
      pos.to_s("position", precision, indent) + "\n" +
      vel.to_s("velocity", precision, indent) + "\n" +
      " " * base_indentation + "end"
  end

  def write(file = $stdout, precision = 16,
            base_indentation = 0, additional_indentation = 2)
    file.print to_s(precision, base_indentation, additional_indentation) + "\n"
  end

end

class Nbody

  TAG = "particle"

  attr_accessor :time, :body, :type

  def initialize(n = 0, time = 0)
    @time = time
    @type = nil
    @body = []
    for i in 0...n
      @body[i] = Body.new
    end
  end

  def to_s(precision = 16, base_indentation = 0, add_indentation = 2)
    subtag = if @type then " "+@type else "" end
    indent = base_indentation + add_indentation
    return " " * base_indentation + "begin " + TAG + subtag + "\n" +
      @body.map{|b| b.to_s(precision,indent,add_indentation)}.join("\n")+"\n"+
      " " * base_indentation + "end"
  end

  def write(file = $stdout, precision = 16,
            base_indentation = 0, additional_indentation = 2)
    file.print to_s(precision, base_indentation, additional_indentation) + "\n"
  end

end

class Dss

  TAG = "DSS"

  attr_accessor :nbody, :type

  def initialize(nbody = Nbody.new)
    @type = nil
    @nbody = nbody
  end

  def to_s(precision = 16, base_indentation = 0, additional_indentation = 2)
    subtag = if @type then " "+@type else "" end
    indent = base_indentation + additional_indentation
    return " " * base_indentation + "begin " + TAG + subtag + "\n" +
      @nbody.to_s(precision, indent, additional_indentation) + "\n"+
      " " * base_indentation + "end"
  end

  def write(file = $stdout, precision = 16,
            base_indentation = 0, additional_indentation = 2)
    file.print to_s(precision, base_indentation, additional_indentation) + "\n"
  end

end

class Acs

  TAG = "ACS"

  attr_accessor :dss, :type

  def initialize(dss = Dss.new)
    @type = nil
    @dss = dss
  end

  def to_s(precision = 16, additional_indentation = 2)
    subtag = if @type then " "+@type else "" end
    indent = additional_indentation
    return "begin " + TAG + subtag + "\n" +
      @dss.to_s(precision, indent, additional_indentation) + "\n"+
      "end"
  end

  def write(file = $stdout, precision = 16, additional_indentation = 2)
    file.print to_s(precision, additional_indentation) + "\n"
  end

end

include Math

def frand(low, high)
  low + rand * (high - low)
end

def spherical(r)
  vector = Vector.new
  theta = acos(frand(-1, 1))
  phi = frand(0, 2*PI)
  vector[0] = r * sin( theta ) * cos( phi )
  vector[1] = r * sin( theta ) * sin( phi )
  vector[2] = r * cos( theta )
  vector
end  

def mkplummer(n, seed, output_file_name, precision, additional_indentation)
  if seed == 0
    srand
  else
    srand seed
  end
  scalefactor = 16.0 / (3.0 * PI)
  nb = Nbody.new(n)
  cumulative_mass_min = 0
  cumulative_mass_max = 1.0/n
  nb.body.each do |b|
    b.mass = 1.0/n
    cumulative_mass = frand(cumulative_mass_min, cumulative_mass_max)
    cumulative_mass_min = cumulative_mass_max
    cumulative_mass_max += 1.0/n
    radius = 1.0 / sqrt( cumulative_mass ** (-2.0/3.0) - 1.0)
    b.pos = spherical(radius) / scalefactor
    x = 0.0
    y = 0.1
    while y > x*x*(1.0-x*x)**3.5
      x = frand(0,1)
      y = frand(0,0.1)
    end
    velocity = x * sqrt(2.0) * ( 1.0 + radius*radius)**(-0.25)
    b.vel = spherical(velocity) * sqrt(scalefactor)
  end
  adjust_center_of_mass(nb)                                                  #1
  STDERR.print "             actual seed used\t: ", srand, "\n"
  if output_file_name == ""
    file = $stdout
  else
    file = File.open(output_file_name, "w+")
  end
  Acs.new(Dss.new(nb)).write(file, precision, additional_indentation)
end

def adjust_center_of_mass(nb)
  vel_com = pos_com = nb.body[0].pos*0     # null vectors of the correct length
  nb.body.each do |b|
    pos_com += b.pos*b.mass
    vel_com += b.vel*b.mass
  end
  nb.body.each do |b|
    b.pos -= pos_com
    b.vel -= vel_com
  end
end

options_text= <<-END

  Description: Plummer's Model Builder, (c) 2004, Piet Hut & Jun Makino, ACS
  Long description:
    This program creates an N-body realization of Plummer's Model.
    (c) 2004, Piet Hut and Jun Makino; see ACS at www.artcompsi.org

    The algorithm used is described in Aarseth, S., Henon, M., & Wielen, R.,
    Astron. Astroph. 37, 183 (1974).


  Short name:		-n
  Long name:            --n_particles
  Value type:           int
  Default value:        1
  Global variable:      n_particles
  Print name:           N
  Description:          Number of particles
  Long description:
    Number of particles in a realization of Plummer's Model.

    Each particle is drawn from the Plummer distribution, from successive
    shells, in a layered manner, starting from the center.  The radial
    distance from the center to the particles thus increases monotonically,
    upon creation.  Subsequently, the center of mass is shifted to the origin,
    which may disturb somewhat the monotonicity of the particle layering.

    Standard Units are used in which G = M = 1 and E = -1/4, where
      G is the gravitational constant
      M is the total mass of the N-body system
      E is the total energy of the N-body system


  Short name:           -s
  Long name:            --seed
  Value type:           int
  Default value:        0
  Description:          pseudorandom number seed given
  Print name:           
  Global variable:      seed
  Long description:
    Seed for the pseudorandom number generator.  If a seed is given with
    value zero, a preudorandom number is chosen as the value of the seed.
    The seed value used is echoed separately from the seed value given,
    to allow the possibility to repeat the creation of an N-body realization.

      Example:

        |gravity> ruby mkplummer1.rb -n 42 -s 0
        . . .
        pseudorandom number seed given	: 0
                     actual seed used	: 1087616341
        . . .
        |gravity> ruby mkplummer1.rb -n 42 -s 1087616341
        . . .
        pseudorandom number seed given	: 1087616341
                     actual seed used	: 1087616341
        . . .


  Short name:		-o
  Long name:		--output_file_name
  Value type:		string
  Default value:	                # nothing, leading to output on $stdout
  Global variable:	output_file_name
  Print name:	 	                       # no name, hence name suppressed
  Description:		Name of the output file
  Long description:
    Name of the snapshot output file, containing a Plummer's model realization.
    The snapshot contains the mass, position, and velocity values for all
    particles in an N-body system.


  Short name:           -p
  Long name:            --precision
  Value type:           int
  Default value:        16
  Description:          floating point precision
  Global variable:      precision
  Long description:
    The precision with which floating point numbers are printed in the output.
    The default precision is comparable to double precision accuracy.


  Short name:           -i
  Long name:            --indentation
  Value type:           int
  Default value:        2
  Description:          incremental indentation
  Global variable:      add_indent
  Long description:
    This option allows the user to set the incremental indentation, i.e.
    the number of white spaces added in front of the output of data, for
    each level that the data are removed from the top level.

    Starting at zero indentation at the level of the top ACS structure,
    one set of incremental indentation is added for each level down,
    from ACS to DSS, from DSS to Nbody, from Nbody to Body, and so on.


  END

  parse_command_line(options_text)

mkplummer($n_particles, $seed, $output_file_name, $precision, $add_indent)
