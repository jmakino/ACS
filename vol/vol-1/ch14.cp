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

*Alice*: That was an even shorter lecture.  Shall we type the code in
the same file <tt>test.rb</tt> where we put the +Body+ class?  We can
still use the +Body+ format for our `relative' particle, as long as we
remember that the mass of that partice corresponds to the sum of the
masses of the original particle.

*Bob*: What about this?

 :inccode: euler1.rb

