= Running the N-body Code

== A 2-Body System

*Alice*: I would like to see the N-body program running for a 2-body system
first, just to check whether we really get the same results.

*Bob*: That's a good idea.  I have tested it so far with a 3-body system,
with some randomly chosen initial conditions, but I agree that it would be
good to test the code from the ground up.  Shall we try to reproduce the
same Kepler orbit that we integrated using our <tt>euler.in</tt> initial
conditions?  They were

 :include: euler.in

*Alice*: Yes, but now we have to be careful how we interpret this mass
of 1 that we used before.  Remember how we introduced the two-body
problem, using the relative position <tex>$\br$</tex> between the two
pairs?  The equation of motion for <tex>$\br$</tex> was

:equation:
\frac{d^2}{dt^2}{\bf r} = - G \frac{M_1 + M_2}{r^3}{\bf r}

So all we know is that the sum of the masses is unity.  We can divide
this over the two particles in any way we like.  We could take them to
be of equal mass, in which case <tex>$M_1 = M_2 = \frac{1}{2}$</tex>.
However, I would prefer unequal masses, just to avoid degenerate
situations where our test may fail to uncover some subtle bug.

*Bob*: Good idea.  We saw already how using a mass of unity could fail
to show an error in mass assignment.  The more asymmetric and non-default
our choice is, the better.  It would be good, though, to calculate the
orbits in the center-of-mass frame, otherwise the results are more
difficult to interpret, when the particles start drifting off, away
from the origin.  How about this choice?  I'll put it in a file
<tt>test1.in</tt>:

 :inccode: test1.in

I will first redo the fourth-order Runge-Kutta run for a time step
of <tex>$10^{-4}$</tex>, using our previous two-body code:

 :inccode: .integrator_driver2h.rb

 :commandoutput: ruby integrator_driver2h.rb < euler.in

Now let's see what my new N-body code will do:

 :inccode: .rknbody1a_driver.rb

 :commandoutput: ruby rknbody1a_driver.rb < test1.in

*Alice*: Huh?  An energy conservation error of order unity?  And our old
code was conserving energy almost on machine accuracy!  Are you sure you
have tested your code?

*Bob*: Yes, I'm sure, I can show you.  And yes, I'm deeply puzzled now.

*Alice*: Let's try a very simple situation, where we absolutely know what
the outcome will be, in explicit form.  Let us take a circular binary star
with equal masses, just to see what will go wrong there.  Perhaps that will
give us a hint.  We can give both stars a mass 1, and start with an initial
distance of 1.  This gives us an initial potential of 1.1/1, since we are
working with <tex>$G=1$</tex>, so the initial kinetic energy has to be 1/2,
because of the virial theorem.  Each star thus needs to get a kinetic
energy of 1/4 in the center-of-mass frame, and this means that the velocity
_v_ of each star should be <tex>$1/\sqrt(2)$</tex>, in order to make
<tex>$E_{kin} = \frac{1}{2}mv^2 = 1/4$</tex> for that star.

*Bob*: Here we go.  I will call the inital file for the circular binary
<tt>test2.in</tt>.

 :inccode: test2.in

I'll use the same parameters for the fourth-order Runge-Kutta integrator,
in my N-body code:

 :commandoutput: ruby rknbody1a_driver.rb < test2.in

*Alice*: Congratulations!  You _do_ have a working integrator, at least
for a circular equal-mass binary.  But of course the question remains:
what went wrong with the non-circular non-equal-mass binary?

*Bob*: I'm stumped.  But this is a bug we should be able to track down
without too much trouble.  The last case, which worked, was special in
at least three ways: the orbit was circular, the masses were equal,
and the masses were also all equal to unity.  The case which didn't
work did have none of these three idealizations.  Let's modify each of
those in turn.

*Alice*: It may be easiest to drop the unity of the masses.  If we make
the mass ten times smaller, the potential energy becomes one hundred times
smaller, and so should the kinetic energy of each particle.  Since the mass
is already ten times smaller, we can make the kinetic energy a hundred
times smaller but lowering the velocity by a factor <tex>$1/\sqrt(10)$</tex>.
This means that the new velocity of each particle should become
<tex>$v = (1/\sqrt(2))(1/\sqrt(10))=1/2\sqrt(5)$</tex>.

*Bob*: Here goes, with <tt>test3.in</tt>:

 :inccode: test3.in

I'll use the same parameters for the fourth-order Runge-Kutta integrator,
in my N-body code:

 :commandoutput: ruby rknbody1a_driver.rb < test3.in

*Alice*: Nothing wrong here.  So changing the masses did not help, at least
not for our circular orbit.  Shall we try to increase the eccentricity,
while leaving the masses both unity?  We can just make the velocities a bit
smaller.  How about:

 :inccode: test4.in

 :commandoutput: ruby rknbody1a_driver.rb < test4.in

*Bob*: Still no cigar.  Nothing wrong here either.  How about changing
both the masses and the eccentricity?
