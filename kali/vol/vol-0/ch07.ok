=  Stellar Evolution and Hydrodynamics

== A Minimal Vision

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

== A Top Down Approach

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

== Choosing Ingredients

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

== An Interface Specification

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
