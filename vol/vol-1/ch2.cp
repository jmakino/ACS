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

 :inccode: .body-init-demo1.rb-demo

Let's call this file <tt>test.rb</tt>.
Now let's run it:

 :command: cp -f body-init-demo1.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*A*: So far so good.  The +p+ command just dumps the value of the object
+b+ that is created.  The first number after <tt>Body:</tt> must be the id or
identifier, the unique name of that particular object for Ruby.  Let
me try to assign values other than the default zeroes:

 :inccode: .body-init-demo2.rb-demo

Let's see a non-trivial output:
 :command: cp -f body-init-demo2.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*B*: Nontrivial indeed -- but how nice to get such clear instructions!
Quite a bit more helpful than <i>segmentation fault</i> or something
cryptic like that.

*A*: Agreed!  And yes, I should have presented the positions and velocities
as arrays, making three arguments in total.

 :inccode: .body-init-demo3.rb-demo

This should work:

 :command: cp -f body-init-demo3.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*B*: Let's try the getter and setter commands.

 :inccode: .body-init-demo4.rb-demo

 :command: cp -f body-init-demo4.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*A*: Well, that's simple and straightforward.  You can even use array
notation, and everything works just like you would hope it would.

*B*: I saw you hesitating when you typed line 6.  I would have
thought you would type something like:

 :inccode: .body-init-demo5.rb-demo

which would have given the same effect:

 :command: cp -f body-init-demo5.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*A*: Yes, you read my mind.  I had understood that "<tt>c.vel =</tt>"
is parsed by Ruby as an assignment operator "<tt>vel=</tt>"
associated with +c+ and frankly I did not expect that I could throw in
the component selector "<tt>[1]</tt>" without complaints from the
interpreter.

*B*: but it did the right thing!  This must be what they mean when
they say that Ruby is based on the principle of minimum surprise.

=== 2.2.2. Improving the Output

*A*: So far we've only use the general dump command "+p+".  Certainly
useful when we run a script, since otherwise we would get no output at
all.  Let us try "+print+", which seems to be a general output command
in Ruby:

 :inccode: .body-print-demo.rb-demo

 :command: cp -f body-print-demo.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

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

*A*: <i>[leave this paragraph out now? -- Piet]</i> Good idea to write
a comment at the top, to remind us what this class is for.  We may as
well get into the habit of writing comments as we code along, since
what seems obvious today may no longer be so next week or next month.

Let's see what this version will do.

 :inccode: .body-to_s-wrong-demo.rb-demo

 :command: cp -f body-to_s-wrong-demo.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*B*: Huh?  Why are the contents of the arrays smeared together this way?

*A*: Remember that Ruby is an untyped language.  The interpreter has
no way of knowing what to do with the components, as long as we don't
give it a hint.  For example, if we would be dealing with an array of
characters, then we might well want to get the all strung together;
that's were the word "string" comes from, after all.

=== 2.2.3. Pretty Printing

*B*: Good point.  So we should define our own field separator.  I saw
something to do that.  Here it is: the method +join+ converts an array
to a string, and you can give a separator as an arguments.  How about:

 :inccode: .body.rb-to_s

*A*: Looks good, let's try:

 :inccode: .body-to_s-demo.rb-demo

 :command: cp -f body-to_s-demo.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

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

 :command: cp -f body-pp-demo.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

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

    |gravity> ruby body-simple-io-demo
    3
    0.1 0.2 0.3
    4 5 6
     3.000000000000000e+00
     1.000000000000000e-01  2.000000000000000e-01  3.000000000000000e-01
     4.000000000000000e+00  5.000000000000000e+00  6.000000000000000e+00
    |gravity> 

*B*: Now let's chain the commands, by invoking it twice, piping the
values from the output of the first invocation to the input for the
second one.

    |gravity> ruby body-simple-io-demo | ruby body-simple-io-demo
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

== 2.4. Moving right along [I'll change these titles/sections soon -- Piet]

<i>[I'm only putting in a few words for now; when we agree upon the
story, I will flesh it out and provide a complete dialogue --
Piet]</i>

Next our friends write an +Nbody+ class:

 :inccode: nbody.rb

Then they write a ruby script to generate files with sample initial conditions:

 :inccode: sample_init.rb

*A*: Time for a demo:

 :inccode: .mk_binary-demo.rb-demo

*B*: Let's run it:

 :command: cp -f mk_binary-demo.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*A* Here is another one:

 :inccode: .mk_triple-demo.rb-demo

*B*: Here goes:

 :command: cp -f mk_triple-demo.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*A*: We can also test the reading and writing, with another file
<tt>test-io</tt>:

 :inccode: .nbody-simple-io-demo.rb-demo

*A*: Now we can pipe the triple output through this last script to
check whether we can the same result:

 :command: cp -f mk_triple-demo.rb       test.rb
 :command: cp -f nbody-simple-io-demo.rb test-io.rb
 :commandoutput: ruby test.rb | ruby test-io.rb
 :command: rm -f test.rb test-io.rb

*B*: Or to make inspection even easier:

 :command: cp -f mk_triple-demo.rb test.rb
 :command: cp -f nbody-simple-io-demo.rb test-io.rb
 :command: rm -f test.out test-io.out
 :commandoutput: (ruby test.rb > test.out)
 :commandoutput: (ruby test.rb | ruby test-io.rb > test-io.out)
 :commandoutput: diff test.out test-io.out
 :command: rm -f test.out test-io.out
 :command: rm -f test.rb  test-io.rb


