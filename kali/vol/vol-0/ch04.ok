= Software Architecture

== Legacy Codes

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

== Modularity

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

== Interface specifications

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

== A Matter of Opinion?

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

== Writing a toy model

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
