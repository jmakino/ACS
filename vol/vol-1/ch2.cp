= Chapter 2.  The Kali Code

== An Educational Project

=== A Toy Model on the Web

*Alice*: I really like your idea of writing a toy model for a code to
similate dense stellar systems.  While it will be useful for your
students, I'm sure we'll learn from it ourselves.

*Bob*: I don't doubt it.  For one thing, you'll see how hard it is to
then mature the code into a real research tool.

*A*: I'm not so sure.  Given that there is a great demand for such a
code, and precious little supply so far, I wouldn't be surprized if
even a toy model would turn out to be useful already, if we do a good
job.

*B*: I would be very surprized, but as we already concluded, no sense
in arguing.  We'll see soon for ourselves.  And it will be interesting
to get feedback from my students.  By now I've been working in this
field for so long that it is difficult for me to guess what is and
what isn't doable for a student, without much guidance.

*A*: I remember very well how difficult it was to get started with
N-body codes.  There was little practical material available to tell
you how to set up and run and analyze experiments.  Plenty of articles
about algorithms and about programming, but very little about how to
put it all together.  I basically learned by knocking on doors here
and there, working with fellow students who were further along than I,
and asking my thesis advisor.

*B*: Yeah, it would be nice to have a more detailed hands-on introduction
available.  Well, I was planning to make my course notes available on the
web in any case, so why not put our toy model on the web as well.  Who
knows, students at other places might find it useful as well.

*A*: And researchers who are not yet specialists in this area.  And perhaps
amateur astronomers, who may have a good background in programming,
but don't have the information available about how to get started in
modeling a star cluster.

*B*: Good point.  You know, it so often happens, when I hear someone talk
about the first computer programs they wrote by themselves, that they
mention two standard examples: calculating the digits of pi and
writing a quick and dirty N-body code, often just a simple
forward-Euler implementation.

*A*: I calculated prime numbers, as my first application, and indeed, my
second one was an 3-body code.  I wanted to model sending a spacecraft
from an orbit around Earth to Mars.  Was I surprised to learn how tricky
it was to deal with Kepler singularities and large scale factors!
Writing a simple code wasn't that hard, but I never got it to work
properly, the few days that I tried.  Not surprising, in retrospect.

*B*: Indeed.  It may seem simple, but handling such an Earth-Mars orbit
is a rather tricky 4-body calculation, much more difficult numerically
than, say computing the orbits of a few planets or even asteroids or
comets around the Sun.

*A*: What was your first N-body calculation?

*B*: I wrote a simple video game for myself, inspired by what I had seen
in officially packaged games.  It seemed natural to me to put in real
gravitational forces.  It was fun to see how much space ships speed up
all of a sudden when they pass close to a star.

*A*: Glad to hear you were more successful than I was in your first venture!

*B*: In any case, it is likely that there is a universal interest in
playing with stars on a computer.  So let's put our toy model on the
web, and see what reactions we get.

=== An Open Source Approach

*A*: If we do that, I suggest that we make it completely open source.

*B*: I agree.  That seems the most natural thing to do.  I have benefitted
so much from all kind of tools that are freely available on the web
that it only seems fair to contribute my own tools in return.
Besides, it offers a great form of quality control: when we write something
which is unclear or has bugs, changes are that we will soon hear about
it from others.

*A*: 

=== Open Source Licence

*A*: To really make it open source, it would be good to put up an open
source licence as well.

*B*: What for?

*A*: To prevent others from taking the material from our web site, and
patenting it, or selling it while hiding the source code, things like
that.

*B*: My first reaction is: would anybody be so mean?  But I suppose,
some people would be, or at least could be.  Yes, it would be very
unpleasant to see someone selling our stuff in a proprietary fashion.
I wouldn't mind if somebody would add value to it in some way, and
then ask money for it, but at the very least the open source character
should be preserved, which implies that the source code should remain
available.  And all of it.  Someone may add some stuff, but they
shouldn't hide or leave out parts.  Or at the very least, when they
want to make a leaner version, they should include a clear pointer to
where the original code could be found on the web.

*A*: 

== A Minimal Vision

=== Applications

*A*: xx

*B*: xx

*A*: It should contain a toy model for stellar evolution.
(describe what we did after MODEST-1).

*B*: Then similarly for hydro: e.g. when to star touch, they stick.

*A*: xx

*B*: xx

=== Individual Time Steps

*A*: xx

*B*: xx

*A*: Minimal complexity: individual time steps, so that you can follow a
100-body system, say, without getting too much bogged down in binaries.

*B*: xx

*A*: Let's pick a name.  How about Kali?  This means `dark' in Sanskrit.
As in the `kali yuga', the dark ages we are currently in according to
Hindu mythology.  Or as in Kali, the Goddess who is depicted as black.

*B*: I have no strong preference.  As least the name is conveniently short.
And since the universe is by and large a rather dark place, the name
is not inappropriate.  We probably should include the option of
modeling black holes too, with this name.  Do you know the Sanskrit
for `hole'?

*A*: Beats me.  But then again, we wouldn't want to only model holes.
And yes, it would be nice if our code would be so robust, simple as it
may otherwise be, that it could handle mass ratios of one to a billion,
like in a brown dwarf circling around one of the most supermassive
black holes in the nucleus of a central galaxy in a rich cluster of
galaxies.

*B*: Okay, the Kali code it will be named then.

*A*: xx

*B*: xx

=== Starting Simple

*A*: xx

