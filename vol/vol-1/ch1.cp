= Chapter 1.  Modeling Dense Stellar Systems

== Introduction

=== A Chat between Two Astronomers

*Alice*: Hi Bob, what are you up to?

*Bob*: Well, to be honest, I'm not quite sure yet.  I'm in between
projects right now.  It's nice to sit back for a bit, looking at the
whole field of astronomy, before plunging into a new project.  But
realistically speaking, it is likely I will continue working on star
clusters.  How about you?

*Alice*: Actually, I'm in a similar situation of having finished old
projects and not yet taken on a major new one.  Since I moved here, a
month ago, I've been dealing with all kind of chores that had been
accumulating, but now I have a clean desk, and I'm ready to start up.

*Bob*: The previous time I felt like this was when I had handed in my
thesis.  I was surprised to find myself suddenly in a pleasant vacuum,
after rushing so much to get everything finished in time.  It was soon
afterward that I started to get involved in various parallel projects,
which never seemed to come to an end.  That was eight years ago.

*Alice*: I heard you just got tenure.  I guess that has something to do
with your finishing up your two latest projects?

*Bob*: how did you guess ;>).  While I enjoyed my work, I did feel a
little constrained.  There were times that I would have loved to go in
some other directions, but that probably would not have been wise to
do at that point.

*Alice*: I think you made the right decision.  When I got tenure, about
ten years ago now, I was in a different situation.  We were in the
middle of a large cosmological project, simulating the formation of
large scale structure in the universe, working together with people
from various teams.  It was an exciting time, in which there were a
number of basic questions that we could address for the first time.

*Bob*: I guess cosmology has now become as detailed a modeling job as
any other field in astronomy?

*Alice*: Yeah.  Now that the standard model is pretty well understood,
cosmology has become a rather ordinary field in astronomy.  But I
never specialized in cosmology.  While most of my work has been in
stellar dynamics, I seem to keep moving between different fields.
For example, I just finished a paper in planetary dynamics on the
dynamical formation of binaries in the Kuiper Belt.

*Bob*: I'm certainly familiar with your publications in cluster dynamics,
especially the analytical treatments you have given.  They were quite
useful for my more applied simulations.  And didn't I see a paper of
yours on black hole dynamics recently?

*Alice*: Yes, that was on the question of whether two massive black holes
spiral in within a Hubble time, after they have been brought into
proximity by the merging of their parent galaxy.  And indeed, I tend
to work on more analytical questions, though I enjoy doing large
simulations too.  Comparing the two and figuring out from both sides
why there are discrepancies is most fun: it keeps surprising me that
you can use pen and paper to predict roughly what a computer will come
up with after performing a trillions of floating point calculations.

*Bob*: Which of all those fields in stellar dynamics do you find most
interesting, having worked on all length scales between asteroids and
cosmology?

*Alice*: Right now I would say star clusters, especially those star
clusters where there is an appreciable chance for stellar collisions
in the densest regions.  Dense stellar systems, in other words.

*Bob*: As you know, this is close to my interest and background.  But
let me play the devil's advocate for a moment.  What is so interesting
about dense stellar systems, in particular?

=== Dense Stellar Systems

*Alice*: A quarter century ago, when I was an undergraduate, astronomy was
much less unified than it was today.  People observing in different
wave length bands did not talk as much with each other as they do
today, and theorists studying stars, star clusters, galaxies, and
cosmology had even less overlap in their research.  Someone simulating
the birth or evolution of a star; someone simulating a star cluster;
someone simulating a collisions between two galaxies; or someone
studying large scale structure of the universe -- all four of them
would be working separately, each on their own island.  In contrast,
we now see bridges everywhere.

When studying large scale structure simulations, we see how normal
galaxies are build up through the process of merging smaller galaxies
and protogalactic gas clouds.  And when we study a collision between
two galaxies, we see how star clusters are formed through the shocks
that occur in the bridges and tails connecting them.  So this already
connects the largest three categories from the largest scale down.
Similarly, when we start at the bottom, studying the formation of an
individual star, we have learned that we can only understand the
formation process in an environmental way, taking into account the
interactions between many protostellar clouds simultaneously.  So this
forces us to study a whole star forming region, a proto stellar
cluster.  In this way all four fields are now connected.

