= An Alternative Version

== A Figure-8 Triple

*Bob*: Now that we're pretty sure that we can integrate the two-body
problem with our new code, how about trying our hand at a three-body
system?  After all, that was the reason I wrote this program, to go
beyond two bodies.

*Alice*: We will have to choose some initial conditions.  Rather than picking
something at random, how about trying to integrate a figure-eight
configuration?

*Bob*: You mean this new `classical' solution to the equal-mass three-body
system that was discovered in the nineteen nineties?  I remember how surprised
I was when I first read about that.  I had always assumed that the
classic celestial mechanicians had found all the interesting solutions
already centuries ago, you know, Legendre, Lagrange, Laplace, and
perhaps a few others Le's and La's.  I was thrilled to see such an elegant
new solution.

*Alice*: Me too.  As soon as I read about it, I tried it out for myself,
and by golly, the configuration was stable: three stars chasing each other
on a figure eight, without the system falling apart.  You could even perturb
the initial conditions by a fraction of a percent, and preserve stability.

*Bob*: This all goes to show that we really should implement some form of
graphics soon.  I'd love to see how the new code will handle that
configuration.

*Alice*: I agree.  But at least for now, we can use the figure-eight triple
to test the code.  I stored the initial conditions somewhere.  Ah,
here they are.  I'll put them in a file named <tt>figure8.in</tt>:

 :inccode: figure8.in

The third particle starts in in the center of the figure eight, in the
exact center of the coordinate system.  The other two bodies move
symmetrically with respect to each other, each one third of a period
displaced.  Ah, my notes tell me that I found the total period of
revolution to be about 6.3264 time units.  Let us integrate our system
for 1/3 of that time: since all particles have the same mass, after 1/3
of the period they should have changed places.  That means an
integration for a total of 2.1088 time units.

== Switching Places

*Bob*: Let's predict what will happen.  The third particle has velocity
components that are negative, both in the _x_ and _y_ direction.  This
means that it moves to the left and downwards.  That would suggest that
it will in due time reach the position of the second particle, the
particle that starts off with a large negative _x_ value.  The second
particle then has no choice but to replace the first particle, and the
first one will in turn wind up in the center.  So I predict that an
integration of 2.1088 time units will produce the following output:

 3
 0
 1
  0        0
 -0.93241 -0.86473
 1
 0.97004  -0.24309
 0.46620   0.43237
 1
 -0.97004  0.24309
  0.46620  0.43237

I have only given five digits, since you gave the total time duration
to that accuracy, and therefore there is no guarantee that we will halt
the calculation exactly after 1/3 of an orbit; in fact we are bound to
either overshoot or undershoot, making an error in the fifth or sixth
significant digit in all coordinates.

*Alice*: That seems reasonable.  This will be a nice test.  Let's try it!

 :commandoutput: ruby rknbody2b_driver.rb < figure8.in

*Bob*: Not quite as good as I had hoped, but I certainly came close.

*Alice*: I'd say!  And you were right about the order of the particles.
The first particle did indeed reach very close to the center, and all
other particles reached your predicted position to within a small fraction
of a percent: in all cases the first three digits are correct.

*Bob*: I would have expected the first four digits to be correct, at least.

*Alice*: Not necessarily.  Don't forget that, although our system is stable,
it is only marginally so.  Therefore, a small error can lead to a relatively
large displacement, at least larger than you might have guessed.  I'm very
pleased with this performance.  And the energy error is almost zero,
basically to within machine accuracy.  Congratulations!

== Single Links in +Body+

*Bob*: Before we move on, let us revisit the choice I have made of
installing double links between the particles in an N-body system and
their parents.  In the +Nbody+ class I have an array of bodies
<tt>@body</tt> that gives access to the members of the +Body+ class,
while in the +Body+ class there is an instance variable <tt>@nb</tt>
that gives each particle access to its parent.

You were not happy with that choice, and I must say, I can see the
danger of modifying, say, the downward link and forgetting to make a
corresponding modification in the upward link.

*Alice*: I'm glad you remembered that.  Yes, let's play with some
alternatives.  The simplest way to do away with the need for a
backward pointer <tt>@nb</tt> would be to give the +Body+ method +acc+
an extra parameter.  Let me take out the declaration of <tt>@nb</tt>
in the <tt>attr_accessor</tt> of the +Body+ class, and let me replace
your +Body+ method

 :inccode: .rknbody2.rb+acc

by a new version, that takes the array of bodies as an explicit parameter:

 :inccode: .rknbody3.rb+acc

Of course, we have to do the same thing for the potential energy calculation,
where we replace

 :inccode: .rknbody2.rb+epot

by the equivalent version:

 :inccode: .rknbody3.rb+epot

== Single Links in +Nbody+

*Bob*: Okay, and let us put the code into a separate file <tt>rknbody3.rb</tt>.
Life gets a bit simpler for the +Nbody+ code: when it creates its daughters,
it no longer has to tell them who their parent is.  Instead of the old version:

 :inccode: .rknbody2.rb+initialize

we now will use the simpler and more natural version:

 :inccode: .rknbody3.rb+initialize

And similarly we can leave out the corresponding line in the
<tt>simple_read</tt> method: instead of 

 :inccode: .rknbody2.rb+simple_read

we now have only:

 :inccode: .rknbody3.rb+simple_read

*Alice*: Good!  I was a bit worried about that repetition.  It would have
been an easy mistake to make to modify the <tt>initialize</tt> method
in one way, and to either forget to modify the <tt>simple_read</tt> method
or worse, modify it in a different way.

*Bob*: Next we have to change the way in which +acc+ gets called from
within +Nbody+.  Let us start with forward Euler, which I had written
as:

 :inccode: .rknbody2.rb+forward

The change is quite minimal: there is only the extra argument in +acc+
and everything else remains the same:

 :inccode: .rknbody3.rb+forward

But I must admit, I don't particularly like to make that one line longer,
especially since it breaks the nice symmetry between +pos+ and +vel+
on the one hand, and +acc+ on the other hand.

*Alice*: But that symmetry is only superficial, and in fact quite dangerous:
+pos+ and +vel+ are actual variables whereas +acc+ is a method.  If
you do not change +pos+ explicitly, you can count on it to keep its
old values.  However, the same is not true for +acc+.  You can call +acc+
and then when you chang +pos+ and call +acc+ again, you get a different
value.  In fact, as soon as you change the position +pos+ for only one
particle, a call to +acc+ for _any_ particle will be affected!

It is much better to warn the user of this side effect, by making
it clear that there is a hidden dependency: and the best way to show this
dependency is by expressing it in the form of an argument to the method
+acc+.  In that case the dependency is no longer hidden, and the user
is warned of the fact that +acc+ depends on the states of all the bodies.

*Bob*: I see your point, but I still like the symmetry of my original
notation.  In any case, I agree that I should have commented the hidden
dependency, at the very least in the form of an explicit comment.
Meanwhile, let me make the same changes for any call to +acc+ in any
of the other integrators.

*Alice*: And don't forget to make the change in +epot+ as well.

*Bob*: Ah yes, I had already forgotten about that one.  In any case, the
interpreter would have complained about a wrong number of arguments in
+epot+, but let me modify +epot+ as well.

== Two Tests

*Alice*: Let's now do the same tests, for our generic two-body problem, and our
figure-8 three-body problem.

*Bob*: Okay, here is the two-body problem that we started our testing cycle
with, but now with the new code:

 :commandoutput: ruby rknbody3a_driver.rb < test1.in

*Alice*: Good!  The same results as for the previous version of the code.

*Bob*: And here is the new figure-8 result:

 :commandoutput: ruby rknbody3b_driver.rb < figure8.in

*Alice*: And that's the same as well.  It seems that you have correctly
transformed the code from doubly-linked to singly-linked.
