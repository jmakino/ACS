= A Kitchen Sink Approach

== The Need for Combining Codes

*Alice*: But as long as only a few strange stars appear in a whole cluster,
why would you make your cluster evolution code so enormously more
complicated in order to try to handle those?  Surely the vast majority
of stars in any globular cluster will never hit another stars.

*Bob*: But I am not only interested in the vast majority of the stars.  I
would like to know the nature of X-ray sources and binaries containing
a millisecond pulsars.  By studying the properties of neutron stars
and black holes in binaries, we can learn a lot of fundamental
physics.  We can learn about dense states of matter in a way we
cannot possibly produce experimentally within a laboratory, and we can
learn about general relativity in strong fields, right up to the
horizon of a black hole.  And since black holes are black when nothing
falls into them, we have little choice but studying them in a binary
context.

*Alice*: I am certainly interested in these objects as well.  But wouldn't
it be good enough to first neglect the stellar evolution complications,
and later make up for them?  As I suggested earlier, you can use a
method of successive approximations.  First you model the whole
cluster history, and only then you take care of the special cases,
given that you already know the state of the environment they are in.

Let me give an example.  When you calculate the evolution of a single
star, it is possible to calculate the evolutionary history using only
a rather small network of nuclear reactions.  Then, if you are
interested in the amount of trace elements formed during the evolution,
you can go over the whole history once again, in a second pass.  You
then take the time dependence of of the structure of the star as given
by the first pass, and against this background you compute the
behavior of a much more detailed nuclear reaction network.

*Bob*: That wouldn't work here.  While it is true that the total number
of weird stars, affected by collisions, is small compared to the total
number of stars, it may not be small compared to the number of stars
in the core.  The densest cores in globular clusters contain only
about 1% of the total mass of a cluster, if that much.  And a large
fraction of stars in such a core may have undergone a close stellar
encounter during its lifetime.

So for the overall cluster properties, you may or may not be right,
that's an arguable point.  But for the detailed core properties, you
are certainly not right.  How the merger products behave in the core
affects the core parameters, and by changing the local environment,
all stars in the core are affected.

*Alice*: Well, maybe that is true.  And, come to think of it, there are
other dense stellar systems where encounters are more dominant.  If we
look at star-forming regions, it now seems pretty clear that many if
not most stars are produced by interactions between two or more
protostellar gas clouds.  So for those systems, you have no choice but
combining hydrodynamics and stellar dynamics and stellar evolution.
And if we look at galactic nuclei, we quickly realize that the stars
in the immediate neighborhood of a central massive black hole have
a mean time between collisions that is far shorter than the age of the
universe.

So, yes, while I still don't like the idea of a kitchen sink approach,
I must admit that there are certainly situations where one has no
choice but to go that route.  And the outcome of such calculations
apply to the formation of all stars, and the final stages of the most
interesting end products of stellar evolution.  Okay, I'm convinced now!

*Bob*: Glad to hear that!  And of course, I am not advertising a kitchen
sink approach for all stars simultaneously.  There is plenty of room
for stellar evolution tracks and binary evolution recipes, as we saw.
But that in itself is good and bad.  It is good in that it is easier
to check the accuracy of tracks and recipes than the accuracy of the
outcome of kitchen sink combinations of codes.  But it is bad in the
sense that the software package you wind up with is maximally
complicated.

Compare the situation with a mindless approach in which you do every
stellar evolution from scratch, without using stellar evolution
tracks.  In addition, you could compute the behavior of each binary by
letting two live evolution codes talk to each other, dump mass from
the one onto the other if need be, and adjust the orbital parameters
according to the mass and energy and angular momentum exchanged.  It
would cost a lot more computer time, perhaps, but it would certainly
make the code simpler.

In contrast, what I have in mind is a code that combines everything:
stellar dynamics, stellar hydrodynamics, stellar evolution tracks,
recipes for binary star evolution, a live stellar evolution code,
and yes, a live binary star evolution code as well.  I'm not looking
forward to putting all that together, but frankly, I see no other
choice.

== Parametrization

*Alice*: If you really succeed in putting together such a code, and you
start doing simulations with it, then what?

*Bob*: publish the results.

*Alice*: I suppose so.  But what have you learned from it?  We have agreed
now that such calculations should be done, but it is a different
question how to interpret them.

*Bob*: I don't see what the problem is.  After you do a detailed
simulation, you just make a reasonable choice of what figures to
publish, based on the most relevant physical parameters, and then you
write the text around it.  And for people who want to know more
details, you can make the raw data available.  What else can you do
with the results of a simulation?

*Alice*: I mean more than that.  You told me that you are interested in
learning more about the properties of neutron stars and black holes.
A theoretical physicist staring at your published figures of
Lagrangian radii is not going to find much deep insight in those.
In the end you want to come up with hard numbers describing some of
the properties of these exotic objects.

Now in order to do that, you have to find a way to characterize the
environment of these objects, at the very least the cluster core, as
you stressed already.  Since all stars interact with each other in the
core, we can only believe the details of a simulation if we can
believe the details of all physical processes that play a role in the
environment.

