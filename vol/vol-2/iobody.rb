require "vector.rb"

class Body

  TAG = "particle"

  attr_accessor :mass, :pos, :vel, :type

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
    @type = nil
    @story = []
  end

  def real_to_s(name, value, precision, indentation)
    " " * indentation +
      name + " = " + sprintf(" %#{precision}.#{precision-8}e\n", value)
  end

  def vector_to_s(name, value, precision, indentation)
    " " * indentation + name + " = " + 
      value.map{|x| sprintf(" %#{precision}.#{precision-8}e", x)}.join + "\n"
  end

  def to_s(precision = 24, base_indentation = 0, additional_indentation = 2)
    if @type
      subtag = " "+@type
    else
      subtag = ""
    end
    indent = base_indentation + additional_indentation
    " " * base_indentation + "begin " + TAG + subtag + "\n" +
      real_to_s("mass", mass, precision, indent) +
      vector_to_s("position", pos, precision, indent) +
      vector_to_s("velocity", vel, precision, indent) +
      " " * base_indentation + "end" + "\n"
  end

  def write(file = $stdout, precision = 24,
            base_indentation = 0, additional_indentation = 2)
    file.print to_s(precision, base_indentation, additional_indentation)
  end

  def read(header, file = $stdin)
    raise unless header =~ /^\s*begin\s+particle/
    a = header.split
    if a.size > 2
      a.shift
      a.shift
      @type = a.join(" ")
    end
    loop {
      s = file.gets
      name = s.split[0]
      case name
        when /^mass/
          @mass = read_real(s)
        when /^position/
          @pos = read_vector(s)
        when /^velocity/
          @vel = read_vector(s)
        when "begin"
          subread(file, s)
        when "end"
          break
        else
          @story.push(s)
      end
    }
  end

  def subread
  end

  def read_real(s)
    s.split("=")[1].to_f
  end

  def read_vector(s)
    s.split("=")[1].split.map{|x| x.to_f}.to_v
  end

end

c = Body.new
c.read("begin particle star giant AGB")
c.write