It is fair to say that dense stellar systems form an central topic in
the current trend toward unification of astrophysics.  Not only is the
topic connected with structure from the scale of individual stars up
to that of galaxies and beyond, it also is connected with the study of
extreme forms of matter, including neutron stars and black holes.  And
finally, on the level of simulations, it combines the simplest and
most elegant theory of complexity, that of self-gravitational systems,
with the a diverse mix of astrophysical effects, as seen in stellar
evolution and stellar hydrodynamics, including the physics of
accretion disks, common envelope evolution, and so on.

*Bob*: That is all nice and fine, from a high-level point of view.
However, when I'm working in the trenches, so the speak, I do not have
much use for the big picture, and my only job is getting the details
straight, and to get the work done.

*Alice*: Of course, the details are what counts, in any real piece of
research, if you want to get any real work done.  But it is equally
important to keep sight of a broader picture.  If not, you'll still get
a lot of work done, but not necessarily in an efficient way, and it
will not necessarily lead to interesting results.

*Bob*: I agree that you have to be sensible.  But you can be sensible
without grand pictures and declarations, I would think.  How can a grand
picture help me to get my work done?

*Alice*: There are many examples.  Just to name one in stellar dynamics:
the introduction of tree codes in the mid eighties made it possible to
do the type of cosmological simulations that we are all familiar with.
Until then, there was a huge gap between P^3M codes and direct summation
codes.  The breakthrough came not by improving details in a particular
piece of work in a given project, but by stepping back and rethinking
the whole approach to large-scale simulations in stellar dynamics.

*Bob*: I guess you could call that a paradigm shift.  But such shifts
are few and far between.  I don't think you can take that as a typical
example.  What I see as the future of dense stellar systems simulations
doesn't require any paradigm shift.  Computers are getting fast enough,
and what is needed is to write more software, of the type that can
handle not only stellar dynamics but also stellar evolution, on the fly.
And we can throw in hydrodynamics as well.  In short: what I see
happening is a type of `kitchen sink' approach to simulating dense
stellar systems.

*Alice*: That doesn't sound very attractive.  At the very least I hope you
come up with a better name!  But more seriously, why would you want to
do that?  And even if you were to do that, why would anyone believe
the results from such a monstrous combination of complicated codes?

*Bob*: I think we have no other choice.  I grant you that idealized
abstractions have their use, by now we have learned enough about a
system of self-gravitating point masses, all of equal mass, and it is
time to move on.

*Alice*: Well, moving on beyond equal-mass point masses, and doing your
kind of kitchen sink simulation, adding stellar evolution and
hydrodynamics, those are quite different concepts.  Surely there is a
middle ground between the two!

*Bob*: Not really, I would say.  It may sound strange, but I think
there are only two reasonable ways to simulate a star cluster: either
you use the most extreme idealization, in which all stars have the
same mass, and with radius zero; or you couple every star to its own
stellar evolution program, to model its internal degrees of freedom,
at least in principle.

*Alice*: Strange indeed.  Please explain.

=== The Equal-Mass Point-Particle Approximation

*Bob*: Let us first look at the history.  I presume you know how star
cluster modeling got started.  You have seen more of it than I have.

*Alice*: When it all got started, I had barely arrived on this planet, and
I wasn't reading the Astrophysical Journal yet, or anything else for
that matter.  But yes, I know the rough history.  It took some 25 to
35 years to understand the dynamical evolution of a star cluster,
modeled as a collection of equal-mass point particles, depending on
how you count, since the earliest N-body calculations were performed
on modern computers, around 1960.  But you know the details better
than I do.  I guess it is your turn to do some summarizing.

*Bob*: During the sixties, simulations by Aarseth, Wielen, and others
showed how three-body interactions form the dynamical engine at the
heart of an N-body system, the agents of change in their evolution.
Even if no binary stars were present initially, they would be formed
dynamically in simultaneous three-body encounters.  And once there,
encounters between these binaries and single stars would complicate
the meek heat flow by two-body relaxation: a single scattering
encounter can suddenly release a large amount of energy when a binary
increases its internal binding energy by a significant amount.

During the seventies, Henon and Spitzer and co-workers used
approximate Monte Carlo Fokker-Planck simulations to model the
contraction of the core of a star cluster, on a time scale that is
only an order of magnitude larger than that of the half-mass
relaxation time scale.  This so-called gravothermal catastrophe was
predicted in the sixties, and actually observed by Henon in
rudimentary form in the sixties as well.  But it was seen much more
clearly in the seventies in statistical simulations.  While more
difficult to observe in actual N-body calculations, because of the
still low number of particles, a few hundred at most, they were seen
there as well.  In fact, one could even argue that in the late
sixties, some N-body simulations already showed hints of this effect.

In the eighties, finally the behavior of an equal-mass point particle
system after core collapse was elucidated, first by Sugimoto and
Bettwieser, who showed that a post-collapse cluster can undergo
so-called gravothermal oscillations, a series of local mini collapses
and expansions in the very center of a star cluster.

*Alice*: And then Goodman gave a detailed stability analysis in which he
predicted the minimal N value for which an equal-mass N-body system
would show such behavior.  At that point, theoretically the equal-mass
evolution was well understood, wouldn't you say?

*Bob*: I only consider something understood if it comes out of my
simulations, in a repeatable and robust way.  I have seen too many
semi-analytic predictions come and go in my young life to have too
much trust in those!

*Alice*: What Goodman did was finding roots in the complex plane of
relatively straightforward and certainly well defined equations; I
wouldn't call that semi-analytic, and I certainly would trust the
complex plane a lot more than the complexities of any complicated
simulation.

*Bob*: I guess we're talking about matters of taste here, though many
would think this an odd thing to say about `hard' science.  But moving
right along, let me make my main point.

