= Diagnostics

== Energy Conservation

*Bob*: That was quite a long session yesterday, when we were chasing
those bugs while we were trying to implement a better +Body+ class!

*Alice*: Yes, but we learned a lot about Ruby.

*Bob*: And even a thing or two about debugging.

*Alice*: Perhaps this is a good time to return to our real project:
to build progressively better integrators.  But before we move on to
second-order integrators, as well as introducing graphics and other
complications, I would like to do one more test.  Even though we now
know that our forward Euler scheme converges, we don't know yet how
well it conserves energy.  Shouldn't we test that explicitly?

*Bob*: Great minds think alike.  I had come to the same conclusion.
For the two-body problem, of course you can check all kind of things.
You can measure how accurately a bound orbit remains the same ellipse,
as it should be, rather than slowly drifting and changing shape, and
whether the motion traverses the ellipse at the right speed, and so on.
The reason is that we have an analytic solution for the two-body problem.

For the many-body problem in general, however, we are a lot less
lucky.  All we know is that the total energy and total angular
momentum of the system are conserved.  That gives us one scalar and
one vector, or in three dimensions it gives us four conserved
quantities.  So we cannot compare all the details of the motion of the
<tex>$N$</tex> particles with theory, not by a long shot.  An
<tex>$N$</tex>-body system in three dimensions has <tex>$6N$</tex>
degrees of freedom, far more than the 4 handles we have.

However, things are not as bad as they may seem.  If you make errors
in your integration, and you always make errors at some level, those
errors will let the simulated orbit drift away from the true orbit in
a random fashion.  It would require a very clever conspiracy for those
errors to work together in such a way as the keep the energy conserved
to a high degree of accuracy, while still introducing much larger errors
elsewhere.  In practice, therefore, checking energy conservation in an
<tex>$N$</tex>-body system has become the standard test to see how
accurate the integration of an <tex>$N$</tex>-body system is.

*Alice*: Another way of saying this is to picture the phase space of
the whole system.

*Bob*: You mean the six-dimensional space in which you plot for each
particle its position and velocity?

*Alice*: I was thinking aoubt the alternative way of picturing the whole
system as one point in a <tex>$6N$</tex>-dimensional system.  The
evolution of an <tex>$N$</tex>-body system can then be viewed as the
complex motion of this one point through this huge space.  Now in this
space you can define layer of subspaces, on each of which the total
energy of the system is constant.  Once the particle starts on such a
hypersurface, it should stay there, because energy is conserved.  But
if we make arbitrary errors in computing its motion, our simulated
particle will show some numerical drift in all dimensions available,
including the direction perpendicular to the energy hypersurface.

*Bob*: Which reaches the same conclusion as what I just summarized.

*Alice*: Yes.  Since the dynamics of this one point in <tex>$6N$</tex>
dimensions is incredibly complicated, the distance away from the energy
hypersurface will typically be of the same order of magnitude as the
total length of the drift of that master particle that symbolizes our
whole <tex>$N$</tex>-body system.

*Bob*: More mathematical minded students might prefer your explanation;
I think I'll just stick to my more lowbrow explanation.  But however
we tell the story, we should of course warn them that this argument
fails when we are dealing with an integrator that has a built-in type
of energy conservation.

*Alice*: Yes, indeed, that is very important.  It would be easy to
construct an integrator that projects the orbit of the master particle
in <tex>$6N$</tex> dimensions back onto the original constant-energy
hypersurface.  But that would be cheating.  It would get rid of only
one degree of freedom of the total error, and leave the other
<tex>$6N-1$</tex> degrees of freedom uncorrected.  It would look great
when you test for energy conservation, but at the cost of having lost
your handle on checking what is going on.

*Bob*: Or the way I would put it, you could just wiggle one particle
far out, perhaps a particle that has already escaped from the system.
By changing the velocity of that particle a little, you will change
its kinetic energy, and so you can accurately balance the numerical
errors that occur in the rest of the system.  But of course that does
not make the rest of the system behave in a more accurate way.

*Alice*: Of course, the two examples that you and I just cooked up
are extreme.  It is not at all a bad idea to conserve energy, if your
approach is part of a systematic way to make the whole integration
more accurate.  Symplectic integrators are a example.

