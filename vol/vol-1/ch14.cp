= Integrators on the Menu

== The Leapfrog Method

*Bob*: It will be easy to show you what I have done, in order to
implement the leapfrog.  My first attempt was to code it up in the
same way as we did for the forward Euler scheme.  Remember how we
did that?  In order to take one forward Euler step, we used the
following method:

   def evolve_step(dt)
     r2 = @pos*@pos
     r3 = r2*sqrt(r2)
     acc = @pos*(-@mass/r3)
     @pos += @vel*dt
     @vel += acc*dt
   end

Switching to the leapfrog, I wrote:

   def evolve_step(dt)
     r2 = @pos*@pos
     r3 = r2*sqrt(r2)
     acc = @pos*(-@mass/r3)
     @vel += acc*0.5*dt
     @pos += @vel*dt
     r2 = @pos*@pos
     r3 = r2*sqrt(r2)
     acc = @pos*(-@mass/r3)
     @vel += acc*0.5*dt
   end

*Alice*: I presume you used the second set of equations which you
wrote before:

<tex>\begin{eqnarray}
{\bf r}_{i+1} & = & {\bf r}_i + {\bf v}_{i} dt + 
{\bf a}_{i} (dt)^2/2                                  \nonumber \\
{\bf v}_{i+1} & = & {\bf v}_i + ({\bf a}_i +
{\bf a}_{i+1})dt / 2                                  \nonumber
\end{eqnarray}</tex>

*Bob*: Yes, they are much more convenient; the first set defined the
velocities at half-step times, but here everything is defined at the
same time: positions, velocities, and accelerations.

*Alice*: Why did you not start by implementing the first equation first?

*Bob*: If I would step forward the position first, I would not be able
to obtain the acceleration <tex>${\bf a}_i$</tex> that is needed for the
second equation, since the acceleration at time _i_ can only be obtained
from the position at time _i_.  Therefore, the first thing to do is to
obtain <tex>${\bf a}_i$</tex>.  Only then can I step forward the position,
in order to calculate <tex>${\bf a}_{i+1}$</tex>.  However, before doing
that, I'd better use the old acceleration <tex>${\bf a}_i$</tex>, since
another acceleration call that would calculate <tex>${\bf a}_{i+1}$</tex>
would overwrite <tex>${\bf a}_i$</tex>.

== Two Different Versions

*Alice*: Can you summarize that in a list of steps?

*Bob*: Here is my recipe:

- calculate <tex>${\bf a}_i$</tex>

- use it to step the velocity forward by half a time step

- step the position forward

- this allows you to calculate <tex>${\bf a}_{i+1}$</tex>

- use that to step the velocity forward by another half step.

