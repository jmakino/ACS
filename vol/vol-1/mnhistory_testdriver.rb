require "mnhistory.rb"

h = History.new(2)
h.set_last_rndot(0,3)
h.set_last_rndot(1,1)
h.set_last_rndot(2,1)
h.extend(3)
p h
print "\n"
print h.rndot(0,0.99), "\n"
print h.rndot(0,1), "\n"
print h.rndot(0,1.01), "\n"
print h.rndot(0,1.99), "\n"
print h.rndot(0,2), "\n"
print h.rndot(0,2.001), "\n"
print h.rndot(0,2.99999), "\n"
print h.rndot(0,3), "\n"
print h.rndot(0,3.01), "\n"
print "\n"
print h.rndot(1,0.99), "\n"
print h.rndot(1,1), "\n"
print h.rndot(1,1.01), "\n"
print h.rndot(1,1.99), "\n"
print h.rndot(1,2), "\n"
print h.rndot(1,2.001), "\n"
print h.rndot(1,2.99999), "\n"
print h.rndot(1,3), "\n"
print h.rndot(1,3.01), "\n"
print "\n"
print h.rndot(2,0.99), "\n"
print h.rndot(2,1), "\n"
print h.rndot(2,1.01), "\n"
print h.rndot(2,1.99), "\n"
print h.rndot(2,2), "\n"
print h.rndot(2,2.001), "\n"
print h.rndot(2,2.99999), "\n"
print h.rndot(2,3), "\n"
print h.rndot(2,3.01), "\n"
