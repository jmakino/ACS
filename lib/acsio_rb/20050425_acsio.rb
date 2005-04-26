class NilClass
  def NilClass.acs_parse(s, indent)
    nil
  end
end

class FalseClass
  def FalseClass.acs_parse(s, indent)
    false
  end
end

class TrueClass
  def TrueClass.acs_parse(s, indent)
    true
  end
end

class Fixnum
  def Fixnum.acs_parse(s, indent)
# check indentation here first
    s.to_i
  end
end

class Bignum
  def Bignum.acs_parse(s, indent)
# check indentation here first
    s.to_i
  end
end

class Float
  def to_acs_special_s(precision, indent)
    " " * indent + sprintf(" %#{precision+8}.#{precision}e", self) + "\n"
  end
  def Float.acs_parse(s, indent)
# check indentation here first
    s.to_f
  end
end

class Vector
  def to_acs_special_s(precision, indent)
    " " * indent +
      self.map{|x| sprintf(" %#{precision+8}.#{precision}e", x)}.join + "\n"
  end
  def Vector.acs_parse(s, indent)
# check indentation here first
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

class Object
  def acs_extend(other)
    (@acsmixins ||= []) << other
    @acsmixins.uniq!
   extend(other)
  end
  attr :acsmixins
end


module ACS_IO

  attr_accessor :story

  ACS_HEADER = "ACS\n"
  ACS_FOOTER = "#{ACS_HEADER.chomp.reverse}\n"
  BASE_INDENT = 0
  @@add_indent = 2
  @@precision = 16
  @@verbosity = 1
  @@acs_verbosity = 1

  def ACS_IO.set_options(c)
    @@add_indent = c.add_indent
    @@precision = c.precision
    @@verbosity = c.verbosity
    @@acs_verbosity = c.acs_verbosity
  end

  def to_acs_s(name, check_acs_output_name, precision = @@precision,
               base_indent = BASE_INDENT, add_indent = @@add_indent)
    indent = base_indent + add_indent
    if defined? self.class::ACS_OUTPUT_NAME and check_acs_output_name
      tag = self.class::ACS_OUTPUT_NAME
    else
      tag = self.class.name
    end
    s = " " * base_indent + tag + " " + name + "\n"
    if self.instance_variables.size > 0
      self.instance_variables.sort.each do |v|
        s += eval(v).to_acs_s(v.delete("@"), check_acs_output_name,
                              precision, indent, add_indent)
        remove_instance_variable(:@story) if v == "@story"
      end
      return s
    end
    case self.class.name
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

  def acs_log(v, s)
    STDERR.print(s) unless v > @@verbosity
    unless v > @@acs_verbosity
      @story = "" unless defined? @story
      @story += s
    end
  end

  def acs_write(file = $stdout, check_acs_output_name = false,
                precision = @@precision, add_indent = @@add_indent)
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
  def ACS_IO.define_empty_module(name)
    eval("module #{name} \nend", TOPLEVEL_BINDING)
  end   

  def io_name_okay?(w)
    if defined? ACS_OUTPUT_NAME
      if w != ACS_OUTPUT_NAME
        raise "first_word = #{first_word} != #{ACS_OUTPUT_NAME}"
      end
    else
      if w != self.class.name
        raise "first_word = #{first_word} != #{self.class.name}"
      end
    end
  end

  def acs_parse(a, base_indent)
    loop do
      break unless a[0]
      variable_name = a[0].split[1]
      if a[0].split[0] =~ /^(NilClass|FalseClass)$/
        val = a[0].split[0] == "NilClass" ? "nil" : "false"
        a.shift
        a.shift
        eval("@#{variable_name} = #{val}")
      else
        result = ACS_IO.acs_parse_manager(a, base_indent)
        if result
          eval("@#{variable_name} = result")
        else
          break
        end
      end
    end
    @acsmixins.each{|x| extend(x)} if defined? @acsmixins
    self
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
    when /^(TrueClass|Fixnum|Bignum|Float|Vector)$/
      return eval("#{class_name}.acs_parse(a.shift, indent)")
    when /^(Module)$/
      modulename = a.shift.split[0]
      begin
         return eval(modulename)
      rescue
        ACS_IO.define_empty_module(modulename)
        retry
      end
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

additional_definitions_string=<<-END

  Long name:            --verbosity
  Value type:           int
  Default value:        1
  Description:          Screen Output Verbosity Level
  Variable name:        verbosity
  Long description:
    The verbosity level determines how much information is printed on the
    screen, via the STDERR channel.  if the level is set to 0, no output
    is printed.  On level 1, the normal amount of output is printed.  Level
    2 functions effectively as a debug level, allowing a lot of extra stuff
    to be printed.


  Long name:            --acs_verbosity
  Value type:           int
  Default value:        1
  Description:          ACS Output Verbosity Level
  Variable name:        acs_verbosity
  Long description:
    The acs verbosity level determines how much information is printed along
    with the regular particle output, within the same ACS data block that
    contains the mass, positions, etc, of each particle.  If the level is set
    to 0, no output is printed.  On level 1, the normal amount of output is
    printed.  Level 2 functions effectively as a debug level, allowing a lot
    of extra stuff to be printed.


  Long name:            --precision
  Value type:           int
  Default value:        16
  Description:          Floating point precision
  Variable name:        precision
  Long description:
    The precision with which floating point numbers are printed in the output.
    The default precision is comparable to double precision accuracy.


  Long name:            --indentation
  Value type:           int
  Default value:        2
  Description:          Incremental indentation
  Variable name:        add_indent
  Long description:
    This option allows the user to set the incremental indentation, i.e.
    the number of white spaces added in front of the output of data, for
    each level that the data are removed from the top level.

    Starting at zero indentation at the level of the top ACS structure,
    one set of incremental indentation is added for each level down,
    from ACS to DSS, from DSS to World, and so on.


END

if defined? Clop
  Clop.add_defs(additional_definitions_string)
  Clop.add_to_initialize_action_list(lambda{|x| ACS_IO.set_options(x)})
  Clop.add_to_initialize_action_list(lambda{|x|
                                       if x.verbosity == 0
                                         x.do_not_print_values = true
                                       end
                                     })
end
