= Chapter 2.  Rapid Prototyping

In this context Alice mentions one extreme: the Sussman/Wisdom book,
which appeals to them in many ways.  Bob vaguely has heard about Lisp,
never about Scheme.  When Alison takes the book from her bookshelf,
Bob is shocked by the parentheses.  Is this computer code, he asks.
Alice talks a bit about it, but agrees that the isolation of Scheme
may be too much.  They discuss Perl and Python as more practical
compromises, better than C and C++ and Fortran, which Bob and Alice
both have experience with, but which Alice has grown frustrated with.
Bob doesn't mind either C++ or Fortran, but he does like the
flexibility and power of quick expression of Perl and Python.

Alison mentions a bold idea: would it be possible to write a N-body
code in such a more flexible language?  Bob: anything is possible, you
can do it in Basic or Awk if you want.  Alison presses her point, and
Bob, more out of politeness than anything else, listens.  For the sake
of curiosity, he asks her what she has in mind, mostly to show her how
wrong she is.  After some more discussion he has to admit that the idea,
while impractical, may not be as ridiculous as he first thought.
Seeing it still as a game, he starts thinking out loud with Alice how
they would do such a strange thing.

Oscillating between Perl and Python, as the most realistic
compromises, they are also afraid that a compromise may have the
disadvantages of both sides, and they both feel that it would be
better to have a single coherent vision that they can agree with.
Coherence is better than committee decisions.  In short, they want a
pragmatic approach.

Just for fun, since they are sitting behind a terminal, they decide to
type in "pragmatic programming", in the hope to find something there,
more as a joke.  To their surprise they find the two books of two
characters that call themselves the Pragmatic Programmers.  They
browse through a few pages of their first book and like it a lot,
since it really reflects their own experience.  Like Kernighan and
Pike, but addressing more broadly the whole of software development,
says Bob.  Like Knuth, very sensible, but a generation later, says
Alice.

Then they look at the second book, about Ruby.  Since they already
thought about Perl and Python, they give it a closer look, and become
convinced that this may be better than just a comprise.  They decide
to give it a try.  After all, 90% of an N-body code takes less than 1%
of the compute, so who cares about the speed of an interpreted language.
It is flexibility and clarity and rapid prototyping and recycling that
counts.  What to do first?  Get familiar with the language, and write
something simple but actually useful, in order to see how it works out
in practice; in other words, a pragmatic program.

Alice suggests to write a constant time step leapfrog.  Bob laughs and
says that he recognizes that Alice has also worked in cosmology.
Alice counters that Bob's SPH experience must make him feel happy with
leapfrog.  Bob says that, yes, for problems with softening that might
not be too bad, but now they want to do stellar dynamics of stellar
systems, and they should be able to handle point particles.  At the
very least they need variable time steps, and much better even
individual time steps.  He quotes the classic paper by Makino and Hut
(1988), Performance Analysis of Direct N-Body Calculations, to make
his point.  And by the way, when you get close encounters, the
leapfrog sucks.  Fourth-order schemes are a must, and Hermite is the
most elegant scheme.

Alice agrees that something like NBODY1h would be a good idea, but she
laughs at the idea of learning a whole new computer language by starting
to write such a complicated code from scratch.  Shouldn't you first learn
to do I/O and to write a simple class for individual particles and for
the whole system?  Sure, says Bob, a bit annoyed that Alice seems to use
her longer experience and seniority to push for what he thinks is an
overly indirect and long-winded approach.  He is still not completely
used to have tenure, and he automatically feels the tension between
what he thinks is best and the need to be respectful to more senior
professors.  But, Bob tries once more, once you write a leapfrog, it
wouldn't take many more lines to write a Hermite code, right?  And
to go from equal to variable time steps is also only a few more lines,
basically having a time step criterion, which in an interpreted language
is rather simple to add.  And even block time steps isn't that big of
a deal, once you figure out what to do, something you pick up from
other codes anyway.

Alice has to admit that an equal-timestep leapfrog code will probably
be more than half as long as an individual time step block time step
code with Hermite.  But there is another argument, she adds.  The main
reason to use an interpreted object-oriented language like ruby is to
be able to rapidly switch between classes and methods, changing
inheritance and mixins; all so much more easily than in C++.  So by
providing both a leapfrog and a Hermite code, they can get valuable
experience in switching between methods.  If they really get serious
in writing a full-fledged dense stellar systems code, they surely will
need to be able to switch methods, at some point.  And to get experience
doing so early on will help them, first of all, to decide whether ruby
is really the right language for the job, and secondly, if they go
with ruby, they will then have the necessary experience to use ruby in
a way that makes such method switching natural.

Meanwhile, although defending her point of view eloquently, Alice is
having her doubts too.  She knows how easy it is to waste time when
trying to do something that is too elegant.  However, before she can
say that, to her surprise Bob brings up a point in favor of starting
with a leapfrog.  If they would start with a complete NBODY1h type
code, he says, they would probably just translate such a code line by
line into ruby.  That will probably work, but they run the risk that
in that way they never really get a flavor of the language, and so
will not be able to tap the real power of ruby.  That, for him, is a
more important argument to start with the leapfrog.

Alice has to laugh again, and tells Bob that she was just about to
agree with him that starting with NBODY1h would not be such a bad
idea after all, but that she must say that Bob's latest argument is
certainly convincing too.  What she doesn't tell Bob is that she
thinks it is funny that he could agree to her idea only after he
came up with a different argument for it; even so, she appreciates his
independence and critical attitude.  Bob, meanwhile, has to smile too,
realizing that Alice is not as inflexible as he had thought, and
reminding himself at the same time that he now has tenure, after all,
and that there really is no reason not to push his own agenda and use
his own judgement.

