include_files = %w{ vector clop }

include_files.each do |f|
  file = f + ".rb"
  require file if $0 != file
end

include Math

VERY_LARGE_NUMBER = 1e30

class Array
  def one_level_deep_copy
    a = []
    a.push(*self)
  end
end

class Float
  alias :original_mult :*
  def *(a)
    if a.class == Vector
      a*self
    else
      original_mult(a)
    end
  end
  def to_acs(name = nil, precision = 16, indentation = 0)
    s = " " * indentation
    s += name + " = " if name
    s += sprintf(" %#{precision+8}.#{precision}e", self)
  end
end

class Fixnum
  alias :original_mult :*
  def *(a)
    if a.class == Vector
      a*self
    else
      original_mult(a)
    end
  end
  def to_acs(name = nil, precision = 16, indentation = 0)
    s = " " * indentation
    s += name + " = " if name
    s += to_s
  end
end

class String
  def to_acs(name = nil, precision = 16, indentation = 0)
    s = " " * indentation
    s += name + " = " if name
    s += self
  end
end

class ACS_Wrapper

  def initialize(tag, content)
    @tag = tag
    @content = content
  end

  def to_acs(precision = 16, base_indentation = 0, additional_indentation = 2)
    indent = base_indentation + additional_indentation
    return " " * base_indentation + "begin " + @tag + "\n" +
      @content.to_acs(precision, indent, additional_indentation) + "\n"+
      " " * base_indentation + "end"
  end

  def write(file = $stdout, precision = 16,
            base_indentation = 0, additional_indentation = 2)
    file.print to_acs(precision, base_indentation, additional_indentation)+"\n"
  end

end
