= Diagnostics

== Energy Conservation

*Bob*: Here is a fancy version, with command line arguments
and energy error diagnostics:

 :inccode: euler.rb

*Bob*: And here is how you run it

 :commandoutput: ruby euler.rb < euler.in > /dev/null

 :commandoutput: ruby euler.rb < euler.in



 . . . . . . . . . . . . 

 . . . . . . . . . . . . 

We can use +irb+ to run it now, changing the options as we want:

  evolve(b, 0.01, 10, 100, 10)

<i>perhaps variable number of arguments?</i>
