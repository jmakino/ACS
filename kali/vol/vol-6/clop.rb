#
# Command line option parser
#
# Jun Makino and Piet Hut 2004
#
require "vector.rb"

class Clop

  class Command_line_option

    attr_reader :shortname, :longname, :type,
                :description, :longdescription, :printname, :defaultvalue
    attr_accessor :valuestring

    def initialize(def_str)
      parse_definition(def_str)
    end

    def parse_definition(def_str)
      while s = def_str.shift
        break if parse_single_lines_done?(s)
      end
      while s = def_str.shift
        break if s =~ /^\s*$/ and  def_str[0] =~ /^\s*$/
        @longdescription += s + "\n"
      end
    end

    def parse_single_lines_done?(s)
      if s !~ /\s*(\w.*?)\:/                  # non-greedy: stops after 1st ":"
        raise "option definition line has wrong format:\n==> #{s} <==\n"
      end
      name = $1
      content = $'
      case name
        when /Short\s+(N|n)ame/
          @shortname = content.split[0]
        when /Long\s+(N|n)ame/
          @longname = content.split[0]
        when /Value\s+(T|t)ype/
          @type = content.sub(/^\s+/,"").sub(/\s*(#.*|$)/,"")
          @valuestring = "false" if @type == "bool"
        when /Default\s+(V|v)alue/
          @defaultvalue = content.sub(/^\s+/,"").sub(/\s*(#.*|$)/,"")
          @valuestring = @defaultvalue
        when /Global\s+(V|v)ariable/
          @globalname = content.split[0]
        when /Print\s+(N|n)ame/
          @printname = content.sub(/^\s+/,"").sub(/\s*(#.*|$)/,"")
        when /Description/
          @description = content.sub(/^\s+/,"").sub(/\s*(#.*|$)/,"")
        when /Long\s+(D|d)escription/
          @longdescription = ""
          return true
        else
          raise "option definition line unrecognized:\n==> #{s} <==\n"
      end
      return false
    end

    def initialize_global_variable
      eval("$#{@globalname} = eval_value") if @globalname
    end

    def eval_value
      case @type
        when "bool"
          eval(@valuestring)
        when "string"
          @valuestring
        when "int"
          @valuestring.to_i
        when "float"
          @valuestring.to_f
        when /^float\s*vector$/
          @valuestring.gsub(/[\[,\]]/," ").split.map{|x| x.to_f}.to_v
        else
          raise ": type \"#{@type}\" is not recognized"
      end
    end

    def to_s
      if @type == nil                    # top level description of the program
        s = @description + "\n"
      elsif @type == "bool"
        if eval(@valuestring)
          s = @description + "\n"
        else
          s = ""
        end
      else
        s = @description
        s += "\t" if @description.size < 8
        s += "\t" if @description.size < 16
        s += "\t" if @description.size < 24
        s += "\t" if @description.size < 32
        s += ": "
        if @printname
          s += @printname
        else
          s += @globalname
        end
        s += " = " unless @printname == ""
        s += "\n  " if @type =~ /^float\s*vector$/
        s += "#{eval("$#{@globalname}")}\n"
      end
      s
    end

    def print_value
      STDERR.print to_s
    end

  end

  def initialize(def_str, argv_array = nil)
    parse_option_definitions(def_str)
    if argv_array
      parse_command_line_options(argv_array)
    end
    print_values
  end

  def parse_option_definitions(def_str)
    a = def_str.split("\n")
    @options=[]
    while a[0]
      if a[0] =~ /^\s*$/ 
        a.shift
      else
        @options.push(Command_line_option.new(a))
      end
    end
  end

  def parse_command_line_options(argv_array)
    while s = argv_array.shift
      if s == "-h"
        parse_help(argv_array, false)
        exit
      elsif s == "--help"
        parse_help(argv_array, true)
        exit
      elsif i = find_option(s)
        parse_option(i, s, argv_array)
      else
        raise "option \"#{s}\" not recognized"
      end
    end
    initialize_global_variables
  end

  def parse_help(argv_array, long)
    all = true
    while s = argv_array.shift
      if i = find_option(s)
        all = false
        print_help(long, i)
      end
    end
    print_help(long) if all
  end

  def parse_option(i, s, argv_array)
    if @options[i].type == "bool"
      @options[i].valuestring = "true"
      return
    end
    if s =~ /^-[^-]/ and (value = $') =~ /\w/
      @options[i].valuestring = value
    else
      unless @options[i].valuestring = argv_array.shift
        raise "option \"#{s}\" requires a value, but no value given"
      end
    end
    if @options[i].type =~ /^float\s*vector$/
      while (@options[i].valuestring !~ /\]/)
        @options[i].valuestring += " " + argv_array.shift    # " " for -v [3 4]
      end
    end
  end

  def print_help(long, i = nil)
    if i
      STDERR.print help_string(@options[i], long)
    else
      @options.each{|x| STDERR.print help_string(x, long)}
    end
  end

  def help_string(option, long_flag)
    s = ""
    if option.type                   # not top level description of the program
      s += option_name_string(option)
    end
    if option.type or not long_flag
      s += "#{option.description}"
      s += default_value_string(option)
      s += "\n"
    end
    if long_flag
      s += "\n#{option.longdescription}\n"
    end
    return s
  end

  def option_name_string(option)
    s = ""
    if option.shortname
      s += "#{option.shortname}  "
    end
    s += "#{option.longname}"
    s += "\t" if s.size < 8
    s += "\t" if s.size < 16
    s += "\t" if s.size < 24
    s += ": "
    return s
  end

  def default_value_string(option)
    s = ""
    if option.type and option.type != "bool"
      descr_size = "#{option.description}".size + 2
      s += "\t" if descr_size < 8
      s += "\t" if descr_size < 16
      s += "\t" if descr_size < 24
      s += "\t" if descr_size < 32
      s += " [default: #{option.defaultvalue}]"
    end
    return s
  end

  def find_option(s)
    i = nil
    @options.each_index do |x|
      i = x if s == @options[x].longname
      if @options[x].shortname
        i = x if s =~ Regexp.new(@options[x].shortname) and $` == ""
      end
    end
    return i
  end

  def initialize_global_variables
    @options.each{|x| x.initialize_global_variable}
    check_required_options
  end    

  def check_required_options
    options_missing = 0
    @options.each do |x|
      if x.valuestring == "none"                 # e.g. none.rb would be okay !
        options_missing += 1
        STDERR.print "option "
        STDERR.print "\"#{x.shortname}\" or " if x.shortname
        STDERR.print "\"#{x.longname}\" required.  "
        STDERR.print "Description:\n#{x.longdescription}\n"
      end
    end
    if options_missing > 0
      STDERR.print "Please provide the required command line option"
      STDERR.print "s" if options_missing > 1
      STDERR.print ".\n"
      exit(1)
    end
  end

  def print_values
    @options.each{|x| x.print_value}
  end

end

def parse_command_line(def_str)
  Clop.new(def_str, ARGV)
end

if __FILE__ == $0

  options_definition_string = <<-END

  Description:		Test program for the Clop class
  Long description:
    This program appears at the end of the file "clop.rb" that contains
    the definition of the Clop class.
    By running the file (typing "ruby clop.rb"), you can check whether
    it still behaves correctly.  Maximum help is provided by the command
    "ruby clop.rb --help".


  Short name: 		-s
  Long name:		--softening_length
  Value type:		float              # double/real/...
  Default value: 	0.0
  Global variable: 	eps                # any comment allowed here
  Description:		Softening length   # and here too
  Long description:                        # and even here
    This option sets the softening length used to calculate the force
    between two particles.  The calculation scheme comforms to standard
    Plummer softening, where rs2=r**2+eps**2 is used in place of r**2.


  Short name: 		-t
  Long name:		--end_time
  Value type:		float
  Default value:	10
  Global variable:	t_end
  Print name:		t
  Description:		Time to stop integration
  Long description:
    This option gives the time to stop integration.


  Short name: 		-n
  Long name:		--number_of_particles
  Value type:		int
  Default value:	none
  Global variable:	n_particles
  Print name:		N
  Description:		Number of particles
  Long description:
    Number of particles in an N-body snapshot.


  Short name:		-x
  Long name:  		--extra_diagnostics
  Value type:  		bool
  Global variable:	xdiag
  Description:		Extra diagnostics
  Long description:
    The following extra diagnostics will be printed:

      acceleration (for all integrators)
      jerk (for the Hermite integrator)


  Short name:		-v
  Long name:		--shift_velocity
  Value type:		float vector          # numbers in between [ ] brackets
  Default value:	[3, 4, 5]
  Global variable:	vcom
  Description:		Shifts center of mass velocity
  Long description:
    The center of mass of the N-body system will be shifted by this amount.
    If the vector has fewer components than the dimensionality of the N-body
    system, zeroes will be added to the vector.
    If the vector has more components than the dimensionality of the N-body
    system, the extra components will be disgarded.


  Short name:		-o
  Long name:		--output_file_name
  Value type:		string
  Default value:	none
  Global variable:	output_file_name
  Print name:	 	                      # no name, hence name suppressed
  Description:		Name of the outputfile
  Long description:
    Name of the snapshot output file.
    The snapshot contains the mass, position, and velocity values
    for all particles in an N-body system.


  Long name:		--star_type             # no short option given here
  Value type:		string
  Default value:	star: MS                # parser cuts only at first ":"
  Global variable:	star_type
  Description:		Star type
  Long description:
    This options allows you to specify that a particle is a star, of a
    certain type T, and possibly of subtypes t1, t2, ..., tk by specifying
    --star_type "star: T: t1: t2: ...: tk".  The ":" separators are allowed
    to have blank spaces before and after them.

      Examples: --star_type "star: MS"
                --star_type "star : MS : ZAMS"
                --star_type "star: giant: AGB"
                --star_type "star:NS:pulsar:millisecond pulsar"

  END

  parse_command_line(options_definition_string)

end
