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
and try other integration schemes as well.  But wait a minute, what
you found here, equation 22.5.22, shows a right-hand side with a
stated energy error of order <tex>$h^4$</tex> -- which would suggest
that the scheme is is only third-order accurate.

*Bob*: Hey, that is right.  If you make <tex>$h$</tex> smaller,
the number of steps will go up according to <tex>$1/h$</tex>, and
therefore the total error for a give problem will grow proportionally
to <tex>$h^4.(1/h) \propto h^3$</tex>.  This would indeed imply that
this method is third-order.  But that would be very unusual.  In all
text books I've seen, you mostly come across second-order and
fourth-order Runge Kuttas.  While you might be able to construct a
third-order version, I wouldn't have expected Abramowitz and Stegun to
feature one.

*Alice*: Could it be a misprint?

*Bob*: Anything is possible, though here that is not very likely.  This
is such a famous book, that you would expect the many readers to have
debugged the book thoroughly.

*Alice*: Let's decide for ourselves what is true and what is not.

*Bob*: Yes.  And either way, whatever the outcome, it will be a good
exercise for the students.  Let's first test it numerically.

*Alice*: Fine with me.  But then I want to derive it analytically as well,
to see whether we really can understand the behavior of the numerical
results from first principles.

== Comparing the Four Integrators

*Bob*: Let us start again with a time step of 0.001, for a duration
of ten time units.

*Alice*: Just to compare, why don't you run them for all four schemes.

*Bob*: Okay: forward Euler, leapfrog, 2nd order R-K, 4th order R-K:

 :commandoutput: ruby integrator_driver2d.rb < euler.in

 :commandoutput: ruby integrator_driver2e.rb < euler.in

 :commandoutput: ruby integrator_driver2f.rb < euler.in

 :commandoutput: ruby integrator_driver2g.rb < euler.in

*Alice*: What a difference!  Not only do we have much better energy
conservation, on the order of a few times <tex>$10^{-9}$</tex>,
also the positional accuracy is already on the level of a few times
<tex>$10^{-8}$</tex>, when we compare the output with our most
accurate previous runs.  And all that with hardly any waiting time.

*Bob*: Yes, I'm really glad I threw that integrator into the menu.
The second-order Runge-Kutta doesn't fare as well as the leapfrog, at
least for this problem, even though they are both second-order.  But
the fourth-order Runge-Kutta wins hands down.

== Fourth Order Runge-Kutta

*Alice*: Let's make the time step ten times smaller:

 :commandoutput: ruby integrator_driver2h.rb < euler.in

*Bob*: What a charm!  Essentially machine accuracy.  This makes it
pretty obvious that fourth order integrators win out hands down, in
problems like this.  Higher order integrators may be even faster, but
that is something we can explore later.

*Alice*: Well done.  And, by the way, this does suggest that the scheme
that you copied from that book is indeed fourth-order.  It almost seems
better than fourth order.

*Bob*: Let me try a far larger time step, but a much shorter duration,
so that we don't have to integrate over a complicated orbit.  How
about these two choices:

 :commandoutput: ruby integrator_driver2i.rb < euler.in

 :commandoutput: ruby integrator_driver2j.rb < euler.in

This does it!  Fourth order, almost exactly.

== Snap, Crackle, and Pop

*Alice*: Still I'd like to make sure with pen and paper as well.
Let us take your implementation

 :inccode:.rkbody.rb+rk4

and let us determine analytically what the first five terms in a Taylor
series in +dt+ look like.  We can then see directly whether the error term
is proportionally to <tex>$h^4$</tex>, as the book claims, or <tex>$h^5$</tex>,
as our calculations indicate.

To start with, let us look at your variable <tt>a2</tt>, which is the
acceleration after a time step +dt+, starting from an initial
acceleration <tt>a0</tt>.  A Taylor expansion can approximate
<tt>a2</tt> as:

:equation:
\ba_2 = \ba_0 + {d\ba\over dt} dt
              + {1 \over 2}{d^2\ba\over dt^2} (dt)^2
              + {1 \over 6}{d^3\ba\over dt^3} (dt)^3
              + {1 \over 24}{d^4\ba\over dt^4} (dt)^4

where all the derivatives are understood to be evaluated at the same
time as <tex>$\ba_0$</tex>, namely at the beginning of our time step.

*Bob*: The first derivative of the acceleration, <tex>${d\ba\over dt}$</tex>,
is sometimes called the <i>jerk</i>.  How about the following notation:

:equation:
\bj = \frac{d^3}{dt^3} \br = \frac{d\br}{dt}

*Alice*: Fine with me.  I believe the term `jerk' has crept into the
literature relatively recently, probably originally as a pun.  If a
car or train changes acceleration relatively quickly you experience
not a smoothly accelerating or decelerating motion, but instead a
rather `jerky' one.

*Bob*: It may be more difficult to come up with terms for the unevenness
in the jerk.

*Alice*: Actually, I saw somewhere that someone had used the words
_snap_, _crackle_, and _pop_, to describe the
next three terms.

*Bob*: As in the rice crispies?  Now that will confuse astronomers
who did not grow up in the United States!  If they haven't seen the
rice crispy commercials, they will have no clue why we would use those
names.  And come to think of it, I don't have much of a clue yet either.