*Bob*: I have heard of the name, but I must admit I don't know what they
are.

*Alice*: They are a lot of fun, from a mathematical point of view.
How useful they may be for real <tex>$6N$</tex>-body simulations,
that is not too clear.  They are more accurately, for sure, but they
are hard to generalize to individual time steps.  And for our game,
we have no choice to go to individual time steps.  Perhaps we can
come back to this topic, some day, but not any time soon.  But just to
give you one example of the simplest symplectic integrator: the good
old leapfrog scheme, also known in some areas of physics as the Verlet
algorithm.

*Bob*: Which is probably the next algorithm we want to implement.

*Alice*: Yes.  So shall we first test energy conservation of our
forward Euler code?

== A Driver

*Bob*: Well, I have a surprise for you.  Yesterday night I continued
to tinker with Ruby, and I had so much fun doing so that I stayed
at it long enough to wind up with a version of our code that produces
energy conservation diagnostics.

*Alice*: How nice!  Can I see it?

*Bob*: First of all, here is the driver, the only part of the program
that we will change, while exploring the effect of different time step
sizes and integration times:

 :inccode: euler.rb

*Alice*: So at the bottom you create a new particle, for the relative
motion of the two bodies in our system, with the call to <tt>Body.new</tt>,
then you read in the initial conditions, as we did before, and then
you tell the particle to evolve itself.

*Bob*: Yes, with +evolve+ I mean that we invoke here a method that computes
the evolution of the system in time.  I have made this now a method that
is included in the +Body+ class, so we can invoke it with <tt>b.evolve</tt>.
The only extra thing needed is to tell the particle how long to integrate,
with what time step, and how frequently to report the degree of energy
conservation and the actual position and velocity of the relative motion.

*Alice*: So the actual program is a mere three lines, at the bottom, with
the four lines of assignments at the top just the way to specify each of
those four parameters you just mentioned.

*Bob*: Exactly.  Ruby is _very_ economical!  And just as a courtesy, I
decided to let the program print out the values of the four parameters.
We can then change those parameters in the current file to our hearts
content, and we allways check from earlier outputs what parameters were
used for a given integration.

*Alice*: Good idea.  Clean and careful.  Of course, some day soon we will
want to give those values as command line options, rather than always editing
the file.

*Bob*: I suppose so, although we'll have to think about that.  It is of
course possible to give Unix-style command line options, but there are
several other ways as well; in fact Ruby has a nice built-in way to do
that.

*Alice*: Later.  Can I have a look at what the +Body+ class now looks like?
It must have grown quite a bit, since it now contains not only the forward
Euler integrator, but also your whole +evolve+ method, as well as whatever
you wrote to get all the energy diagnostics done.

*Bob*: It's not that bad, actually.  Ruby really is a compact language.
Here, let me show you the whole file <tt>body.rb</tt> first, and then
we can go through each of the methods used.

== The Body Class

 :inccode: body.rb

*Alice*: Indeed, not as long as I would have guessed.  Can you give me
a guided tour?

*Bob*: My pleasure!

== Evolve

You are familiar already with the accessor shorthand that gives read
and write handles to the internal variables for the mass, position,
and velocity, as well as with the initialization method, that uses our
nifty vector class.

The first new function is is +evolve+, the one function that the driver
program invokes, and that guides all the work.  Here it is, by itself:

 :inccode: .body.rb+evolve

*Bob*: At the top, it sets two counters to zero.  One is +time+, which
measures how the simulated time during wchich the two-body motion is
integrated, so that the calculation can be halted properly after an
amount of time equal to <tt>dt_end</tt> has passed.  The other is
+nsteps+, the number of time steps that the system has taken.  I keep
track of that, since I like to know how much work the code has done,
at any given point.

*Alice*: With a constant time step, you could also divide the total
integration time by the step size.

*Bob*: Of course, but in future codes we'll soon switch to variable
time steps, in which case you have no independent way of knowing how
many steps the code took.  So we may as well get into the habit of
reporting that number.

*Alice*: In the third line you introduce a variable.  What is it
doing there, all by itself?  It looks a bit lonely and lost.

== Brevity and Modularity

*Bob*: Ah, the brevity of Ruby!  +e_nit+ is not a variable, it is a
method, in other words a function or subroutine, depending on your
dialect.

