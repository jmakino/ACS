= Testing Forward Euler

== Starting Again

*Bob*: Time to run the driver for our new integrator, which will give
us the energy conservation diagnostics we have been talking about for
so long now!  Let us start with the same variables we used for our
very first forward Euler run:

 :inccode: .euler0a.rb-barebones

We concluded that the position was probably right to within a percent
or so, from comparing it with a more accurate integration.  I'm
curious whether the energy check gives us a similar answer.

 :command: cp -f euler0a.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Alice*: Did you set this up?  A relative energy accuracy of
<tex>$0.94\%$</tex>, very close!

*Bob*: That was just pure luck, my argument was only an order of
magnitude estimate.

== Linear Scaling

*Alice*: As we argued, a first-order method like the forward Euler
should converge linearly: making the step size ten times smaller
should decrease the error size also by roughly a factor of ten.
Shall we try?

 :inccode: .euler0b.rb-barebones

*Bob*: So we expect an error of <tex>$0.1\%$</tex> now.  Let's see.

 :command: cp -f euler0b.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Alice*: Almost too good: the relative error is now <tex>$0.094\%$</tex>.
It has decreased by the predicted factor ten.  Let's do this:

 :inccode: .euler0c.rb-barebones

making the time step smaller by a jump of a factor a hundred.

 :command: cp -f euler0c.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Bob*: And the error comes down by yet another factor ten.  Note that the
integrator halts after exactly 100,000 steps, not one more and not one
less.  Yesterday evening, when I tried it out, it kept overshooting to
100,001 steps, so that was why I added that extra half step in the halting
criteria.

== A Full Orbit

*Alice*: Let's follow your earlier suggestion to do a longer integration,
for ten time units.  And let's use your flexible output options.  I would
like to see four energy diagnostics, but only one final position and
velocity output, so that we don't get too distracted.

*Bob*: That means:

 :inccode: .euler0d.rb-barebones

I have given +dt_out+ a value larger than the total run time +dt_end+,
so we won't get the regular particle poutput, only the diagnostics.

 :command: cp -f euler0d.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Alice*: That's funny, a large energy error right away and then the energy
seems to be frozen in.

*Bob*: Ah yes, this was the run where the system exploded, with the
relative distance increasing to a value of about ten.

== xxx

*Alice*: And look, the velocity vector is almost parallel to the position
vector, and pointing in the same direction.  Clearly, the two bodies are
escaping from each other.

*Bob*: And that explains why the energy error is frozen: there is no longer
much interaction going on between the particles.  The numerical explosion
must have happened at pericenter, as we deduced earlier.  Let's have a
closer look at what happens in the first part:

 :inccode: .euler0e.rb-barebones

A five times shorter run, with five times more frequent diagnostics.

 :command: cp -f euler0e.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Alice*: So the disaster happened between a time of 1 and 1.5.  That must
have been when the first periastron occurred.

== xxx

*Bob*: Let's check:

 :inccode: .euler0f.rb-barebones

I'll ask only for particle output, once every quarter time unit:

 :command: cp -f euler0f.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Alice*: This is sure hard to read.  It is definitely time to get some form
of graphics going.  This must be how people analyzed the first N-body
calculations, in the sixties!

*Bob*: Yes, we'll get to graphics soon.  But let's do some real lab bench
work here.  Ah!  You see, at time 1.25 the separation of the particles is
clearly smaller than at other times.  That must have been the periastron
passage, roughly.  So the period of the orbit must be 2.5, give or take.

== xxx

*Alice*: Let's check!

*Bob*: We can only check when we use smaller stepsizes, since this one
here is numerically exploding.  Let's try this:

 :inccode: .euler0g.rb-barebones

A ten times smaller step size, and both types of output at time 2.5:

 :command: cp -f euler0g.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Alice*: You were right, close to apocenter, the place we started from.
Let's print out the initial conditions for comparison:

 :inccode: euler.in

*Bob*: Close indeed.  But still a larger energy error.  Let's try a ten
times smaller time step.

 :inccode: .euler0h.rb-barebones

 :command: cp -f euler0h.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Alice*: Better!  And a linear behavior, with the energy errors getting
ten times smaller.  Let's shrink the time step by one more factor of ten:

 :inccode: .euler0i.rb-barebones

 :command: cp -f euler0i.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Bob*: Good.  Another factor ten more accurate.  It is converging slowly
but surely.  And we remain close to apocenter.

== xxx

*Alice*: You know, we really should be able to derive the orbital period
analytically.  Let me try to remember my celestial mechanics.  I remember
that there was one equation that had no factors of <tex>$\pi$</tex> or
whatever in it.  Ah yes, <tex>$$G(M_1+M_2)=a^2\omega^3$$</tex>.  Here
<tex>$a$</tex> is the semi-major axis, <tex>$\omega$</tex> is the
angular frequency of the motion, in other words the period of the orbit
is given in terms of <tex>$\omega$</tex> as <tex>$T = 2\pi / \omega$</tex>.

