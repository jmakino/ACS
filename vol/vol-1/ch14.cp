= The Two-Body Problem

== Equations of Motion

*Alice*: That was great, to be able to do such rapid prototyping in a
language we hardly knew.  I can see the advantages of an interpreted
dynamically typed computer language.  If we had tried to do this in C++,
it would have taken quite a bit more time, and we would have had to write
much more lines of code.

*B*: Yes.  Defining a class, getting it to behave, providing I/O, and
chaining it together by piping data from one program to another, all
that is a nontrivial beginning.  This is encouraging!  Let's move on,
to see how much we have to add before we can let the integrator
integrate.

*Alice*: Not much, I think, at least if we start with the simplest
integrator possible, forward Euler, for the simplest N-body system,
namely to solve the 2-body problem.

*Bob*: The other day I was asked by a friend outside astronomy why we
call it the N-body problem.  What was the problem with those bodies,
she asked.  I had to think for a moment, because we are just in a
habit to talk about it that way.

*Alice*: We talk about solving the equations of motion; normally when
you solve something, you solve a problem.  I guess the terminology
comes from seeing a differential equation as posing a problem, that
you then solve, either analytically of numerically.  Newton's equations
of motion for N bodies form a system of N differential equations, and
the challenge to obtain solutions is called the N-body problem.

I must admit, I don't like the term "N-body" at all.  Why N, and not
some other symbol?  I would much rather use the term "many body", as
they do in solid state physics, say.  Don't you agree that the
"gravitational many-body problem" sounds much more elegant than the
"gravitational N-body problem"?

*Bob*: I really don't care, as long as I can solve it.  Okay, let's
start with the 2-body problem, which is really a 1-body problem of
course, since you only have to solve the relative motion between the
two particles.

*Alice*: You say "of course", but if this is going to be a presentation
for students, we'd better be more explicit.  We should start with
Newton's equations of motion for a gravitational many-body system:

<tex>
\def\br{{\bf r}}
\begin{equation}
\frac{d^2}{dt^2}\br_i =  G \sum_{j=1 \atop j \neq i}^N M_j
\frac{\br_j - \br_i}{\,|\br_j - \br_i|^3}            
\end{equation}
</tex>

Here <tex>$M_j$</tex> and <tex>${\bf r}_j$</tex> are the mass and
position vector of particle <tex>$j$</tex>, and <tex>$G$</tex> is the
gravitational constant.  To bring out the inverse square nature of
gravity, we can define
<tex>${\bf r}_{ji} = {\bf r}_j - {\bf r}_i$</tex>, with
<tex>$r_{ji} = |{\bf r}_{ji}|$</tex>, and unit vector
<tex>$\hat {\bf r}_{ji} = {\bf r}_{ji} / r_{ji}$</tex>.  The
gravitational acceleration on particle <tex>$i$</tex> then becomes:

<tex>
\begin{equation}
{\bf a}_i = G \sum_{j=1 \atop j \neq i}^N
\frac{M_j}{r_{ji}^2} \,\hat{\bf r}_{ji}\label{newton}
\end{equation}
</tex>

== Relative Motion

For a 2-body system, everything simplifies a lot.  Instead of dealing
with position <tex>${\bf r}_1$</tex> for the first particle and
<tex>${\bf r}_2$</tex> for the second particle, we can write the
above system of equations of motion as a single equation instead, in
terms of the relative position, defined as:

<tex>
\begin{equation}
{\bf r} = {\bf r}_2 - {\bf r}_1                      
\end{equation}
</tex>

This can be visualized as a vector pointing from particle 1 to particle
2, in other words with its head at the position of particle 2, and its
tail at the position of particle 1.  Introducing
<tex>$r = |{\bf r}_2 - {\bf r}_1|$</tex>, we then get:

<tex>
\begin{eqnarray}
\frac{d^2}{dt^2}{\bf r} & = &
\frac{d^2}{dt^2}{\bf r}_2 - \frac{d^2}{dt^2}{\bf r}_1              \nonumber \\
& = & G M_1 \frac{{\bf r}_1 - {\bf r}_2}{\,|{\bf r}_1 - {\bf r}_2|^3}
- G M_2 \frac{{\bf r}_2 - {\bf r}_1}{\,|{\bf r}_2 - {\bf r}_1|^3}  \nonumber \\
 & = & - G \frac{M_1 + M_2}{r^3}{\bf r}                            \nonumber
\end{eqnarray}
</tex>

We can choose physical units for mass, length, and time in such a way
that <tex>$G=1$</tex>.  With the shorter notation <tex>$M=M_1+M_2$</tex>,
we then get 

<tex>
\begin{equation}
\frac{d^2}{dt^2}{\bf r} = - \frac{M}{r^3}{\bf r}
\end{equation}
</tex>

