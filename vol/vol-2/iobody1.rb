require "vector.rb"

class Body

#:segment start: tag
  TAG = "particle"                                          #1 vol 2, 2.3 ka na
#:segment end:

#:segment start: type_acc
  attr_accessor :mass, :pos, :vel, :type
#:segment end:

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
#:segment start: type
    @type = nil                                                   #2 vol 2, 2.3
#:segment end:
    @story = []
  end

  def f_to_s(name, value, precision, indentation)  # from floating-point number
    " " * indentation +
      name + " = " + sprintf(" %#{precision+8}.#{precision}e\n", value)
  end

  def f_v_to_s(name, value, precision, indentation)   # from floating-pt vector
    " " * indentation + name + " = " + 
      value.map{|x| sprintf(" %#{precision+8}.#{precision}e", x)}.join + "\n"
  end

#:segment start: to_s_def
  def to_s(precision = 16, base_indentation = 0, additional_indentation = 2)
#:segment end:
    subtag = if @type then " "+@type else "" end
    indent = base_indentation + additional_indentation
    " " * base_indentation + "begin " + TAG + subtag + "\n" +
      f_to_s("mass", mass, precision, indent) +
      f_v_to_s("position", pos, precision, indent) +
      f_v_to_s("velocity", vel, precision, indent) +
      " " * base_indentation + "end" + "\n"
  end

  def write(file = $stdout, precision = 16,
            base_indentation = 0, additional_indentation = 2)
    file.print to_s(precision, base_indentation, additional_indentation)
  end

end