*Bob*: That's a handy formula to remember.  What does that give in our
case? We started with <tex>$G = 1$</tex> and took <tex>$M_1+M_2 = 1$</tex>
but what was our initial value for <tex>$a$</tex> ?

*Alice*: We'll have to reconstruct that.  It must be larger than 0.5, but
not much larger.  At the time of pericenter the particles were much closer
than at apocenter, which means that the eccentricity was fairly large,
and the apocenter distance not much smaller than <tex>$2a$</tex>.

I remember another handy formula: the total energy is a two-body system
is equal to <tex>$$E = -G \frac{M_1M_2}{2a}$$</tex>.  That is easy to
remember, since the virial theorem tells you that the potential energy
is twice as large as the kinetic energy, on average, and therefore the
total energy is half the potential energy, also on average.  Now in a
Kepler orbit, it turns out that the average of <tex>$1/r$</tex>
happens to be <tex>$1/a$</tex>.

*Bob*: I can see that it is useful to remember those qualitative facts.
That is easier than trying to remember factors of 3 or 4 in formulas that
you learn by heart and than later half forget.

*Alice*: Yes, the only numbers I like are 1 and 0 and infinity.  So let
us determine the total energy, and then we now our semi-major axis
<tex>$a$</tex>.

*Bob*: But we have that already: according to the output of my program
it is <tt>E_tot = -0.875</tt>, initially.  In other words,
<tex>$E = 7/8$</tex>.

*Alice*: Inverting my previous equation, we get
<tex>$$a = -G \frac{M_1M_2}{2E}$$</tex>.
With <tex>$G = M_1+M_2 = 1$</tex>, we get
<tex>$$a = - \frac{1}{8E}$$</tex>.  So with your value <tex>$E = 7/8$</tex>,
that gives us <tex>$a = 1/7$</tex>.  But hey, that can't be right.  It
should be larger than 0.5, since the maximum distance between two particles
in _any_ Kepler orbit is <tex>$2a$</tex>, and we started at a distance of
unity!

*Bob*: That is puzzling.  But you just stepped through my code.  You
were so happy with the clarity of the statements like
<tt>etot = ekin + epot</tt>.

*Alice*: Let's do an independent check.  This is like debugging, but
now on the level of the physics, rather than the numerics.  Let me just
pen and paper to determine the initial total energy.  Here are the
equations:
<tex>$$
E = E_{kin} + E{_pot} = {\textstyle\frac{1}{2}} M_1v_1^2 +
{\textstyle\frac{1}{2}} M_1v_1^2 - G\frac{M_1M_2}{r}
$$</tex>
with <tex>$G = 1$</tex> and <tex>$M_1 = M_2 = 0.5$</tex>, right?

*Bob*: Right.  And these are velocities in the center of mass frame
of the two particles.  These are equal in magnitude but opposite in
direction, so each one is one half of the relative velocity.  The
original relative velocity was 1/2, so each of the two is 1/4, and
<tex>$v_1^2 = v_1^2 = 1/16$</tex>.  Let's do it very carefully, to
make sure we don't drop some factor somewhere.  We have now for the
initial total energy:
<tex>$$
E = {\textstyle\frac{1}{2}}{\textstyle\frac{1}{2}}{\textstyle\frac{1}{16}}
+ {\textstyle\frac{1}{2}}{\textstyle\frac{1}{2}}{\textstyle\frac{1}{16}}
- \frac{{\textstyle\frac{1}{2}}{\textstyle\frac{1}{2}}}{1} =
{\textstyle\frac{7}{32}}
$$</tex>

*Alice*: That is exactly four times smaller than the value that your
program gave us.  And it would imply a semi-major axis of
<tex>$$
a = - \frac{1}{8E} = \frac{4}{7}
$$</tex>
Now that is a much more reasonable number!  Look:

    |gravity> bc -lq
    .57142857142857142857

Just as I had predicted: larger than 0.5, but not much larger.  This
has to be correct.  We did it for first principles, little step for
little step, and the result is just what was expected.

== xxx

*Bob*: If this is right, then the question is what went wrong with my
program?  I agree that a value of <tex>$a = 1/7$</tex> is unphysical.
But like I said, you just checked with me every statement in the code!

*Alice*: Well, your calculation can't be all wrong.  You had the factor
7 in the denominator, that's unlikely to come out correctly by chance.
You were off by a factor 4.  I think there must be something wrong with
your units.

*Bob*: My units???  You saw as well as I did that I used <tex>$G = 1$</tex>,
and there were no other scaling units involved.  We gave each particle
a mass of 0.5, with a total mass of 1.0, which went into the +Body+
description for the relative motion of the two particles.

Hey, wait a minute.  We use a mass of 1 for our _relative_ particle
and a mass of 0.5 for each _individual_ particle.  There is a factor
two between them, and two times two makes four.

*Alice*: Indeed, the factor four that your program was off with.
And you found the solution!

You have been computing your energies
per unit of reduced mass, I bet, and not in absolute units!

.... 



























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
