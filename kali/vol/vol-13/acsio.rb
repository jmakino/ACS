class Float
  def to_acs_special_s(precision, indent)
    " " * indent + sprintf(" %#{precision+8}.#{precision}e", self) + "\n"
  end
end

class Vector
  def to_acs_special_s(precision, indent)
    " " * indent +
      self.map{|x| sprintf(" %#{precision+8}.#{precision}e", x)}.join + "\n"
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
end

class Array
  def to_acs_special_s(name, precision, indent, add_indent)
    s = ""
    self.each_index do |i|
      s += self[i].to_acs_s(name+"["+i.to_s+"]", precision, indent, add_indent)
    end
    s
  end
end

module ACS_IO

  ACS_HEADER = "ACS"
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
    file.print ACS_HEADER + "\n" +
               to_acs_s("", precision, add_indent, add_indent)
  end

  def acs_read(file = $stdin)
    
  end

end

class Object

  include ACS_IO

end

