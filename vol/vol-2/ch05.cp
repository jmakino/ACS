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