*B*: xx

*A*: To make it really accessible for students, with no prior background
in numerical methods, we should really start with a first-order scheme
such as forward-Euler?

*B*: Really, not with the Hermite scheme?

*A*: What is a Hermite scheme?

*B*: Hmm, maybe not start with a Hermite scheme then.  This is the
workhorse of N-body methods in which close encounters can occur.

*A*: I admit that it is a long time ago since I looked at such an
N-body code.  My recollection was that a rather complicated
predictor-corrector method was used, where the force calculation from
various previous time steps was remembered.  The book keeping was
complicated.

*B*: xx

*A*: xx

*B*: xx

== Environment

=== Operating System

*A*:

*B*:

=== Choosing a Language

*B*: Before we get started, we have to pick a particular language.
I have no strong preference among Fortran, C, or C++.  They all have
their strengths and weaknesses.  C is nice and fast and straightforward.
Fortran has by now caught up with C quite well, after lagging behind for
a long time in a lack of appropriate date structures.  C++ is an unwieldy
beast of a language, but if you use an appropriate subset, and you don't
try to become a language lawyer dealing with multiple inheritance and all
that stuff, it's a good tool; it's fast enough, and you don't look at
the unreadable stuff the compiler produces.  Your choice: which of the
three?

*A*: Perhaps none of the above?

*B*: Huh?  Do you want to use Cobol?  Or Lisp?  Too late for Pascal,
I'm afraid.

*A*: Frankly, I'd love to use Lisp, or the dialect Scheme to be more
precise, but don't worry; I know that most non-Lispers have an aversion
to too many parentheses.  While I love the language, I don't have the
illusion that I can single-handedly convert a critical mass fraction
of the astrophysics community to start writing and thinking in terms
of lambda calculus.

*B*: What calculus?

*A*: Forget I said that.  No, I'm not thinking about Lisp or other
more traditional languages.  Rather, I have been considering using a
scripting language.

*B*: You mean Perl?

*A*: Perl is one example of a scripting language, but there are others.

*B*: I've heard many good things about Perl.  I've never used it myself,
but I, too, have been thinking more and more about learning it.  There
are just too many times that I feel hampered by the limitations of C
shell scripts, or whatever similar shells you can use in Unix.  A
little awking and grepping will help you in making C shell script more
powerful, to analyze the output from N-body codes, for example.  But
still, I could easily be convinced to use Perl instead.  I know several
colleagues who swear by it.

*A*: I wasn't thinking about Perl.  Nor Python.

*B*: Ah yes, Python, I couldn't remember the name, just now.  What's
the difference with Perl?

*A*: It's object oriented, like C++ but less unwieldy.

*B*: Hmm, that I like.  I've grown quite fond of using classes in C++,
for the types of problems where I know what I want to do.  It would be
nice to have a more playful language in which to prototype classes,
and throw them away if you don't like them, without having to redeclare
everything all over the place to make the gods of the C++ compiler happy.

*A*: Exactly, that was my thought.  So given the two, I would prefer
Python.  But in fact, there is another choice, even better in my eyes.
It is called Ruby.

*B*: Never heard of it.  What's the difference with Perl and Python.

*A*: Since it was developed after Perl and Python had already come
into being, the author of the language, Yukihiro Matsumoto, had the
advantage of being able to learn from his experience with those other
two languages.  It is in fact more fully object oriented than Python.
It has iterators and all kind of neat stuff.  And very importantly,
it is simple and natural.  It seems to be build on what is called the
principle of least surprise: immediately after you get a quick and
rough familiarity with Ruby, you will be able to guess in most cases
what a given expression will do.  That sounds like a strong claim, but
I know several computer science friends who have told me that this is
really the case.

*B*: Well, since I have never yet learned any scripting language, I
don't care which one I will learn.  You choose.

*A*: Same for me, it will be my first one too.  Okay, let's take Ruby.
It is freely available on the web, all fully open source, and there
are already a number of good books available.  The fist book to
introduced it in the English language is in fact fully available on the
web, so we can get started immediately.

*B*: Okay, Ruby it will be.

*A*: I'll have to warn you, though: a scripting language is slower
than a compiled language.  Ruby especially: besides being an interpreted
language, almost everything goes through two layers of indirection, which
makes for a marvelous flexibility, but a large penalty in
performance.

*B*: There you go again, choosing flexibility over performance.  But
this time I don't care, as long as we have agreed to write only a toy
model.  My students will be slower in understanding stellar dynamics
than the computer will be in running their programs, even if they are
ten times slower.

*A*: Hmm, would you settle for a hundred times slower?

*B*: What?!?  Surely you're joking!

*A*: I'm not joking.  But the good news is: Ruby defines a way to
interface with C code.  So you can write the most time-critical part
of a code directly in C, if you want to make your code run faster.

*B*: That sounds much better already.  But as long as we want students
to study the cold collapse of, say, a 25-body system, we can even live
with a factor of 100.  What do I care?  People ran 25-body systems
back in the sixties, when computers where much more than a million
times slower than they are now, so we can loose a couple zeroes in
factors of speed.  And it will prevent you from ever claiming that
your toy model have much to do with the reality of production runs!

*A*: We'll see about that.

=== Graphics

*A*:

*B*:

== Style

=== Literate Programming

*A*:

*B*:

=== Coherent Programming

*A*:

*B*:

=== A Lab Note Mechanism

*A*:

*B*:

= old stuff (040120)

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