:figure: scp.jpg 5cm scp
Snap, Crackle, and Pop

*Alice*: Ah, but the point is that the names of these three cheerful
characters lend themselves quite well to describe more-than-jerky behavior.
After all, the popping of a rice crispy is a rather sudden change of state.

*Bob*: Okay, now I get the drift of the argument.  A sudden snap comes to
mind, as a change in jerk.  And what changes its state more suddenly than
a snap?  Well, perhaps something that crackles, although that is pushing
it a bit.  But a pop is certainly a good word for something that
changes high derivatives of positions in a substantial way!

== An Analytic Proof

*Alice*: Okay, let's make it official:

:eqnarray:
\bs &=& \frac{d^4}{dt^4} \br \quad = \quad \frac{d^2}{dt^2} \ba \nonumber\\
\bc &=& \frac{d^5}{dt^5} \br \quad = \quad \frac{d^3}{dt^3} \ba \nonumber\\
\bp &=& \frac{d^6}{dt^6} \br \quad = \quad \frac{d^4}{dt^4} \ba

Which turns my earlier equation into:

:equation:
\ba_2 = \ba_0 + \bj_0 dt
              + {\textstyle {1 \over 2}}\bs_0 (dt)^2
              + {\textstyle {1 \over 6}}\bc_0 (dt)^3
              + {\textstyle {1 \over 24}}\bp_0 (dt)^4

*Bob*: Much more readable.

*Alice*: And your other variable <tt>a1</tt>, which indicates the acceleration
after only half a time step, now becomes:

:equation:
\ba_1 = \ba_0 + {\textstyle {1 \over 2}}\bj_0 dt
              + {\textstyle {1 \over 8}}\bs_0 (dt)^2
              + {\textstyle {1 \over 48}}\bc_0 (dt)^3
              + {\textstyle {1 \over 384}}\bp_0 (dt)^4

*Bob*: Ah, and now we can evaluate the last two lines in my <tt>rk4</tt>
method:

 :inccode:.rkbody.rb-1

*Alice*: Yes.  These two lines translate to:

:eqnarray:
\br_1 &=& \br_0 + \bv_0 dt
          + {\textstyle {1 \over 6}}(\ba_0+2\ba_1)(dt)^2 \nonumber\\
\bv_1 &=& \bv_0 + {\textstyle {1 \over 6}}(\ba_0+4\ba_1+\ba_2)dt

Upon substitution the first line becomes:

:eqnarray:
\br_1 &=& \br_0 + \bv_0 dt
          + {\textstyle {1 \over 6}}\ba_0(dt)^2
          + {\textstyle {1 \over 3}}\ba_1(dt)^2 \nonumber\\
&=& \br_0 + \bv_0 dt
          + {\textstyle {1 \over 2}}\ba_0(dt)^2
          + {\textstyle {1 \over 6}}\bj_0 (dt)^3
          + {\textstyle {1 \over 24}}\bs_0 (dt)^4
          + {\textstyle {1 \over 144}}\bc_0 (dt)^5
          + O((dt)^6) \nonumber

And the second line becomes:

:eqnarray:
\bv_1 &=& \bv_0
          + {\textstyle {1 \over 6}}\ba_0dt
          + {\textstyle {2 \over 3}}\ba_1dt
          + {\textstyle {1 \over 6}}\ba_2dt \nonumber\\
&=& \bv_0 + \ba_0dt
          + {\textstyle {1 \over 2}}\bj_0 (dt)^2
          + {\textstyle {1 \over 6}}\bs_0 (dt)^3
          + {\textstyle {1 \over 24}}\bc_0 (dt)^4
          + {\textstyle {5 \over 576}}\bp_0 (dt)^5
          + O((dt)^6) \nonumber

*Bob*: Very nice.  In both cases the terms up to <tex>$(dt)^4$</tex> are
perfectly correct, and the error starts in the <tex>$(dt)^5$</tex> term,
where the Taylor series coefficient would have been <tex>${1 \over 120}$</tex>.
So the leading error terms are:

:eqnarray:
\delta \br_1 &=& {\textstyle -{1 \over 720}}\bc_0 (dt)^5 \nonumber\\
\delta \bv_1 &=& {\textstyle {1 \over 2880}}\bp_0 (dt)^5 \nonumber

*Alice*: Which proves that your Runge-Kutta method is truly fourth-order,
and that there was a typo in Abramowitz and Stegun.

*Bob*: And now that that is settled once and for all, I suggest we continue
to move toward introducing some sort of graphics.  I would really like
to see this eccentric Kepler orbit in front of me, after having talked
so much about it, integrated it, and diagnosed it both numerically and
analytically.

*Alice*: Me too.  However, when we set up some form of graphics, it would
be nice not to have too many parallel version.  Our current code gives
output appropriate for the two-body problem, which is translated, as a
kind of trick, into a pseude-one-body problem: we only output the
relative position and velocity of the two particles.

I think it would be better if we first would translate our current
code to a true N-body version.  When we then develop a graphics
package for it, we can use it for any number of particles we like.

*Bob*: You have a point.  And now that we have four different integrators,
it would be a pity not to use them for the real N-body problem.  I think
we have done most of the work already, anyway.  Going from the pseudo-1-body
problem to the N-body problem will require some careful thought, but
not a whole lot of extra work.
