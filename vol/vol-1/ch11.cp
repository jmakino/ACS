= Testing Forward Euler

== TODO

*Bob*: Here is a fancy version, with command line arguments
and energy error diagnostics:

 :inccode: euler.rb

*Bob*: And here is how you run it

 :commandoutput: ruby euler.rb < euler.in > /dev/null

 :commandoutput: ruby euler.rb < euler.in



 . . . . . . . . . . . . 

  def epot                     # potential energy per unit of mass
    @ep = -1/sqrt(@pos*@pos)
  end

  def epot                     # potential energy per unit of mass
    r2 = 0
    @pos.each {|p| r2 += p*p}
    r = sqrt(r2)
    @ep = -1/r
  end

 . . . . . . . . . . . . 

  def ekin                     # kinetic energy per unit of mass
    @ek = 0.5 * (@vel * @vel)
  end

not

  def ekin                     # kinetic energy per unit of mass
    @ek = 0.5 * @vel * @vel
  end

Ask for free ordering of Ruby as interpreter option?

 . . . . . . . . . . . . 

 . . . . . . . . . . . . 

 . . . . . . . . . . . . 

We can use +irb+ to run it now, changing the options as we want:

  evolve(b, 0.01, 10, 100, 10)

<i>perhaps variable number of arguments?</i>
