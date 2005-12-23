#! /usr/bin/env ruby
#
#----------------------------------------------------------------------
# 
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or(at your option)
# any later version. This program is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details; it is available at
# <http://www.fsf.org/copyleft/gpl.html>, or by writing to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
# 
# Written by Gordon Miller <gmiller@bittwiddlers.com>. Inspired by and partly
# derived from the Python version by Michael Haggerty. If you find a problem
# or have a suggestion, please let me know at <gmiller@bittwiddlers.com>. 
# Other feedback would also be appreciated.
#
#---------------------------------------------------------------------- 

#:include: README

version = '0.5'
cvs_version = '$Revision: 1.6 $'

module Gnuplot

  class Writer
    def writeln(s)
      write("#{s}\n")
    end

    def flush
    end
  end

  # This class keeps a string of commands that are written to the GnuplotSink. 
  # Each call to a command that passes data into the class writes the data to
  # a member string that can then be accessed via the data accessor. The
  # principle purpose of this class is for use in testing.
  # 
  class StringWriter < Writer
  
    attr_reader :data		# Accumulator for data written to object

    # Initializes the instance to contain an empty data member.
    #
    def initialize()
      @data = ""
    end

    # Appends the contents of the string s onto the data member.
    #
    def write(s)
      @data += s
    end

    def to_s 
      @data
    end
  end

  # Sometimes it is convenient to write gnuplot commands to a command file for
  # later evaluation. In that case, one of these objects is used as a mock
  # gnuplot process. Note that temporary files may be deleted before you have
  # time to execute the file!
  # 
  class FileWriter < Writer

    # Initialize the instance to write to the specified filename
    #
    def initialize(filename)
      @file = open(filename, 'w')
      writeln("#!/bin/env gnuplot")
    end

    # Write an arbitrary string to the file.
    #
    def write(s)
      @file.write(s)
    end  

    def flush
      @file.flush
    end
  end

  # This represents a running gnuplot program and the means to communicate
  # with it at a primitive level(i.e., pass it commands or data). When the
  # object is destroyed, the gnuplot program exits(unless the 'persist'
  # option was set). The communication is one-way; gnuplot's text output just
  # goes to stdout with no attempt to check it for error messages. 
  
  class ProcessWriter < Writer

    attr_reader :cmd		# Gnuplot command

    # Create a 'GnuplotProcess' object. This starts a gnuplot program and
    # prepares to write commands to it.
    #
    # persist - If set to true then the gnuplot command will be run with the
    #           'persist' command line option. 
    # 
    def initialize(persist = true)
      IO.popen( "which gnuplot" ) { |io| @cmd = io.readline.chomp }
      @cmd += " -persist" if persist
      @gnuplot = IO::popen(@cmd, "w")
    end

    # Write pending output immediately
    #
    def flush
      @gnuplot.flush
    end

    # Pass an arbitrary string to the gnuplot program
    #
    def write(s)
      @gnuplot.write( s )
      flush
    end
  
  end

  
  # Base class of all objects that are capable of being plotted within Ruby
  # Gnuplot. Child classes are categorized by the source of their data.
  #
  # == Options
  #
  # This base class understands and supports the following plotting options,
  # which are common to all plot types.
  #
  # xrange - The range of values for the x axis
  #
  # yrange - The range of values for the y axis
  #
  # using  - The columns of a data that are to be used for the plot
  # 
  # with   - How the item should be plotted, e.g., with='points 3 3'.
  #
  # title  - The text to be associated with the item in the plot legend. If
  #          this is set to 'nil', then this will cause the 'notitle' option to
  #          be passed to gnuplot, which will cause the plot item to be omitted
  #          from the legend.

  class DataSet

    # The first option that will be passed to the 'plot' or 'splot' command in
    # gnuplot. This is a characteristic of the derived class. The basecommand
    # is the string that tells gnuplot what its plotting. For example, in the
    # command 'plot sin(x)' the basecommand is 'sin(x)'. Probably not a very
    # intuitive name, but it works for now.
    #
    attr_reader :basecommand
    
    # Returns the set of options that will be used in the call to the base
    # command. 
    #
    attr_reader :options

    # The order in which options need to be passed to gnuplot.
    #
    @@optionSequence = ['xrange', 'yrange', 'binary', 'using', 'title', 'with']
  
    # Construct a new object that has the specified basecommand and options.
    #
    def initialize(basecommand, opts=nil)
      @basecommand = basecommand
      @options = Hash.new

      opts.each { |n,v| @options[n] = v } if opts
    end

    # Return the setting of an option.
    def getOption(name)
      return @options[name]
    end

    # Clear(unset) a plot option.  No error if option was not set.
    def clearOption(name)
      @options.delete(name)
    end

    def pipein(f)
      writeData(f)
      f.write("e\n")
    end

    # Build and return the 'plot' command, with options, necessary
    # to display this item.
    # 
    def command

      cmd = ""

      xval = @options['xrange']
      cmd << " #{xval}" if( xval )
      
      yval = @options['yrange']
      cmd << " #{yval}" if( yval and xval )

      cmd << " #{@basecommand}"

      cmd << " using #{@options['using']}" if( @options['using'] )

      tstr = @options["title"]
      cmd += if( !tstr ) then  ''
	     elsif( tstr == 'notitle' ) then 'notitle'
	     else  "  title '#{tstr}'"
	     end

      cmd << " with #{@options['with']}" if( @options["with"] )

      cmd
    end

    # Invoked by Ruby when a method is invoked on this class that doesn't
    # exist. The method name is taken to be the name of an option and the
    # argument is taken to be a value. The name/value pair is inserted into
    # the options hash.
    #
    def method_missing(id, *args)

      name = id.id2name
      if( args.length == 1 ) 
	@options[name] = args[0]
      elsif( args.length == 0 )
	return @options[name]
      end
    end

  end				# class DataSet


  # A DataSet that understands how to plot instances of Array objects. The
  # basecommand that is used in the plot and splot commands is the '-'
  # character which tells Gnuplot that the data is inline. The pipein method
  # is then used after the plot command to write out the data into the
  # GnuplotWriter that is the sink for the Gnuplot commands.
  #
  # == Options
  # 
  # xgrid - The x grid for the data object.  Must be of the same length as the
  #         first dimension of the data array.
  # 
  class ArrayDataSet < DataSet

    # Constructs a new object that will plot the specified data. The data
    # parameter is expected to be an instance of an Array object. The
    # parameters that are understood by this object will used and removed and
    # the remaining options will be passed to the super.initialize method. 
    # That means that the opts object is modified while in this method.
    #
    def initialize(data, opts=nil)
      super("'-'", opts)
      @data = data
    end

    # Writes the data member of the object into the specified GnuplotWriter w. 
    # The data is written in the Gnuplot format which is the one data point
    # per line.
    # 
    # If the data member is one dimensional, then the data inserted into the
    # writer will have two columns. The first column contains the x value of
    # the point and the second will contain the y value. If the data is two
    # dimensional, then the output will contain three columns, x, y, and z.
    # 
    def writeData(f)
      xgrid = getOption("xgrid")
      sigma = getOption("sigma")

      @data.length.times { |i|
	x = if xgrid then xgrid[i].to_s else i.to_s end
	sig = if sigma then sigma[i].to_s else " " end
	
	f.writeln( x + " " + @data[i].to_s + " " + sig)
      }
    end
  end


  class MatrixDataSet < ArrayDataSet

    def writeData(f)
      xgrid = getOption("xgrid") ||(0...@data.column_size).to_a
      ygrid = getOption("ygrid") ||(0...@data.row_size).to_a

      xgrid.length.times { |i|
	ygrid.length.times { |j|
	  f.write "#{xgrid[i]} #{ygrid[j]} #{@data[i,j]}\n"
	}
      }
    end

  end

  class NArrayDataSet < DataSet
  
    def initialize(data, opts)
      super("'-'", opts)
      if( data.type != NArray )
	raise "Invalid type"
      end
      @data = data
    end

    # Writes the data member of the object into the specified GnuplotWriter w. 
    # The data is written in the Gnuplot format which is the one data point
    # per line.
    # 
    # If the data member is one dimensional, then the data inserted into the
    # writer will have two columns. The first column contains the x value of
    # the point and the second will contain the y value. If the data is two
    # dimensional, then the output will contain three columns, x, y, and z.
    # 
    def writeData(w)
      ygrid = getOption('ygrid')
    
      elems = @data.shape
      if   ( elems.length == 1 ) then write_data_1D(w)
      elsif( elems.length == 2 ) then write_data_2D(w)
      end
    end


    def write_data_1D(w)
      xgrid = getOption("xgrid") ||(0...@data.shape[0]).to_a
      xgrid.length.times { |i|
	w.writeln( "#{xgrid[i]} #{@data[i]}" )
      }
    end


    def write_data_2D(w)
      xgrid = getOption("xgrid") ||(0...@data.shape[0]).to_a
      ygrid = getOption("ygrid") ||(0...@data.shape[1]).to_a

      xgrid.length.times { |i|
	ygrid.length.times { |j|
	  w.writeln( "#{xgrid[i]} #{ygrid[j]} #{@data[i, j]}" )
	}
      }
    end
  end



  # StringFunc represents a mathematical expression that is to be computed by
  # gnuplot itself.  For example, creating a StringFunc object with the string
  # 'sin(x)' like so:
  #
  # StringFunc('sin(x)')
  # 
  # will have the same effect as typing the following into gnuplot itself:
  # 
  # 'plot sin(x)'
  #
  class StringFunc < DataSet

    # Initializes a new StringFunc object that will have gnuplot plot the
    # specified function. The str argument is expected to be a string that
    # represents a gnuplot function. The opts are passed directly to the base
    # class.
    # 
    def initialize(str, opts = {})
      super
    end

    def pipein(f)
    end
  end



  # Base class for all objects that execute plotting within Gnuplot. Each plot
  # within Gnuplot is created using either a plot or splot command. This
  # basecommand is then passed arguments that specify what to plot and how to
  # plot it.
  #
  # This class should not be directly instantiated but instead should be
  # subclassed.
  #
  class Plot
  
    # The basic plotting command that will create this plot.
    #
    attr_reader :plotcmd
  
    # The DataSets to be plotted. 
    #
    attr_reader :itemList
  
    # Initialize a new object that will use the given plot command to execute
    # the specified gnuplot interpreter.  
    #
    def initialize(filename=nil, persist=1)
      @plotcmd = 'plot'

      @gnuplot = if filename == nil then
		   ProcessWriter.new(persist=persist)
		 elsif( filename.kind_of? String )  then
		   FileWriter.new(filename)
		 else
		   filename
		 end

      @itemList = Array.new()
    end


    # Clear the current plot and create a new 2-d plot containing the
    # specified items. Each arguments should be of the following types:
    # 
    # 'PlotItem'(e.g., 'Data', 'File', 'Func') -- This is the most flexible
    # way to call plot because the PlotItems can contain suboptions. Moreover,
    # PlotItems can be saved to variables so that their lifetime is longer
    # than one plot command; thus they can be replotted with minimal overhead.
    # 
    # 'string'(e.g., 'sin(x)') -- The string is interpreted as 'Func(string)'
    #(a function that is computed by gnuplot).
    # 
    # Anything else -- The object, which should be convertible to an array, is
    # converted to a 'Data' item, and thus plotted as data. If the conversion
    # fails, an exception is raised.
    #
    def draw(*items)
      clearQueue()
      addToQueue(items)
      plot
    end

    # Send the string s as a command to gnuplot, followed by a newline. All
    # communication with the gnuplot process(except for inline data) is
    # through this method.
    #
    def writeln(s)
      @gnuplot.writeln(s)
    end
  
    # Method that is executed at the start of the plot to initialize the plot. 
    # Typical use of this method is to execute 'set' statements for the plot.
    #
    def configurePlot
    end
    
    # Refresh the current plot by reissuing the gnuplot plot command
    # corresponding to the current itemList.
    #
    def plot
      if( @itemList.length > 0 ) 
        configurePlot()
      
        plotcmds = Array.new()
      
        @itemList.each { |item| plotcmds.push( item.command() ) }
        fullcmd = @plotcmd + " " + plotcmds.join(", ")
        writeln( fullcmd )
            
        @itemList.each { |item| item.pipein( @gnuplot ) }
        @gnuplot.flush()
      else
        puts "No items to plot"
      end
    end

    # Removes the items that currently reside in the plot item list.
    #
    def clearQueue
      @itemList.clear()
    end
  
    # Add a list of items to the itemlist(but don't plot them). 'items' is a
    # sequence of items, each of which should be a 'PlotItem' of some kind, a
    # string(interpreted as a function string for gnuplot to evaluate), or a
    # Numeric array(or something that can be converted to a Numeric array).
    #
    def addToQueue(items)
      if( items.is_a? DataSet ) then
	@itemList.push( items )
      elsif( items.is_a? Array ) then
	items.each { |i| addToQueue(i) }
      end
    end

    # Replot the existing graph, using the items in the current itemlist. If
    # arguments are specified, they are interpreted as additional items to be
    # plotted alongside the existing items on the same graph. See 'plot' for
    # details.
    #
    def redraw(*items)
      addToQueue(items)
      plot
    end

    # Clear the plot window(without affecting the current itemlist).
    #
    def clear
      writeln("clear")
    end

    # Reset all gnuplot settings to their defaults and clear itemlist.
    #
    def reset
      writeln('reset')
      clearQueue()
    end

    # Insert a label with the given text into the plot at the specified
    # position and with the given justification. If the position or
    # justification are not specified then they default to the gnuplot
    # defaults. Executing this command is exactly equivalent to executing the
    # gnuplot command.
    #
    # 'set #{text} [at #{position}] [#{justification}]'
    #
    def label(text, position = nil, justification = nil)
      args = "'#{text}'"
      args += " at #{position}"   if( position )
      args += " #{justification}" if( justification )
      set_string("label", args)
    end

    # Load a file using gnuplot's 'load' command.
    #
    def load(filename)
      writeln("load '#{filename}'")
    end

    # Invoked when there is no defined method for the given id.  The method
    # call is translated into a call to set_string.
    #
    def method_missing(id, *args)
      name = id.id2name
      case args.length
      when 1
	set_string(name, args.join)
      when 0
	set_string(name)
      end
    end

    # Save the current plot commands using gnuplot's 'save' command.
    #
    def save(filename)
      writeln("save '#{filename}'")
    end

    # Set a string option, or if s is omitted, unset the option. If the method
    # determines that the option requires a quoted string, it will double
    # quote the string in the set call. Double quoting is performed so that
    # gnuplot will interpret backslash(\) characters. In order to prevent
    # ruby from interpreting the \ in characters like \n or \r, be sure to
    # escape the initial \. Therefore to set a linefeed, pass \\n or \\r
    # instead.
    #
    def set_string(option, s=nil)
      requireString = ["xlabel", "output", "title", "ylabel"]
      
      if s == nil or s.length == 0 then
	writeln("set #{option}")
      else
	val = if requireString.include? option
		"\"#{s}\""
	      else
		s
	      end
	
	writeln("set #{option} #{val}")
      end
    end
    
    # Set the xrange of the plot.
    #
    def xrange(min, max)
      set_string("xrange", "[#{min}:#{max}]")
    end
    
    # Set the yrange of the plot
    #
    def yrange(min, max)
      set_string("yrange", "[#{min}:#{max}]")
    end

  end

  # Base class for all objects that create plots on a three-dimensional
  # coordinate system.
  #
  class Splot < Plot
    
    # Initialize the plot. If filename is specified then the result of the
    # plot will be a file containing the gnuplot commands.
    #
    def initialize(filename=nil,persist=1)
      super
      @plotcmd = 'splot'
    end
    
    # Set the zlabel for the plot. If the label is not specified then the
    # label will be reset.
    #
    def zlabel(s=nil)
      set_string("zlabel", "'#{s}'")
    end
  end

end				# Module Gnuplot

module Gnuplotable

  def gnuplot(opts = nil)
    plot = Gnuplot::Plot.new
    plot.draw( gnuplot_dataset(opts) )
  end

  def gnusplot(opts = nil)
    plot = Gnuplot::Splot.new(opts)
    plot.draw( gnuplot_dataset(opts) )
  end

end

class Array 
  include Gnuplotable

  def gpds(opts=nil)
    Gnuplot::ArrayDataSet.new(self, opts)
  end
end


class NArray
  include Gnuplotable

  def gpds(opts=nil)
    Gnuplot::NArrayDataSet.new(self, opts)
  end
end
  

class String 
  include Gnuplotable

  def gpds(opts=nil)
    Gnuplot::StringFunc.new(self, opts)
  end
end

class Matrix
  include Gnuplotable
  
  def gpds(opts=nil)
    Gnuplot::MatrixDataSet.new(self, opts)
  end
end

# $Id: Gnuplot.rb,v 1.6 2003/02/07 20:42:20 gmiller Exp $