*Alice*: Without parentheses?

*Bob*: Without parentheses!  This is a method without arguments.
So why put parentheses around nothing?

*Alice*: But in any language I have worked with, if a fuction
+do_something+ has no arguments, you still have to invoke it with
<tt>do_something()</tt>.  Otherwise how will the compiler know that
it is dealing with a function, sorry, method, rather than a variable?

*Bob*: The compilers of the languages you are familiar with cannot.
But here we are dealing with a really _smart_ language.

*Alice*: You mean the Ruby compiler can figure out . . . 

*Bob*: . . . the Ruby interpreter, you mean . . .

*Alice*: . . . ah yes, the Ruby interpreter, how can it figure out
when a name stands for a variable and when it stands for a method?

*Bob*: Ruby will look around within the proper context, in this case
within the body of the class definition we are in . . .

*Alice*: . . . the body of +Body+ you mean . . . 

*Bob*: . . . yes, names can get confusing, even for human interpreters.
And the Ruby interpreter will then see a statement <tt>def e_init</tt>,
and will realize that the name <tt>e_init</tt> pertains to a method.

*Alice*: How elegant.

*Bob*: Indeed.  Of course, you are perfectly free to write <tt>e_init()</tt>
instead, if you like, but I prefer brevity.  So this third line of +evolve+
invokes the methode +e_init+ below.  We'll look at that in a moment, but
all it does is to compute the total energy of the system at the beginning
of a run, and stores the value in an internal variable, so that we can
compare the energy at a later time with the initial value, to see how much
the energy has drifted.

The next line invokes the method +write_diagnostics+, which does
exactly what its name says.  It has two arguments, +nsteps+ and +time+,
since those two counters have been defined locally within the body of
the +evolve+ method, and therefore they are not visible outside that
method.  I want +write_diagnostics+ to write those numbers as well, so
I have to pass them explicitly.

*Alice*: You could of course have defined them as internal variables
for the +Body+ class, how did you call them again?  Ah, instance variables.

*Bob*: Yes, but that would be confusing and potentially dangerous.  The
+Body+ class is growing bigger now, so I don't like to have an unnecessary
proliferation of internal variables that float around inside, visible for
every function.  Debugging then becomes much harder.  There is a reason
that the human body has cells and that cells have cell walls and that
within a cell there is the membrane around the nucleus as well as a variety
of other compartmentalized structures.

*Alice*: See, that is what I mean when I insist on modular programming.

*Bob*: If so, I agree with you on this level.  The problem I had with your
earlier descriptions was that you seemed to want to compartmentalize on
higher levels as well.  I am glad that each cell of my body does not share
its contents with every other cell.  But I would be unhappy if my left hand
would not know what my right hand would be doing.  And even though I don't
like to put my foot in my mouth, I'm glad I have the freedom to do so!

*Alice*: We'll come back to that.  For now, I'm glad we're in agreement
about modularity on a local level.  While creating larger programs, we will
move up to more global issues, and there we can fight things out with
concrete examples.  Meanwhile, I'll try not to tease you any longer about
modularity.  This code looks already as modular as you can ask for.

== Keeping Time

*Bob*: Moving right along, we see three lines of assignment.  Here I set
up a book keeping process that will tell us when we need to take some
action other than integrating.  The variable +t_dia+ stores the next time
at which we want to report energy diagnostics.  Initially we set it equal
to the time interval between diagnostics output, which is +dt_dia+.  Later
on during the integration, there may be many times at which you want to
see the energy being reported.  Each time that is done, the variable +t_dia+
is updated to the subsequent diagnostics output time, as we will see in a
moment.  So first we have a trivial looking <tt>t_dia = dt_dia</tt>, but
after the first output <tt>t_dia</tt> will be changed to <tt>2dt_dia</tt>,
then to <tt>3dt_dia</tt>, and so on, until the integration finishes.

In a similar way, the variable +t_out+ stores the time at which the next
particle output will be done, <i>i.e.</i> the time at which we print the
mass, position, and velocity of the relative motion.  The mass will
not change, of course, although we could introduce mass losing stars
or even exploding stars at some point later on.  But the position and
velocity will change, as we have seen.  Sometimes we may be interested
in getting frequent output of these numbers, for example when we will
make a movie of a particle motion, as we will do soon.  At other occasions
we may be more interested in studying the energy drift with a fine comb.
So by having +dt_out+ and +dt_dia+ as free dials to determing the output
frequency of of positions and velocities on the one hand, and the energy
on the other hand, gives us a lot of flexibility.

