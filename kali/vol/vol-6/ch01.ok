= A Hierarchical Data Format

== Thinking Ahead

*Alice*: Hi, Bob!  How are things?

*Bob*: Hi, Alice!  As far as out project goes, things are fine.  As for
chores, there are two referees reports I still haven't written, an NSF
proposal that is due soon, a committee meeting for which I have to
prepare, and so on.

*Alice*: Business as usual, I take it!  I have quite a backlog of
things-to-do too.  However, if we would wait for everything to be
cleared out of the way, we would never get around to do some useful
work.  Shall we just sit down and see how far we can get, for the rest
of the afternoon?

*Bob*: Yeah, why not.  I'm pretty excited by how much we got done so far,
and it is high time we get some form of graphics working, so that we
can see what we've been doing so far.

*Alice*: I agree.  But before rushing into that, let us take stock of what
we have done so far.  We have written a two-body orbit integrator,
which is quite flexible in the sense that it has a choice of algorithms.
On the other hand, the interface of the program with the user and with
external data is quite primitive.

*Bob*: Well, it's a toy model, isn't it, what else do you expect?  And why
should that hold us back from visualizing our orbits right away?

*Alice*: I'm trying to think ahead, to see how our toy model may evolve.
Even though we call our programs toy models, if we are successful in
our design, it is likely that they will be used quite widely, and that
they will be modified for purposes we can't even think about right
now.

*Bob*: That would be nice, but why should we care about that now?

*Alice*: If we don't look ahead, and let things simply evolve, it is
quite likely that we will design a better I/O format, before too long.
Also, we will probably want to implement the option to invoke our
programs with command line arguments, rather than always having to
change the parameters in the driver file, as we have done so far.
If we change I/O format next month, and command line options the month
after that, then we will wind up with three different versions of each
program, doing more or less the same thing.

This proliferation of versions will be a source of confusion,
especially for new users.  What is worse, if we write a graphics
interface, we will have to write at least two parallel versions, one
for the original I/O data format, and one for the later one; and more
if we keep changing the data format once more.

*Bob*: But even if we would try to be a bit more careful now, how can we
guarantee that we won't change our mind about the data format, say,
half a year from now?  It would be very hard to predict what our needs
might be by that time.

*Alice*: This is indeed a challenge, but I think it is a challenge we
can live up to.  If we design our data format with sufficient care and
generality and flexibility, we might be able to absorb even quite
unexpected future additions, without having to change the data framework.

*Bob*: Are you thinking about a self-describing data format, like the
FITS format that many observers use?

*Alice*: Indeed, but then more powerful and more flexible.  Something
like XML would be a natural choice.

== Tree Structure

*Bob*: Hmm.  The only thing I know about XML is what I have seen in the
VOTable format for the virtual observatory initiative.  It seems to be
modeled on the HTML format for web pages.

*Alice*: I don't know much about XML either.  The basic structure is
indeed list-based, like Lisp, and like HTML.  Unlike HTML, XML is more
consistent in always requiring closing brackets of the same type to
follow an opening bracket.  From what I have seen of it, the basic idea
seems sound and very general.  What worries me a bit is that there are
still relatively few tools and applications, especially in the open
source world.

*Bob*: I must admit, it did strike me as rather wordy, and I'm not sure
that we want to stick to such a precise protocol.

*Alice*: We could make a compromise, by designing something simpler and
more straightforward, but in such a way that we can translate it easily
into XML.  We can then implement both versions for our I/O routines,
our own format as well as an XML format.

*Bob*: The question is, do we want to have two parallel systems?  I guess
it would be okay.  There is something to say for having a full XML
compatible version, since XML is likely to remain a generally accepted
format for quite a while to come.  And at the same time, I like to
have something a bit more user friendly to work with.

*Alice*: The only constraint would be to give our own format a lisp-like
tree structure, like XML, by providing opening and closing tags.

*Bob*: That would make sense.  The first thing I would vote for, is to
leave out the full <tt></something></tt> description after a
<tt><something></tt> opening, since that is certainly unnecessarily
tedious.  And we can always automatically reconstruct the full ending
tags, when we translate into XML.

*Alice*: I agree.  How about having a "begin something" followed by only
an "end", and not a full "end something"?

*Bob*: I like that, since it fits in nicely with the Ruby syntax.  So we
just have to introduce a few special key words, to signal the start of
the data for a single particle, and for a whole N-body system.

*Alice*: And also for the beginning and end of a file, of our type.  We need
the first line of a such a file to be "begin our-system".  Now how shall we
call our system?

*Bob*: If we want to be modest, we can call it "toy-simulations".  Or more
grandiosely, "toy-computational-science", if we follow your vision and
claim that we can apply it to any and all form of scientific simulations.

*Alice*: I know that you are joking, but frankly, I see no reason why our
approach should not be widely applicable to all of astrophysics, or physics,
or natural science for that matter.  But to call it a toy may give the wrong
emphasis.

*Bob*: If not a toy, you could call it real science.  But the science of
computational science sounds silly.  Hey, how about art?  The Art of
Computational Science.

