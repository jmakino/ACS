require "vector.rb"

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
      " " * base_indentation + "end" + "\n"
  end

  def write(file = $stdout, precision = 16,
            base_indentation = 0, additional_indentation = 2)
    file.print to_s(precision, base_indentation, additional_indentation)
  end

end
