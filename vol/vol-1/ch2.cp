= Chapter 2.  Getting Started

== 2.1. Choosing a Flexible Language

<p>we need some explanation about why they choose ruby.  They are
probably tired with Fortran, have significant experience with C and C++,
and have played around a bit with other languages.  Any
suggestions?</p>

[Piet, I think the selection of the language comes _after_ we did some
experiments, writing things in fortran, C and something else, or at
least after discussing on the possibility of using them.

I *think* here we *have to* discuss the pros and cons of nemo and
starlab, in reasonable details. This may make Steve and/or Peter
unhappy, but do you think you can explain the reason to start from
scratch without discussing the limitation of existing packages?

I would have something like the following plot:

* first discuss the possibility to use nemo
  * its data structure is hardcoded to array, which is not ideal for many applications.
  * its data structure (for particles) cannot be extended easily
* then go to starlab
  * its data I/O is, well, great, but still lacks flexibilities
* Then discuss the limitation of statically-typed compiler-based   language
  * we could go around the limitations, by using templates and STL,  but...
  
* Then we can introduce a dynamically-typed language

* Then we naturally have speed concern.

* So we will write, as an experiment, substantial program in Ruby and  see the speed penalty
      
== 2.2. Rapid Prototyping

*A* So we are thinking about spending a few years to set up a software
environment for modeling dense stellar systems.  Where shall we start?

*B*: Certainly stellar dynamics is the easiest place to begin.  We
just consider all stars to be point masses, and model their gravitational
interactions.

*A*: Fine.  I guess this means we'll have to write a code to model the
gravitational N-body problem.  And for that, we may as well start with
the central engine: the integrator that provides numerical solutions
to the differential equations, the equations describing the effect of
Newtonian gravitation.

*B*: Let's do it.  An integrator first has to read in the positions
and velocities and masses of all particles at the start of the
calculation.  It then does its thing, and at the end prints out the
new positions and velocities.

*A*: And we may as well print out the masses too, even though they did
not change.  That way, we can chain subsequent invocations of the
integrator together, feeding the output of one command as the input to
the next command.

*B*: Good idea.  And at some point we will treat the masses as dynamical
variables as well, on a similar footing with positions and velocities.
When we will model realistic stars, they will undergo mass loss and
accretion.

=== 2.2.1. A Body Class

*A*: Let's build a simple skeleton for the integrator, something
that just reads in the data and prints them out, without doing anything.

*B*: But before doing that, we need to decide upon a data format.
Let's not get too fancy, for now.  How about just listing for each
particle the seven numbers on one line: mass, position, and velocity,
with the latter two being a vector with three components in three
dimensions?

*A*: In that case, let's start even simpler: with reading a writing a
single particle.  For an object oriented language like Ruby, that
suggests that we create a class +Body+ for a particle in an N-body
system.  How about:

 :inccode: .body.rb-barebones
  

*B*: This is only the class definition.  Let's see whether we can do a
little bit of useful work, by adding the following lines

 :inccode: .body-init-demo.rb-demo


    |gravity> irb -r body01.rb
    irb(main):001:0> b = Body.new
     => #<Body:0x401c7e7c @mass=0, @vel=[0, 0, 0], @pos=[0, 0, 0]>
    irb(main):002:0>

*A*: So far so good.  The interpreter echoes the value of the object
+b+ that is created.  The first number +0x401c7e7c+ must be the id or
identifier, the unique name of that particular object for Ruby.  Let
me try to assign values other than the default zeroes:

    irb(main):002:0> c = Body.new(1, 0.5, 0, 0, 0, 0.7, 0)
    ArgumentError: wrong # of arguments(7 for 3)</tt>
	    from (irb):2:in `initialize'
	    from (irb):2:in `new'
	    from (irb):2
    irb(main):003:0> 

*B*: How nice to get such clear instructions!  Quite a bit more helpful
than <i>segmentation fault</i> or something cryptic like that.

*A*: Indeed.  And yes, I should have presented the positions and velocities
as arrays, making three arguments in total.

    irb(main):003:0> c = Body.new(1, [0.5, 0, 0], [0, 0.7, 0])
    => #<Body:0x4023b31c @mass=1, @vel=[0, 0.7, 0], @pos=[0.5, 0, 0]>
    irb(main):004:0> 

*B*: Let's try to getter and setter commands.

    irb(main):004:0> c.mass
    => 1
    irb(main):005:0> c.pos[0]
    => 0.5
    irb(main):006:0> c.vel[1] = 0.8
    => 0.8
    irb(main):007:0> c.vel
    => [0, 0.8, 0]
    irb(main):008:0> 

*A*: Well, that's simple and straightforward.  You can even use array
notation, and everything works just like you would hope it would.

*B*: I saw you hesitating when you typed line 6.  I would have
thought you would type something like:

    irb(main):006:0> c.vel = [0, 0.8, 0]
    => [0, 0.8, 0]

which would have given the same effect.

*A*: Yes, you read my mind.  I had understood that "<tt>c.vel =</tt>"
is parsed by Ruby as an assignment operator "<tt>vel=</tt>"
associated with +c+ and frankly I did not expect that I could throw in
the component selector "<tt>[1]</tt>" without complaints from the
interpreter.

*B*: but it did the right thing!  This must be what they mean when
they say that Ruby is based on the principle of minimum surprise.

=== 2.2.2. Improving the Output

*A*: So far we've gotten output as a side effect, with the interpreter
echoing the value of everything it evaluates.  Let's see whether we
can get specific output.  Ruby seems to have a general command "+p+":

    irb(main):008:0> p c
    #<Body:0x4023b31c @mass=1, @vel=[0, 0.8, 0], @pos=[0.5, 0, 0]>
    => nil
    irb(main):009:0>

*B*: That produces the same output we already saw: "+p+" must be a
general type of dump command.  Certainly useful when we run a script,
since otherwise we would get no output at all.  But when we use an
interpreter, it doesn't add any information.  Let us type "+print+",
which seems to be a general output command in Ruby:

    irb(main):009:0> print c
    #<Body:0x4023b31c>=> nil
    irb(main):010:0> 

*A*: It still gives some information, but only the id.  And as with
the previous command, it returns +nil+ which probably means that there
is no particular internal value associated with this operation.  In
order to get some useful output, it seems that we have to do some work.
The manual page for +print+ states: "Objects that aren't strings
will be converted by calling their +to_s+ method."

*B*: And we haven't defined a +to_s+ method yet.  We will have to
construct a single string that contains all the information in a
nicely formatted way.  Following the principle of minimum surprise,
arrays such as <tt>pos[]</tt> and <tt>vel[]</tt> as well as single
variables such as +m+ will probably have already a +to_s+ method
associated with them.  Let's check.  How about this:

 :inccode: .body-to_s-wrong.rb-to_s

*A*: Good idea to write a comment at the top, to remind us what this
class is for.  We may as well get into the habit of writing comments
as we code along, since what seems obvious today may no longer be so
next week or next month.  Let's see what this version will do.

    |gravity> irb -r body02.rb
    irb(main):001:0> b = Body.new(3, [0.1, 0.2, 0.3], [4, 5, 6])
    => #<Body:0x401bfb50 @mass=3, @vel=[4, 5, 6], @pos=[0.1, 0.2, 0.3]>
    irb(main):002:0> print b.to_s
      mass = 3
       pos = 0.10.20.3
       vel = 456
    => nil
    irb(main):003:0> 

*B*: Huh?  Why are the contents of the arrays smeared together this way?

*A*: Remember that Ruby is an untyped language.  The interpreter has
no way of knowing what to do with the components, as long as we don't
give it a hint.  For example, if we would be dealing with an array of
characters, then we might well want to get the all strung together;
that's were the word "string" comes from, after all.

=== 2.2.3. Pretty Printing

*B*: Good point.  So we should define our own field separator.  I saw
something to do that.  Here it is: the method +join+ converts an array
to a string, and you can give a separator as an arguments.  Let's try:

 :inccode: .body.rb-to_s

*A*: The rather lengthy prompt of +irb+ is beginning to bother me.  I
read that we can customize it.  Shall we leave out the <tt>irb(main)</tt>
part?

*B*: Fine.  Here is the default definition; we may as well put that in
a comment statement, for future comparison, and then define our shot
prompt by leaving out the first two parts:

    ##  .irbrc

    ## This is the definition of the default prompt:
    ## 
    ##    IRB.conf[:PROMPT_MODE][:DEFAULT] = {
    ##          :PROMPT_I => "%N(%m):%03n:%i> ",
    ##          :PROMPT_S => "%N(%m):%03n:%i%l ",
    ##          :PROMPT_C => "%N(%m):%03n:%i* ",
    ##          :RETURN => "%s\n"
    ##    }
    ##
    ## This short version leaves out the first two parts.
    ##
    ## usage: irb --prompt short_prompt

    IRB.conf[:PROMPT][:SHORT_PROMPT] = {
          :PROMPT_I => "%03n:%i> ",
          :PROMPT_S => "%03n:%i%l ",
          :PROMPT_C => "%03n:%i* ",
          :RETURN => "%s\n"
    }

*A*: Okay, let's now see whether +join+ manages to disjoin our array
components:

    |gravity> irb --prompt short_prompt -r body03.rb
    001:0> b = Body.new(3, [0.1, 0.2, 0.3], [4, 5, 6])
    #<Body:0x401be944 @vel=[4, 5, 6], @pos=[0.1, 0.2, 0.3], @mass=3>
    002:0> print b.to_s
      mass = 3
       pos = 0.1, 0.2, 0.3
       vel = 4, 5, 6
    nil
    003:0> 

*B*: Much better!  And I like the shorter prompt.  Interesting that
the order of the dumped variables seems to be arbitrary: the mass
value suddenly appeared at the end.  One more reason to define our own
<tt>to_s</tt> method, to have consistency in the output.

*A*: I'm curious to see what happens when we run a script directly
from the command line, rather than using the interpreter.  We may as
well use the <tt>to_s</tt> method to define a pretty printing method
+pp+ that we can then invoke directly, as follows:

 :inccode: .body.rb-pp

 :inccode: .body-pp-demo.rb-demo

*B*: Let's run it:

    |gravity> ruby body04.rb
      mass = 3
       pos = 0.1, 0.2, 0.3
       vel = 4, 5, 6
    |gravity>

*A*: Pretty indeed, and much less clutter, only the essentials.

*B*: But at the cost of having to edit the file each time you want to
do something different.

*A*: I guess it depends on your taste, whether you find it easier to
make many small edits or to use an interpreter.

=== 2.2.4. Full Accuracy I/O

*B*: So where are we?  We have successfully defined a class +Body+,
and have learned to give it an initial value and to print it out.
Now we have to provide an input routine as well.

*A*: And remember our plan to chain our integrator?  If we use the
output of one invocation as the input for the next one, we don't want
to use accuracy.  If we use 64-bit floating point accuracy, often
called "double precision" for historical reasons, we'd better print
everything out with enought digits.

*B*: Good point.  Let's see.  If we use 15 digits, at roughly 10 bits
for every 3 digits, we cover 50 bits, which may be good enough.

*A*: And since this type of I/O is meant for machine-to-machine
handshaking, I suggest we output all seven numbers on one line, so that
it will be easy for the input routine to read particles in, having to
read exactly one line per particle.

*B*: Simple enough: let's call them <tt>simple_read</tt> and
<tt>simple_print</tt>.  The first one is simple, once you are familiar
with the C language.  For the second one we can use the +split+ method
that by default splits a string into space separated pieces, each of
which are loaded successively into an array.

*A*: That sounds like a good strategy.  But now we have to tell Ruby
that we live in a three-dimensional world, otherwise there would be no
easy way of knowing where <tt>pos[]</tt> ends and <tt>vel[]</tt> begins.

*B*: Of course we could count the number of fields in one line,
subtract one, divide by two and take that to be the dimensionality of
our problem.  But that is not very elegant.  And besides, different
lines might have erroneously different numbers of input values.  But
let us not worry about error handling for now.

*A*: Yes, we have enough on our hands to get things going.  Okay,
let's define the dimensionality as <tt>NDIM = 3</tt>.  This will make
it easy to deal with, say, two-dimensional systems such as a two-body
orbit, or a planetary system in a planar approximation, later on.

*B*: Since I don't like global variables, let's encapsulate +NDIM+ as
a class constant, with the +Body+ class:

 :inccode: .body.rb-simple-IO

 :inccode: .body-simple-io-demo.rb-demo

*A*: Let's see.  To run this, we now have to put in the values by hand:

    |gravity> ruby body05.rb
    3 0.1 0.2 0.3 4 5 6
     3.000000000000000e+00 \
     1.000000000000000e-01  2.000000000000000e-01  3.000000000000000e-01 \
     4.000000000000000e+00  5.000000000000000e+00  6.000000000000000e+00
    |gravity> 

*B*: And you quickly inserted some backslashes "<tt>\\</tt>" by hand to break
up the single line we so carefully crafted.  I guess you don't like to have
a line run over more than eighty columns?

*A*: It must be an atavism from my old FORTRAN days.  Now let's chain
the commands, by invoking it twice, piping the values from the output
of the first invocation to the input for the second one.

    |gravity> ruby body05.rb | ruby body05.rb
    3 0.1 0.2 0.3 4 5 6
     3.000000000000000e+00 \
     1.000000000000000e-01  2.000000000000000e-01  3.000000000000000e-01 \
     4.000000000000000e+00  5.000000000000000e+00  6.000000000000000e+00
    |gravity> 

*B*: Congratulations!  This is what a mathematician would call a fixed point,
if we would view the operation <tt>ruby body05.rb</tt> as a mapping.

*A*: Which would be appropriate: when we finish our integrator, it
will transform initial conditions to final conditions after a certain
time +t+.  In that way, the integrator will act as a propagator,
mapping initial conditions onto final conditions.

*B*: I have noticed that you like to use terms from elementary
particle physics.  Well, I guess that our point particles are about as
elementary as they come, so it does make sense ;>).

== 2.3. Looking Back

*A*: That was great, to be able to do such rapid prototyping in a
language we hardly knew.  I can see the advantages of an interpreted
typeless computer language.  If we had tried to do this in C++, it
would have taken quite a bit more time, and we would have had to write
quite a bit more lines of code.

*B*: Yes.  Defining a class, getting it to behave, providing I/O, and
letting it chain, all that is a nontrivial beginning.  This is
encouraging!  Let's move on, to see how much we have to add before we
can let the integrator integrate.
