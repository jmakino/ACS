= Chapter 1.  Introduction

== A Chat between Two Astronomers

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

== Dense Stellar Systems

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

== The Equal-Mass Point-Particle Approximation

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
