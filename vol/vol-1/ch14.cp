= The Two-Body Problem

== 

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
for students, we'd better be more explicit.  Starting with the general
equations of motion for a gravitational many-body system:

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
<tex>$\hat {\bf r}_{ji} = {\bf r}_{ji} / r_{ji}$</tex>:

<tex>
\begin{equation}
{\bf a}_i = G \sum_{j=1 \atop j \neq i}^N
\frac{M_j}{r_{ji}^2} \,\hat{\bf r}_{ji}\label{newton}
\end{equation}
</tex>