*Alice*: I like that!  ACS, for short.  Okay, the first line for any file
in our new date format will then be <tt>begin ACS</tt>, and the last
line will be +end+.

== Physical Quantities

*Bob*: Now how do we specify the connection between hard numbers and the
physical variables that they contain the values of?  If we want to be
strict, we could write

 begin mass
   0.1
 end

and similarly

 begin position
   1.0  0.0  0.5
 end

but that seems to be a bit too much of a good thing.

*Alice*: Yes, it would be better if we could introduce an in-line notation.
Even so, it would be good to keep the form you just wrote down as a legal
option within our system.  The in-line notation would then be just syntactic
sugar.

*Bob*: Indeed nicely Ruby-like.  What is happening today?  We seem to be
in agreement all the time!

*Alice*: It's because you agreed so quickly to device a standard format.
I expected that you would put up a fight and resist such extra overhead!

*Bob*: I would have, a few years ago.  But in the mean time, I've been
bitten too often by the problems that can happen when you use
different file formats without properly distinguishing them.  And the
notion of self-describing data is something I was familiar with
through FITS files.  So you see, I based my agreement on real experience,
and not on a wish to be theoretically pure in some way or other.

*Alice*: Good theoretical ideas arise from the distillation of
a large body of collective experience, so in that sense too, I agree
with you.  In any case, I'm sure we'll develop differences again
pretty soon.  Moving right along, do you have a suggestion for an
inline notation?

*Bob*: How about

 mass=0.1
*Alice*: I like the idea of introducing an equal sign, to separate variable
and value, making it look again rather Ruby like.  But I don't like the look
of

 position=1.0 0.0 0.5

I suggest that we either introduce commas to separate the three
numbers on the right, or allow spaces around the <tt>=</tt> sign.

*Bob*: I prefer spaces over commas.  Okay, let's make both legal, what
you wrote above and also

 position = 1.0 0.0 0.5

In other words, an equal sign is the delimiter between variable and
value, a single space is a delimiter between components of a vector,
and in addition you can add as many spaces elsewhere as you like.  So
the following would be fine too:

 position  =  1.0   0.0 0.5

== A Particle Format

*Alice*: fair enough; let it be so.  Now how shall we bundle all these
numbers into one particle?

*Bob*: We can just call it +particle+.  Here is one particle, all by itself
in a file:

 begin ACS
   begin particle
     mass = 0.1
     position = 1.0 0.0 0.5
     velocity = 0.0 1.0 0.0
   end
 end

*Alice*: That looks nice.  And if we don't want to use the syntactic sugar,
we can write:

 begin ACS
   begin particle
     begin mass
       0.1
     end
     begin position
       1.0 0.0 0.5
     end
     begin velocity
       0.0 1.0 0.0
     end
   end
 end

*Bob*: Yeah, it is good to keep this as a legal option, but I doubt whether
anyone will ever use that.  In fact, once we allow levels of syntactic sugar,
how about introducing an even more compact version:

 begin ACS
   begin particle
     mass = 0.1 ; position = 1.0 0.0 0.5 ; velocity = 0.0 1.0 0.0
   end
 end

*Alice*: Good idea!  That again conforms with Ruby usage, and it is nice
to have freedom of expression.

== Name Spaces

*Bob*: If you really want to give me freedom of expression, I have another
suggestion.  I think that the above version is still a bit verbose.
How about using +part+ instead of +particle+ and +pos+ instead of
+position+.  We could even use +m+ instead of mass, abbreviate things
further:

 begin ACS
   begin part
     m = 0.1 ; r = 1.0 0.0 0.5 ; v = 0.0 1.0 0.0
   end
 end

*Alice*: nonono, that's really bad programming.  Right now, we know
what we mean, but later on, when we get stellar evolution to mix with
stellar dynamics and who knows what other types of complications, we
will soon run out of the 26 letters of the alphabet!  I feel strongly
that we should keep using full English words.

*Bob*: your prediction that we would soon disagree has come true even
quicker than I would have thought!  I must say, I really like the
compact notation.  But I don't seem to feel as strongly about it as
you do.  Oh, well, let's stick with the longer descriptions then.

*Alice*: And come to think of it, +ACS+ is too short a name too.  Something
like +ArtCompSci+ or even <tt>art-of-computational-science</tt> would be
much better.

*Bob*: Now you're really pushing it!  And it is my turn to feel strongly;
I like to be able to talk about what I use, and to talk about an ACS
file rolls off the tongue much more easily than talking about artcompsci
files, let alone art-of-computational-science files.

*Alice*: Before we settle this issue, let us think ahead a bit further.
The reason to choose the name ACS is that we want to introduce our ideas
as templates for use in computational science in general, not only in
astrophysics.  If a chemist or biologist will start using our system,
in future extended versions, they are likely to use a world like
+Particle+ for their own purpose.

*Bob*: So you are suggesting an extra level of headers, effectively
something like a type of +namespace+, as you have in C++?

