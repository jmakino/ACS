= Chapter 5.  An Educational Project

== A Toy Model on the Web

*Alice*: I really like your idea of writing a toy model for a code to
simulate dense stellar systems.  While it will be useful for your
students, I'm sure we'll learn from it ourselves.

*Bob*: I don't doubt it.  For one thing, you'll see how hard it is to
then mature the code into a real research tool.

*Alice*: I'm not so sure.  Given that there is a great demand for such a
code, and precious little supply so far, I wouldn't be surprised if
even a toy model would turn out to be useful already, if we do a good
job.

*Bob*: I would be very surprised, but as we already concluded, no sense
in arguing.  We'll see soon for ourselves.  And it will be interesting
to get feedback from my students.  By now I've been working in this
field for so long that it is difficult for me to guess what is and
what isn't doable for a student, without much guidance.

*Alice*: I remember very well how difficult it was to get started with
N-body codes.  There was little practical material available to tell
you how to set up and run and analyze experiments.  Plenty of articles
about algorithms and about programming, but very little about how to
put it all together.  I basically learned by knocking on doors here
and there, working with fellow students who were further along than I,
and asking my thesis adviser.

*Bob*: Yeah, it would be nice to have a more detailed hands-on introduction
available.  Well, I was planning to make my course notes available on the
web in any case, so why not put our toy model on the web as well.  Who
knows, students at other places might find it useful as well.

*Alice*: And researchers who are not yet specialists in this area.  And perhaps
amateur astronomers, who may have a good background in programming,
but don't have the information available about how to get started in
modeling a star cluster.

*Bob*: Good point.  You know, it so often happens, when I hear someone talk
about the first computer programs they wrote by themselves, that they
mention two standard examples: calculating the digits of pi and
writing a quick and dirty N-body code, often just a simple
forward-Euler implementation.

*Alice*: I calculated prime numbers, as my first application, and indeed, my
second one was an 3-body code.  I wanted to model sending a spacecraft
from an orbit around Earth to Mars.  Was I surprised to learn how tricky
it was to deal with Kepler singularities and large scale factors!
Writing a simple code wasn't that hard, but I never got it to work
properly, the few days that I tried.  Not surprising, in retrospect.

*Bob*: Indeed.  It may seem simple, but handling such an Earth-Mars orbit
is a rather tricky 4-body calculation, much more difficult numerically
than, say computing the orbits of a few planets or even asteroids or
comets around the Sun.

*Alice*: What was your first N-body calculation?

*Bob*: I wrote a simple video game for myself, inspired by what I had seen
in officially packaged games.  It seemed natural to me to put in real
gravitational forces.  It was fun to see how much space ships speed up
all of a sudden when they pass close to a star.

*Alice*: Glad to hear you were more successful than I was in your first venture!

*Bob*: In any case, it is likely that there is a universal interest in
playing with stars on a computer.  So let's put our toy model on the
web, and see what reactions we get.

== Free and Open

*Alice*: If we do that, I suggest that we make it freely available, without
restrictions.

*Bob*: I agree.  That seems the most natural thing to do.  I have benefited
so much from all kind of tools that are freely available on the web
that it only seems fair to contribute my own tools in return.
Besides, it offers a great form of quality control: when we write
something which is unclear or has bugs, changes are that we will soon
hear about it from others.

*Alice*: I feel the same.  I have been using Unix as long as I have been
working with computers, and I was delighted when the Linux operating
system became available.

*Bob*: Same here.  And without all its GNU tools, Linux wouldn't exist.
Take the gcc compiler for example.  And I certainly wouldn't want to
live without emacs.

*Alice*: I wonder how we should present our toy model.  Call it `open
source' or `free software' or something like that?

*Bob*: Both terms are in wide use, and there are huge political
discussions about what exactly is and is not open or free in what
way.  I'm not sure whether we want to get into all that.

*Alice*: But if we don't write anything along those lines, there may be
drawbacks as well.  What if someone uses our integrator to design a
bridge, and then the bridge falls down because there was a bug in our
code, and we will be held liable for damages?

*Bob*: Are you serious?

*Alice*: Not really.  I mean, us introducing a bug?

*Bob*: Very funny.  But I guess in this world you never know who will
knock on your cyber door and copies your software.  Do you have a
suggestion as to how to prevent liability?

*Alice*: The easiest way would be to use a type of open source license,
preferably a rather simple one, which at least includes the usual
disclaimers.

*Bob*: Perhaps it is time to have a look at the web, and do a search
for `open source.'

*Alice*: Here is something: "http://www.opensource.org" with a long list
of open source licenses.

*Bob*: That was quick!  Let's see what they say.  Hmmmm.  Most of them
are far too long to read, let alone figure out, for my taste.  Isn't
there a simple one?

== Open Source License

*Alice*: I remember someone mentioning the X window system as having a
straightforward license.  Here, that is probably the MIT license.
It is short and sweet:

  The MIT License

  Copyright (c) <year> <copyright holders>

  Permission is hereby granted, free of charge, to any
  person obtaining a copy of this software and associated
  documentation files (the "Software"), to deal in the
  Software without restriction, including without
  limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software
  is furnished to do so, subject to the following
  conditions:

  The above copyright notice and this permission notice
  shall be included in all copies or substantial portions
  of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
  ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
  TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
  SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.

*Bob*: Great! We have our disclaimers, at the end in capitals,
and the first paragraph sounds reasonable too.  But does it really
cover enough ground?  I don't mind if someone else would sell our
software, unlikely as that may be, but I sure would be unhappy if
they would not include a pointer back to our own web site, and if
they would sell only the executables, while hiding the source code.
The real source will be on our web site, but who would be able to
find it?  They would even be allowed to change the name of the
package, so it may then become impossible for a user to find our
web site.  Hmm.

*Alice*: Yes, that wouldn't be much fun.  Hmm indeed.  But if the X
window people have lived with this license for all that time, there
is perhaps not too much danger involved.

*Bob*: Not if our toy model will become as famous as X, but I doubt
that we will see the day of an N-body model for each man, woman and
child.

*Alice*: Maybe the best solution for now is to start with the MIT
license.  I presume that we can always later replace it with a more
elaborate and more restrictive version.  What do you think?

*Bob*: Fine with me.
