= Chapter 2.  Stellar Evolution

== The Role of Stellar Evolution

*Alice*: I guess mass segregation had already been observed in equal-mass
systems, as soon as hard binaries were formed, since most of the time
these tight binaries behave as point masses that have twice as much
mass as single stars.

*Bob*: Yes and no.  If you would neglect three-body interactions, you
would be right.  And indeed, if you would start a simulation with a
significant number of primordial binaries, that would definitely be
the case.  But those simulations were done later, for the first time
in the late seventies, and more systematically in the early nineties,
after observations had shown that globular clusters have indeed
significant numbers of primordial binaries.  In contrast, if you only
deal with dynamically formed binaries, you don't see much subsequent
mass segregation.  First of all, such binaries are formed in the core,
where the density is highest.  Secondly, recoil from three-body
reactions will tend to kick binaries away from the center, even while
mass segregation will tend to let it move back toward the inner part
of the core.

*Alice*: I see.  So mass segregation was studied systematically only in
the seventies?

*Bob*: Yes.  Monte Carlo Fokker Planck simulations as well as direct
N-body integrations showed in detail how heavier stars tend to wind up
in or near the core of a star cluster.  At the same time, lighter
stars were seen to be more likely to escape from the system.  However,
the problem with these early multi-mass calculations is that they were
inconsistent.

*Alice*: Inconsistent?

*Bob*: They were not very realistic, by and large, since most of them
did not include the effects of stellar evolution.  In the real world,
while heavier stars do tend to sink to the center of a star cluster,
roughly on a two-body relaxation time, they also tend to burn up more
quickly.  As you know, a star 25 times more massive than the sun burns
up a 1000 times faster.  So it would be totally unrealistic to follow
a multi-mass system, and to leave the heavier mass particles in the
center, just sitting there.  You really would have to remove them, or
at least most of their mass, as soon as their age exceeds that
dictated by stellar evolution.  Few multi-mass simulations did that.

There were some exceptions.  In the early eighties Aarseth and
coworkers in their simulations began to remove some or all of the mass
of stars after they reached the end of their life.  And in the late
eighties, Chernoff and Weinberg provided a treatment of mass loss from
stellar evolution, in a Fokker-Planck calculation.  But most
multi-mass simulations in the seventies and eighties did not include
stellar evolution.

*Alice*: But it can't be that hard to just diminish the mass of a point
particle at the end of its nominal lifetime, from that of a main
sequence star to that of a white dwarf, neutron star, or black hole,
as the case may be.

*Bob*: Or put it equal to zero, for those stars where you believe that
no remnant is left at all.  Yes, that can be done, but that will not
get you very far.  Fiddling only with the masses of stars is not
enough.  You would then be faced with the question: what to do with
binary stars?  Take two stars in a binary with a semi-major axis that
is much larger than the some of their radii while they are on the main
sequence.  Soon before the heaviest star will end its life, it will
want to evolve into a giant phase, and as a result, it may well dump
much of its mass on its companion star.  How are you going to model
that?  Not by simply removing the mass from the system.  That would
not do justice to a large fraction of close binary evolution -- and
close binaries are exactly the dynamical heat source of an N-body
system.

In other words, the only way to make a meaningful simulation of a
non-equal-mass system of particles is to add a realistic form of
stellar evolution, for single stars and especially for binary stars.
Nothing else makes sense to compare with observations.

== The Case for Including Stellar Evolution

*Alice*: You like to make strong statements, but I can make a few too!
We don't know much of anything about the quantitative evolution of
close binaries, so what is the point of adding a largely unknown
quantity to an otherwise very detailed stellar dynamical situation?

But let me answer my own question, if I may.  We know that the
dynamical evolution of a star cluster is not very sensitive to the
nature of the central heat source.  As long as we know the amount of
heat lost at the half-mass radius, the core will adjust itself to
whatever size it needs to have to let the central heat source produce
the amount of energy needed.  The situation is quite similar to the
evolution of a single star, such as the sun: given the heat lost from
the photosphere, the central temperature and density can adjust quite
easily to create the right amount of energy in nuclear reactions.

*Bob*: Well, that may give you a rough agreement for the overall
structure, but for me that is not good enough.  I am interested in
predicting the number of X-ray binaries and millisecond pulsars and
their binaries, as well as the physical characteristics of those
systems.  They are important from an observational point of view.
As you know, a significant fraction of X-ray binaries are located in
globular clusters, even though these clusters contain much less than
1% of all the stars in our galaxy.  It is clear that the vast majority
of those X-ray binaries in clusters are formed through dynamical
effects, as a result of the high density of stars.

*Alice*: But you can do that in a two-step process.  First you make a good
enough dynamical model of the dynamical evolution of a star cluster.
Since X-ray binaries and binary pulsars form only trace populations,
neglecting them will not greatly influence the overall dynamical
evolution, I would think.  Now, having done that first step, you can
then statistically sprinkle your exotic binaries into the outcome of
your first step.  This second step will have a Monte Carlo nature, but
who cares?  Your precise N-body calculation started of from random
initial conditions, after all.

