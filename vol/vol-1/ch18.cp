= Diagnostics

== Energy Conservation

*Bob*: That was quite a long session yesterday, when we were chasing
those bugs while we were trying to implement a better +Body+ class!

*Alice*: Yes, but we learned a lot about Ruby.

*Bob*: And even a thing or two about debugging.

*Alice*: Perhaps this is a good time to return to our real project:
to build progressively better integrators.  But before we move on to
second-order integrators, as well as introducing graphics and other
complications, I would like to do one more test.  Even though we now
know that our forward Euler scheme converges, we don't know yet how
well it conserves energy.  Shouldn't we test that explicitly?

*Bob*: Great minds think alike.  I had come to the same conclusion.
For the two-body problem, of course you can check all kind of things.
You can measure how accurately a bound orbit remains the same ellipse,
as it should be, rather than slowly drifting and changing shape, and
whether the motion traverses the ellipse at the right speed, and so on.
The reason is that we have an analytic solution for the two-body problem.

For the many-body problem in general, however, we are a lot less
lucky.  All we know is that the total energy and total angular
momentum of the system are conserved.  That gives us one scalar and
one vector, or in three dimensions it gives us four conserved
quantities.  So we cannot compare all the details of the motion of the
<tex>$N$</tex> particles with theory, not by a long shot.  An
<tex>$N$</tex>-body system in three dimensions has <tex>$6N$</tex>
degrees of freedom, far more than the 4 handles we have.

However, things are not as bad as they may seem.  If you make errors
in your integration, and you always make errors at some level, those
errors will let the simulated orbit drift away from the true orbit in
a random fashion.  It would require a very clever conspiracy for those
errors to work together in such a way as the keep the energy conserved
to a high degree of accuracy, while still introducing much larger errors
elsewhere.  In practice, therefore, checking energy conservation in an
<tex>$N$</tex>-body system has become the standard test to see how
accurate the integration of an <tex>$N$</tex>-body system is.

*Alice*: Another way of saying this is to picture the phase space of
the whole system.

*Bob*: You mean the six-dimensional space in which you plot for each 




If you picture the orbit of 





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