It would be another decade before the predictions would be tested in
simulations.  In the late eighties, various simulations based on
Fokker-Planck approximations as well as gas models verified what
Sugimoto and Bettwieser had seen in the early eighties, and in the mid
nineties saw the first observation of gravothermal oscillations in a
real N-body simulation, by Makino.  In a way, this was the end of a
chapter in stellar dynamics, and further progress had to come from
more realistic systems.

*Alice*: But surely people had used a mass spectrum long before that.

*Bob*: Yes, in fact the very first paper by Aarseth already described a
multi-mass simulation, in the early sixties.  But it was only in the
seventies that more detailed studies elucidated the main physical
mechanisms of mass segregation.

== Stellar Evolution

=== The Role of Stellar Evolution

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

=== The Case for Including Stellar Evolution

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

=== Tracks and Recipes for (Binary) Star Evolution

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

=== Limitations of Tracks and Recipes

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

== A Kitchen Sink Approach

=== The Need for Combining Codes

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

=== Parametrization

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

=== Comparison with Observation

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

== Software Architecture

=== Legacy Codes

*Bob*: The basic idea is to start with an N-body code, then to add
stellar evolution tracks and binary evolution recipes, then to add a
stellar evolution code, and possibly an extra version of it that can
handle binary stellar evolution, then to put in a hydro code as well,
and make it all work together.

*Alice*: That reminds me of a description of a human being as worth a few
hundred dollars in terms of chemical ingredients.  Just how to put the
ingredients together to make a human being is not that simple.

*Bob*: That's not a fair comparison.  That would be like saying that I
had to start with ones and zeroes, or had to start at the byte level.
Fortunately I can start with so-called legacy codes: codes for stellar
dynamics, stellar evolution, and stellar hydrodynamics, that have been
developed over many years by many people.

*Alice*: Okay, I admit, my example was too extreme.  I should have said
that you take a fish and a reptile and a bird, and put it together to
make a primate.

*Bob*: Still not fair, but I won't argue with analogies.  I can only
repeat our mantra: what else can we do?  You're not proposing that I'm
going to write a kitchen sink code from scratch, do you?  That would
mean reproducing more than a hundred person-years work of code writing!
In fact, that would be an apt use of your analogy of the chemical worth
of a human being.

*Alice*: I'm not sure what the best solution is, but I am sure that there
are various ways that are definitely not the best.  For example, if
you don't make your kitchen sink code modular, you are asking for
trouble and you will become bogged down hopelessly.  Also, if you do
not insist that the legacy code providers give you completely robust
codes, you're inviting disaster.  Can you imaging handholding some of
your stars, because they don't know how to complete their full stellar
evolution track through difficult phases along the giant branch or
horizontal branch?  With a few hundred thousand stars, you can't
afford holding even 1% of them by the hand!