The variable +t_end+ is similar to the other two I just discussed, except
that it will not be updated during the integration: when +t_end+ is first
reached, that will be the end . . . 

*Alice*: . . . of a single call to +evolve+.  I presume that you can give
repeated calls to the +evolve+ method, from your driver, if you chose to
do so.

*Bob*: Indeed.  But each time you invoke +evolve+, everything starts from
scratch again within the body of +evolve+; the +evolve+ method itself has
no idea whether it has been called before or not.  It just does its thing,
starting with what it thinks is time zero.

*Alice*: So if you call +evolve+ twice, the time is set back to zero
in between.

*Bob*: For now, yes.  That just was the simplest way to implement it.
Time is used here only internally, for bookkeeping during a single
invocation of +evolve+.  Of course, I could have introduced a time
variable on the level of the driver.  But then I would have to pass
that also to +evolve+ and I thought that four arguments was enough for
a first attempt.  Anything can be made arbitrarily complex quickly, and
I like to keep things simple.

*Alice*: That makes a lot of sense.  So all we have left to look at within
+evolve+ is the +while+ loop, which keeps track of time.  When the time
+time+ exceeds the value of +t_end+, the method stops and control is
handed back to the driver, which called +evolve+.

*Bob*: Indeed.  And the first thing that happens is a call to +evolve_step+.
Like +evolve+, it integrates the orbit of our particle, but only for one
step.

*Alice*: Divide and conquer.

*Bob*: Yes, the level of +evolve+ is pure management; all the footwork is
down by +evolve_step+, step for step.

== xxx

*Alice*: In the next line, the time gets updated, by the amount of time
that was spent while stepping our particle forward by one step, and
similarly the +nsteps+ counter is increased by one.  I see what happens
next.  Unless there is an immediate need to do a form of output, for
energy of particle positions and velocities, we just move on.  And in
those cases where you do an output, you immediately schedule the next
output, by increasing the future output time by the corresponding output
interval.

*Bob*: Indeed.  And that's it!  Then next method we have seen before:

 :inccode: .body.rb+evolve_step

Newton in action: the acceleration is calculated and both position and
velocity are updated.

*Alice*: Using this coordinate-free notation.  I love it.  It is so compact!
Like a minicar that you love to drive in a crowded city.

*Bob*: Wait till you get to the diagnostics method, you'll love that even
more.  But first we have three short methods.  The first two calculate
the kinetic and potential energy for the relative motion:

 :inccode: .body.rb+ekin

 :inccode: .body.rb+epot

*Bob*: xxx

 :inccode: .body.rb+e_init

*Bob*: xxx

 :inccode: .body.rb+write_diagnostics

*Bob*: xxx




 
 . . . . . . . . . . . . 

 . . . . . . . . . . . . 

 . . . . . . . . . . . . 












*Bob*: Here is a fancy version, with command line arguments
and energy error diagnostics:

 :inccode: euler.rb

*Bob*: And here is how you run it

 :commandoutput: ruby euler.rb < euler.in > /dev/null

 :commandoutput: ruby euler.rb < euler.in



 . . . . . . . . . . . . 

  def epot                     # potential energy per unit of mass
    @ep = -1/sqrt(@pos*@pos)
  end

  def epot                     # potential energy per unit of mass
    r2 = 0
    @pos.each {|p| r2 += p*p}
    r = sqrt(r2)
    @ep = -1/r
  end

 . . . . . . . . . . . . 

  def ekin                     # kinetic energy per unit of mass
    @ek = 0.5 * (@vel * @vel)
  end

not

  def ekin                     # kinetic energy per unit of mass
    @ek = 0.5 * @vel * @vel
  end

Ask for free ordering of Ruby as interpreter option?

 . . . . . . . . . . . . 

 . . . . . . . . . . . . 

 . . . . . . . . . . . . 

We can use +irb+ to run it now, changing the options as we want:

  evolve(b, 0.01, 10, 100, 10)

<i>perhaps variable number of arguments?</i>
