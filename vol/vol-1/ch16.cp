= A Fourth-Order Integrator

== Coefficients

*Bob*: Now it is really time to show off my fourth-order integrator.

*Alice*: Can you show me again the code?

*Bob*: Here it is:

 :inccode:.rkbody.rb+rk4

*Alice*: That's quite a bit more complicated.  I see coefficients of
1/6 and 1/3 and 2/3.  How did you determine those?

*Bob*: I just looked them up from a book.  There are several ways to
choose those coefficients.  Runge-Kutta formulas form a whole family,
and the higher the order, the more choices there are for the
coefficients.  In addition, there are some extra simplifications that
can be made in the case of a second order differential equation where
there is no dependence on the first derivative.

*Alice*: As in the case of Newton's equation of motion, where the
acceleration is dependent on the position, but not on the velocity.

*Bob*: Exactly.  Here are the equations.  They can be found in that
famous compendium of equations and tables and graphs: Handbook of
Mathematical Functions, by M. Abramowitz and I. A. Stegun,
eds. [Dover, 1965].  You can even find the pages on the web now.
Here is what I copied from a web page:

link:rk4.gif

*Alice*: Quite a bit more complex than the second order one.

*Bob*: I think I'll give this as an exercise to my students.  After
they have followed our detailed discussion of how to construct the
various second-order integrators, perhaps they can figure out how to
code up these equations from Abramowitz and Stegun.

*Alice*: Good idea.  Once they have learned how to translate these
formulas into Ruby code, they can go to any book on numerical methods,
and try other integration schemes as well.  Meanwhile, let's have a
look at how well your fourth-order scheme performs in practice.

== Comparing the Four Integrators

*Bob*: Let us start again with a time step of 0.001, for a duration
of ten time units.

*Alice*: Just to compare, why don't you run them for all four schemes.

*Bob*: Okay: forward Euler, leapfrog, 2nd order R-K, 4th order R-K:

 :command: cp -f integrator_driver2d.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

 :command: cp -f integrator_driver2e.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

 :command: cp -f integrator_driver2f.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

 :command: cp -f integrator_driver2g.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Alice*: What a difference!  Not only do we have much better energy
conservation, on the order of a few times <tex>$10^{-9}$</tex>,
also the positional accuracy is already on the level of a few times
<tex>$10^{-8}$</tex>, when we compare the output with our most
accurate previous runs.  And all that with hardly any waiting time.

*Bob*: Yes, I'm really glad I threw that integrator into the menu.

== Fourth Order Runge-Kutta

*Alice*: Let's make the time step ten times smaller:

 :command: cp -f integrator_driver2h.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Bob*: What a charm!  Essentially machine accuracy.  This makes it
pretty obvious that fourth order integrators win out hands down, in
problems like this.  Higher order integrators may be even faster, but
that is something we can explore later.

*Alice*: Well done.  And now it is time to get some graphics up and
running, so that we can see what is going on with those particles!
