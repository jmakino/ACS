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

module ACS_IO

  PRECISION = 16
  BASE_INDENT = 0
  ADD_INDENT = 2

  def to_acs_s(name, precision = PRECISION, base_indent = BASE_INDENT,
               add_indent = ADD_INDENT)
    indent = base_indent + add_indent
    s = " " * base_indent + self.class.to_s + " " + name
    if self.class == String
      s += " " + add_indent.to_s + "\n"
      s += self.gsub(/^/, " " * indent)
      if s =~ /\n$/
        s += " " * indent
      end
      return s + "\n"
    end
    s += "\n"
    vv = self.instance_variables
    if vv.size > 0
      vv.each do |v|
        s += instance_variable_to_acs_s(v, precision, indent, add_indent)
      end
      return s
    end
    if self.class == Array
      self.each_index do |i|
        s += self[i].to_acs_s(name+"["+i.to_s+"]", precision,
                              indent, add_indent)
      end
      return s
    end
    if self.class == Float or self.class == Vector
      s += " " * indent + self.to_acs_special_s(precision) + "\n"
      return s
    end
    s += " " * indent + self.to_s + "\n"
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

