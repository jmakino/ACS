require 'testclass'

# ----- Object creation -----

include Testclass
print "Creating some objects:\n"
c = Shape.new(2,3)
p c.x
c.move(1,2)
p c.x
