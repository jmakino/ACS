class Handler
  attr_accessor :pipe
  
  def initialize()
    @xpoints, @ypoints = [],[]
    @pipe = false
  end
  
  def save_vector(vector)
    @xpoints.push(vector[0])
    @ypoints.push(vector[1])
  end
end

# ---------------------------------------------------------
class GnuplotHandler < Handler
  
  require 'gnuplot.rb'
  
  def initialize(style='points', legend='Data')
    @style = style
    @legend = legend
    @xpoints, @ypoints = [],[]
    @pipe = true
  end
  
  def new_device(type='window', filename=nil)
    case type
      when 'window'
        @plot = Gnuplot::Plot.new
      when 'file'
        @plot = Gnuplot::Plot.new
        @plot.terminal 'postscript eps'
        if !filename.nil? then 
          puts "printing out to file #{filename}"
          @plot.output "#{filename}"
        else
          puts 'using default file vp.ps'
          @plot.output 'vp.ps'
        end
    end
	end

	def plot_points()
	 ds = @ypoints.gpds('with'=>@style, 'title'=>@legend, 
	                    'xgrid'=>@xpoints)
	 @plot.draw ds
	end
	
	def set_limits(xlimits, ylimits)
	 @plot.xrange(xlimits[0], xlimits[1])
	 @plot.yrange(ylimits[0], ylimits[1])
	end
	
	def set_labels(xlabel, ylabel)
	 @plot.xlabel(xlabel)
	 @plot.ylabel(ylabel)
	end
	
	def set_title(title)
	 @plot.title(title)
	end
	
	def close_device()
	end
end

# ----------------------------------------------------------
class SMHandler < Handler
  def initialize
    @xpoints, @ypoints = "", ""
    @pipe = true
  end
  
  def new_device(type='window', filename=nil)
    @filename = filename
    @plot = IO.popen("/usr/bin/env sm", "w+")
    case type
      when 'window'
        puts "SM window mode is buggy. The window closes as"+
          " soon as the program closes. Needs to be fixed by "+
          " the SM maintainer himself."
        @plot.puts "dev x11"
      when 'file'
        if !@filename.nil? then
          puts "printing out to  #{@filename}" 
          @plot.puts "dev postfile #{@filename}"
        else
          puts "using default filename vp.ps"
          @plot.puts "dev postfile vp.ps" 
        end
        @file_flag = true
    end
    @limits_flag = false
  end
  
  def save_vector(vector)
    @xpoints += "#{vector[0].to_s} "
    @ypoints += "#{vector[1].to_s} "
  end
  
  def plot_points()
    @plot.puts "set x = {#{@xpoints}}"
    @plot.puts "set y = {#{@ypoints}}"
    @plot.puts "limits x y" if @limits_flag == false
    @plot.puts "box"
    @plot.puts "points x y"
  end
  
  def set_limits(xlimits, ylimits)
      xmin, xmax = xlimits[0], xlimits[1]
      ymin, ymax = ylimits[0], ylimits[1]
      @plot.puts "limits #{xmin} #{xmax} #{ymin} #{ymax}"
      @limits_flag = true
  end
  
  def set_labels(xlabel, ylabel)
    @plot.puts "xlabel #{xlabel}"
    @plot.puts "ylabel #{ylabel}"
  end
  
  def set_title(title)
    @plot.puts "toplabel #{title}"
  end
  
  def close_device
    @plot.puts "hardcopy" if @file_flag == true
    @plot.close_write
  end
end

# ----------------------------------------------------------
class TiogaHandler < Handler
  
  begin
    require 'Tioga/FigureMaker'
    include Tioga
    include FigureConstants
  rescue LoadError
    raise "\nTioga not installed\n"
  end
  
  def t
    @figure_maker
  end
  
  def new_device(type="file", filename=nil)
    @filename = filename    
    @figure_maker = FigureMaker.new
    case type
      when "window"
        puts "Tioga does not have a built-in real-time graphical "+ 
          "display.\nOpen the pdf or ps file to view the plot"
        raise
      when "file"
        if !filename.nil? then
          puts "Tioga only supports .pdf and .tex formats. "+
            "Use your own pdf2ps to get the ps file\n" end
        @ps_flag = true
    end
    @xpoints = Dvector.new
    @ypoints = Dvector.new
    @plot = t.def_figure("#{filename.chop.chop.chop}") { plot }
    @pipe = false
    @limits_flag = false
    @labels_flag = false
  end
  
  def set_limits(xlimits, ylimits)
    @plot_bounds = [xlimits[0], xlimits[1],
                    ylimits[0], ylimits[1]]
    @limits_flag = true
  end
  
  def set_labels(xlabel, ylabel)
    if @labels.nil? then @labels = [xlabel, ylabel]
    else @labels.push(xlabel, ylabel) end
    @labels_flag = true
  end
  
  def set_title(title)
    if !@labels.nil? then @labels.unshift(title)
    else @labels = [title] end
    @labels_flag = true
  end
  
  def save_vector(vector)
    @xpoints.push(vector[0])
    @ypoints.push(vector[1])
  end
  
  def plot
    if @labels_flag == true then 
      t.do_box_labels(@labels[0], @labels[1], @labels[2])
    end
    xs = @xpoints
    ys = @ypoints
    if @limits_flag == false then 
      @plot_bounds = [xs.min, xs.max, ys.max, ys.min] end 
    t.show_plot(@plot_bounds) do 
      t.show_marker( 'Xs'=> xs, 'Ys'=> ys,
            'marker'=> Circle, 'scale'=> 0.2, 'color'=> Blue) end 
    puts "printed vp.pdf, vp.tex"
  end
  
  def plot_points
    t.make_preview_pdf("vp")
  end
  
  def close_device
    newfilename = @filename.chop.chop.chop
    files = ["#{newfilename}.log", "#{newfilename}.aux", 'pdflatex.log',
             "#{newfilename}_figure.txt", "#{newfilename}_figure.pdf"]
    files.each { |x| File.delete(x) }
  end
end