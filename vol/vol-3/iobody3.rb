require "vector.rb"

class Body

  TAG = "particle"

  attr_accessor :mass, :pos, :vel, :type

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
    @type = nil
    @rest = ""
  end

  def to_s(precision = 16, base_indentation = 0, additional_indentation = 2)
    subtag = if @type then " "+@type else "" end
    indent = base_indentation + additional_indentation
    " " * base_indentation + "begin " + TAG + subtag + "\n" +
      f_to_s("mass", mass, precision, indent) +                              #2
      f_v_to_s("position", pos, precision, indent) +                         #2
      f_v_to_s("velocity", vel, precision, indent) +                         #2
      rest_to_s(indent) +                                                    #3
      " " * base_indentation + "end" + "\n"
  end

  def f_to_s(name, value, precision, indentation)  # from floating-point number
    " " * indentation +
      name + " = " + sprintf(" %#{precision+8}.#{precision}e\n", value)
  end

  def f_v_to_s(name, value, precision, indentation)   # from floating-pt vector
    " " * indentation + name + " = " + 
      value.map{|x| sprintf(" %#{precision+8}.#{precision}e", x)}.join + "\n"
  end

  def rest_to_s(indent)
    @rest.gsub(/^\s*/, " "*indent)
  end

  def write(file = $stdout, precision = 16,
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
          @mass = s_to_f(s)
        when /^position/
          @pos = s_to_f_v(s)
        when /^velocity/
          @vel = s_to_f_v(s)
        when /^begin/
          subread(file, s)
        when /^end/
          break
        else                                                                 #1
          if s =~ /^\s*\w+\s*=/                                              #1
            @rest += s                                                       #1
          else                                                               #1
            raise                                                            #1
          end
      end
    }
  end

  def subread
  end

  def s_to_f(s)                               # string to floating-point number
    s.split("=")[1].to_f
  end

  def s_to_f_v(s)                             # string to floating-point vector
    s.split("=")[1].split.map{|x| x.to_f}.to_v
  end

end
