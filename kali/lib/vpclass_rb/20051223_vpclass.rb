# Pradeep Elankumaran, (c) 2005
# ------------------------------------------------------

require 'handlers.rb'
include Math

class VirtualPlotter
	def initialize(plotter="gnuplot", output_file=nil)
    @plotter = plotter    

    case @plotter
      when "gnuplot"
        begin
          if !Kernel.system("gnuplot --version") then raise end
          @handler = GnuplotHandler.new()
        rescue
          puts "\nGnuplot not installed\n"
          exit(0)
        end
      when "tioga"
        # Tioga exception-handling in the handlers.rb file due
        # to technical reasons
        @handler = TiogaHandler.new()
      when "sm"
        begin
          if !Kernel.system("sm -V") then raise end
          @handler = SMHandler.new()
        rescue
          puts "\nSuperMongo not installed\n"
          exit(0)
        end
    end
    
    @handler.new_device('file', output_file) if !output_file.nil?
    @handler.new_device('window') if output_file.nil?
	end

  def data(file_or_array, cols=[1,2])
    if file_or_array.class == Array then
      if file_or_array[0].class == Array then
        # using an array of arrays ex: [[1,2], [2,4]]
        file_or_array.each do |line|
          vector = [ line[cols[0]-1], line[cols[1]-1] ]
          @handler.save_vector(vector)
        end
      end
    elsif file_or_array.class == String 
      if file_or_array != 'stdin' then
        # assume this is a filename
        data = IO.readlines(file_or_array)
        data.map!{|x| x.split.collect!{|x| x.to_f}}
        data.each do |line|
          vector = [line[cols[0]-1], line[cols[1]-1]]
          @handler.save_vector(vector)
        end
      elsif file_or_array == 'stdin'
        # using input standard input
        while line = gets
          line = line.chomp!.split(' ').collect!{|x| x.to_f}
          vector = [line[cols[0]-1], line[cols[1]-1]] if !cols.nil?
          @handler.save_vector(vector)
        end
      end
    end
    self
  end
  
  def plot
    @handler.plot_points
  end
  
  # self-explanatory wrapper methods
  # ----------------------------------
  def new_device(type, filename=nil)
    @handler.new_device(type, filename)
	end

	def set_limits(xlimits=[-5,5], ylimits=[-5,5])
	 @handler.set_limits(xlimits, ylimits)
	end

	def set_labels(xlabel="x", ylabel="y")
	 @handler.set_labels(xlabel, ylabel)
	end

	def write_text(text_str, x,y)
	 @handler.write_text(text_str, x,y)
	end

	def draw_line(x,y)
	 @handler.draw_line(x,y)
	end

	def set_title(title)
	 @handler.set_title(title)
	end
	
	def close_device
	 @handler.close_device
  end
end
