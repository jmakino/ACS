= Diagnostics

== Energy Conservation

*Bob*: Here is a fancy version, with command line arguments
and energy error diagnostics:

 :inccode: euler.rb

*Bob*: And here is how you run it

 :commandoutput: ruby euler.rb < euler.in > /dev/null

 :commandoutput: ruby euler.rb -o 10 -d 0.0001< euler.in

 :commandoutput: ruby euler.rb -o 10 -d 0.00001 < euler.in
