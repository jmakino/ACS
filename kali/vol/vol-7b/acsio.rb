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
      blanks = ACS_IO.count_initial_blanks(line)
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
  def to_acs_special_s(name, check_acs_output_name, precision, indent,
                       add_indent)
    s = ""
    self.each_index do |i|
      s += self[i].to_acs_s(name+"["+i.to_s+"]", check_acs_output_name,
                            precision, indent, add_indent)
    end
    s
  end
  def Array.acs_parse(a, base_indent)
    ar = []
    loop do
      result = ACS_IO.acs_parse_manager(a, base_indent)
      if result
        ar.push(result)
      else
        return ar
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

  def to_acs_s(name, check_acs_output_name, precision = PRECISION,
               base_indent = BASE_INDENT, add_indent = ADD_INDENT)
    indent = base_indent + add_indent
    if defined? self.class::ACS_OUTPUT_NAME and check_acs_output_name
      tag = self.class::ACS_OUTPUT_NAME
    else
      tag = self.class.to_s
    end
    s = " " * base_indent + tag + " " + name + "\n"
    if self.instance_variables.size > 0
      self.instance_variables.sort.each do |v|
        s += eval(v).to_acs_s(v.delete("@"), check_acs_output_name,
                              precision, indent, add_indent)
      end
      return s
    end
    case self.class.to_s
    when "String"
      return s.chomp + self.to_acs_special_s(indent, add_indent)
    when "Array"
      return s + self.to_acs_special_s(name, check_acs_output_name,
                                       precision, indent, add_indent)
    when /^(Float|Vector)$/
      return s + self.to_acs_special_s(precision, indent)
    end
    s += " " * indent + self.to_s + "\n"
  end

  def acs_write(file = $stdout, check_acs_output_name = false,
                precision = PRECISION, add_indent = ADD_INDENT)
    file.print ACS_HEADER +
         to_acs_s("", check_acs_output_name, precision, add_indent,
                  add_indent) +
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

  def acs_read(array = [], file = $stdin)
    array = [array] unless array.class == Array
    ACS_IO.acs_read(array.push(self.class), file)
  end

  def ACS_IO.acs_read(top_class = nil, file = $stdin)
    s = ACS_IO.read_acs_string(file)
    return nil if not s
    a = s.split("\n")
    first_line = a.shift
    indent = ACS_IO.count_initial_blanks(first_line)
    top_class_name = first_line.strip
    if top_class
      tc = acceptable_class_name(top_class_name, top_class).new
    else
      top_class_name = first_line.strip
      begin
        tc = eval(top_class_name).new
      rescue NameError
        ACS_IO.define_new_class(top_class_name)
        retry
      end
    end
    tc.acs_parse(a, indent)
  end

  def acceptable_class_name(class_name, class_request)
    class_request = [class_request] if class_request.class != Array
    class_request.each do |r|
      r_init = r
      while r
        return r_init if r.to_s == class_name
        r = r.superclass
      end
    end
    separator = ", "
    raise "#{class_name} not in [#{class_request.join(separator).to_s}]"
  end

  def ACS_IO.define_new_class(name)
    eval("class #{name} \nend", TOPLEVEL_BINDING)
  end   

  def io_name_okay?(w)
    if defined? ACS_OUTPUT_NAME
      if w != ACS_OUTPUT_NAME
        raise "first_word = #{first_word} != #{ACS_OUTPUT_NAME}"
      end
    else
      if w != self.class.to_s
        raise "first_word = #{first_word} != #{self.class.to_s}"
      end
    end
  end

  def acs_parse(a, base_indent)
    loop do
      return self unless a[0]
      variable_name = a[0].split[1]
      result = ACS_IO.acs_parse_manager(a, base_indent)
      if result
        eval("@#{variable_name} = result")
      else
        return self
      end
    end
  end

  def ACS_IO.acs_parse_manager(a, base_indent)
    first_line = ACS_IO.next_line?(a, base_indent)
    return nil unless first_line
    indent = ACS_IO.count_initial_blanks(first_line)
    word = first_line.split
    class_name = word[0]
    case class_name
    when "String"
      add_indent = word[2].to_i
      return String.acs_parse(a, indent + add_indent)
    when "Array"
      return Array.acs_parse(a, indent)
    when /^(Fixnum|Bignum|Float|Vector)$/
      return eval("#{class_name}.acs_parse(a.shift, indent)")
    else
      begin
        x = eval("#{class_name}.new")
      rescue NameError
        ACS_IO.define_new_class(class_name)
        retry
      end
      return x.acs_parse(a, indent)
    end
  end

  def ACS_IO.next_line?(a, base_indent)
    first_line = a.shift
    return nil unless first_line
    indent = ACS_IO.count_initial_blanks(first_line)
    if indent <= base_indent
      a.unshift(first_line)
      return nil
    end      
    first_line
  end

  def ACS_IO.count_initial_blanks(s)
    s =~ /^ */
    $&.size
  end

  def dump_contents
    a = []
    self.instance_variables.each do |v|
      a.push([v, eval(v)])
    end
    a
  end

  def restore_contents(from)
    from.dump_contents.each do |a|
      eval(a[0] + "= a[1]")
    end
    self
  end

end

class Object

  include ACS_IO

end