*Alice*: Yes, that would be a good idea, I think.  And in that case,
I wouldn't mind keeping the short version +ACS+.  Even if for some
reason some people would introduce files for Advanced Computational
Software that would also be called +ACS+, and if we and they would
start using XML, there would be no confusion.  If we would get one of
their files, than the next line would not have the proper name space
tag that we require for our particular use.

== Compromise

*Bob*: I'm glad you found a compromise!  But how many levels of name
space do you want to introduce?  I could argue that some day you will
do hydrodynamics with SPH particles, that may have a rather different
structure than our particles.  If we give you total freedom, I bet you
will come up with something unwieldy pretty soon.

*Alice*: Let me not disappoint you.  If we introduce another abbreviation,
+DSS+ for simulations involving dense stellar systems, surely short enough
for your taste, then how about:

 begin ACS
   begin astrophysics
     begin DSS
       begin stellar_dynamics
         begin particle
           mass = 0.1
           position = 1.0 0.0 0.5
           velocity = 0.0 1.0 0.0
         end
       end
     end
   end
 end

*Bob*: Are you serious?

*Alice*: Well, a typical data file will contain quite a number of
particles, possibly with many more variables than we have here, so
the total length of the file won't change much, even with these three
levels of name spaces in between +ACS+ and +particle+.

*Bob*: Even so, this is too much of a proliferation, certainly at this
stage.

*Alice*: Perhaps.  I don't insist that we will implement all these
levels right away, since we don't know at this stage how our system
will evolve.  But at least we would be prepared to move in the
direction of a full hierarchy of name spaces, if the need arises.  And
this implies that we should not make any design decisions that would
make a future implementation difficult.

*Bob*: Fair enough.  And I am willing to make a compromise by allowing
the +DSS+ level of name space to be present right now.  It is indeed
short, and it tells us about the broad type of application within ACS.
So this will give us:

 begin ACS
   begin DSS
     begin particle
       mass = 0.1
       position = 1.0 0.0 0.5
       velocity = 0.0 1.0 0.0
     end
   end
 end

*Alice*: If we have to add levels of name spaces in the future, we'll
have to write a few short conversion programs.  One will add the extra
levels to old data files, to make them compatible with new programs.
Another conversion program will subtract the extra levels from new
data files in order to be read by old programs.

*Bob*: That should be easy to do.

==  Hierarchical particle structures

*Alice*: Now that we can define a single particle, we also need a way
to define a whole N-body system.

*Bob*: Introducing a tag +System+ would be confusing, since it would be
too general a name.  How about +Nbody+?

*Alice*: Here is an idea.  We can use the same tag +Particle+ in an
hierarchical way.  A two-body system could be written as:

 begin ACS
   begin DSS
     begin particle
       begin particle
         mass = 0.1
         position = 1.0 0.0 0.5
         velocity = 0.0 1.0 0.0
       end
       begin particle
         mass = 0.3
         position = -1.0 0.0 0.5
         velocity = 0.0 -0.2 0.0
       end
     end
   end
 end

*Bob*: I see.  And the higher-level particle could be interpreted as
the center-of-mass particle of the two-body systems.  It could carry
its own information.  In this case that would be:

 begin ACS
   begin DSS
     begin particle
       mass = 0.4
       position = -0.5 0.0 0.5
       velocity = 0.0 0.1 0.0
       begin particle
         mass = 0.1
         position = 1.0 0.0 0.5
         velocity = 0.0 1.0 0.0
       end
       begin particle
         mass = 0.3
         position = -1.0 0.0 0.5
         velocity = 0.0 -0.2 0.0
       end
     end
   end
 end

*Alice*: Yes, perfect.

== Extended Tag Names

*Bob*: One problem with using the name +particle+ hierarchically is that it
can lead to confusion as to which +particle+ is which.  For example,
if you want to set up two galaxies, and let them approach each other
to simulate a collision, each galaxy would be represented by one
+particle+ structure that would in turn contain many
+particle+'s, one for each body used in the simulation.  And the whole
system would be represented by a top +particle+.

*Alice*: Ah, here is an idea!  We can allow a second name in our +particle+
tag, to indicate the particular type of +particle+ we are dealing with.
Your example would then become:

 begin ACS
   begin DSS
     begin particle simulation
       begin particle galaxy
         begin particle star
         end
         begin particle star
         end
         . . . 
         begin particle star
         end
       end 
       begin particle galaxy
         begin particle star
         end
         begin particle star
         end
         . . . 
         begin particle star
         end
       end 
     end
   end
 end

*Bob*: Great idea.  In that way, programs that only need to know that a
particular object is a +particle+ can just read the first word in the tag.
Other programs that need more information, or a human reader inspecting
the data file, can read the subsequent information; we might even allow
arbitrary many tags.  An star on the asymptotic giant branch could be:

 begin ACS
   begin DSS
     begin particle star giant AGB
       mass = 0.1
       position = 1.0 0.0 0.5
       velocity = 0.0 1.0 0.0
     end
   end
 end

*Alice*: I like that.  I think we are getting there.
