require "acs.rb"
require "vector.rb"

#:segment start: helperclass
class Clop_Option

  attr_reader :shortname, :longname, :type,
              :description, :longdescription, :printname, :defaultvalue
  attr_accessor :valuestring

  def initialize(def_str)
    parse_option_definition(def_str)
  end

  def parse_option_definition(def_str)
    while s = def_str.shift
      break if parse_single_lines_done?(s)
    end
    while s = def_str.shift                                                  #9
      break if s =~ /^\s*$/ and  def_str[0] =~ /^\s*$/                       #9
      @longdescription += s + "\n"                                           #9
    end                                                                      #9
  end

  def parse_single_lines_done?(s)
    if s !~ /\s*(\w.*?)\s*\:/                                               #10
      raise "\n  option definition line has wrong format:\n==> #{s} <==\n"
    end
    name = $1
    content = $'
    case name
      when /Short\s+(N|n)ame/
        @shortname = content.split[0]
      when /Long\s+(N|n)ame/
        @longname = content.split[0]
      when /Value\s+(T|t)ype/
        @type = content.sub(/^\s+/,"").sub(/\s*(#.*|$)/,"")                 #11
        @valuestring = "false" if @type == "bool"                           #12
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
        @longdescription = ""                                               #13
        return true                                                         #13
      else
        raise "\n  option definition line unrecognized:\n==> #{s} <==\n"
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
        raise "\n  type \"#{@type}\" is not recognized"
    end
  end

  def add_tabs(s, reference_size, n)
    (1..n).each{|i| s += "\t" if reference_size < 8*i}
    return s
  end

  def to_s
    if @type == nil                                                         #14
      s = @description + "\n"                                               #14
    elsif @type == "bool"                                                   #15
      if eval(@valuestring)                                                 #15
        s = @description + "\n"                                             #15
      else                                                                  #15
        s = ""                                                              #15
      end                                                                   #15
    else
      s = @description                                                      #16
      s = add_tabs(s, s.size, 4)                                            #16
      s += ": "                                                             #16
      if @printname                                                         #16
        s += @printname                                                     #16
      else                                                                  #16
        s += @globalname                                                    #16
      end                                                                   #16
      s += " = " unless @printname == ""                                    #17
      s += "\n  " if @type =~ /^float\s*vector$/                            #18
      s += "#{eval("$#{@globalname}")}\n"                                   #19
    end
    return s
  end

end
#:segment end:

class Clop

  def initialize(def_str, argv_array = nil)
    parse_option_definitions(def_str)                                        #8
    if argv_array
      parse_command_line_options(argv_array)
    end
    initialize_global_variables
    print_values
  end

  def parse_option_definitions(def_str)
    def_str += HELP_DEFINITION_STRING
    a = def_str.split("\n")                                                  #1
    @options=[]
    while a[0]
      if a[0] =~ /^\s*$/                                                     #2
        a.shift                                                              #2
      else
        @options.push(Clop_Option.new(a))                                    #3
      end
    end
  end

  def parse_command_line_options(argv_array)
    s1 = argv_array.one_level_deep_copy
    while s = s1.shift
      if s == "-h"
        parse_help(s1, false)
        exit
      elsif s == "--help"
        parse_help(s1, true)
        exit
      end
    end
    while s = argv_array.shift
      if i = find_option(s)
        parse_option(i, s, argv_array)
      else
        raise "\n  option \"#{s}\" not recognized; try \"-h\" or \"--help\"\n"
      end
    end
  end

  def print_values
    @options.each{|x| STDERR.print x.to_s}
  end

  def find_option(s)
    i = nil
    @options.each_index do |x|
      i = x if s == @options[x].longname
      if @options[x].shortname
        i = x if s =~ Regexp.new(@options[x].shortname) and $` == ""         #4
      end
    end
    return i
  end

  def parse_option(i, s, argv_array)
    if @options[i].type == "bool"
      @options[i].valuestring = "true"
      return
    end
    if s =~ /^-[^-]/ and (value = $') =~ /\w/                                #6
      @options[i].valuestring = value                                        #6
    else
      unless @options[i].valuestring = argv_array.shift                      #5
        raise "\n  option \"#{s}\" requires a value, but no value given;\n" +#5
              "  option description: #{@options[i].description}\n"           #5
      end
    end
    if @options[i].type =~ /^float\s*vector$/                                #7
      while (@options[i].valuestring !~ /\]/)                                #7
        @options[i].valuestring += " " + argv_array.shift                    #7
      end
    end
  end

  def initialize_global_variables
    @options.each{|x| x.initialize_global_variable}
    check_required_options
  end    

  def check_required_options
    options_missing = 0
    @options.each do |x|
      if x.valuestring == "none"
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

  def parse_help(argv_array, long)
    all = true
    while s = argv_array.shift
      all = false
      if i = find_option(s)
        print_help(long, i)
      else
        print_help_warning(s)
      end
    end
    print_help(long) if all
  end

  def print_help(long, i = nil)
    if i
      STDERR.print help_string(@options[i], long)
    else
      @options.each{|x| STDERR.print help_string(x, long)}
    end
  end

  def print_help_warning(s)
    STDERR.print "WARNING : ", s, " : ==> this option is not recognized <==\n"
  end

  def help_string(option, long_flag)
    s = ""
    if option.type or option.shortname == "-h"
      s += option_name_string(option)
    end
    if option.type or option.shortname == "-h" or not long_flag             #20
      s += "#{option.description}"                                          #20
      s += default_value_string(option)                                     #20
      s += "\n"                                                             #20
    end                                                                     #20
    if long_flag                                                            #21
      s += "\n#{option.longdescription}\n"                                  #21
    end                                                                     #21
    return s
  end

  def option_name_string(option)
    s = ""
    if option.shortname
      s += "#{option.shortname}  "
    end
    s += "#{option.longname}"
    s = option.add_tabs(s, s.size, 3)
    s += ": "
    return s
  end

  def default_value_string(option)
    s = ""
    if option.type and option.type != "bool"
      reference_size = "#{option.description}".size + 2
      s = option.add_tabs(s, reference_size, 4)
      s += " [default: #{option.defaultvalue}]"                             #22
    end
    return s
  end

  HELP_DEFINITION_STRING = <<-END


  Short name: 		-h
  Long name:		--help
  Description:		Help facility
  Long description:
    When providing the command line option -h, followed by one or more
    options, a one-line summary of each of the options will be printed.

    These options can be specified in either short or long form, i.e
    typing "some_command -h -x" or "some_command -h --extra" will produce
    the same output, if "-x" and "--extra" invoke the same option.

    When providing the command line option -h, with nothing else following,
    a one-line summary of all options will be printed.

    When providing the command line option --help, instead of -h, the same
    actions occur, the only difference being that instead of a one-liner,
    a longer description will be printed.

    Anything that appears on the command line between the name of the
    program and "-h" or "--help" will be ignored.  For example, typing
    "some_command gobbledygook -h -x" will produce the same output as
    typing "some_command -h -x"


  END

end

def parse_command_line(def_str)
  Clop.new(def_str, ARGV)
end

if __FILE__ == $0

  options_definition_string = <<-END

  Description: Command line option parser
  Long description:
    Test program for the class Clop (Command line option parser),
    (c) 2004, Piet Hut and Jun Makino; see ACS at www.artcompsi.org

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
