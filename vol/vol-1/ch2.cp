= Chapter 2.  The Kali Code

== An Educational Project

=== A Toy Model on the Web

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

=== Free and Open

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

=== Open Source License

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

== Stellar Evolution and Hydrodynamics

=== A Minimal Vision

*Alice*: It's time to decide what to put into our toy model.  It would
be good to have a clear vision of what we want to accomplish, before
we get started.  At the same time, it should be a minimal vision, to
keep the software simple enough to serve as a toy model.  We can have
some simple stellar dynamics as a backbone, but we should at least
include some stellar dynamics and hydrodynamics.

*Bob*: That's asking a lot.  The simplest stellar evolution code
will be huge, much larger than a small N-body program.  For
hydrodynamics we can use SPH, that won't be much that large.  But in
both cases we'll have a lot of explaining to do, if we really want to
describe to the students what is going on in those codes.

*Alice*: I'm not sure that a stellar evolution code would have to be
that large.  It may be possible to write a code that is not longer
than the stellar dynamics code.  However, we really don't have to be
that fancy.  All that we need to do is to implement the correct
interfaces for a real evolution code and a real hydro code.  Having
done that, for our toy model we can then finish the job by adding
something very simple at the other side of the interface.  In other
words, we can write a minimal skeleton version for stellar evolution;
and similarly for stellar hydrodynamics.

*Bob*: A look-up table for stellar evolution, for a few simple tracks?

*Alice*: Even simpler.  For example, we could let the evolution program
determine everything just by giving it a mass.  It will then compute
the ZAMS radius, as well as the lifetime, by a simple power law formula.
It can keep the radius constant for the first 90% of its lifetime,
and then linearly increase the radius to a 100 times larger radius
during the giant face.  After that, the radius will go to zero, and
the mass becomes that of a typical white dwarf.  This is all at least
qualitatively correct for relatively low-mass stars.

*Bob*: Simple indeed.  Now what would be the equivalent for
hydrodynamics?  Obviously something simpler than SPH.  Well, how about
giving each star a radius, according to a recipe what you just outlined,
and then stipulating that two stars stick and merge when they pass to
within the some of their radii?

*Alice*: Good idea.  That is the right level of approximation: almost
trivially simple, but still it will allow you to study the effect of
collisions.  And it can even produce run-away mergers of many stars if
the stars are crowded together sufficiently.

*Bob*: Yes, especially if we let the radius grow with mass in an
appropriate way.  For starters, we could just take a radius that is
simply proportional to mass, which is not a bad approximation for low
mass stars anyway.  We might even use this toy model to mimic the
formation of intermediate mass black holes!

*Alice*: What is an intermediate mass black hole?

*Bob*: A black hole that is significantly heavier than stellar remnants,
but still a lot lighter than the supermassive black holes in the
nuclei of galaxies.  There is some observational evidence, suggesting
that these types of black holes may be present in young dense star
clusters, and perhaps also in older clusters.  One possible
explanation involves repeated mergers of stars.  If you start with a
young and very dense star cluster, where collisions occur frequently
in the core already within the first million years, it is possible to
get a run-away merger going before even the most massive stars undergo
a supernova, something which takes a few million years.  The most
massive stars are most likely to merge, since they provide more
gravitational focusing, and the more massive a merger product becomes,
the more likely it is that it will eat yet another stars, more and
more rapidly, as a snowball effect.

*Alice*: Eating snow?  I'm not sure about your mixed metaphors, and it
sounds like a rather speculative scenario.  But you make an interesting
point, that students will already be able to start playing with such
scenarios right away, even with our simple toy model, right on their
own laptops.

*Bob*: Good.  So now we have specified the nongravitational astrophysics
part of the code.  That was easy!  Of course we'll have to implement it,
but these simple approximations shouldn't be too difficult.

*Alice*: Wait a minute, we haven't done our specifications yet.

=== A Top Down Approach

*Bob*: What more would you like to specify?

*Alice*: The main reason to write a toy model was to show the importance
of modular programming, with interface specifications between each module,
remember?

*Bob*: Sure I remember.  But we have decided upon the stellar evolution
and hydro approaches, and as soon as we decide how to do the stellar
dynamics, we can start coding.  It will become clear pretty quickly
how to call the stellar evolution from the stellar dynamics module,
for example.  As soon as we see how that works, we make a precise list
of all the parameters in the subroutine that calls the stellar evolution,
and voila, there we have your formal specification.  Wasn't that the idea?

*Alice*: No, no, no.  That is a completely bottom-up approach.  Yes,
that is the way most programs are written in theoretical astrophysics.
But no, that is not what is meant by an interface specification, which
is a top-down approach.  The main reason to have an interface
specification is that the different parts of the program can be
written by different people, in different styles, in different
languages, and at different times.  You cannot expect them to wait for
each other, or to know the details of each other's codes.  The whole
point is that one module is like a complete black box, as seen from
the other module.  This can only be accomplished if you specify
exactly what is being passed through the interface in what way.

*Bob*: That sounds unnatural, and quite a bit of overkill.  But since
you are so serious about all this, and it is only a toy model, I don't
mind trying it.  Once it works, I'd be more than happy to point out to
you how inefficient such an approach will be.  But first I have to get
a better idea of what you have in mind.  What so said so far sounds
just too abstract.

*Alice*: Okay, let us do the exercise.  First let us see what happens
when two stars come close together.  The stellar dynamics (SD) module
should be careful to flag such an occurrence, since this is something
that the stellar evolution (SE) module cannot know about.  In our
case, we have a stellar hydrodynamics (SH) module that will have the
responsibility to follow up on a close encounter.  So when the SD
module notices a close encounter, it notifies the SH module.  If the
SH module decides to merge two stars, it has to tell this to both the
SE and SD modules.  The SE module then has to construct a new module
for the merger product, and independently the SD module will replace
the two previous particles with one new particle, with a mass equal to
that of the merger mass: the some of the previous two masses minus the
mass that may be lost in the process.

*Bob*: I'm happy to leave out the hydro part of the story, for now,
and just discuss the interface between SD and SE.  How do you see
this?

*Alice*: The first question is:
who is knocking on the door from which side, in order to request data
to be passed through the interface.  There are several ways to decide
this.  In the SD case, we will have individual time steps, so
different stars will be updated at different times.  In the SE case,
in general each star will have its own natural internal time step as
well.  Given all this diversity, it may be complicated to give either
module the responsibility to trigger the other module to advance in
time, at the right time.  It may be better to introduce a separate
scheduler module, that takes care of all the timing management.

So the scheduler will inform the SD module which stars have to be updated
next.  In this case, this will involve taking one integration time
step forward.  At the same time, the scheduler will inform the SE module
which stars need to update their internal state.

