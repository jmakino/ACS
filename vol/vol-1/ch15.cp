= Orbit Integration

== Forward

*Bob*: Let's test it:

 :command: cp -f euler1.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Bob*: ten times smaller step size:

 :inccode: .euler3.rb-barebones

and run it:

 :command: cp -f euler3.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Bob*: ten times smaller step size:

 :inccode: .euler4.rb-barebones

and run it:

 :command: cp -f euler4.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Bob*: ten times smaller step size:

 :inccode: .euler5.rb-barebones

and run it:

 :command: cp -f euler5.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

#*Bob*: ten times smaller step size:
#
# :inccode: .euler6.rb-barebones
#
#and run it:
#
# :command: cp -f euler6.rb test.rb
# :commandoutput: ruby test.rb < euler.in
# :command: rm -f test.rb
#
#*Bob*: ten times smaller step size:
#
# :inccode: .euler7.rb-barebones
#
#and run it:
#
# :command: cp -f euler7.rb test.rb
# :commandoutput: ruby test.rb < euler.in
# :command: rm -f test.rb

*Bob*: Here is a fancy version, with command line arguments
and energy error diagnostics:

 :inccode: euler.rb

*Bob*: And here is how you run it

 :commandoutput: ruby euler.rb < euler.in > /dev/null

 :commandoutput: ruby euler.rb -o 10 -d 0.0001< euler.in

 :commandoutput: ruby euler.rb -o 10 -d 0.00001 < euler.in