*Bob*: Not so fast.  Have you ever looked at the HR diagrams based on
recent observations of globular clusters?

*Alice*: Not recently, no.  I do remember that astronomers traditionally
talked about something like a second parameter effect.  What was that
again.  I believe that metallicity was the first parameter, and that
it was claimed that you need at least one more parameter.  In other
words, two clusters of the same metallicity can still show different
types of HR diagrams.

*Bob*: Yes, that was a long time ago, even before the first observations
with the Hubble Space Telescope.  Since then we have learned that
there is an amazing variety of morphologies in the HR diagrams.  And
here we are talking about large numbers of stars, not your occasional
X-ray source or other exotic object.  The only way we are ever going
to explain the structure and evolution of globular clusters is by
making our simulations detailed enough to at least be able to
reproduce, say, the position and shape of the horizontal branch in
each well observed globular cluster.

*Alice*: Okay, you make a strong case for including some form of stellar
evolution right away, when performing a star cluster simulation,
besides the basic stellar dynamics.  The main question then is: how
much?  What is wrong with using a rather crude toy model?

*Bob*: I can't imagine modeling globular clusters on a cluster by
cluster basis using such a rough approach.  I'm afraid we have to do
better than that.  At the same time, I know that there are large
uncertainties in our understanding of contact binary evolution.  But
what can we do?  We do the best we can, and hopefully, `the best' will
get better every year, perhaps when our stellar evolution colleagues
will use two- and three-dimensional models.

*Alice*: I won't hold my breath.  But just to set the stage, why don't you
continue your historical summary.  When did people start using more
detailed stellar evolution information in their dynamical simulations
of globular clusters?

== Tracks and Recipes for (Binary) Star Evolution

*Bob*: It was only in the nineties that a few groups began to used
look-up tables for single star stellar evolution, and simple recipes
to describe what would happen to the evolution of binary stars.  While
they followed the stellar dynamical evolution in the point mass
approximation, as before, their stars would loose mass, and their
binary stars would exchange mass, in a roughly realistic fashion.

Actually, it is pretty amazing that it took so long for stellar
dynamics and stellar evolution to be come together, even in this
rather rudimentary way.  It could easily have been done twenty years
earlier.  After all, in the early seventies, single star evolution was
well understood, at least in outline, and plenty of evolution tracks
had become available.  And the main phases of binary star evolution,
including the conditions for gradual and catastrophic mass transfer,
had been analyzed as well.

*Alice*: Yes, I remember learning about cases A, B, and C of mass transfer
in binary stars, during the undergraduate course in stellar evolution
that followed.  Why do you think people waited twenty years?

*Bob*: I don't quite know.  But if I were to guess, I think the problem
is that the bridge was made from stellar dynamics to stellar evolution.
Stellar dynamicists had enough problems on their hands as it was, to
figure out what happened during core collapse and afterward, with
gravothermal oscillations.  Only after all that had been sorted out,
and especially with the observational discovery of primordial binaries,
did an urgent need for stellar evolution treatments make itself felt.

*Alice*: But you had just convinced me that mass segregation simulations
were unrealistic without stellar evolution.  This would suggest that
Aarseth should have added stellar evolution to his code in the early
sixties!

*Bob*: Well, he was the first one to do so, as I mentioned, but I guess
he too had more urgent problems to deal with early on, such as the
treatment of close binaries, inventing and adapting regularization
schemes to avoid numerical errors during close encounters.  But in the
end, I think astrophysicists have inertia, like all human beings:
stellar dynamicists just won't get excited about bringing in stellar
evolution in their codes until they really have to.

*Alice*: So why did stellar evolution folks not add dynamics to their
simulations, or at least knock on the door of dynamicists?

*Bob*: They, too, could have and perhaps should have done so in the
seventies.  They did start to do population synthesis studies around
that time, Tinsley and others.  But of course, they too had other
worries to take care of first.  It was already a big job to get single
star evolution treated correctly in a statistical fashion, for a
complete star cluster or a whole galaxy.  Then, and even now, there
are significant questions about what exactly happens to various types
of stars in the late phases of their evolution.  And when people
started to add the signatures of binary star evolution, they quickly
realized how little we know about many important phases of close
binary evolution, and contact binaries in particular.

So it is perhaps not too surprising that they did not rush to put in
dynamical effects of stellar collisions.  Even though blue stragglers
had been known to exist in star clusters since the fifties, it was not
yet clear how important collisions were in producing those stragglers,
notwithstanding the pioneering work by Hills in the seventies.  In
fact, the first conference dedicated to blue stragglers was held in
the early nineties, and the first conference on stellar collisions
took place in the year 2000!

In contrast, tidal capture of a neutron star by a main sequence had
been studied in the mid seventies, because it seemed to be a promising
route to explaining the large overabundance of X-ray binaries in
globular clusters, which I mentioned before.  But that type of
analysis was largely local, studying individual encounters, rather
than modeling the behavior of a whole cluster in much detail.

*Alice*: Given that it took twenty years for stellar dynamics and stellar
evolution to meet, in evolution recipe mode, once they met it must
have produced a major shift in star cluster modeling.  Before that,
most simulations were exercises in mathematical physics, a lot of fun
in itself, but of limited use for comparison with observations, as you
have just argued.  But once you put in tracks and recipes, aren't you
in a good position to start explaining the observational data?

*Bob*: No, not really.  I consider the stage of using recipes as not
much more than playing scales, as warm-up exercises before moving on
to the type of kitchen sink simulations that I mentioned before.

== Limitations of Tracks and Recipes

*Alice*: Let me come back to my previous objection.  You admitted that we
know rather little about the evolution of contact binaries.  What is
the point of computing detailed stellar evolution models if there are
vast areas of stellar evolution where we don't even have a clue of
what is going on, beyond a qualitative hand-waving hunch?  Take the
case of common envelope evolution.  If a white dwarf starts to spiral
in toward the core of a red giant, it seems plausible from an
energetic argument that the envelope of the giant might be lost before
the white dwarf reaches the core, leaving behind a tight pair of what
will look like two white dwarfs.  And it seems equally plausible that
not enough mass is lost not fast enough, and that the white dwarf will
merge with the core.  Detailed 3D calculations of this process are
very hard to do, given the fact that the initial stages cover very
many dynamical crossing times.

Given this fundamental uncertainty, why bother doing detailed
calculations elsewhere in a simulation?  A chain is as weak as its
weakest link.  It seems like a waste of computer time and software
effort to build some very strong and detailed links as long as other
links such as common envelope evolution are too weak to contemplate.

*Bob*: To some extent I agree with you.  First of all, okay, I see no
use for live stellar evolution codes to compute the evolution of
single stars, within a stellar dynamical simulation.  And as a second
okay, I agree that even the evolution of primordial binaries can be
treated adequately through a combination of recipes and stellar
evolution tracks.  Where I differ from you is in my view of the
treatment of merger products.

*Alice*: Before we get to differences, let me point out that your first
and second okay are very different types of okay.  The first one
applies to the use of relatively accurate and robust information.
There is pretty good agreement between different stellar evolution
experts as to the quantitative behavior of the tracks of normal stars,
apart from perhaps the very most massive stars.  Your second okay
addresses the use of rather ad hoc and rough treatments of binary star
evolution, where quantitative certainty is far less good, some would
say almost absent.

*Bob*: Yes, I agree with all that, but what can a poor boy do?  We do
the best we can.  And in order to get at least some new insight in the
evolution of star clusters, I think those tracks and recipes are good
enough, even though the latter are far from ideal of course.  But let
me move on to my main difference with your view.  Whenever two stars
collide with each other and merge, you wind up with a merger product
that is totally unlike the type of normal ZAMS (zero age main
sequence) star that stellar evolution tracks all start with.

A merger remnant has not only a very different metallicity than
ZAMS stars in the same cluster had, what is worse, the chemical
composition is different at different radii in the star, due to
incomplete mixing during the collision, which in general will be
significantly off center.  In addition, for a hundred million years
or so, the merger product will be out of thermal equilibrium, and
therefore will have a quite different structure from a normal star.

The only way you are getting even roughly close to determining the
structure of such a star is to use not only a live stellar evolution
code, but also a live hydrodynamics code to follow the collision.
So what I envision is that when two stars come close together within
the stellar dynamics part of the simulation, these point particles are
handed over to a hydrodynamics code, which replaces them with blobs of,
say, SPH particles, layered in the proper way as specified by the
stellar evolution information in the simulation.  From this point on,
the power of SPH is let loose until we arrive at a dynamically settled
merger remnant.  A stellar evolution code will then follow the thermal
settling, as well as the subsequent more normal evolution.

But if this does not convince you, consider what happens subsequently
with a merger remnant.  It will be formed in the core of the cluster,
most likely, since there the chance for collisions is highest.  It
will remain in the core, since on average it will be more massive than
typical single stars.  Therefore, it will stand a significant chance
to undergo yet another collision, or be captured as a binary member in
an exchange reaction.  Even if it avoids collision through such a
three-body dance, subsequent evolution is likely to lead to mass
overflow.  How can you possibly use recipes to treat mass overflow
between stars that are not parametrized only by mass and chemical
composition, but by the full functional dependence of weird
composition gradients, and are possibly still out of thermal
equilibrium?

To sum up, while it is possible to make tables of stellar evolution
tracks for unperturbed stars, and while it is just possible to combine
pairs of these tracks with elaborate prescriptions for any conceivable
combination of two stars in orbit around each other, it is utterly
impossible to prepare beforehand for all types of strange merger
remnants that can be formed, let alone for their subsequent interactions.