*Bob*: Your second point is certainly well taken.  Until recently, in
fact no stellar evolution codes could really run by themselves, in a
fully automated fashion.  However, there now are several experts in
stellar evolution who have developed more robust codes, and who have
shown an interest in sharing their codes with me.  Still, it will be
up to me to connect all the codes, from stellar dynamics to stellar
evolution to hydrodynamics.

*Alice*: That brings me back to the notion of modularity.

*Bob*: What exactly do you mean?

*Alice*: If you can define a set of interfaces, between each of your
legacy codes, through which a limited number of exactly defined
variable values can be passed, your life will be a lot simpler.

*Bob*: Forcing yourself to do that will be very complicated, and
frankly, unnatural.  How can such an exercise in formality help you?

*Alice*: What is your alternative, what do you consider natural?

*Bob*: Any stellar dynamics legacy code I have ever seen has many
different places where two stars can come in close proximity.  Think
about the various regularization techniques, as well as the ways in
which perturbers are treated, not to mention stars with unusually high
velocity that can suddenly show up unexpectedly.  All these places in
the code can and probably should be coupled to a hydro code, since in
all these cases a stellar collision or close encounter can occur.

*Alice*: That will never work well.  Even if you can get it to work ever,
based on a particular legacy code, before you know it either the
legacy code will change in small and subtle ways, throwing you off, or
you may decide to ask slightly different questions, requiring you to
revisit all these many connection points between the two codes.  And
you want to do this with at least three legacy codes and put in tracks
and recipes to boot!  This will be hopeless.

*Bob*: Do you have a _realistic_ alternative?  Surely your formal
picture of limited well-defined interfaces can't possibly be
implemented in a real stellar dynamics code with regularization and
perturbers and all that.  You'll have to rewrite everything from
scratch, and even then, it will be far slower than your old code.

=== Modularity

*Alice*: Let us start with what modularity means.  If a code is truly
modular, you can understand and test each part separately.  Then, to
see how it all works together, you can make sure that you have at
least two independent implementations for each part, so that you can
mix and match and see how swapping modules influences the outcome of
a calculation.

Now, to answer your question about regularization and perturbers,
I do not suggest that you rewrite the non-gravitational codes.
You can put a wrapper around a stellar evolution code, with a well
defined interface, and similarly you can put a wrapper around a
hydrodynamics code.  But the stellar dynamics code will have to be
rewritten, I'm pretty sure, for the reasons you just gave.  Let us
take a bird's eye view of the situation.

Logically speaking, you could introduce four elements.  A manager,
and three legacy codes, all wrapped in an interface.  The three codes
will be like black boxes.  If you open the hydro box or the evolution
box, you will see lots of physics, and lots of wires connecting the
various physics modules.  But if you open the dynamics box, you will
see very little physics, and almost only wires.  The part of the code
that solves Newton's equations of motion is literally only a few lines,
occurring in only a few places.  Everything else is bookkeeping,
complex orchestration of coordinate transformations, updating of
perturber lists, handling exceptions, shrinking or extending groups of
particles that need special treatment and so on.

*Bob*: Ah, you know more about direct N-body codes that you have let me
to believe, so far!

*Alice*: Yeah, it has been a while, but I did crawl through one of them,
years ago.  It wasn't easy.  But you see my point.  Logically speaking,
you could introduce a manager module, which takes care of all the
bookkeeping needed to let the three modules talk together.  But you
would be replicating much of the bookkeeping that is already happening
in the stellar dynamics module.  It would be much more natural to
rewrite the stellar dynamics module, elevating it in the process to
the status of manager module, and letting it do its interfacing with
the two other modules.  And it can take care of the interface timing
between the two other modules as well, as a good manager should.

*Bob*: You're going way over my head here, talking about two different
ways of doing your modular thing, while I have no interest whatsoever
in being formal, or modular, or whatever other paradigm computer
scientists may come up with.  But as far as I can follow you, your
second alternative goes more in the direction of what I would do.
I would simply take an existing N-body code, and splice in all the
connections that are needed with the stellar evolution code, and with
the hydrodynamics code.  The only difference I can see is that you
want me to write the N-body code from scratch, while I maintain that
it is a lot more efficient to stat from a legacy N-body code.

