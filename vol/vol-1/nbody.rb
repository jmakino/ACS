#<em> Nbody models an N-body system </em>

require "body.rb"

class Nbody

  attr_accessor :time, :body

  def n
    @body.size
  end

  def initialize(n=0, time = 0.0)
    @body = [Body.new]
    for i in 0...n
      @body[i] = Body.new
    end
    @time = time
  end

  def pp            # pretty print
    print "     N = ", @body.size, "\n"
    print "  time = ", @time, "\n"
    @body.each do |b| b.pp end
  end

  # Jun:
  # how about renaming "simple_print" as "write" and "simple_read" as "read"
  # here and also in body.rb ?  --  Piet

#:segment start: simple-IO
  def simple_print
    print @body.size, "\n"
    printf("%22.15e\n", @time)
    @body.each do |b| b.simple_print end
  end

  def simple_read
    n = gets.to_i
    @time = gets.to_f
    for i in 0...n
      @body[i] = Body.new
      @body[i].simple_read
    end
  end
#:segment end:

  # Jun:
  # it seems a bit silly to have an extra set of these two, for general file
  # use, rather than stdio; do you have a suggestion to avoid repetition?

  def simple_fprint(file_name)
    file_name.print @body.size, "\n"
    file_name.printf("%22.15e\n", @time)
    @body.each do |b| b.simple_fprint(file_name) end
  end

  def simple_fread(file_name)
    @body = Array.new
    n = file_name.gets.to_i
    @time = file_name.gets.to_f
    for i in 0...n
      @body[i].simple_fread(file_name)
    end
  end

end
