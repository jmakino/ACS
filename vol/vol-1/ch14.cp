= XXX

== xxx

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

This is the reason for the ordering:

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

Okay, I like your two integrator methods, while seperating out the
acceleration, but where is the magic that enables you to call the
shots, when you call +evolve+.  How does evolve know that a first
argument of the string +"forward"+ directs it to execute the method
+forward+, and similarly that a first argument of the string
+"leapfrog"+ directs it to execute the method +leapfrog+?

*Bob*: That happens through the +send+ method.

*Alice*: 













<tex>$$</tex>
<tex>$$</tex>
<tex>$$</tex>
