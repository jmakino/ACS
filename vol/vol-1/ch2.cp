= Chapter 2.  The Kali Code

== An Educational Project

=== A Toy Model on the Web

*Alice*: I really like your idea of writing a toy model for a code to
similate dense stellar systems.  While it will be useful for your
students, I'm sure we'll learn from it ourselves.

*Bob*: I don't doubt it.  For one thing, you'll see how hard it is to
then mature the code into a real research tool.

*Alice*: I'm not so sure.  Given that there is a great demand for such a
code, and precious little supply so far, I wouldn't be surprized if
even a toy model would turn out to be useful already, if we do a good
job.

*Bob*: I would be very surprized, but as we already concluded, no sense
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
and asking my thesis advisor.

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

*Bob*: I agree.  That seems the most natural thing to do.  I have benefitted
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

*Alice*: The easiest way would be to use a type of open source licence,
preferably a rather simple one, which at least includes the usual
disclaimers.

*Bob*: Perhaps it is time to have a look at the web, and do a search
for `open source.'

*Alice*: Here is something: "http://www.opensource.org" with a long list
of open source licences.

*Bob*: That was quick!  Let's see what they say.  Hmmmm.  Most of them
are far too long to read, let alone figure out, for my taste.  Isn't
there a simple one?

=== Open Source Licence

*Alice*: I remember someone mentioning the X window system as having a
straightforward licence.  Here, that is probably the MIT licence.
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
window people have lived with this licence for all that time, there
is perhaps not too much danger involved.

*Bob*: Not if our toy model will become as famous as X, but I doubt
that we will see the day of an N-body model for each man, woman and
child.

*Alice*: Maybe the best solution for now is to start with the MIT
licence.  I presume that we can always later replace it with a more
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
module the responsability to trigger the other module to advance in
time, at the right time.  It may be better to introduce a separate
scheduler module, that takes care of all the timing management.

So the scheduler will inform the SD module which stars have to be updated
next.  In this case, this will involve taking one integration time
step forwards.  At the same time, the scheduler will inform the SE module
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
percent per timestep.  And given other approximations, you might decide
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

*Alice*: The choice of language deserves a seperate discussion; let's
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
binary, with a semimajor axis that is a thousand times smaller than
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

*Bob*: No no, that would be far more complex.  But it would be a stap in
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
time we'll have a lot of actual code to work with, to strenghten our
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

*Alice*: You suggested an individual timestep scheme, and I think that
is fine.  Actually, we may want to start with shared timesteps, but
that is a matter of presentation.

*Bob*: Yes, but the choice of time step is only one choice we have to
make.  We haven't choosen an integration scheme yet.  If you want to
start with shared timesteps, you can do that using a leapfrog
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
of a leepfrog integrator, apart from a few coefficients which would be
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
was in basic, and soon afterwards in C.  Nowadays, when students talk
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
integrator, and how to analyse the results.

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

*Alice*: Does that mean that when we develop our toymodel under Linux,
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
for it, or signed any licence.

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
to a differen graphics package in the future, it would be very unpleasant
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

=== Literate Programming

*Alice*: Not quite.  We have talked about writing programs and providing
an environment, but we haven't said anything yet about documentation.

*Bob*: Well, my class notes will be the documentation, I thought.

*Alice*: 

=== Coherent Programming

*Alice*:

*Bob*:

=== A Lab Note Mechanism

*Alice*:

*Bob*:

= old stuff (040120)

#In this context Alice mentions one extreme: the Sussman/Wisdom book,
#which appeals to them in many ways.  Bob vaguely has heard about Lisp,
#never about Scheme.  When Alison takes the book from her bookshelf,
#Bob is shocked by the parentheses.  Is this computer code, he asks.
#Alice talks a bit about it, but agrees that the isolation of Scheme
#may be too much.  They discuss Perl and Python as more practical
#compromises, better than C and C++ and Fortran, which Bob and Alice
#both have experience with, but which Alice has grown frustrated with.
#Bob doesn't mind either C++ or Fortran, but he does like the
#flexibility and power of quick expression of Perl and Python.
#
#Alison mentions a bold idea: would it be possible to write a N-body
#code in such a more flexible language?  Bob: anything is possible, you
#can do it in Basic or Awk if you want.  Alison presses her point, and
#Bob, more out of politeness than anything else, listens.  For the sake
#of curiosity, he asks her what she has in mind, mostly to show her how
#wrong she is.  After some more discussion he has to admit that the idea,
#while impractical, may not be as ridiculous as he first thought.
#Seeing it still as a game, he starts thinking out loud with Alice how
#they would do such a strange thing.
#
#Oscillating between Perl and Python, as the most realistic
#compromises, they are also afraid that a compromise may have the
#disadvantages of both sides, and they both feel that it would be
#better to have a single coherent vision that they can agree with.
#Coherence is better than committee decisions.  In short, they want a
#pragmatic approach.
#
#Just for fun, since they are sitting behind a terminal, they decide to
#type in "pragmatic programming", in the hope to find something there,
#more as a joke.  To their surprise they find the two books of two
#characters that call themselves the Pragmatic Programmers.  They
#browse through a few pages of their first book and like it a lot,
#since it really reflects their own experience.  Like Kernighan and
#Pike, but addressing more broadly the whole of software development,
#says Bob.  Like Knuth, very sensible, but a generation later, says
#Alice.
#
#Then they look at the second book, about Ruby.  Since they already
#thought about Perl and Python, they give it a closer look, and become
#convinced that this may be better than just a comprise.  They decide
#to give it a try.  After all, 90% of an N-body code takes less than 1%
#of the compute, so who cares about the speed of an interpreted language.
#It is flexibility and clarity and rapid prototyping and recycling that
#counts.  What to do first?  Get familiar with the language, and write
#something simple but actually useful, in order to see how it works out
#in practice; in other words, a pragmatic program.
#
#Alice suggests to write a constant time step leapfrog.  Bob laughs and
#says that he recognizes that Alice has also worked in cosmology.
#Alice counters that Bob's SPH experience must make him feel happy with
#leapfrog.  Bob says that, yes, for problems with softening that might
#not be too bad, but now they want to do stellar dynamics of stellar
#systems, and they should be able to handle point particles.  At the
#very least they need variable time steps, and much better even
#individual time steps.  He quotes the classic paper by Makino and Hut
#(1988), Performance Analysis of Direct N-Body Calculations, to make
#his point.  And by the way, when you get close encounters, the
#leapfrog sucks.  Fourth-order schemes are a must, and Hermite is the
#most elegant scheme.
#
#Alice agrees that something like NBODY1h would be a good idea, but she
#laughs at the idea of learning a whole new computer language by starting
#to write such a complicated code from scratch.  Shouldn't you first learn
#to do I/O and to write a simple class for individual particles and for
#the whole system?  Sure, says Bob, a bit annoyed that Alice seems to use
#her longer experience and seniority to push for what he thinks is an
#overly indirect and long-winded approach.  He is still not completely
#used to have tenure, and he automatically feels the tension between
#what he thinks is best and the need to be respectful to more senior
#professors.  But, Bob tries once more, once you write a leapfrog, it
#wouldn't take many more lines to write a Hermite code, right?  And
#to go from equal to variable time steps is also only a few more lines,
#basically having a time step criterion, which in an interpreted language
#is rather simple to add.  And even block time steps isn't that big of
#a deal, once you figure out what to do, something you pick up from
#other codes anyway.
#
#Alice has to admit that an equal-timestep leapfrog code will probably
#be more than half as long as an individual time step block time step
#code with Hermite.  But there is another argument, she adds.  The main
#reason to use an interpreted object-oriented language like ruby is to
#be able to rapidly switch between classes and methods, changing
#inheritance and mixins; all so much more easily than in C++.  So by
#providing both a leapfrog and a Hermite code, they can get valuable
#experience in switching between methods.  If they really get serious
#in writing a full-fledged dense stellar systems code, they surely will
#need to be able to switch methods, at some point.  And to get experience
#doing so early on will help them, first of all, to decide whether ruby
#is really the right language for the job, and secondly, if they go
#with ruby, they will then have the necessary experience to use ruby in
#a way that makes such method switching natural.
#
#Meanwhile, although defending her point of view eloquently, Alice is
#having her doubts too.  She knows how easy it is to waste time when
#trying to do something that is too elegant.  However, before she can
#say that, to her surprise Bob brings up a point in favor of starting
#with a leapfrog.  If they would start with a complete NBODY1h type
#code, he says, they would probably just translate such a code line by
#line into ruby.  That will probably work, but they run the risk that
#in that way they never really get a flavor of the language, and so
#will not be able to tap the real power of ruby.  That, for him, is a
#more important argument to start with the leapfrog.
#
#Alice has to laugh again, and tells Bob that she was just about to
#agree with him that starting with NBODY1h would not be such a bad
#idea after all, but that she must say that Bob's latest argument is
#certainly convincing too.  What she doesn't tell Bob is that she
#thinks it is funny that he could agree to her idea only after he
#came up with a different argument for it; even so, she appreciates his
#independence and critical attitude.  Bob, meanwhile, has to smile too,
#realizing that Alice is not as inflexible as he had thought, and
#reminding himself at the same time that he now has tenure, after all,
#and that there really is no reason not to push his own agenda and use
#his own judgement.
