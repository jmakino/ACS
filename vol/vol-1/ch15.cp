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

*Alice*: It does.  Differences of one unit in the fourth decimal place.
And most of the differences in comparing the last two runs must come
from the much less accurate run before the last one.  So the last one
can be expected to be accurate to about one unit in the fifth decimal
place.  So it would seem safe to say that the first component of the
relative position will be <tex>$0.43187 \pm 0.00001$</tex>.  I think
we have shown that we can solve the 2-body problem with Ruby!

== A Vector Class

*Bob*: Now that we can integrate, let me come back to these lines in
the code that you were not so happy with, containing a +k+ for the
components, remember?

*Alice*: Yes, do you have a way of avoiding any component notation,
even on the code level?

*Bob*: I think I do.  In a way, it is pure syntactic sugar, but then
again, you may say that a compiler or intepretar is one giant heap of
syntactic sugar on top of the machine language.  If it makes the code
much easier to read, I'm happy with it.

The trick must be to make sure that you can add vectors the way you
expect to add them in physics.

*Alice*: But according to the principle of least surprise, I would have
guessed that Ruby would be able to add two vectors, in array notation,
at least if they contain numerical entrees, no?

*Bob*: One person's expectation is another person's surprise!  Not
everyone approaches arrays with your physicist's view of the world.
Here, this is a good question for which to go back to +irb+.

    |gravity> irb --prompt short_prompt
    001:0> a = [1, 2]
    [1, 2]
    002:0> b = [3, 3]
    [3, 3]
    003:0> a + b
    [1, 2, 3, 3]

*Alice*: Hey, that's not fair!  Making a four-dimensional vector out
of two two-dimensional ones, how did that happen??

*Bob*: Ah, but see, if you were a computer scientist, or anyone not
used to working frequently with vectors, what could it possibly mean
to add two arrays?  You have an order collections of components, and
another such collection.  The components can be apples and oranges.

*Alice*: Or cats.

*Bob*: Yes.  Or numbers.  And the most sensible way to add those
collections is to make one large ordered collection, in the order
in which they were added.  Note that there is something interesting
hore for a mathematician: addition suddenly has become noncommutative.

*Alice*: I see what you mean.  Least surprise for most people.  How was
that again, about striving for the largest happiness for the largest
amount of people?  But still, I sure would like to have my vector addition.

*Bob*: Now the good news, in Ruby, is: you can, and rather easily!  All
you need to do is to introduce a new class, let us call it +Vector+,
and implement it as an array for which the addition operator <tt>+</tt>
does what you want it to do.  Let's try it right away.  I will put the 
definition of the +Vector+ class in the file <tt>vector.h</tt>

 :inccode: vector.rb

== Debugging

Before explaining what all this means, let us first see whether it works.

    |gravity> irb --prompt short_prompt -r vector.rb
    001:0> a = Vector[1, 2, 3]
    [1, 2, 3]
    002:0> b = Vector[0.1, 0.01, 0.001]
    [0.1, 0.01, 0.001]
    003:0> a += b
    [1.1, 2.01, 3.001]
    004:0> p a
    [1.1, 2.01, 3.001]
    nil
    005:0> a = b*2
    [0.2, 0.02, 0.002]
    006:0> a*b
    TypeError: cannot convert Vector into Integer
    	from (irb):6:in `*'
    	from (irb):6

*Alice*: Ah, what a pity.  We had vector addition, and also scalar
multiplication of a vector, but the inner product of two vectors
didn't work.

*Bob*: That is puzzling.  Hmmm.  Cannot convert Vector into Integer.
And all that somewhere in the definition of *.  





And it does!  So now we can rewrite our integrator, in file <tt>test.rb</tt>:

 :inccode: euler2.rb



==

*Bob*: Here is a fancy version, with command line arguments
and energy error diagnostics:

 :inccode: euler.rb

*Bob*: And here is how you run it

 :commandoutput: ruby euler.rb < euler.in > /dev/null

 :commandoutput: ruby euler.rb -o 10 -d 0.0001< euler.in

 :commandoutput: ruby euler.rb -o 10 -d 0.00001 < euler.in