You often see an even more abbreviated `dot' notion for time derivatives.
If we place a dot on top of a variable for each time derivative we take,
we wind up with

<tex>
\begin{equation}
\ddot{\bf r} = - \frac{M}{r^3}{\bf r}
\end{equation}
</tex>

*Bob*: End of lecture.  Yes, let's start solving that last equation.
Here is how the forward Euler integration scheme works:

<tex>
\begin{eqnarray}
{\bf r}_{i+1} & = & {\bf r}_i + {\bf v}_i dt \label{forward-euler-step1} \\
{\bf v}_{i+1} & = & {\bf v}_i + {\bf a}_i dt \label{forward-euler-step2}
\end{eqnarray}
</tex>

for the position <tex>${\bf r}$</tex>, the velocity <tex>${\bf v}$</tex>, and
the acceleration <tex>${\bf a}$</tex> of our single particle, that now
stands in for the relative motion between two particles.  The index
<tex>$i$</tex> indicates the values for time <tex>$t_i$</tex> and
<tex>$i+1$</tex> for the time <tex>$t_{i+1}$</tex> after one more time
step has been taken: <tex>$dt = t_{i+1} - t_i$</tex>.  So, all we have
to do now is to code it up.

*Alice*: That was an even shorter lecture.  Yes, let's do it.

== Modularity

*Bob*: Shall we type the code in the same file <tt>test.rb</tt> where
we put the +Body+ class?  We can still use the +Body+ format for our
`relative' particle, as long as we remember that the mass of that
partice corresponds to the sum of the masses of the original particle.

*Alice*: We can certainly use the +Body+ class, but I suggest that we
put the definition of the +Body+ class in a file <tt>body.rb</tt>, and
only the actual integrator in our file <tt>test.rb</tt>.

*Bob*: A modular approach, I take it?

*Alice*: Sure, whenever I can get away with it!

*Bob*: It may not be a bad idea, in this case.  Yes, you can do something
similar as you do in C and C++ with an include file -- only more
easily so.  Remember those constructs in C where you had to write things
like <tt>#ifndef</tt> _this_ and <tt>#ifndef</tt> _that_ before you could
be sure that it was safe to include a file without including it more than
once?  Well, in Ruby you can use a construct called <tt>require</tt>
<i>"filename"</i>: it only includes the file if it hasn't been
included yet, directly or indirectly.

Okay, so here is the file <tt>body.rb</tt>

 :inccode: body.rb

It is just as we left it, but without my one-liner I/O hacks.  Now
give me some time to figure out how to implement the forward Euler
idea . . . .

== The First Integrator

. . .  Here it is, the new version of <tt>test.rb</tt>.  As you can
see, it starts with requiring that <tt>body.rb</tt> gets included at
the top.

 :inccode: euler1.rb

*Alice*: xxx

*Bob*: xxx

== Input

*Bob*: We have to provide an input file, let's call it <tt>euler.in</tt>:

 :inccode: euler.in

*Alice*: A nice example already for our dimensional freedom: a
two-body problem is intrinsically two-dimensional.  Even in three
dimensions you can always find a plane in which the relative motion
takes place.

*Bob*: That may not be immediately obvious for a student.  When I heard
this for the first time, I thought about two particles passing each
other at right angles at a distance, like the two arms of a cross but
then offset in the third dimension.

*Alice*: You are right.  The best way to convince a student is probably
to let her or him do the exercise of translating the motion to the
center of mass system of the two particles.  In that system, the
motion of the one particle is the same but opposite as the motion of
the other particle, apart from a scaling factor involving the ratios
of the two masses.  The position vector of one particle as defined
from the center of mass, together with the velocity vector of that
particle, spans a unique plane.  The fact that the other particle
moves in the (scaled) opposite way then implies that the other
particle moves in the same plane as well.

*Bob*: Yes, even if you know the answer, it always requires some thinking
to reconstruct the reason for the answer.  So because a two-body
problem is inherently two-dimensional, we might as well start with
specifying only two components for the position and velocity of the
relative motion of the two particles, which made me choose the above
numbers.  Total mass of unity, initial position on the x-axis, also
unity, and initial velocity perpendicular to that, and one half, for a
change.

#*Bob*: Here is a minimal version, let's call it <tt>euler2.rb</tt>
#
# :inccode: euler2.rb

== Forward

*Bob*: Let's test it:

 :command: cp -f euler1.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Bob*: ten times smaller step size:

 :inccode: .euler3.rb-barebones

and run it:

 :command: cp -f euler3.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Bob*: ten times smaller step size:

 :inccode: .euler4.rb-barebones

and run it:

 :command: cp -f euler4.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

*Bob*: ten times smaller step size:

 :inccode: .euler5.rb-barebones

and run it:

 :command: cp -f euler5.rb test.rb
 :commandoutput: ruby test.rb < euler.in
 :command: rm -f test.rb

#*Bob*: ten times smaller step size:
#
# :inccode: .euler6.rb-barebones
#
#and run it:
#
# :command: cp -f euler6.rb test.rb
# :commandoutput: ruby test.rb < euler.in
# :command: rm -f test.rb
#
#*Bob*: ten times smaller step size:
#
# :inccode: .euler7.rb-barebones
#
#and run it:
#
# :command: cp -f euler7.rb test.rb
# :commandoutput: ruby test.rb < euler.in
# :command: rm -f test.rb

*Bob*: Here is a fancy version, with command line arguments
and energy error diagnostics:

 :inccode: euler.rb

*Bob*: And here is how you run it

 :commandoutput: ruby euler.rb < euler.in > /dev/null

 :commandoutput: ruby euler.rb -o 10 -d 0.0001< euler.in

 :commandoutput: ruby euler.rb -o 10 -d 0.00001 < euler.in