*Bob*: I should have known it!  In your able hands we have a quick
proliferation of modules.  I wonder how many more you will introduce.
But let me just follow your logic.  Right here I can immediately see
all kind of exceptions to this nice little rule you are specifying
here `from above'.  For example, as soon as the SD module gives the
green light to one star to step forward, there is a chance that it
will run into another star immediately, on a time scale that can be
arbitrarily much shorter than the time step you thought it would take.
This means that all the other stars that happily stepped forward are
now ahead of their time, and we should call them back to their previous
position, to make sure that they were not affected by the collision.

*Alice*: That is true if your time step criterion does not include the
possibility of a collision.  Normally the time step in an N-body code
is determined by a concern for the accuracy of the numerical orbit
integrator, but it is perfectly possible to include an extra criterion,
to predict when a collision will take place.  But there are many other
options.  For example, you could decide to postpone the actual collision
treatment until the next time step.  After all, two stars will not
suddenly jump on top of each other.  They may approach each other in
such a way that their mutual distance decreases at a rate of a few
percent per time step.  And given other approximations, you might decide
that starting the collision treatment a few percent nearer or further
does not make much of a difference.

Similarly, if the SE module would want to let a star explode into a
supernova, affecting all stars nearby, and potentially all stars in
the system, we could ask the module to postpone the explosion until
the next time step.  Or we could include a prior warning mechanism,
letting the SE module tell the SD not to take a larger time step than
warranted before the supernova explosion.  Either choice would be fine,
but that type of choice has to be made before the interface specification.
Once you have made that decision, you cannot tinker and play with it
as you like, on a daily basis.

*Bob*: What if you want to use one criterion under one set of circumstances,
and another one in other situations?

*Alice*: If it really turns out to be important to do both, we can
of course incorporate that choice within the interface, indicated by a
special flag, say.  But I would want to be careful before making such
decisions.  Before we know it, throwing in more and more options will
make the interface as complicated as many of the codes currently in use.
The idea is to use the interface specification to protect you from
spaghetti, not to create more.

*Bob*: I still don't see clearly what you may want to pass how.

=== Choosing Ingredients

*Alice*: Okay, let us be more precise.  We can think of passing data
across the interface, or of making data available at the interface.
Notice how the first formulation implies an active version, in the
form of a question and answer type of handshaking, and the second one
is more passive, like looking something up in memory.

Clearly, a lot of thought, as well as experimentation, will have to go
into the writing of a detailed interface specification.  But for now,
to give a more concrete idea, let us take the first model, in terms of
handshaking.  We also have to say which side initiates the handshake.
Let us put that responsibility for now at the side of the stellar
dynamics module.  It can ask a particle to evolve its internal state,
or tell it that it is no longer needed, in the case of a merger.

We can find out what type of instructions should be passed through the
interface, by looking at the physical requirement of the system that
we are modeling.  Let us make a list:

- a star evolution function

- a star destruction function

- a function providing the mass of a star

- a function providing the radius of a star

*Bob*: So these will be subroutine calls from the SD module, which can
be evaluated by the SE module, after both modules have been compiled and
linked, correct?  I like to translate your abstract reasoning in concrete
terms that I am familiar with.

*Alice*: Yes, you can think of them as subroutines.  You are not allowed
to add more arguments, unless you start a new version of the software,
and document how and why you made that particular change.  And under
no circumstances are common blocks allowed in the interface.

*Bob*: Your list is far from complete.  Don't you need a star constructor
as well?

*Alice*: Perhaps.  I was thinking that you start the N-body calculations
with as input initial conditions that come from a file, or directly from
another program.  Presumably that other program knows how to create stars.

*Bob*: But after a merger occurs, what do you do?  You can destroy one
of the two stars, changing the other star into the merger, but that
would be rather arbitrary.  Wouldn't it be better to destroy both of
the pre-merger stars, and create a new star instead, for the merger
product?

*Alice*: Yes, you are right, that is much cleaner.  See, you are starting
to think in a more modular way already!

*Bob*: Hmm, you still haven't defined exactly what modular means, but I
won't push for yet more specifications.  Coming back to the functions,
we also need a way to pass time information.  Even though your nifty
scheduler module will give wake up calls, I presume that we need to
have a local way within each module to keep track of time.  And then
there is a question of synchronizing that time, or at least to know
whether the information you get across this interface barrier is up to
date or slightly out of date, stemming from the last time step.

*Alice*: I agree.  So we get already six functions then:

- a star creation function

- a star evolution function

- a star destruction function

- a function providing the mass of a star

- a function providing the radius of a star

- a function providing the current time for a star

Are you happy with this list?

*Bob*: From an astrophysical point of view, this is still not enough.
The reason that mergers are interesting is that they evolve in a very
different way from ZAMS stars.  A merger product will be far from
thermal equilibrium, it will most likely be in a state of high
rotation, and there will be strong gradients in chemical composition.
In order to convey all this information, your interface specification
will have to include sets of tables and functions with many parameters!

*Alice*: For the time being, while we are developing our toy model,
we do not have to take all those effects into account.  After we
reach a stable state, we can give our software a version number 1.0,
say, we can then update the interface specification in order to
include more physics.

Also, most of the complexities you talk about are a matter of
information transfer between the SE and SH modules, and the SD module
doesn't have to know those details.

*Bob*: Fine, as long as it is understood that we will have to widen up
those interfaces significantly later on, I'm happy to stay with the toy
interface.  But even so, we need to convey at least some information about
chemical composition, to show that mergers have higher helium content,
and higher metallicity in general, compared to freshly born stars.  We
don't need to worry about too many details, perhaps but at the very
least we need to communicate Y and Z, helium and metallicity
abundance.

=== An Interface Specification

*Alice*: Fine!  So we get a list of eight functions then:

- a star creation function

- a star evolution function

- a star destruction function

- a function providing the mass of a star

- a function providing the radius of a star

- a function providing the current time for a star

- a function providing the total helium fraction of a star

- a function providing the total metallicity of a star

And since you asked for more concreteness in the examples, we can
write the specifications for these functions in Fortran.  Similar
specifications can be prescribed for other languages, such as C or
C++, and in many cases the interface will be used to connect two
modules that may be written, say, in Fortran and C++.  However, it is
convenient to choose one language in which to give the specification.
The alternative would be to specify the data passing on the byte level.

*Bob*: And in that case you would have to worry about architecture
dependencies, such as big-endian versus little-endian byte ordering
in integers and floating point numbers.  But there would be much more
involved.  The subroutine passing would have to be specified on the
level of assembly language, and that would in turn be architecture
dependent.  So yes, it is almost unavoidable to pick a particular
language.  Which language shall we choose?  Why do you prefer Fortran;
wouldn't C be more natural, and closer to the machine architecture?

*Alice*: The choice of language deserves a separate discussion; let's
do that in a moment, for our toy model.  For now, just to give you a
concrete example of an interface specification, let's take Fortran.
Starting with the first function on our list, here is how we can
create a star.

The function
    integer function
        CreateStar(M, Y, Z)

accepts as arguments <tt>real*8</tt> variables for the initial mass,
the helium abundance and metallicity of a star created at the zero age
main sequence.  The return value is a unique integer that acts as the
identifier for the particular star that has been created.  A negative
return value will signal an error (e.g. not enough storage left;
unreasonable initial conditions provided; or some other internal error
in the stellar evolution module).

*Bob*: We'd better specify the units for the variables as well.  How
about: 

   M in solar masses

   Y helium abundance fraction by weight; 0 <= Y <= 1

   Z metallicity abundance by weight; 0 <= Y + Z <= 1

*Alice*: Fine.  Moving right along to the second function, we can write:
    real*8 function
        EvolveStar(id, dtmax, dMmax,
                   dRmax, dYmax, dZmax)

for a function that accepts as first argument an integer variable for
the identifier <tt>id</tt>, followed by five <tt>real*8</tt> variables
that determine halting criteria.  The stellar evolution code will
start evolving the star, from the current time  <tt>tnow</tt>, and it
will stop as soon as one of the following halting criteria is satisfied:

- if the time exceeds that of the specified period: <tt>t >= tnow + dtmax</tt>;

- if the mass +M+ obeys <tt>| M - Mnow | > dMmax</tt>;

- if the radius +R+ obeys <tt>| R - Rnow | > dRmax</tt>;

- if the helium fraction +Y+ obeys <tt>| Y - Ynow | > dYmax</tt>;

- if the metallicity +Z+ obeys <tt>| Z - Znow | > dZmax</tt>.

This function then returns the new time +tnew+; a negative value for
+tnew+ indicates an error condition.

*Bob*: And we need additional units, for example:

   t in millions of years

   R in solar radii

Okay, that all gives me a better idea of what you had in mind.  No
need right now to write down the other six functions.

== Stellar Dynamics

=== Regularization

*Bob*: As for the stellar dynamics, how fancy shall we make our code?

*Alice*: at least we should be able to handle core collapse for a small
number of particles, say a few hundred or so, so that we can model a
small cluster, and see the inner parts shrink gravothermally.

*Bob*: In that case, it would be nice to be able to deal with close
binaries as well.  Otherwise we have to stop the simulation before
core collapse is completed.  In addition, we may want to study the
effect of primordial binaries.

*Alice*: But I would prefer not to get into regularization and all that.
Whatever I have seen from special treatment of close encounters convinced
me that that whole topic is rather specialized, and not something we want
to throw at the students right away.  Wouldn't it be sufficient to use
only one integration scheme globally, for all particles, without
making any local exceptions?

*Bob*: As for the global dynamics, we are pretty much forced to
introduce an individual time step scheme.  That will require quite a
bit of explaining to the students, since it goes way beyond what they
can find in the standard text books on numerically solving
differential equations, but we have little choice.  If all particles
would share the same time step size, the whole system would get down
on its knees when somewhere two stars would come close together, or
worse, would form a tight binary.

However, even with individual time steps, almost all of the computer
time for a few hundred body system could go into just one tight binary.
And what is worse, the integration of such a binary could lead to an
unacceptable build-up of roundoff errors.

Here is an example.  It can easily happen that two stars form a hard
binary, with a semi-major axis that is a thousand times smaller than
the size of the system.  The eccentricity _e_ will fluctuate, due to
the perturbing forces from encounters with other stars, and
occasionally, _e_ can get close to unity.  If, say, _e_ = 0.999, then
at the distance of closest approach the stars will be a million times
closer than the size of the system.  In order to calculate the force
between the stars, we have to first subtract the position vectors of
both stars, and this will lead to a loss of precision of at least six
digits, reducing an original 64-bit double precision calculation
almost to 32-bit single precision.  We simply cannot afford that, if
we want to integrate the system in an accurate way.

One way to overcome this problem is to introduce a special coordinate
system.  Expressed in relative coordinates, with respect to the center
of mass of the two stars, there is no longer any problem of round-off.
An even better way is to go one step further: to replace the orbit of
a very tight binary star by the analytic solution of the Kepler
problem, an elliptical orbit for which we only have to solve Kepler's
equation to know where the two particles are with respect to each
other, at any given time.

Now sooner or later, a third particle may happen to be close to a
hard binary on an eccentric orbit.  In that case we can simply replace
the numerical binary treatment by an analytic Kepler orbit.  We could
introduce a celestial mechanics perturbation treatment.  But in general,
the best solution is to use what is called a Kustaanheimo-Stiefel
transformation, or other variants that have been introduced later.
These of course are only the first steps.  There are many other very
nice tricks of the trade.  If you use chain regularization and Stumpf
. . . 

*Alice*: Yes, yes, I'm sure there are many wonderful tricks, but we
have to offer your students something that they can understand and
apply, all within half a year.  Whoever Stumpf is, or the other two
gentlemen you mentioned, let's leave them out for now.

*Bob*: But we should do something, on the local level, to make the
whole calculation meaningful.  The question is what we can introduce
that goes partway toward full regularization.  One think we cannot get
around is to do the analytic regularization that I just mentioned.
Perhaps that is good enough to get them started.  It will actually be
instructive for the students to see how things fail, and then to see
what solutions can be found to repair the situation.  The only way to
do that, I think, would be to introduce the separate coordinate
patches I talked about.  While that will rapidly get too complicated,
it would be fun, and it would students give a more realistic sense
of the complexities that a real code has to deal with.  Who knows,
when we really get going, we might even want to give that a try.

*Alice*: Aha, do I detect the possibility of a smooth evolution from a
toy model to an actual production code?

*Bob*: No no, that would be far more complex.  But it would be a step in
the right direction, for sure.

*Alice*: If the main problem would be a loss of accuracy, how about use
multiple precision?  Instead of 64-bit word length for floating point
numbers, using 128-bit word length?

*Bob*: You mean quadruple precision?  Yes, that would be an option, but
it will slow you down.  Depending on the machine you use, it could slow
you down anywhere from a factor of a few to a factor of a hundred or so,
if you can do it at all: not every compiler has a quadruple precision
option, and you don't want to write all the routines by hand for computing
multiplication and division and square roots and all that.

In any case, yes, there are plenty of options.  Right now, I wouldn't
be able to guess what would and would not work well, under which
conditions.  So I'll be learning quite a bit from this exercise, I
think, even though it's only a toy model.  The more I think about it,
the more it seems like a fun project.

=== Local versus Global

*Alice*: If we really cannot avoid introducing some local treatment
in the stellar dynamics part of the code, it would be best to split
the stellar dynamics itself into two modules, and a clear interface.

*Bob*: I told you, that you would want to introduce more modules,
before long!  How many do we have now?  A scheduler (SC), a stellar
evolution (SE) and a hydro (SH) module, and now a global stellar
dynamics (GD) and a local stellar dynamics (LD) module, five in total!

*Alice*: Nothing wrong with having five modules in a code.  This means
that you have a lot of freedom!  You can replace each module, rewrite
it, experiment with it, all without affecting anything in any of the
other modules.  But lets not repeat our arguments.  After we finish
our first toy code, we can take up this discussion again, and by that
time we'll have a lot of actual code to work with, to strengthen our
arguments.

*Bob*: Fair enough.  But now I'm puzzled.  Passing information between
the SD and SE module was natural enough, since we were dealing with two
rather different types of physics.  But the GD and LD modules both are
dealing with the same physics, plain old Newtonian gravitational
attraction.  Putting an interface between them would seem like trying
to draw a line on the water!

*Alice*: I want to draw a line in the sand, frankly, and insist that
we divide the stellar dynamics into these two different modules.  Look
at it this way.  From the point of view of the global dynamics, the
local dynamics is a way to protect ourselves from the Kepler
singularities, right?  I presume that is the meaning of the word
regularization, removing the singularities.

*Bob*: Yes, I think that is were the term comes from.

*Alice*: In that case, the SH module is also a form of regularization.
When two point particles come close, the SH module will replace them
by, for example, a blob of SPH particles.  We could call this a form
of hydro regularization.  It looks a bit like softening, where every
point particle is replaced by a small Plummer model.  Softening could
also be called regularization.

Strictly speaking, the mathematical term regularization implies that
you do not change the underlying equations of motion, so in that sense
hydro treatment and softening should not be called regularization.
But we are not mathematicians.  In astrophysics, we start with
extended objects.  The idealization of replacing a start by a point
mass is only a matter of convenience.  And when this replacement turns
out to be inconvenient, in close approaches between stars, we are free
to use other idealizations.  We are only `regularizing' what had been
`singularized' as a too extreme idealization.

*Bob*: But you can't simulate a star cluster with a code that is only
using softening.  The code will run, but it will not give you a
sufficient amount of two-body relaxation.  And it will not admit hard
binaries.

*Alice*: That is true if you use a large softening length, like
people do when they collide two galaxies, and they want to suppress
two-body relaxation, which would be unphysical anyway in that case,
since each single particle represents thousands of stars, if not more.
But in the case of a star cluster, you could give each star a softening
radius that is equal to the physical radius of a star.  In that case,
binaries could form and two-body relaxation will be just fine.  You
would have to do more, since you would have to prevent stars from
passing through each other, as softened particles would.  When you
thus add a hard core and a friction term to let the particles stick
together upon close approach, you have gone well above the simple
softening recipe.  But if you add those other prescriptions, using a
small softening in itself is not necessarily wrong.

*Bob*: So you view hydro treatment and regularization as somewhat
similar, from the point of view of the GD module, which only deals
with point particles?

*Alice*: Yes.  And the interface between GD and LD can be similar as
what we saw before.  We can specify a function to create a local clump
of particles, and similarly we can specify a clump destructor function.
We can have functions providing the mass and effective radius of a clump,
as a function providing the current time.  It all follows rather closely
to what we just discussed for stellar evolution.

*Bob*: Hmmm.  I still think it is like drawing a line in the water.
But early on I agree to do the experiment, to write a modular toy model,
and if this is what you want, this is what we'll try to do.  But allow
me to laugh loudly if your approach will result in an unwieldy code,
with many extra lines to circumvent your interface restrictions, and
a very slooooow overall execution speed.  And I won't hesitate to tell
my student's whose brilliant idea it was to do all this ;>).

*Alice*: If that is what will happen, you can use it as a case study
in how not to follow the advice of computer scientists who tell you to
use modular programming.  I'm convinced that this will not happen, but
there is only one way to find out: we will continue, and see who is
right.

*Bob*: I can't wait!

=== The Name of the Game

*Alice*: I can't wait either, and whatever the outcome will be, it
will be a useful experiment.  Before we get started, though, there is
one more thing we have to decided.  As long as we put all this
wonderful stuff soon-to-come on the internet, it would be good to give
it a name.  I wouldn't want to refer to it as `our kitchen sink toy
model' all the time.

