= Orbit Integration

== Forward

*Bob*: Time to test our Forward Euler code:

 :command: cp -f euler1.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Alice*: That looks reasonable.  We started with a relative velocity
of 0.5, so in one time unit of integration, you would expect the
position vector to have changed by something like half a length unit.
We started with position {1.00, 0.00} and we now have {0.44, 0.38}.
So we have crossed a distance vector of {-0.56, 0.38}, which has a
length of <tex>$\sqrt{0.56^2 + 0.38^2} = 0.68$</tex>, not far from one
half.

*Bob*: And look, the velocity has increased.  It started off with
vector length unity, and now it is about 1.3 length.  Since we started
with a velocity direction perpendicular to the line connecting both
particles, we were either at pericenter or apocenter.

*Alice*: You'll have to explain that to the students.

*Bob*: Pericenter is the point in a Kepler orbit where the distance
between two particles is smaller than anywhere else in that orbit.
The apocenter is reached when the two particles are furthest away
from each other.  From the Greek _peri_ or near, and _apo_ or away.
Now if you are closest, you move fastest, since the gravitational
force is larger at smaller distances.  Similarly, when you are at
apocenter, you move slowest.  Moving away from pericenter, you slow
down, while moving away from apocenter you speed up.  Ergo: we
obviously started the particles at the maximum distance, given their
energy and angular momentum, specified at the start.

But all this will makes much more sense to the students when we show
them the orbit with some real graphics.  We'll come to that soon.  For
now, let's make sure that the integrator does what it is supposed to do.
Let me make the step size ten times smaller, to see whether we get
roughly the same answer.  I will make the number of time steps ten
times larger, to make sure the integration time stays the same:

 :inccode: .euler3.rb-barebones

And then we run it:

 :command: cp -f euler3.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Alice*: Not bad, for such a simple integrator.  The differences with
the previous run are typically only one unit in the second decimal
place, in each vector component.

*Bob*: About all that I would have expected from such a simple integrator.

== Error Scaling

How do we expect things to scale
when we will make the time step even smaller?  We are dealing with a
first order integrator.  How did that go?

*Bob*: A first order integrator is first-order accurate. which means
that the errors per time step are quadratic in the size of the time
step.  Making the time step ten times as small will give you errors
that are each one hundred times as small.  However, you have ten times
more steps, so if the errors are systematic, as they often are, the
total error is ten times larger than a single error per time step.
In other words, making the time step ten times smaller will make the
total error will go down also by a factor of ten.

Or so I think.  Let me put in the values

 :inccode: .euler4.rb-barebones

and run it:

 :command: cp -f euler4.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Alice*: How nice.  The differences between the last two runs are
now mostly one unit in the third decimal place.  Just as you predicted!

*Bob*: Let me push my luck with another factor of ten

 :inccode: .euler5.rb-barebones

and see whether the trend continues:

 :command: cp -f euler5.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

It does.  Differences of one unit in the fourth decimal place.  And
most of the differences in comparing the last two runs must come from
the much less accurate run before the last one.  So the last one can
be expected to be accurate to about one unit in the fifth decimal
place.  So it would seem safe to say that the first component of the
relative position will be <tex>$0.43187 \pm 0.00001$</tex>.  I think
we have shown that we can solve the 2-body problem with Ruby!

== A Full Orbit

*Alice*: Before we declare victory, I'd like to see us integrating at
least a full orbit.  How about increasing the time from 1 to 10 time
units?  That should be more than enough.

<i>[More to be added here soon -- Piet]</i>