*Alice*: The main difference is what you just called this splicing thing.
Where you would to make a few dozen connections between the dynamics
code and the other codes, I want to limit the connections to a few,
at most.  And then I want to define and document those connections
in a completely precise way, down to the byte level.  That will
require you to rewrite your N-body code.  That will take time, but you
will soon gain time, over the years to come, when you want to make
later extensions.  As you know, several of the current legacy codes
have a life time of twenty or thirty years, if not longer.  It is well
worth your time to spend a few years setting things up correctly, and
then reaping the benefits for a long time afterward.  After all, the
time that is spent by experimentalists and observers building a
laboratory or observatory is a fair fraction of the time they spend
using it.  If they wouldn't carefully plan in building a new lab or
telescope, and just build new stuff haphazardly on top of old pieces,
they would fail.

=== Interface specifications

*Bob*: Wait a minute: you said that modularity means flexibility, and
that means that you can later extend things more easily.  But then you
said that you need well-defined interfaces.  That to me sounds like
the opposite of flexibility.  We have seen plenty of examples where
progress was hampered by rigid definitions.

To take a real life example: for decades now, the telephone system in
the United States has been running up against the barrier of the
ten-digit specification of telephone numbers, with three for an area
code and seven for the number within an area code.  As a result, they
have split up existing regions for a given area code into two halves,
one of which retained the old area code, and one of which got a new
one.  Think of all the effort in people having to reprint all the
material that has their telephone number on it, having to repaint
their delivery car, and so on.

*Alice*: Yes, that is a good example of the problems you can get with an
inflexible specification.  But think of the alternative: if any telephone
number could have had any length, back in the days in which the
telephone systems was set up, based on mechanical relays, it would
probably have been more cumbersome then.  In any case, whatever
solution they would have come up with would sooner or later have led
to unforeseen problems.

The real mistake with the telephone system was not so much that they
ran into a limitation of a specification, but rather that they did not
change the specification.

*Bob*: What is the point of a specification if you can easily change it?

*Alice*: You don't want to change it in arbitrary ways, whenever there is
a small problem for which it seems convenient to stretch the
specification a bit.  But you do want to make a change when it is
unavoidable to do so, and you don't want to postpone such a change
until you walk straight into a wall.  In the case of the telephone
system, it would probably have been much better to extend the number
of digits a few decades ago, as soon as computers began to replace
mechanical relays.  That way there would have been only one change
for everybody, rather than an annoying and interruptive change of
area codes once every few years or more.

I suspect that the real reason for the long delay in a change of
specification for the telephone system has to do more with politics
than with engineering.  It is probably too difficult to get everyone
involved to agree on all levels.

*Bob*: The same problem applies to the internet address protocol, of
course.  The limitations of a 32-bit address are being felt severely
now, especially outside the United States, in countries that were
given far fewer addresses than the U.S. had already taken, but where
there are now a comparable number of users.  The problem there, too,
is politics: until the U.S. will agree to go to a wider address
protocol, it is unlikely that it will change.

*Alice*: Fortunately, we don't have to worry too much about high level
political pressure when developing star cluster simulation software.
In our case we can learn from these and other mistakes.  I suggest to
allow a controlled update of the specifications, whenever you define a
new major version number for your code, in a very well defined manner.

*Bob*: Why not just allow a common block, and let people pass whatever
they want?

*Alice*: If you allow that, you may as well forget about modularity.
In that case, there are so many ways that a small modification can
cause a major perturbation that will be almost impossible to trace
down.  And if you want to parallelize your code, things only will
get worse.

=== A Matter of Opinion?

*Bob*: I see that you are really serious with your modularity speak.
I will give you the benefit of the doubt, for argument's sake.
You have just shown that you are familiar with the basic lay-out of an
N-body code, which I respect, since there are few astrophysicists who
have actually gone through a direct N-body code that has all the bells
and whistles to do collisional stellar dynamics.  So let's play the
game: you describe to me how I should modularize my code.  Then I'll
tell you whether it would even have a chance of being reasonable.

*Alice*: First of all, you should treat the dynamics code in exactly the
same way as we have treated the overall kitchen sink code: it should
be split up into smaller modules, connected with well-defined interfaces.
For starters, the N-body code will be split in a global and local part.
The global part will compute the particle orbits throughout the whole
cluster, while the local part will handle all the close encounters,
coordinate transformations, regularization, and so on.