*Bob*: A name.  Hmm.  Many options.  We could use an acronym.
Fortran comes from formula translator.  How about Dentran, to
translate your modular ideas for modeling dense stellar systems?

*Alice*: That sounds like something to do with dentists, perhaps a
new type of tooth paste.  How about using a Greek term?  The word
astrophysics comes from the Greek after all.

*Bob*: I'm not very strong on languages.  Do you have a specific
suggestion?

*Alice*: A while ago I asked a Greek astronomer what the word for
star cluster research would be.  You know, geology is the study of
the Earth, astronomy is the study of the stars, so what kind of
something-logy or something-nomy would be the study of star clusters?
After some thought, he suggested `smenology'.  I believe that `smenos'
means something like a swarm, like a swarm of bees.

*Bob*: I am often enough stung by bugs in N-body codes, so that would
be quite appropriate, I'm afraid, although when I hear the word bug,
I think of small little critters, not as big or ferocious as bees.
But how do you turn that into a name?

*Alice*: We could call it the Smenology Code?

*Bob*: Too long, too difficult to pronounce, and besides, the logic
doesn't work.  You wouldn't want to call a code the Astrophysics Code,
would you -- as if there would be only one such thing.

*Alice*: And no comment.  Okay, well, what else.  You want a short
name, I take it?