So the `leap' part of the frog shows itself in this half step
business for the velocity.

*Alice*: Ah, I see now.  You really had to add 
<tex>${\bf v}_{i} dt + {\bf a}_{i} (dt)^2/2$</tex> to the position
in the third of your five steps.  But since you had already updated
the velocity from <tex>${\bf v}_i$</tex> to effectively 
<tex>${\bf v}_{i+1/2} = {\bf v}_i + {\bf a}_i dt/2$</tex>, you could
abbreviate the position update.  You just added
<tex>${\bf v}_{i+1/2} dt$</tex> to the position instead.  Clever!

The only drawback of writing it in such a compact and smart way is
that the correspondence between the equations is not immediately
clear, when you look at the code.

*Bob*: You could call it FORMula TRANslating with an optimizer.

*Alice*: You may want to write a version where you switch off your
clever optimizer.  Let's see whether we can translate your equations
more directly:

   def evolve_step(dt)
     r2 = @pos*@pos
     r3 = r2*sqrt(r2)
     old_acc = @pos*(-@mass/r3)
     @pos += @vel*dt + old_acc*0.5*dt*dt
     r2 = @pos*@pos
     r3 = r2*sqrt(r2)
     new_acc = @pos*(-@mass/r3)
     @vel += (old_acc + new_acc)*0.5*dt
   end

Hey, it is even one line shorter than what you wrote!

*Bob*: But it introduces an extra variable, so there is a trade off.
Yes, that would be fine as well.  It depends what you find most elegant.

In fact, note that none of these methods are very efficient.  At the
beginning of the next step, we will calculate the old acceleration,
which is the same as what was called the new acceleration in the
previous step.  So while we are stepping along, we do every
acceleration calculation twice, in your version as well as in mine.
I was thinking about avoiding that redundancy, but I realized that
that would make the code less clear.  And as long as we are integrating
only two bodies, there is really no need to optimize at this point.

*Alice*: That reminds me of Alan Perlis' saying: ``premature optimization
is the root of all evil''.

*Bob*: He has a point.  But postponing optimization too long will never
grow fruit.

*Alice*: Did you just make that up?

*Bob*: I just thought about it, as a warning against too much abstraction,
modularity, cleanliness, and all that stands in the way of optimized code.
But for now, as long as we are still working on a toy project, I really
don't mind.

== Tidying Up

*Alice*: You have told us how you wrote your first leapfrog version of
+evolve_step+; and now I can see what you did in the much more condensed
version in your code:

 :inccode:.lbody.rb+leapfrog

Instead of repeating the three line calculation of the acceleration twice
you put in into a method instead:

 :inccode:.lbody.rb+acc

Another nice example of a method without parameters, like the +ekin+
and +epot+ that we saw before.

*Bob*: I know you would like that.  I'm curious, how would your version
of the leapfrog look, in this notation?  You start by introducing variables
+old_acc+ and +new_acc+, so that must then become:

  def leapfrog(dt)
    old_acc = acc
    @pos += @vel*dt + old_acc*0.5*dt*dt
    new_acc = acc
    @vel += (old_acc + new_acc)*0.5*dt
  end

So there: one line longer than my version, and with longer lines to boot.

*Alice*: That is interesting, isn't it?  At first I really prefered my
version, both because it was shorter and because it remained closer to
the mathematical formulae that you had written down.  But I must admit
that I like the way you encapsulated -- dear I say modularized -- the
acceleration into a separate method.  And now suddenly my version looks
a bit clumpsy and lengthy.

*Bob*: But I'm glad we're going over the code so carefully together.
I wouldn't have thought about your way of writing the leapfrog, and
it is nice to try different variations, in order to see which one you
like best.

*Alice*: So I agree: we'll stick with yours.  And your forward Euler now
also looks very short, just one line short of being a one-liner!

 :inccode:.lbody.rb+forward

*Bob*: I suppose I could have put the position and velocity into a vector,
a higher-order vector really, and the velocity and the acceleration as well.
In that way I could have presented you with the type of one-liner you seem
to like so much.

*Alice*: Seriously, when we're going to explore very high-order integrators,
a notation like that might well come in handy.  But not right now.

== Ordering an Integrator

*Bob*: Agreed.

*Alice*: Okay, I like your two integrator methods, while seperating
out the acceleration, but where is the magic that enables you to call
the shots, when you call +evolve+.  How does evolve know that a first
argument of the string <tt>"forward"</tt> directs it to execute the method
+forward+, and similarly that a first argument of the string
<tt>"leapfrog"</tt> directs it to execute the method +leapfrog+?

*Bob*: That happens through the +send+ method.

*Alice*: Another piece of Ruby magic, I take it.  But how does it work?
It seems too easy, though.  You send the integration method on its way?

*Bob*: +send+ is a method that comes with each object in Ruby.  It is
one of the general methods that each object inherits as soon as it is
created.  What +send+ does is to call another method.  +send+ first
reads the name of the other method from the string in its first argument,
and then it passes its remaining arguments to the other method.  So in
our case <tt>send("leapfrog", dt)</tt>, for example amounts to the
same as giving the command <tt>leapfrog(dt)</tt> directly.

*Alice*: That's really nice.  Clearly, the designer of Ruby had a very
good sense of what is needed to built a truly flexible language.  What
is his name?

*Bob*: Matsumoto.  I don't know much about him, but yes, he clearly
knows how to create a clean yet powerful language.

*Alice*: I'd like to meet him someday.  Does he live in Japan?

*Bob*: He does.

*Alice*: Well, there are many astronomy conferences in Japan, so I hope
I'll get a chance, some day.  And if we really get to build a nice toy
model in Ruby, he may enjoy playing with stars.

*Bob*: Now that you see he I have extended our +Body+ class with a menu
of integrators, would you like to see it all in action?

*Alice*: Sure.  Let's try the hard case again, for ten time units.  That
seemed to take forever with the forward Euler method.  Let's try it for
a few time steps, both with forward Euler and with leapfrog.

== A Comparison test

*Bob*: Okay.  Let me first repeat forward Euler, for a couple choices.
This was the smallest time step for which we did not get an explosion:

 :inccode: .integrator_driver1a.rb-barebones

 :command: cp -f integrator_driver1a.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

And here is the same run with a ten times smaller time step.

 :command: cp -f integrator_driver1b.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

As expected for a first order method, we got a ten times smaller error.
Now let us try the leapfrog method:

 :inccode: .integrator_driver1c.rb-barebones

 :command: cp -f integrator_driver1c.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Alice*: Ah, already _much_ better.  What will happen with a ten times
smaller time step?

 :command: cp -f integrator_driver1d.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Bob*: A hundred times smaller error, as is appropriate for a second order
method.  What a relief, after this slooooow convergence of forward Euler!
Clearly leapfrog is a much much better integrator.

*Alice*: Yes, to get such a high accuracy would have taken forever with
a forward Euler integrator.  Congratulations, Bob!

*Bob*: Thanks, but don't get too excited yet: note that the error in the
position is quite a bit larger than the error in the energy.  The difference
between the positions in the last two runs is of the order of
<tex>$10^{-4}$</tex>, while the next-to-last run already had an energy
accuracy of a few time <tex>$10^{-7}$</tex>.

*Alice*: This must be an example of what we talked about, when we first
discussed the limits of using energy conservation as a check.  The leapfrog
is an example of a symplectic integration scheme, one that is build to
conserve energy.  Clearly it conserves energy better than position.

*Bob*: But since it is a second order scheme, also the accuracy of the
position should increase by a factor hundred when we decrease the time
step by a factor ten.  Let's try and see.

 :command: cp -f integrator_driver1e.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Alice*: You are right.  The difference between the _x_ component of the
position in the last two runs is <tex>$2.10^{-6}$</tex>, while the
difference between the previous run and the run before that was
<tex>$2.10^{-4}$</tex>.  So the scheme really is second-order accurate.
Still, it would be nice to try another second order integrator, one
that does not have built-in energy conservation.

*Bob*: I'll try to see what I can do, this evening.  And who knows,
I might even try my hand at a fourth-order integrator.  But that will
be it; we can't go on testing the two-body problem forever.  We have
other work to do: finding a reasonable graphics package, for one thing.

*Alice*: Definitely.  Okay, see you tomorrow!

