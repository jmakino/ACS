class Float
  def to_acs_special_s(precision)
    sprintf(" %#{precision+8}.#{precision}e", self)
  end
end

class Vector
  def to_acs_special_s(precision)
    self.map{|x| sprintf(" %#{precision+8}.#{precision}e", x)}.join
  end
end

class ACS_Wrapper

  def initialize(tag, content)
    @tag = tag
    @content = content
  end

  def to_acs_s(precision = 16, base_indent = 0, add_indent = 2)
    indent = base_indent + add_indent
    return " " * base_indent + "begin " + @tag + "\n" +
      @content.to_acs_s(precision, indent, add_indent) + "\n"+
      " " * base_indent + "end"
  end

  def write(file = $stdout, precision = 16,
            base_indent = 0, add_indent = 2)
    file.print to_acs_s(precision, base_indent, add_indent)+"\n"
  end

end

module ACS_IO

  PRECISION = 16
  BASE_INDENT = 0
  ADD_INDENT = 2

  def to_acs_s(name, precision = PRECISION, base_indent = BASE_INDENT,
               add_indent = ADD_INDENT)
    indent = base_indent + add_indent
    s = " " * base_indent + "begin " + self.class.to_s + " " + name + "\n"
    vv = self.instance_variables
    if vv.size > 0
      vv.each do |v|
        s += instance_variable_to_acs_s(v, precision, base_indent + add_indent,
                                        add_indent)
        end
    elsif self.class == Array
      self.each_index do |i|
        s += self[i].to_acs_s(name+"["+i.to_s+"]", precision,
                              base_indent + add_indent, add_indent)
      end
    else
      s += " " * (base_indent + add_indent)
      if self.class == Float or self.class == Vector
        s += self.to_acs_special_s(precision) + "\n"
      else
        s += self.to_s + "\n"
      end
    end
    s + " " * base_indent + "end" + "\n"
  end

  def instance_variable_to_acs_s(var, precision, base_indent, add_indent)
    eval(var).to_acs_s(var[1...var.size], precision, base_indent, add_indent)
  end

  def acs_write(name, file = $stdout, precision = PRECISION,
                base_indent = BASE_INDENT, add_indent = ADD_INDENT)
    file.print to_acs_s(name, precision, base_indent, add_indent)
  end

end

class Object

  include ACS_IO

end