*Bob*: The shorter the better.

*Alice*: The A code?

*Bob*: Unless you will do all the writing, it should at least be the
AB code, for both of our initials.  But seriously, it doesn't have to
be quite that short.  Hey, you like languages; it doesn't have to come
from Greek -- why not pick something from Chinese or Sanskrit or
some other side street.

*Alice*: Sanskrit, now that's an idea.  How about Kali?  That means
`dark' in Sanskrit.  As in the `kali yuga', the dark ages we are
currently in according to Hindu mythology.  Or as in Kali, the Goddess
who is depicted as black.

*Bob*: As least the name is conveniently short.  And since the universe
is by and large a rather dark place, the name is not inappropriate.
We probably should include the option of modeling black holes too,
with this name.  Do you know the Sanskrit for `hole'?

*Alice*: Beats me.  But then again, we wouldn't want to only model holes.
And yes, it would be nice if our code would be so robust, simple as it
may otherwise be, that it could handle mass ratios of one to a billion,
like in a brown dwarf circling around one of the most supermassive
black holes in the nucleus of a central galaxy in a rich cluster of
galaxies.

*Bob*: Okay, the Kali code it will be.

=== An Integration Scheme

*Alice*: So we have a name for our toy model, and we have decided
how we will model the stellar dynamics, the stellar evolution and the
stellar hydrodynamics: as simple as possible, but not simpler.
Shall we get started?

*Bob*: Just a moment, we haven't yet decided what to do with the stellar
dynamics.

*Alice*: You suggested an individual time step scheme, and I think that
is fine.  Actually, we may want to start with shared time steps, but
that is a matter of presentation.

*Bob*: Yes, but the choice of time step is only one choice we have to
make.  We haven't chosen an integration scheme yet.  If you want to
start with shared time steps, you can do that using a leapfrog
integrator, or more reasonably a fourth-order integrator of one type
or another.

*Alice*: Ah, of course.  And yes, plenty of choices.  Well, to make it
really accessible for students, with no prior background in numerical
methods, we should really start with a first-order scheme such as
forward-Euler, shouldn't we?

*Bob*: Really, not with the Hermite scheme?

*Alice*: What is a Hermite scheme?

*Bob*: This is the current workhorse of N-body methods in which close
encounters can occur.

*Alice*: I admit that it is a long time ago since I looked at such an
N-body code.  My recollection was that a rather complicated
predictor-corrector method was used, where the force calculation from
various previous time steps was remembered.  The book keeping was
very complicated.

*Bob*: That must have been a long time ago indeed!  For the first thirty
years or so, this was indeed the method of choice, until in the late
eighties Makino came up with a simpler self-starting scheme, which he
called the Hermite integrator, since it uses some ideas put forward
first by mister Hermite, a couple centuries ago, I think.  This is what
is now being used almost exclusively in stellar dynamics of dense
stellar systems.  And it's not that difficult to code.  You can write
it in such a way that it looks like an almost obvious generalization
of a leapfrog integrator, apart from a few coefficients which would be
hard to guess off-hand.

*Alice*: In that case, it would be good to incorporate that scheme in
Kali code.  Even so, I like to start simple.  I don't want to scare
the students by presenting complex integration schemes until they have
gotten a sense of what the notion of integration means.

*Bob*: Wouldn't that be too much hand-holding?  Why not let them jump
in right away?

*Alice*: The drawback of giving them a fancy working tool right away
would be that they would immediately run with it and move on to fun
applications.  Chances are they would never bother to think about how
and why it can work in the first place.  These days, students are all
too much focused on learning how to use a package, rather than writing
it and getting a feel for what is under the hood.

*Bob*: I'm afraid you're right.  When I did my first programming, it
was in basic, and soon afterward in C.  Nowadays, when students talk
about programming, they mean tweaking the parameters of a packages,
while using GUIs.

*Alice*: Perhaps we begin to sound like old folks, shaking our heads
about the behavior of youngsters nowadays.  But in any case, I really
think it is best to start with the simplest possible integration
scheme, forward Euler.  That way they can get a feel of what it means
to follow the tangent of a curve in literally the most straightforward
way.

*Bob*: A clumsy way to start, for sure, but at least they will then
learn to appreciate the power of higher-order integration schemes.
Okay, let's do that.

== Environment

=== A Toolbox

*Alice*: Now that we know what we want to model, we have to address
the question of the environment in which we want to work.

*Bob*: You mean what type of computer hardware, or operating system,
or computer language?

*Alice*: All of these questions need to be addressed, and they are
general concerns for any software projects.  However, in our particular
case there is also the question of how to get data in and out of our
integrator, and how to analyze the results.

*Bob*: Of course, you have to produce some initial conditions,
and you want to look at the results, but basically we are dealing with
one big program, and a bunch of small ad hoc programs, which could be
some shell scripts or whatever you can put together quickly.  The main
point is to provide the students with a robust integrator.  The rest
they can take care of themselves.

*Alice*: But what about standard questions, such as deriving the
potential energy for each particle, and the local density around
each particle.  You almost always want to know those quantities.
Wouldn't it be better to build separate programs to compute those?

*Bob*: It would be much better to make that part of the integrator.
The potential will already be calculated at regular intervals anyway,
to provide accuracy diagnostics.  And it wouldn't be too difficult to
built in a density estimator as well.

*Alice*: Combining everything into one big program has been the
traditional approach for a long time, but it is far from optimal.
What I would prefer, certainly for a student demonstration project, is
to provide something that is more like a toolbox.  I'm happy to let
the calculation of quantities such as potential energy be done in the
integrator, as part of the necessary on-the-fly analysis.  But in
addition, I would want to have a separate stand-alone program that can
compute the potential as well.

*Bob*: Why?

*Alice*: Imagine that you want to calculate the total mass of a bound
subsystem within a larger N-body system.  You take the whole system,
and remove the particles that our not bound to the subsystem.  Perhaps
you want to iterate a few times, removing more particles until you
really have a self-gravitating subsystem.  At that point you may want
to check virial equilibrium, so you have to calculate the kinetic and
potential energy of all particles.

*Bob*: That makes sense.  And come to think of it, when you then want
to calculate Lagrangian radii, it may be nice for the students to have
yet another little program to do just that.  Ah, we can follow the way
of the Unix system: a large number of small tools, where you can pipe
the results from one tool into the next one.

*Alice*: That would be one example of what I had in mind, but it is not
the only one.  There are other ways of combining a number of small tools.
However, this type of decision we can make later.  The main thing now
is to agree upon the general approach: to provide a number of simple
tools that can be used together to analyze the results of a calculation.

*Bob*: Fair enough.  But you started out talking about choices we have
to make with respect to the software environment.  I thought you would
talk about operating systems and languages and stuff like that.  In
short: what type of platform do we run our toy model integrator and
toolbox on?

=== Operating System

*Alice*: Given that we are both used to working in Unix, there is not
much of a question, as for as the operating system goes.  Most of my
work I do on a laptop, running Linux.

*Bob*: me too, but that is not necessarily true for most of the students.
If we do not make our environment available for Windows and for the Mac,
we may not have much of an audience.

*Alice*: I have no experience at all, porting software to a different
platform.

*Bob*: I have some.  For example, putting stuff on a Mac is not that
hard, after they switched to Unix as the underlying environment.  As
for windows, a lot of the GNU tools have been ported there as well.
If you use cygwin, basically any program written for Unix is likely
to work.

*Alice*: Can you just type gcc and expect to find a GNU compiler,
under cygwin?

*Bob*: Yes, you can.

*Alice*: Amazing.  And how about emacs?

*Bob*: Yes, emacs is there, as well as all the standard Unix commands
and programs.

*Alice*: Does that mean that when we develop our toy model under Linux,
we can make a tar ball, copy that to a machine running Windows and
cygwin, untar it, and expect everything to work right away?

*Bob*: Well, you can be lucky, but realistically speaking, you'll have
to test it first, and you may well find some minor problems here and
there.  But of course, if you would switch from one Unix system to
another, you would also have to make minor adjustments.

*Alice*: I know.  Even switching from one Linux distribution to another
has given me plenty of surprises in the past.  Often it was just a
question of tools being stored in different places.

*Bob*: Yes, but that's no big deal, and easy to fix.

*Alice*: For you perhaps, but if I had to do that myself I might be
stuck for hours, without someone more experienced to help me.  Well,
I'm glad we're doing this project together.  I'll count on you to
do what you call the easy things.

*Bob*: We aim to please!

=== Choosing a Language

*Bob*: Finally we will have to make the decision which programming
language to use.  I have no strong preference among Fortran, C, or
C++.  They all have their strengths and weaknesses.  C is nice and
fast and straightforward.  Fortran has by now caught up with C quite
well, after lagging behind for a long time in a lack of appropriate
date structures.  C++ is an unwieldy beast of a language, but if you
use an appropriate subset, and you don't try to become a language
lawyer dealing with multiple inheritance and all that stuff, it's a
good tool; it's fast enough, and you don't look at the unreadable
stuff the compiler produces.  Your choice: which of the three?

*Alice*: Perhaps none of the above?

*Bob*: Huh?  Do you want to use Cobol?  Or Lisp?  Too late for Pascal,
I'm afraid.

*Alice*: Frankly, I'd love to use Lisp, or the dialect Scheme to be more
precise, but don't worry; I know that most non-Lispers have an aversion
to too many parentheses.  While I love the language, I don't have the
illusion that I can single-handedly convert a critical mass fraction
of the astrophysics community to start writing and thinking in terms
of lambda calculus.

*Bob*: What calculus?

*Alice*: Forget I said that.  No, I'm not thinking about Lisp or other
more traditional languages.  Rather, I have been considering using a
scripting language.

*Bob*: You mean Perl?

*Alice*: Perl is one example of a scripting language, but there are others.

*Bob*: I've heard many good things about Perl.  I've never used it myself,
but I, too, have been thinking more and more about learning it.  There
are just too many times that I feel hampered by the limitations of C
shell scripts, or whatever similar shells you can use in Unix.  A
little awking and grepping will help you in making C shell script more
powerful, to analyze the output from N-body codes, for example.  But
still, I could easily be convinced to use Perl instead.  I know several
colleagues who swear by it.

*Alice*: I wasn't thinking about Perl.  Nor Python.

*Bob*: Ah yes, Python, I couldn't remember the name, just now.  What's
the difference with Perl?

*Alice*: It's object oriented, like C++ but less unwieldy.

*Bob*: Hmm, that I like.  I've grown quite fond of using classes in C++,
for the types of problems where I know what I want to do.  It would be
nice to have a more playful language in which to prototype classes,
and throw them away if you don't like them, without having to redeclare
everything all over the place to make the gods of the C++ compiler happy.

*Alice*: Exactly, that was my thought.  So given the two, I would prefer
Python.  But in fact, there is another choice, even better in my eyes.
It is called Ruby.

*Bob*: Never heard of it.  What's the difference with Perl and Python?

*Alice*: Since it was developed after Perl and Python had already come
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

*Bob*: Well, since I have never yet learned any scripting language, I
don't care which one I will learn.  You choose.

*Alice*: Same for me, it will be my first one too.  Okay, let's take Ruby.
It is freely available on the web, all fully open source, and there
are already a number of good books available.  The fist book to
introduced it in the English language is in fact fully available on the
web, so we can get started immediately.

*Bob*: Okay, Ruby it will be.

*Alice*: I'll have to warn you, though: a scripting language is slower
than a compiled language.  Ruby especially: besides being an interpreted
language, almost everything goes through two layers of indirection, which
makes for a marvelous flexibility, but a large penalty in
performance.

*Bob*: There you go again, choosing flexibility over performance.  But
this time I don't care, as long as we have agreed to write only a toy
model.  My students will be slower in understanding stellar dynamics
than the computer will be in running their programs, even if they are
ten times slower.

*Alice*: Hmm, would you settle for a hundred times slower?

*Bob*: What?!?  Surely you're joking!

*Alice*: I'm not joking.  But the good news is: Ruby defines a way to
interface with C code.  So you can write the most time-critical part
of a code directly in C, if you want to make your code run faster.

*Bob*: That sounds much better already.  But as long as we want students
to study the cold collapse of, say, a 25-body system, we can even live
with a factor of 100.  What do I care?  People ran 25-body systems
back in the sixties, when computers where much more than a million
times slower than they are now, so we can loose a couple zeroes in
factors of speed.  And it will prevent you from ever claiming that
your toy model have much to do with the reality of production runs!

*Alice*: We'll see about that.

=== Graphics

*Bob*: One more thing to decide, that is just as essential as choosing
an operating system and a programming language.  We have to pick a
plotting package.  The students must have a way to display the results
of their calculations on the screen, and also a good way to print the
pictures they produce.

*Alice*: I am using pgplot for my own work.  That seems to be doing all
that I want, normally.

*Bob*: Same here.  I have some familiarity with Matlab, as well as a
number of other packages, but none of them are open source.  And since
we decided to go the route of free software, we cannot rely on
proprietary packages.  So pgplot would be a reasonable choice.
However, I'm not hundred percent sure that it is open source.

*Alice*: No?  I was convinced it was open source.  I've never paid
for it, or signed any license.

*Bob*: I know.  But when I checked on their web site, it was not really
clear to me what their status was.  Yes, they are open source in the
sense that their source is open: you have free access to all the
source code.  However, in their case this does not automatically imply
that you can distribute it freely to third parties.

*Alice*: That would be inconvenient.  It would be far easier if we can
bundle our own environment together with the libraries that we invoke.
And graphics will indeed form an essential part of our whole package.
Are you sure we can't make it available?  If we and everybody else can
copy it freely from the web, what difference would it make if we put
up our own copy?

*Bob*: That's the point: not everybody can pick it up freely.  If you
read the fine print, you will see that it is only really free for
educational purpose.

*Alice*: That may be good enough for us, since we are only dealing
with students.

*Bob*: For our classes, yes, we can make pgplot available.  But it seems
that we cannot make it available on the web, since anybody can access
our web site, also people who are not officially within the educational
sphere.

*Alice*: In that case, we can put a pointer on our web site to the
place where one can pick up the source code.  That's not as convenient
as providing everything in a bundled way, though.

*Bob*: I know.  They have a line on their web site, saying that you
can only copy their software to your web site after getting official
permission from them.  Right now may be premature, but once we have a
reasonable package installed on our web site, we may want to send
them an email, asking for permission to put a copy of pgplot directly
on our web site.

*Alice*: That seems like a good idea.  Are there any alternatives?

*Bob*: The only thing I can think of is Gnuplot, but I have found pgplot
to be far more convenient for scientific plotting.

*Alice*: I have tried Gnuplot too, and I agree.  So let's at least get
started with pgplot.  But here is an idea.  Since we may want to switch
to a different graphics package in the future, it would be very unpleasant
if we then had to translate all pgplot commands into the equivalent
commands in that other package.  We could instead define our own
virtual plotting commands, and write a simple wrapper to translate them
into a subset of pgplot commands that is large enough for our purposes.

*Bob*: Well, I don't like all those extra layers of code, and I'm afraid
I've already given you too much leeway with your interface demands.  Why
not just write everything in pgplot for now.  If and when we switch to
a different plotting package, only then do we write a wrapper that
emulates pgplot.

*Alice*: But it might be a lot of work to write a wrapper around a
whole package.  If we decide right from the beginning to use only a
small subset of pgplot, and if we keep a list of the commands we use,
we only have to write a wrapper around those commands, not around the
whole package.  Finally, the best way to insure that we really don't
use anything else but our small list of commands is to give each
command we use an alias.  That way we cannot make any mistakes.

*Bob*: If a very good function is available in pgplot, why not use it?

*Alice*: Oh, it would be fine to add extra functions to our list of what
we use.  We can certainly extend the subset, whenever we like.  My main
point is that we keep close tabs on exactly what we use, and that we think
carefully, before adding something to the subset.

*Bob*: The problem with providing a virtual layer between the user and
pgplot is that we have to make a decision of what type of plotting model
we want to use.  Even if we restrict ourselves to two dimensional plots,
there are several models.  For example, in postscript you deal  directly
with a set of coordinates on paper, in terms of inches.  And other
graphics packages provide transformations between model coordinates,
often called world coordinates, and device coordinates.  So we are dealing
with a situation that is much more complex than just providing a set of
aliases.

*Alice*: I see.  Well, perhaps you are right.  Maybe it would be okay to
go ahead with pgplot for now.  But let us come back to this question
before too long, after we have build a skeleton version of our toy model.
By that time, we might have a better idea of how fancy our requirements
will turn out to be.

*Bob*: I'm glad I could finally convince you not to add an extra layer
of software somewhere.  So now we can finally get started?

== Style

=== Documentation

*Alice*: Not quite.  We have talked about writing programs and providing
an environment, but we haven't said anything yet about documentation.

*Bob*: Well, my class notes will be the documentation, I thought.  I will
give them enough hints to get them started, and when they go through the
code, it will become obvious what we intended.  Of course, we'll put in
enough comments to clarify our methods.

*Alice*: I would prefer a rather different approach.  Comments in a code
are only the first level of documentation.  A good code needs to have
manual pages, at the very least, and even better a form of introduction,
a primer or that sort of thing.  When you want to work with a new program,
don't you look at the manual pages?

*Bob*: Not often.  I prefer to just try things out.  And if it is an
important enough program, I am happy to go look at the source code
directly.  And while I agree that good manual pages can be of some
help for a commercial product, I don't see why we have to be so fancy
for an educational project.

*Alice*: There are several reasons why.  For one thing, I like to make
it attractive and easy for others to use my code.  Once I spend a lot
of time writing a good piece of code, I'm happy to spend a bit more time
to polish it and make it available in a more userfriendly way.  And if
more people will be using it, I will get more feedback which in turn
can improve the product.  Also, others are more likely to write
extensions or complementary programs, and there is a better chance
that a project will take off around what I initially wrote, which can
then serve as the nucleus for something larger.

*Bob*: If you insist on more documentation than code comments, a good
addition would be to include a help mechanism in each program.  Typing
<tt>density --help</tt> could give you some help about the various
command line arguments available for the program <tt>density</tt>, for
example.

*Alice*: Yes, that would be a good addition, but it does not address
the issue of a really comprehensive explanation of not only what a code
does, but why it was written that way: the motivation for the overall
approach and the choice of algorithm used, as well as the particular
organization of the program itself.

You see, the most important reason to write good documentation is
long-term efficiency.  If you force yourself to reason out loud why
you are doing what you just did, you will quickly find that there are
many aspects you haven't thought through deeply enough.  While writing,
you will get new insights that will improve your program significantly.
So writing detailed documentation is not only a good idea, it is
especially a good idea of doing it right away, while you are writing
your code.  If you are doing it later, you may also discover better
alternatives, but you are unlikely to implement those, because by then
you have already invested so much time into the older approach.

*Bob*: that all sounds nice on paper, but in practice, when you deal
with any complicated program, there always comes a time that you have
to beat the thing into shape.  You can start in a nice clean way,
following this paradigm or that, but then when the program gets going,
you find that this is missing or that, and that such and such is
slowing down the execution much to much.  You start throwing in this
and that, and taking short cuts here and there to get significant
speedup.  How are you going to document that?  Spend a few hours every
day rewriting the documentation, each time you have tinkered with the
code?  That seems hardly efficient!

*Alice*: What you just described is a sledge hammer approach.  If you
have only two weeks left before a conference where you have to present
your latest results, I can imagine that you take that approach.  But
unless there is a serious emergency, I cannot see how that will gain
you in the long run.  But I don't think we can convince each other by
arguing.  We clearly have a very different style.  Let us agree that
we at least will be careful to write clear comments and a good help
mechanism for each code.  For myself I will keep track of what major
and minor decisions we make while working on our project, so that I
can document what happened, and why, and how we learned from initially
wrong assumptions.  We can later see what we do with that material.

*Bob*: I'm glad we agree to disagree.  It's time to get our hands dirty
doing some real programming.  Hey, I see that you have a copy of Knuth'
book `The Art of Programming'.  He is one of my heroes.  Those books
are great.  And without TeX and LaTeX, where would we be in writing
articles?  That he singlehandedly invented TeX in the late seventies
was very impressive.  I bet he didn't let himself be side tracked by
documentation.