*Bob*: But what about perturber lists?

*Alice*: You mean particles that are too far from a local clump to be part
of the clump, but too close, so that their influence on the clump can
not be neglected, unlike the bulk of stars that are further removed?

*Bob*: Exactly.  Those perturbers will give you a bunch of spaghetti
that cross wires between your two neat local and global modules.

*Alice*: No, they won't.  I won't let them.  One solution would be to
introduce a separate perturber module, besides the local and global
modules.  Another solution would be to include the perturber lists
within the local module.  Yet another solution is to device a
different and novel algorithm that dispenses with the use of
perturbers altogether.

*Bob*: The last case is just silly.  Decades of experience has shown
that you get an enormous penalty in computer time needed when you
leave out the use of perturber lists.  As for hiding the lists, in
whatever module, and then passing their information through a few
narrow interface bottlenecks, that is not much better.  You'll get
a huge data passing overhead.

*Alice*: The penalty may not be that bad.  And you won't know until you
try.

*Bob*: Until you try -- that's easier said than done.  What you just
proposed does require a complete rewrite of a stellar dynamics code.

*Alice*: That, too, may not be as bad as you think.  While it has taken
decades to write a legacy code, to rewrite it will surely take a lot
less time.  You can learn from past insights as well as from past
mistakes.  You don't have to reinvent the wheel.

*Bob*: I strongly disagree.  Writing a skeleton version, sure, but
getting the details right is like finetuning a race car, it requires
skill and patience.  

*Alice*: Even so, inventing the concept of a race car must have taken far
more time than any subsequent designing and building of race cars.
In any case, it is clear that we have a different was of looking at
things.  Perhaps you are right, and I have too optimistic a view of
the power of modularity.  It is possible that the problem is just
too dirty, and that any approach no matter how clean will get bogged
down into details which force you break the rules you started with.
I doubt it, but it is a logical possibility.  But perhaps I am right,
and you have too pessimistic a view, colored by too much frustration
with less modular approaches.  How are we going to find out who is
right?  You said you wanted to give me the benefit of the doubt.
Well, what type of benefit are you willing to give me?

=== Writing a toy model

*Bob*: I have a suggestion.  How about working together for a while in
writing a toy model version of your grand vision.  You can have the
satisfaction of seeing the first steps being taken toward your castle
in the sky.  And I will then have the satisfaction of you realizing
how much more complicated it will be than you know think, to get from
the toy model to reality.

*Alice*: But if you don't believe me, why would you even go to the trouble
of writing a toy model?

*Bob*: Pragmatism.  I have to teach a stellar dynamics class, and I had
been thinking about developing some form of toy version of an N-body
code, to give the students some hands-on experience without requiring
them to walk through a legacy code, which they would never be able to
do within the time frame of a single course.

*Alice*: So you will allow my ideas to spoil the young?

*Bob*: I'll take that risk -- as long as I can then point out to them in
the end, how very far they will still be from a competitive N-body code!

*Alice*: Good, we have a deal.  I have thought for a long time about the
question of how to structure a stellar dynamics code in a more clean
and modular way, and still let it be able to handle close encounters
and all that.  I had not thought about combining it with stellar
evolution and hydrodynamics, but now that we discussed these options,
I feel even more convinced that a modular approach is called for.

If we can manage to construct a flexible modular framework for a
general simulation code, getting the overall bookkeeping straight,
we can then postpone any decision about what physics to put in, and
what approximations to allow.  You want stellar evolution?  Fine, plug
in your favorite module.  You prefer to use Monte Carlo integration of
orbits, rather than the more accurate but much more expensive direct
N-body integration?  Fine, replace the global dynamics module for a
Monte Carlo module.  You like perturbers?  Use a local module in which
perturbers lists are implemented.  You think you can do without?  Then
design a local module built on an algorithms that doesn't require
perturbers.  Let a hundred flowers bloom!

*Bob*: I'd be happy to let one flower bloom, just writing one toy model,
good enough for my students to play with.  I'll leave you with the
seeds for the other ninety nine flowers.

*Alice*: No need to try to convince you, at this point.  Let's roll up our
sleeves, and see where our toy model will lead us.