*Bob*: But we have already seen that, regrettably, it will be a very
long time indeed, before we will be able to give a quantitative
treatment of common envelope evolution and similar such ill understood
stellar evolution phases.  I think we just have to live with what we
can do.

*Alice*: I think we can do better.  Let us take the example of the
evolution of a single star.  Imagine that you were the first person
ever to attempt to do a computer calculation modeling the evolution of
a single star.  You want to write a computer code, but first you have
to decide how to represent the physics.  Now I walk by your office,
ask what you are going to work on, and then I tell you that it cannot
be done, since we don't know convection.  Sure, we can make order of
magnitude estimates, a form of dimensional analysis.  We can talk
about the typical size of a convective cell, but beyond that we know
next to nothing.  How can we possibly make quantitative calculations
about a star in which convection plays an important role, if we don't
even know the value of the most significant digit describing the local
state of a fluid in convective motion?

*Bob*: I imagine that I would be unhappy, hearing that, but that I would
argue that I would try anyway, just to see how good or bad the outcome
of my calculation would be.  After all, that is what happened
historically, and we're all glad people didn't give up in the late
fifties!

*Alice*: Well, yes, that is what happened, but there was an essential step
that was taken first.  Even without any detailed knowledge of convection,
a brave step was taken by introducing the concept of convective scale
length _h_.  And while there really was no reason to believe that this notion
had much physical meaning beyond an order of magnitude characterization,
it did provide us with a quantitative handle, a number _h_ that could be
determined afterward, by comparing theory and observation.

It was altogether possible that no single good value for _h_ would
have reproduced the observed structure and evolution of stars.
Perhaps convection should have been modeled by a function that is
fully dependent on temperature and density and chemical composition.
A priori you just don't know.  The happy surprise was that a _single_
number _h_ turned out to be enough to find rather good agreement
between the model produced and the stars observed.

What I would propose for a kitchen sink calculation of a globular
cluster, or any dense stellar system for that matter, would be a
parametrization of _all_ the unknown physics, by however many
parameters it would take.

== Comparison with Observation

*Bob*: You'll wind up with dozens of parameters, do you realize that?

*Alice*: I do, and yes, that may present us with significant problems.
But now it is my turn to say: what else can we do?  But I can say
more.  First of all, any globular cluster simulation already requires
you to choose dozens of parameters.  There is the choice of initial
model for the density distribution, if we assume the simplest case of
isotropy.  Then there is the choice of mass spectrum.  And for the
primordial binaries you have to give a prescription for the initial
distribution function, as a function of mass ratios, separation and
eccentricity.

*Bob*: And a prescription for the tidal field, which can be time dependent.
And perhaps you'd like to take into account disk shocking, when the
cluster passes through the galactic plane.  Yes, I know, there are
many numbers you have to specify already.  And these may well be
different for different globular clusters.

*Alice*: Exactly, that is my point.  If you can make detailed realistic
models for a number of different globular clusters, you have to provide
all those initial conditions, and vary them until you get good fits
with the observations.  So the situation will not be much worse if you
add a handful of parameters modeling the physical uncertainties.  And
the good thing is, a parameter describing the outcome of common envelope
evolution, for example, can be expected to be the same for different
clusters, at least in first approximation.  So the more clusters you
model, the less these extra parameters will cause problems.

Even so, it still seems worrisome to fit dozens of parameters to one
set of observations.  There is this nice quote about John von Neumann,
who certainly knew how to use computers to fit data.  He used to say:
"with four parameters I can fit an elephant, and with five I can make
him wiggle his trunk."

*Bob*: It just shows that globular clusters are more complicated than
elephants.

*Alice*: I don't think von Neumann would have agreed.  But it all depends
on the accuracy of the available data, and most importantly, upon the
lack of degeneracy in the types of data available.  And this brings me
to my second point.  If we would just try to fit density profiles and
velocity dispersions, we would be hopelessly lost.  The only thing
that can save us is detailed observations of specific types of stars,
such as binaries of different types.  In that way, we can hope to
obtain more independent constraints than there are independent initial
conditions.

*Bob*: Are you thinking of X-ray binaries and binary pulsars?  Given the
many hundreds of such X-ray sources that have been found in galactic
globular clusters, they can help constrain quite a few parameters
already.  And similarly, the wealth of pulsars that is being
discovered in globulars is constantly growing as well.

*Alice*: Yes, but I am also thinking about more normal binaries.  If
future observations can pin down more of the parameters of the current
population of binaries, we still will have to search around in
parameter space to find the original distribution, but at least that
will constrain the uncertainty quite a bit.  Primordial binaries
provide the largest number of free parameters in setting up a
realistic set of initial conditions for a cluster evolution, I bet.

*Bob*: That is right.  And yes, the observations are improving.  Also, I
don't expect to have a working kitchen sink code any time soon.  I fully
trust the observers to have quite a bit more data in hand by the time my
code will work and is debugged.

*Alice*: Is a code ever fully debugged?

*Bob*: Well, good enough is good enough, I'd say.  No person is perfect,
and no code is perfect.

*Alice*: Given that you have convinced me that a kitchen sink code is a
reasonable goal to work toward, I'm curious to hear a bit more about
how you intend to write such a code.
