#
# Command line option parser
#
# Jun Makino and Piet Hut 2004
#
class Clop

  attr_reader :unparsed

  class Option

    attr_reader :shortname, :longname, :type, :description, :longdescription
    attr_accessor :valuestring, :value

    def initialize(a)
      self.from_sa(a)
    end

    def parse_definition(str,a)
      raise unless  str =~ /\s*(\w(\w|\s)*)\:/
      name = $1
      content = $'
      case name
      when /Short\s+(N|n)ame/
	@shortname = content.gsub(" ","")
      when /Long\s+(N|n)ame/
	@longname = content.gsub(" ","")
      when /Value\s+(T|t)ype/
	@type = content.split[0]
      when /Default\s+(V|v)alue/
	@defaultvalue = content
	@valuestring = content
      when /Global\s+(V|v)ariable/
	@globalname = content.gsub(" ","")
      when /Description/
	@description = content.gsub(/^\s+/,"")
      when /Long\s+(D|d)escription/
	@longdescription=""
	while s=a.shift
	  @longdescription += s+"\n"
	  break if s =~ /^\s*$/ and  a[0] =~ /^\s*$/ 
	end
	return 1
      end
      nil
    end

    def from_sa(sa)
      while str=sa.shift
	break if parse_definition(str,sa)
      end
    end

    def parse_value
      case @type
      when "float"
	val = @valuestring.split[0].to_f
      when "int"
	val = @valuestring.split[0].to_i
      when "string"
	val = @valuestring
      when "bool"
	val = eval(@valuestring)
      end
    end

    def assign_global
      eval ("$#{@globalname}=parse_value") if @globalname
    end

  end

  def setfromtext(s)
    a = s.split("\n")
    @options=[]
    while a[0]
      unless a[0]=~ /^\s*$/ 
	@options.push(Option.new(a))
      else
	a.shift
      end
    end
  end

  def initialize(s,a=nil)
    setfromtext(s)
    
    if a
      parse_commandlineoptions(a) 
    end

  end

  def assign_global
    @options.each{|x| x.assign_global}
    if check_required_options
      STDERR.print("aborting\n")
      exit (1)
    end
  end    

  def find_option(s)
    i=nil
    @options.each_index{|x|
      i = x if @options[x].shortname == s or @options[x].longname == s
    }
    print "find option returns #{i} for #{s}\n" if $DEBUG
    i
  end

  def value(s)
    val=nil;
     if i=find_option(s)
       val=@options[i].parse_value 
     end
    val
  end
  
  def print_shorthelp
    @options.each{|x|
      STDERR.print "   #{x.shortname}  #{x.longname}: #{x.description}\n"
    }
  end

  def print_longhelp
    @options.each{|x|
      STDERR.print " #{x.shortname}  #{x.longname}:\n#{x.longdescription}\n"
    }
  end


  def check_required_options
    option_missing=nil
    @options.each{|x|
      if x.valuestring =~ /^\s*none(\s|#|$)/
	option_missing = true
	STDERR.print "option #{x.shortname} required. Description:\n#{x.longdescription}\n"
      end
    }
    option_missing
    end

  def parse_commandlineoptions(optionarray)
    @unparsed=[]
    while s=optionarray.shift
      if s == "-h"
	print_shorthelp
	exit
      elsif s== "--help"
	print_longhelp
	exit
      elsif i=find_option(s)
	if @options[i].type == "bool"
	  @options[i].valuestring = "true"
	else
	  unless @options[i].valuestring = optionarray.shift
	    raise "option #{s} requires a value, but no value given"
	  end
	end
	print "new value for option #{@options[i].shortname} = " if $DEBUG
	print "#{@options[i].valuestring}\n " if $DEBUG
      else
	@unparsed.push(s)
      end
    end
    assign_global
    unparsed
  end

end


if __FILE__ == $0

  optionstext= <<-END

  Short name: -s
  Long name:  --softening_length
  Value type:  float #double/real...
  Default value: 0.0 #none to cause error
  Description: softening length
  Global variable: eps
  Long description:
  This option set the softening length used to calculate the force
  between two particles. The calculation scheme is the standard
  Plummer softening, where rs2=r**2+eps**2 is used in place of r**2.


  Short name: -t
  Long name:  --end_time
  Value type:  float #
  Default value: 10 #none to cause error)
  Description: Time to stop integration
  Global variable: tend
  Long description:
  This option gives the time to stop integration.


  Short name: -o
  Long name:  --output_file_name
  Value type:  string
  Default value: none
  Description: Name of the outputfile
  Global variable: output_file_name
  Long description:
  Name of the snapshot output file


  END

  clop= Clop.new(optionstext,ARGV)
  print "Option values after command-line parsing\n"
  p $eps
  p $tend
  p $output_file_name
  p clop.value("-s")
  p clop.unparsed
end