=== Literate Programming and Coherent Programming

*Alice*: Wrong!  Knuth was very keen on documentation.  Have you heard
of Literate Programming?

*Bob*: Literary Programming??

*Alice*: No, Literate Programming.  This is a term that Knuth invented,
for the process in which he wrote TeX.

*Bob*: Process?  I presume he just wrote TeX, just like you write any
programs.  He just happens to be a lot better in programming than most
of us.  What do you mean with process?

*Alice*: He did not `just write programs.'  On the contrary, he put a
lot of thought into the process of designing programs, coming up with
a view first, then with a way to map out the various pieces needed,
and so on.  And most importantly, he came up with the idea that
computer code should first of all be human readable, and only machine
readable as an afterthought.  You should read one of his bundles of
collected essays on algorithms and programming.  Since he is one of
your heroes, perhaps he will convince you more than I can, about the
value of documentation.

*Bob*: You haven't told me yet what he meant with Literature Programming.

*Alice*: <i>Literate</i> Programming.  The idea is that code and
documentation are interwoven in one book.  You write a piece of code,
and at the same time you write a page of text explaining your
motivation for writing that piece of code, what it is supposed to do,
what were the reasons going into the decisions of writing this part in
this way and that part in that way.  So the code parts and text parts
are literally interwoven in one long manuscript.

*Bob*: But how can you run the code if it is spread through the text
of a manuscript?

*Alice*: He wrote special programs to extract both the book part and
the code part of what he wrote.  He called those +weave+ and +tangle+.
The program +weave+ went through the original, and produced the book
text.  The program +tangle+ also went through the same original,
extracting the bits and pieces of code, putting it all together as one
large code, the source code for TeX, that then could be compiled in
order to produce the TeX executable.

*Bob*: Why did he call the second one +tangle+?

*Alice*: He literally wrote +tangle+ in such a way as to entangled the
source code, in such a way that it was no longer human readable.

*Bob*: Why would he do that??

*Alice*: He wanted to force himself and others not to make quick changes
in the source code alone, without touching the book.  For him documentation
was so essential that he forced himself to change the code only within the
context of the original manuscript, where code and text were living together.
In that way, the threshold was lowered to explaining in the text part what
you just changed in the source code part, and thus the two could grow
together harmoniously.

So this answers directly your objection that when you want to get something
done, you reach a stage where you drop documentation and just beat things
into shape.  It seems that Knuth more or less tied his hand on his
back, to prevent himself for using a sledge hammer approach -- or so
it must have seemed to many people.  But I can easily believe that in
the long run, or even in the medium run, his approach was more efficient.

*Bob*: I did not know that.  I'm really surprised.  And now that you caught
me with a story about Knuth that I didn't know about, are you going to force
me to use literate programming?

*Alice*: No, don't worry.  I don't think that literature programming is
still the right answer, in this day and age.  It was a good thing at the
time, a visionary move really.  But it was developed in the early days
of line editors, even before screen editors came into being, let alone
window systems.  Nowadays it is easy to keep an eye on several files
at once, but in those days the best you could hope for is to see a few
dozen lines on a screen at the same time.  And therefore the only way
to achieve a close coupling between text and code was to literally
weave them together.

*Bob*: What would be a more modern equivalent?

*Alice*: I have been thinking about that, off and on.  I think that
the most important aspect of Knuth' idea, an aspect that will survive
his particular implementation, was the notion of coherence.  With his
trick, he could keep source code and documentation text coherent.
Everything was updated in step, and he leaned way over backward to
guarantee that the two remained in lockstep.  That part I would like
to preserve, the notion of coherence.  My proposal is to introduce
what I would call <i>coherent programming</i>.

The idea would be to allow different files for source code and for
documentation.  However, the two are linked through frequent pointers,
in a hypertext kind of way, perhaps as it is done on web pages.  I
must say, I do not yet have very clear notions of how to implement
this, and what to choose, but the main two points will be to, a), make
it very natural to update code and text in unison, and b), provide
some sort of penalty against violating that simultaneous type of update.
I'm not sure whether I would go to the extreme that Knuth went, of
making his source code unreadable, even to himself, but perhaps even
that might not be a bad idea.  I'll let you know if and when I get
more inspiration in that direction.

*Bob*: I'm glad you let me off the hook so easily, without converting
me into a coherent programmer!  We can talk about those ideas later,
over a drink, whenever you like, and yes, I'll probably have a look at
those essays by Knuth.  I must say that you have peeked my curiosity.
But for now, let's get started doing some programming ourselves!

=== A Lab Note Mechanism

*Alice*: One more thing.

*Bob*: I was afraid you would say that.  What is it now?  Don't we have
all the pieces we need to get started, and more?

*Alice*: This will be the last ingredient, I promise.  I won't insist on
documentation, literate or coherent or otherwise.  But what I do insist
on is a systematic way of keeping lab notes.

*Bob*: Lab notes?  For which lab?  What notes?

*Alice*: Our toy model and its environment will form a kind of lab for
your students.  They can perform experiments, by setting up initial
conditions, running a simulation, and analyzing the results.  It is
all virtual of course, but apart from that, the procedures and the
skills needed are not that different from doing a lab experiment.

*Bob*: I remember burning my fingers while learning how to blow glass
during my freshman physics glass.  After glass stops to glow, it is
still incredibly hot.  I sure wished that I had infrared eyesight then.
But okay, if you want to stretch metaphors, I've burned myself with
hasty programming as well.  And there are parallels between lab work
and working with software.  Or in astronomical terms, between working
with a simulation or with a telescope.  If you walk into an astronomer's
office, and see a pretty picture on his or her screen, you have no way
of knowing, <i>a priori</i>, whether you're looking at an observation
or a simulation.  So I grant you that a lab is not such a bad metaphor,
after all.  But what about your notes?

*Alice*: I suggest that each time we get together to work on our toy
model software, we keep some notes about the main decisions we make,
and the reasons behind them.  That will not take much time, and it
will help us writing documentation later on.  What is more, it will
enable us to go back and trace down why we did what we did, when we
come back to a piece of code a few weeks later, trying to debug it,
and wondering why we ever wrote this or that.

*Bob*: Now that sounds easy.  We can just scribble down some notes
while we go along.  Probably better to write that in a file somewhere,
rather than on scraps of paper.

*Alice*: Ideally, we should do better than that.  As part of a coherent
programming approach, it would be good to combine computer code AND
documentation text AND lab notes, all in one environment, with many
pointers from each of the two to the third element in the triangle.
But again, I won't try to convince you.  All I'm asking for is that we
both keep notes, together when we are working jointly, and individually
when one of us extends something in our toy model environment.

*Bob*: I have no trouble with that.  How shall we send each other the
notes we write down?  In the form of emails?

*Alice*: The best way would be to use a form of source code control.
The standard package to do this would be CVS, but an even better
alternative would be to use subversion.

*Bob*: Yeah, I've heard about that approach, from people who use it
when they work with big teams on forever-running projects.  But for
the two of us, for a toy project?  You accused me of a sledgehammer
approach, but I'm afraid you've just taken up quite a sledgehammer
yourself!

*Alice*: I bet you will like it, once you get used to it.  I started
to use it a few years ago, and now I use it even for things I'm working
on myself, like when I write a lengthy review article, and I want to
make sure I have all the information at hand, from previous revisions
of chapters.  But not to worry.  It's easy to set up a source code
control system, so I'll do it for us.  I'll show you a few commands
you can use to commit changes from your side, and to update your side,
to get the changes I have made.  These two commands are the most
important, in fact.  We should put everything under this source code
control system: the codes we write, whatever documentation I will add,
and the lab notes we will both keep writing regularly.

*Bob*: Is this really your last requirement, as you have promised,
before we can start to do some <i>real</i> programming?

*Alice*: Yes, this is it!  We can get started now.

*Bob*: I thought I never would see the day.  Well, as long as you take
care of this code control business -- what did you call it?

*Alice*: Source code control.

*Bob*: As long as you take care of it, and it really is as simple as you
said, I'm willing to try it.  Anything to get to the point of starting
our programming!





