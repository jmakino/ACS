#<i>Demo for +print+ method</i>

require "body.rb"

# we have to switch off the +to_s+ method, for this demo to work:

class Body
  def to_s                                # :nodoc:
  end
end

c = Body.new(1, [0.5, 0, 0], [0, 0.7, 0])
#:segment start: demo
print c
print "\n"
#:segment end:
