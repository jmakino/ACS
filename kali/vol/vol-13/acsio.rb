class Fixnum
  def Fixnum.acs_parse(s, indent)
# check indentation here first)
    s.to_i
  end
end

class Bignum
  def Bignum.acs_parse(s, indent)
# check indentation here first)
    s.to_i
  end
end

class Float
  def to_acs_special_s(precision, indent)
    " " * indent + sprintf(" %#{precision+8}.#{precision}e", self) + "\n"
  end
  def Float.acs_parse(s, indent)
# check indentation here first)
    s.to_f
  end
end

class Vector
  def to_acs_special_s(precision, indent)
    " " * indent +
      self.map{|x| sprintf(" %#{precision+8}.#{precision}e", x)}.join + "\n"
  end
  def Vector.acs_parse(s, indent)
# check indentation here first)
# and this here below should go partially to vector.rb
    s.split.map{|x| x.to_f}.to_v
  end
end

class String
  def to_acs_special_s(indent, add_indent)
    s = " " + add_indent.to_s + "\n"
    s += self.gsub(/^/, " " * indent)
    if s =~ /\n$/
      s += " " * indent
    end
    s + "\n"
  end
  def String.acs_parse(a, indent)
    s = ""
    loop do
      line = a.shift
      return s.chomp unless line
      blanks = count_initial_blanks(line)
      if blanks < indent
        a.unshift(line)
        return s.chomp
      else
        s += line[indent...line.size] + "\n"
      end
    end
  end
end

class Array
  def to_acs_special_s(name, precision, indent, add_indent)
    s = ""
    self.each_index do |i|
      s += self[i].to_acs_s(name+"["+i.to_s+"]", precision, indent, add_indent)
    end
    s
  end
  def Array.acs_parse(a, base_indent)
    ar = []
    loop do
      first_line = a.shift
      return ar unless first_line
      indent = count_initial_blanks(first_line)
      if indent <= base_indent
        a.unshift(first_line)
        return ar
      end      
      first_line_word = first_line.split
      class_name = first_line_word[0]
      variable_name = first_line_word[1]
      if class_name == "Float"
        eval("ar.push(Float.acs_parse(a.shift, indent))")
      elsif class_name == "Fixnum"
        eval("ar.push(Fixnum.acs_parse(a.shift, indent))")
      elsif class_name == "Bignum"
        eval("ar.push(Bignum.acs_parse(a.shift, indent))")
      elsif class_name == "Vector"
        eval("ar.push(Vector.acs_parse(a.shift, indent))")
      elsif class_name == "Array"
        eval("ar.push(Array.acs_parse(a.shift, indent))")
      elsif class_name == "String"
        add_indent = first_line_word[2].to_i
        eval("ar.push(String.acs_parse(a, indent + add_indent))")
      else
        begin
          eval("x = #{class_name}.new")
        rescue NameError
          define_class(class_name)
          retry
        end
        eval("ar.push(x.acs_parse(a, indent))")
      end
    end
  end
end

module ACS_IO

  ACS_HEADER = "ACS\n"
  ACS_FOOTER = "#{ACS_HEADER.chomp.reverse}\n"
  PRECISION = 16
  BASE_INDENT = 0
  ADD_INDENT = 2

  def to_acs_s(name, precision = PRECISION, base_indent = BASE_INDENT,
               add_indent = ADD_INDENT)
    indent = base_indent + add_indent
    if defined? self.class::ACS_IO_NAME
      tag = self.class::ACS_IO_NAME
    else
      tag = self.class.to_s
    end
    s = " " * base_indent + tag + " " + name + "\n"
    if self.instance_variables.size > 0
      return s + instance_variables_to_acs_s(precision, indent, add_indent)
    end
    case self.class.to_s
      when "String"
        return s.chomp + self.to_acs_special_s(indent, add_indent)
      when "Array"
        return s + self.to_acs_special_s(name, precision, indent, add_indent)
      when /^(Float|Vector)$/
        return s + self.to_acs_special_s(precision, indent)
    end
    s += " " * indent + self.to_s + "\n"
  end

  def instance_variables_to_acs_s(precision, indent, add_indent)
    s = ""
    self.instance_variables.each do |v|
      s += eval(v).to_acs_s(v.delete("@"), precision, indent, add_indent)
    end
    s
  end

  def acs_write(file = $stdout, precision = PRECISION, add_indent = ADD_INDENT)
    file.print ACS_HEADER +
               to_acs_s("", precision, add_indent, add_indent) +
               ACS_FOOTER
  end

  def ACS_IO.read_acs_string(file = $stdin)
    s = file.gets
    while s =~ /^\s*$/
      s = file.gets
    end
    return nil if not s
    raise "Not an ACS file" if s != ACS_HEADER
    contents = ""
    s = file.gets
    while s != ACS_FOOTER
      contents += s
      s = file.gets
    end
    contents
  end

  def ACS_IO.acs_read(file = $stdin)
    s = ACS_IO.read_acs_string(file)
    return nil if not s
    a = s.split("\n")
    first_line = a.shift
    indent = count_initial_blanks(first_line)
    top_class_name = first_line.strip
    begin
      tc = eval(top_class_name).new
      rescue NameError
      define_class(top_class_name)
      retry
    end
    tc.acs_parse(a, indent)
  end

  def define_class(name)
    eval("class #{name} \nend", TOPLEVEL_BINDING)
  end   

  def io_name_okay?(w)
    if defined? ACS_IO_NAME
      if w != ACS_IO_NAME
        raise "first_word = #{first_word} != #{ACS_IO_NAME}"
      end
    else
      if w != self.class.to_s
        raise "first_word = #{first_word} != #{self.class.to_s}"
      end
    end
  end

  def count_initial_blanks(s)
    s =~ /^ */
    $&.size
  end

  def acs_parse(a, base_indent)
    loop do
      first_line = a.shift
      return self unless first_line
      indent = count_initial_blanks(first_line)
      if indent <= base_indent
        a.unshift(first_line)
        return self
      end      
      first_line_word = first_line.split
      class_name = first_line_word[0]
      variable_name = first_line_word[1]
      if class_name == "Float"
        eval("@#{variable_name} = Float.acs_parse(a.shift, indent)")
      elsif class_name == "Fixnum"
        eval("@#{variable_name} = Fixnum.acs_parse(a.shift, indent)")
      elsif class_name == "Bignum"
        eval("@#{variable_name} = Bignum.acs_parse(a.shift, indent)")
      elsif class_name == "Vector"
        eval("@#{variable_name} = Vector.acs_parse(a.shift, indent)")
      elsif class_name == "Array"
        eval("@#{variable_name} = Array.acs_parse(a, indent)")
      elsif class_name == "String"
        add_indent = first_line_word[2].to_i
        eval("@#{variable_name} = String.acs_parse(a, indent + add_indent)")
      else
        begin
          eval("@#{variable_name} = #{class_name}.new")
        rescue NameError
          define_class(class_name)
          retry
        end
        eval("@#{variable_name}.acs_parse(a, indent)")
      end
    end
  end

end

class Object

  include ACS_IO

end
