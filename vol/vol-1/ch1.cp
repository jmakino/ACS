= Chapter 1.  Dense Stellar Systems

Alice and Bob meet each other.  Alice is 45, started out in particle
physics, then worked on various astrophysical topics, including
cosmology, and also has extensive experience in simulating dense
stellar systems..  Bob is 35, just got tenure, and has worked on dense
stellar systems modeling for most of his career, mostly stellar dynamics,
but with some work in SPH.

They talk about how the astrophysics of dense stellar systems is going
to be a central topic for decades to come.  Cosmology, from the top
down, is now able to resolve star cluster size masses, and in addition
star formation is now the largest uncertainty in large scale structure
simulations.  Star formation, from the bottom up, is now being modeled
in the context of giant molecular clouds, and before long will be able
to model the formation of reasonably large star clusters.  Besides
forming the meeting ground of stellar astrophysics and cosmology,
dense stellar systems include the most fascinating astrophysical
environments, those that can be found around supermassive black holes
in the nuclei of galaxies.

They then lament the fact that many hundreds of astronomers are
observing dense stellar systems, but that only a few dozen individuals
are involved in modeling those systems.  They wonder what the reasons
are.  They suggest the fact that cosmology has been the most fascinable
topic; the fact that the elegance of equal-mass modeling ended in the
late eighties when the mathematical physics of those systems were
finally understood; the problem with kitchen-sink simulations that you
are not sure what to believe of the outcome.

Next, they discuss the state of the art of modeling dense stellar systems.
They briefly look back at the history.  Alice talks about the late eighties,
when she started her first work on dense stellar systems, after getting
her PhD, using NBODY5, and later working with Kira.  Bob also has used
NBODYx codes in his work, as well as Kira.

Alice talks about how little modularity there is, and how difficult
it will be to validate and modify the codes, two things that prior
experience has taught her to be absolutely crucial.  Bob can see her
point theoretically, but his experience has shown him, in contrast,
that any clean idea will soon get dirty as soon as you start working
on realistic applications.  They both think the other person is a bit
unreasonable, but decide not to stress their points, and instead they
continue to talk about what they have in common.

They discuss the way in which (astro)physicists write code, how their
education does not teach them anything about good style and approach.
They swap anecdotes about stellar evolution codes that are unreadable
and about a recent book by a well known hydro expert without even
subroutines.  They also talk about stories they have heard from people
in other areas of scientific modeling where people finally decided to
rewrite legacy code, how it took a few years, but how that resulted in
a much more flexible to build further software on.

== old material

Two astronomers meet each other over tea, one afternoon.

*Alice*: Hi Bob, what are you up to?

*Bob*: Well, to be honest, I'm not quite sure yet.  I've just about
wrapped up the two main projects that have occupied my attention for
the last few years.  <p>shall we say which projects?</p>  It's nice to
sit back for a bit, looking at the whole field of astronomy, before
plunging into a new project.

*A*: Actually, I'm in a similar situation.  Since I moved here, a
month ago, I've been dealing with all kind of chores that had been
accumulating, but now I have a clean desk, and I'm ready to start up.
So I'm in a similar situation.

*B*: The previous time I felt like this was when I had handed in my
thesis.  I was surprised to find myself suddenly in a pleasant vacuum,
after rushing so much to get everything finished in time.  It was soon
afterwards that I started working on xxx.  <p>let's think of something
interesting here</p>  That was six years ago.

*A*: I heard you just got tenure.  I guess that has something to do
with your finishing up your two projects?

*B*: how did you guess ;>).  While I enjoyed my work, I did feel a
little constrained.  There were times that I would have loved to go in
some other directions, but that probably would not have been wise to
do at that point.

*A*: I think you made the right decision.  When I got tenure, about
ten years ago now, I was in a different situation.  We were in the
middle of xxxx <p>how about a large project in cosmology?</p>, and we
were really excited about gettings things figured out.  But by now
the standard model is pretty well understood, and cosmology has become
a rather ordinary field in astronomy.  Frankly, this was one of the
reasons that I decided to move here, to get a fresh perspective, and
perhaps to start working in a new area.

*B*: So we are in comparable situations then.  Well, what are your plans.
Are you thinking of moving out of cosmology altogether?

*A*: I'm not sure yet.  The next phase of progress in cosmology is
likely to come from a detailed understanding of the formation of stars
and galaxies.  But given the difficulty of those topics, and the large
theoretical uncertainties that go into even the best models, this
phase may last a long time.  Twenty years may be a lower limit.

*B*: For sure.  If you see how long people have been working on these
problems already, and how slow the progress has been, I would bet on
thirty years, at least, before our theoretical models can really predict
how stars and galaxies form under given conditions in the interstellar
and intergalactic medium.

*A*: While the physics is interesting, the simulations seem to be
riddled with all kind of ad hoc assumptions, which makes you wonder
what to believe from any particular attempt at modeling.

<p>from here on, they then conclude that dense stellar systems are
actually doable, already on the level of stellar dynamics, and to some
extent on the level of stellar evolution and hydro.  While the last
two are still uncertain, perhaps it would be worth while to make such
a clear and clean simulation environment that at least you can bracket
all uncertains in a reliable and reproducable way.  And then you might
be able to use the wealth of observational data to constrain
simultaneously all the theoretical uncertainties.  Shall we say
something like this?</p>

xxxx

[Piet, well, _something_ like this, certainly... -- JM]

Perhaps they talk a bit more about the nature of simulations.

Perhaps they make a rough outline of what a big project could be like.

<p>We'll have to make it natural and believable that they commit
themselves to a many-year project; otherwise it would not make sense
for them to spend a lot of time on designing mechanisms for lab notes
and docbook and all that.  Do you have suggestions of how to present
that?</p>

[Piet, well, even if it is _not_ a multi-year project, we can start
talking about how we had difficulty digging out a script to generate
figures for some paper we wrote some years ago, and whether or not
there would be some better way -- Jun]


They then decide to get together the next day to get started.
