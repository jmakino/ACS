= A Story Mechanism

== Anything Goes

*Bob*: Our first to-do time was to implement a scratch pad, to contain
unrecognized data that can be read in, and to reproduce those safely
at the time of output.  I cill copy the file <tt>iobody2.rb</tt>, with
which we have been working so far, into a new file <tt>iobody3.rb</tt>.

*Alice*: Let's call the scratch pad a +story+.  After all, it may contain
whatever you like it to contain.  Obvious choices are lines like

    acceleration = 0.1 -0.3

or

    density = 345.18

for physical quantities, or something like

    neighbors = 15 18 23

for a list of nearest neighbors, with particles identified by their numbers,
or in any other way for that matter, for example:

    neighbors = star5, star7, GMC3, triple8

As long as the appropriate programs can handle that format, the I/O routines
for the +Body+ class don't have to worry about them.

*Bob*: But how do you know which line makes sense?  You don't have the
information to know what all the other programs can handle.  What if someone
was planning to write an email, reporting on a run, and somehow by mistake
got the text mixed up with the input for a +Body+ instance.  Surely it would
be an error to read in:

    velocity = 1 0
    acceleration = 0.1 -0.3
    Hi Joe!  Look what a cool run I just produced.
    This AGB star is out of control!  Man, it's
    evolving with a time step much less than a year,
    and shrinking.  This way we'll never reach the
    horizontal branch.  Meanwhile, wanna come over
    for a beer?

*Alice*: Actually, this does not have to count as an error at all.  What if
the writer is planning to send Joe a data set, and would want to communicate
to Joe what the data are all about?  It would be much safer to add this
message to the actual data file, rather than sending it in a separate email.
We all know how many emails we get every day, and how difficult it is to
retrieve the right one, after weeks or months.

*Bob*: But there is no way of knowing the form that a human narrative
can take.  People might write <tt>!@#*!!</tt> if they are in a bad
mood.  By letting people write what they want you would allow
literally any line!

*Alice*: Why not?

*Bob*: I know a good reason why not: next thing you know is to see
someone type <tt>begin sparticle</tt>.  And we both agreed that that
should give an error message.  No supersymmetric AGB stars allowed,
at least not in this universe!

*Alice*: You're right, but that is an implementation detail.  What we
need to do is to indicate where the story begins and ends.  And then,
<i>within</i> the story, anything goes.

*Bob*: Except typing a line that will end the story, since that line
cannot occur as a regular line of the story.  But that limitation
cannot be helped, and can be escaped in some way.  I see your point,
and I like the idea of extending the notion of self-describing data.
Your story idea will introduce self-chatting data!

*Alice*: I prefer self-narrating data.  Good!  Since we both agree,
let's implement a story mechanism.

== xxx

*Bob*: That shouldn't be hard.
Here is your story variable, a <tt>@story</tt> instance variable
for the +Body+ class, in the form of an array of story lines.  Initially
each +Body+ will be created with an empty array:

 :inccode: .iobody3.rb+initialize

The only line that we need to change in the +read+ method, is to
replace the old

 :inccode: .iobody2.rb-2

in <tt>iobody2.rb</tt> by

 :inccode: .iobody3.rb-1

*Alice*: Simple indeed!  But we have to make a change in our output
mechanism as well.

*Bob*: 


