= An N-Body Code

== First Attempt

*Bob*: Hi, Alice!  How are things?

*Alice*: I'm fine.  But I can see from your face that you can't wait to
show me something.  Did you do some more coding?

*Bob*: Well, yes, some.

*Alice*: Don't tell me you wrote a whole N-body implementation?

*Bob*: Well, yes.

*Alice*: Now that's impressive!  Even though you told me it wasn't going
to be _that_ much work.

*Bob*: It was more work than I thought, I admit, but not terribly much
more work.

*Alice*: I guess that `more' is a relative concept.
Can you show me what you did?

*Bob*: My pleasure.  Here is the whole listing.  I have called it
<tt>rknbody1.rb</tt>, +rk+ for Runge-Kutta, and <tt>1</tt> because
it is my first attempt, and I'm sure we'll come up with some
modifications and improvements.

 :inccode: .rknbody1.rb

== Driver

*Alice*: I recognize the overall structure.  Can you show me the driver
program, to give me an idea where to start?

*Bob*: Here it is, <tt>rknbody1_driver.rb</tt>

 :inccode: .rknbody1_driver.rb

*Alice*: The last three lines are almost exactly the same as the last three
lines in the driver for our pseudo-one-body integrator:

 :inccode: .integrator_driver2.rb-1

apart for three +n+'s and one +N+.  The rest is exactly the same, with
of course an extra +n+ in the file name that is being required on top.
A very minimal change.

*Bob*: Yes, I prefered to stay as close as I could to our initial 2-body
relative coordinates integrator.

*Alice*: I'm very curious to see how you integrated the combined equations
of motion for an arbitrary number of particles, but let me follow the logic
of the driver first, to feel my way around in the program flow.  You start
by creating a whole N-body system, and then reading in the data.  And besides
the old class +Body+, you have introduced a class +Nbody+, to contain the
data for that whole system.

*Bob*: Yes.  The initializer has two parameters, as you can see:

 :inccode: .rknbody1.rb+initialize+Nbody

By default an N-body system is created empty, containing 0 particles, at
starting at time 0, but if you like, you can create it was several particles
right from the start.  In our case, the driver creates an empty default system,
leaving it up to the <tt>simple_read</tt> input function to create the
necessary particle slots, depending on how many are needed to store
the input data.

*Alice*: Since all particles share the same time step in this code, it makes
sense to make the shared simulation time an instance variable <tt>@time</tt>
for the +Nbody+ class.  And <tt>@body</tt> must be an array of +Body+
instances, since if you specify +n+ particles to be present, you fill the
array by creating +n+ new bodies, from <tt>@body[0]</tt> through
<tt>@body[n-1]</tt>.  But what about the +for+ loop?  Ah, yes, I
remember now: in Ruby <tt>0...n</tt> means that you exclude +n+, while
<tt>0..n</tt> does indeed exclude +n+.

*Bob*: A handy short notation, but like every short notation, potentially
confusion if you're not yet used to it.

== Input

*Alice*: But what about this line

 :include: .rknbody1.rb-1

*Bob*: I have given the +Body+ class an extra instance variable <tt>@nb</tt>,
which for each body instance will contain the address of the parent,
an instance of the +Nbody+ class.  In that way, each +Body+ daughter
is doubly linked to her +Nbody+ parent.  The parent can call the
appropriate daughter, by selecting her from the <tt>@body[]</tt> array,
and the daughter can call the parent directly through her own <tt>@nb</tt>
variable.  Remember that the expression +self+ gives the address of the
+Nbody+ instance itself, which then gets handed down to each body.

*Alice*: Hmm.  In general, I am quite wary of doubly linked list.  It is
all too easy to change the link in one direction and to forget to change
the link in the other direction, or to change it in the wrong way.
And even if you change the other direction correction, it is still possible
to be one off in your counting, overlooking the first or last particle.

*Bob*: Typical one-off errors that often happen in C and C++ are less
likely to occur in Ruby, because so much of the book keeping is handling
behind the scenes, as long as you don't confuse <tt>0...n</tt> and
<tt>0..n</tt>.  But I see your point, and perhaps we should change that,
later on.  For now, let's just go through the code, and then we can decide
whether it will be easy to unlink the backwards pointers from daughter
to parent.

My motivation to provide backward links was to give each daughter the
possibility to communicate with her siblings.  If one daughter wants
to compute her acceleration, she would need to find the positions of
all other daughters, and the simplest way to do that, I thought, was
to give her a way to ask her parents how to find all the others.

*Alice*: Fine for now.  In the <tt>simple_read</tt> program you also
provide those backward links for each particle, after which you invoke
the <tt>simple_read</tt> method for that particle:

 :inccode: .rknbody1.rb+simple_read+Body

The format you have chosen is to start with the number of particles
and the time, followed by the data for each particle.

*Bob*: Yes.  It seemed safer to tell the input routine how many particles
to expect, rather than to let it read in everything to the end of the file.
In some cases you might want to store more than one snapshot, for example,
in one file.

In fact, my code normally will output a series of snapshots, one after each
<tt>dt_out</tt> interval, just as we did it for the single pseudo-body case.
This will make it possible to restart a run: you can later sequentially read
in a number of those snapshots, each with a <tt>nb.simple_read</tt> statement
in the driver, selecting the proper one by checking the time variable
specified.

For example, when you invoke the code with:

  dt_out = 5
  dt_end = 10
  nb = Nbody.new
  nb.simple_read
  nb.evolve(method, dt, dt_dia, dt_out, dt_end)

you can continue the run from the output of this first run, by reading
in that output and discarding the first snapshot:

  nb = Nbody.new
  nb.simple_read
  nb.simple_read
  nb.evolve(method, dt, dt_dia, dt_out, dt_end)

Alternatively, and more safely, you could check for the time:

  nb = Nbody.new
  nb.simple_read
  while (nb.time < 10)
    nb.simple_read
  end

It would be better to use a slightly smaller value than 10, if you want
to pick up the snapshot corresponding to <tt>nb.time = 10</tt>, since it
is quite possible that it will have been output at <tt>nb.time = 9.99999</tt>
or so.  Also, you would want to check whether the snapshot time is not too
far beyond <tt>nb.time = 10</tt>.  But those are details.

== Inspecting the Leapfrog

*Alice*: Yes, I get the idea, and that all makes a lot of sense.
And now that understand how the data get read in, let's see what will happen
with them.

In <tt>rknbody1.rb</tt>, I see that you have shifted all the integration
methods from the +Body+ to the +Nbody class, as well as the +evolve+ function
that calls them.

*Bob*: The +evolve+ function orchestrates the whole integration process, and
it is called by the driver, which only knows about the one +Nbody+ instance
that it has created.  So it is logical to put the +evolve+ method inside
the +Nbody+ class.  And since +evolve+ calls the various integration methods,
it also seemed logical to have +leapfrog+, +rk2+, and so on, reside there.

*Alice*: I could imagine an alternative, where each particle is given the
freedom to use its own integration method, in which case you would
want to shift those methods back into the +Body+ class, but that would make
more sense when you use an individual time step algorithm, where each
particle has its own time step length.  For the simple shared time step case
that we are starting with, your choice is surely the best.

*Bob*: I could imagine many things, but coding them takes more time than
imagining them!  I do like the idea of relatively autonomous particles,
integrating themselves as they want, with stars in denser regions having
perhaps more specialized integrators, but not today.

*Alice*: Looking at +evolve+, I see almost exactly the same function that
we used for the two-body problem.  The only difference is that now the
time is an instance variable for the +Nbody+ class, which means that we
don't have to pass the time as an argument to the <tt>write_diagnostics</tt>
class.

*Bob*: Yes.  If I would have left the time as a normal variable that would
be passed around, the +evolve+ method would have been _exactly_ the same.
A nice example of recycling code: whether you are dealing with one
pseudo particle or with _N_ particles, the top level instructions are
basically the same.

*Alice*: But of course the actual work is different, and in our case more
complicated.  The forward Euler implementation is a bit hard to recognise,
at first sight.  Let me start with the new leapfrog method, which
looks more familiar.  The two-body version was:

 :inccode: .rkbody.rb+leapfrog

while now we have

 :inccode: .rknbody1.rb+leapfrog

This is easy to understand: for each body, basically the same actions are
taken as was the case for our single pseudo-body, containing the relative
position information for the two-body case.

*Bob*: The difference being that, invisibly at this level, the +Body+
method +acc+, which computes the acceleration, has to ask all other
particles for their position.

== Acceleration

*Alice*: Indeed, +acc+ has grown quite a bit bigger.  In the two-body case,
we started with

 :inccode: .rkbody.rb+acc

and your new N-body version reads:

 :inccode: .rknbody1.rb+acc

*Bob*: The main difference is the loop that our body has to execute over
all other bodies.  It is here that I am using my backpointer <tt>@nb</tt>
that links back to the parent +Nbody+ instance.  In that way, the array of
bodies becomes visible for our particular body as <tt>@nb.body</tt>, and
it is this array over which we iterate using the familiar +each+ construct.

*Alice*: And you are excluding the body itself from the loop, to avoid
getting an infinitely large self interaction, through the line

 :include: .rknbody1.rb-2

*Bob*: Indeed.  For all other bodies, we compute the in a similar way
as before.  The main difference is that the vector connecting the two
bodies is not given, as was the case for the two-body problem, where
there was only one relative vector.  Here we compute the vector pointing
from the calling particle to the called particle first, as follows:

 :include: .rknbody1.rb-3

*Alice*: And the acceleration _seems_ to have the same mass dependence
in both cases, the two-body and the N-body case, but here appearances
deceive: in the two-body case we had an equation of moment for our
pseudo particle, while here we are now dealing with real particles.

*Bob*: Yes, I thought about that carefully.  Actually, the tricky thing is
to get the two-body case right, where it is easy to make a mistake, as we
saw when I was a bit too quick in coding up the diagnostics there.
For the N-body case, in contrast, it is all a piece of cake.  The line

 :include: .rknbody1.rb-4

Directly implements Newton's law of gravity.

== Newton's Gravity

*Alice*: When we present this to our students, it would be good to summarize
the connection specifically.  To wit: the expression for the
acceleration felt by particle _i_ is given by summing together the
Newtonian gravitational attraction of all other particles _j_, where
both _i_ and _j_ take on values from 1 up to and including _N_:

:equation:
\frac{d^2}{dt^2}\br_i =  G \sum_{j=1 \atop j \neq i}^N M_j
\frac{\br_j - \br_i}{\,|\br_j - \br_i|^3}

Here <tex>$M_j$</tex> and <tex>$\br_j$</tex> are the mass and position
vector of particle _j_, and _G_ is the gravitational constant.  To
bring out the inverse square nature of gravity, we can define
<tex>$\br_{ji} = \br_j - \br_i$</tex>, with <tex>$r_{ji} = |\br_{ji}|$</tex>,
and unit vector <tex>$\hat \br_{ji} = \br_{ji} / r_{ji}$</tex>:

:equation:
\ba_i = G \sum_{j=1 \atop j \neq i}^N
\frac{M_j}{r_{ji}^2} \,\hat\br_{ji}
\label{newton}

Note that the summation excludes self-interactions: every particle
feels the forces of the other <tex>$N-1$</tex> particles, but not its
own force, which, as we already mentioned, would be infinitely large
in case of a point mass.

*Bob*: End of lecture.  Yes, that's a crisp summary.  And what is really
nice in our Ruby implementation, is that we _never_ have to introduce
the counters _i_ and _j_ that are so ubiquitous in any N-body code I have
ever seen.  Just as we could dispence with the _k_ variable for the
components of a vector, we can avoid the other two counters by asking
arrays and vectors to just loop over themselves.

*Alice*: And this is one of the features that makes Ruby eminently suited
for prototyping and development work in general.  Whether Ruby will be used
eventually for an industrial-strength production-type code, that remains
to be seen.

*Bob*: If so, we'll have to do some very serious speed-up.  My impression
so far has been that Ruby is at least a couple orders of magnitude slower
than the equivalent C or Fortran implementation.

*Alice*: And in addition, our leapfrog calculates the acceleration twice
on the +Nbody+ level, and for each particle pair, the relative acceleration
is also computed twice.

*Bob*: If we had been a little more clever, we would have saved a factor
four there too.  I bet we can speed up our code by a stunning factor of
a thousand or so, if we pull all stops!

*Alice*: Maybe, we'll see in due time.  For now, I think we _are_ being
clever, by not worrying at all about optimization.  The point is to bring
out the underlying structure, which is complex enough all by itself.  Once
we really see that clearly, we can start optimizing while avoiding confusing
clutter.

== A Matter of Taste

*Bob*: Note, by the way, one more difference between the 2-body and
N-body case: in the latter case we have to accumulate the results,
through the summation you just showed.  Before traversing the loop
over particles, we have to clear the vector where the acceleration +a+
on our particle is being stored.  I experimented with various ways to
do so, but the most compact notation I found was what I wrote on the top
of the +acc+ method:

 :include: .rknbody1.rb-5

Isn't that a nifty and compact expression?

*Alice*: I see.  In order to provide a null vector for the acceleration
with the right number of components, you use the position as a template,
and after copying the position, you fill all entries with zeroes.  I'm
glad you put a command line in, since otherwise the meaning wouldn't
have been so obvious at first reading.

Hmmm.  While I agree that it is compact, perhaps a longer expression would
have been a bit more clear.  How about

    a = ([0]*@pos.size).to_v            # null vector of the correct length

*Bob*: Yes, that would bring out the fact that you use the position vector
_only_ because you want to extract its size, and not for any other reason.
And you explicitly show how you start with an array of length 1,
filled with a single 0, and then extend that array to contain
<tt>@pos.size</tt> components.  But then you still have to convert it into
a vector.  You see, I avoided the last step by starting with a copy of
<tt>@pos</tt>, which was already a vector.

*Alice*: Yes, your construction was clever, but I'm still wondering about
the unsuspecting reader, who has to make sense of your cleverness.  In fact,
in my more lengthy alternative, notice that I left your comment line
in, since upon first reading, even my longer line would still not be fully
clear, Im afraid.

If I really wanted to be self-explanatory, I would write:

    vector_size = @pos.size
    a = ([0]*vector_size).to_v

That way I would express the fact that +a+ is a vector, that it needs
to be off the right size, that it should contain all zeroes, and that
it should be converted to a proper vector at the end of the day.

*Bob*: A long day, if you ask me.  I prefer to stick with my 

 :include: .rknbody1.rb-5

*Alice*: Fine with me.  This is really a matter of taste.

*Bob*: While we're at in, let me walk you through the rest of the +Body+
class definition.  The potential is constructed in a very similar way
as the acceleration, by doing a body walk through the whole system.
The difference is that the potential energy includes a product of the
mass of the calling particle and the called particle:

:equation:
E_{pot} =  G \sum_{j=1 \atop j \neq i}^N 
\frac{M_i M_j}{|\br_j - \br_i|}

*Alice*: Yes, of course, and that is something that is easy to leave out.
If I would have written the potential method +epot+, using the
acceleration method +acc+ as a template, I might have forgotten to include
the factor <tt>@mass</tt> for the calling particle.

*Bob*: As a matter of fact, I did leave it out, at first, and I only noticed
it when I could not get energy conservation.  Fortunately, I ran a test with
masses that were not all unity.

*Alice*: Good point!  It is all too easy to use a mass of 1 for each particle,
when you are testing code, and in that way you may never find out whether
the masses have been included properly.

== xxx

*Bob*: The rest of the input and output routines are unchanged, compared to
our earlier two-body code.  Let's return to the +Nbody+ class.  You mentioned
that the +leapfrog+ method was almost the same as before.  Unfortunately,
that is the only one of our four integration methods that remained quite
simple to read.  The other three have become a bit more crowded, I'm afraid.

*Alice*: I'll start with the forward Euler case again.  In +forward+, you
have replaced the previous form

 :inccode: .rkbody.rb+forward

by:

 :inccode: .rknbody1.rb+forward

*Bob*: This is a tricky point.  Before we stored the original acceleration
in the vector <tt>old_acc</tt>, which was a single physical vector, containing
the relative acceleration between our two particles.  In the N-dimensional
analogue that we have here, we need to store _N_ initial acceleration vectors,
one for each particle.  

we have to loop over all components.  And 



