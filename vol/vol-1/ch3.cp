= Chapter 3.  A Simple Integrator

== Getting Started

=== Ruby

*Alice*: So here we are, ready to begin writing a toy model to simulate
a dense stellar system.  We have decided to begin with a very simple
integrator, since we will use the material in a course for students
with little or no prior background in differential equastions and
numerical methods.

*Bob*: That pretty much defines the first integrator to discuss: one
based on the forward Euler integration scheme.  To take one step, you
add to each variable its derivative multiplied by the value of the
time step.  In other words, you just step forward by incrementing each
variable by its derivative, as specified by the differential equation.

*Alice*: The only way to make that sentence clear to someone with little
experience in this area is to give an example.  So let's code it up.

*Bob*: We decided to do this in Ruby, but neither of us have any
background in the language.  To get started, I had a look at an
introductory book, yesterday evening.  It is called <i>Programming Ruby</i>
by Dave Thomas and Andy Hunt, a.k.a. the Pragmatic Programmers.  I found
it quite useful, and it is also well written.

*Alice*: I'll have a look at it then as well.  Did you find enough to get
started?

*Bob*: I guess so.  Let's try and see.  Before getting to gravity,
and equations of motion, how about building a simple skeleton first,
something that just reads in the data and prints them out, without
doing any integrating?

*Alice*: But before doing that, we need to decide upon a data format.
Let's not get too fancy, for now.  How about just listing for each
particle the seven numbers on one line: mass, position, and velocity,
with the latter two each being a three-dimensional vector, and
therefore with three components each?

*Bob*: Well, in that case, let's start with an even simpler problem,
with reading and writing the data for a single particle.  For an
object oriented language like Ruby, that suggests that we create a
class +Body+ for a particle in an N-body system.

=== A Body Class

*Bob*: How about:

 :inccode: body1.rb

*Alice*: That was quick!  I can see that you've studied a manual or two.
What does +attr_accessor+ mean?

*Bob*: It is a short-hand expression.  The name itself is short for
being an accessor for attributes of a class, namely instance variables
for which the names start with the symbol <tt>@</tt>.  The meaning of the
expression 

 :inccode: .leap_attr_explanation.rb-attr

is syntactic sugar for

 :inccode: .leap_attr_explanation.rb-meaning

Here the first line is a type of `getter' which gets the value of such
an instance variable.  Using it, we can assign the value of the mass
of a Body to another variable, say +the_mass+:

 :inccode: .leap_attr_explanation.rb-use1

The second line is a type of `setter' which sets the value of an
instance variable.  We can assign a value to the mass of a Body as
follows:

 :inccode: .leap_attr_explanation.rb-use2

*Alice*: That makes sense, and I like the compact way in which <tt>mass=</tt>
is in fact the name of an operator that assigns values to internal
variables, what did you call them, instance variables?

*Bob*: Yes, instance variables are internal variables that hold
different values for each object you create of a class, where each
object is a different instance of the class.  This in contrast to
class variables, with names that start with <tt>@@</tt>, where there is only
one variable with a given name that holds the same value across a
class, for all instances of that class.

*Alice*: Hmm, I guess I should have a quick look at one of the tutorials
too, to keep up with you.

*Bob*: Yes, and then you will start calling these functions `methods'
instead of operators.

*Alice*: At least I didn't call them subroutines!  Is there a particular
book you recommend?

*Bob*: The standard book for learning the language is <i>Programming Ruby</i>
by Dave Thomas and Andy Hunt, a.k.a. the Pragmatic Programmers.

*Alice*: For now, let's move on, and I'll catch up with you later.  I
presume that the +initialize+ method tells Ruby how a new instance of
a Body class is created.

*Bob*: Indeed.  And it is invoked by a predefined method <tt>Body.new</tt>;
in general a method associated with a class is invoked by placing a
period between the class name (starting with a capital letter) and its
method (starting with a lower case method).  Let's try it out, and see
whether we can make a print-out of a newly created Body instance:

 :inccode: .body-init-demo1.rb-demo

Let's call this file <tt>test.rb</tt>.
Now let's run it:

 :command: cp -f body-init-demo1.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*Alice*: So far so good.  The +p+ command just dumps the value of the object
+b+ that is created.  The first number after <tt>Body:</tt> must be the id or
identifier, the unique name of that particular object for Ruby.

*Bob*: Aha!  I wondered whether you really had not looked at any manual yet.
You're right, but how did you guess?

*Alice*: Okay, I admit, I did browse through a few Ruby books in the
library, just for a few minutes, to get acquainted.

*Bob*: Let me try to assign values other than the default zeroes:

 :inccode: .body-init-demo2.rb-demo

And let's see a non-trivial output:

 :command: cp -f body-init-demo2.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*Alice*: Nontrivial indeed -- but how nice to get such clear instructions!
Quite a bit more helpful than <i>segmentation fault</i> or something
cryptic like that.

*Bob*: Agreed!  And yes, I should have presented the positions and velocities
as arrays, making three arguments in total.

 :inccode: .body-init-demo3.rb-demo

This should work:

 :command: cp -f body-init-demo3.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*Alice*: Much better.  Let's try the getter and setter commands.

 :inccode: .body-init-demo4.rb-demo

 :command: cp -f body-init-demo4.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

Well, that's simple and straightforward.  You can even use array
notation, and everything works just like you would hope it would.

*Bob*: I saw you hesitating when you typed that third line.  I would have
thought you would type something like:

 :inccode: .body-init-demo5.rb-demo

which would have given the same effect:

 :command: cp -f body-init-demo5.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*Alice*: Yes, you read my mind.  I had understood that "<tt>c.vel =</tt>"
is parsed by Ruby as an assignment operator "<tt>vel=</tt>"
associated with +c+ and frankly I did not expect that I could throw in
the component selector "<tt>[1]</tt>" without complaints from the
interpreter.

*Bob*: but it did the right thing!  This must be what they mean when
they say that Ruby is based on the principle of minimum surprise.

<b>[This is how far I got with my rewrite, 2004/1/19 -- Piet]</b>

== Improving the Output

*Alice*: So far we've only use the general dump command "+p+".  Certainly
useful when we run a script, since otherwise we would get no output at
all.  Let us try "+print+", which seems to be a general output command
in Ruby:

 :inccode: .body-print-demo.rb-demo

 :command: cp -f body-print-demo.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*Alice*: It still gives some information, but only the id.  And as with
the previous command, it returns +nil+ which probably means that there
is no particular internal value associated with this operation.  In
order to get some useful output, it seems that we have to do some work.
The manual page for +print+ states: "Objects that aren't strings
will be converted by calling their +to_s+ method."

*Bob*: And we haven't defined a +to_s+ method yet.  We will have to
construct a single string that contains all the information in a
nicely formatted way.  Following the principle of minimum surprise,
arrays such as <tt>pos[]</tt> and <tt>vel[]</tt> as well as single
variables such as +m+ will probably have already a +to_s+ method
associated with them.  Let's check.  How about this:

 :inccode: .body-to_s-wrong.rb-to_s

*Alice*: <i>[leave this paragraph out now? -- Piet]</i> Good idea to write
a comment at the top, to remind us what this class is for.  We may as
well get into the habit of writing comments as we code along, since
what seems obvious today may no longer be so next week or next month.

Let's see what this version will do.

 :inccode: .body-to_s-wrong-demo.rb-demo

 :command: cp -f body-to_s-wrong-demo.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*Bob*: Huh?  Why are the contents of the arrays smeared together this way?

*Alice*: Remember that Ruby is an untyped language.  The interpreter has
no way of knowing what to do with the components, as long as we don't
give it a hint.  For example, if we would be dealing with an array of
characters, then we might well want to get the all strung together;
that's were the word "string" comes from, after all.

== Pretty Printing

*Bob*: Good point.  So we should define our own field separator.  I saw
something to do that.  Here it is: the method +join+ converts an array
to a string, and you can give a separator as an arguments.  How about:

 :inccode: .leap.rb+to_s

*Alice*: Looks good, let's try:

 :inccode: .body-to_s-demo.rb-demo

 :command: cp -f body-to_s-demo.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*Bob*: Much better!  And I like the shorter prompt.  Interesting that
the order of the dumped variables seems to be arbitrary: the mass
value suddenly appeared at the end.  One more reason to define our own
<tt>to_s</tt> method, to have consistency in the output.

*Alice*: I'm curious to see what happens when we run a script directly
from the command line, rather than using the interpreter.  We may as
well use the <tt>to_s</tt> method to define a pretty printing method
+pp+ that we can then invoke directly, as follows:

 :inccode: .leap.rb+pp

 :inccode: .body-pp-demo.rb-demo

*Bob*: Let's run it:

 :command: cp -f body-pp-demo.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*Alice*: Pretty indeed, and much less clutter, only the essentials.

*Bob*: But at the cost of having to edit the file each time you want to
do something different.

*Alice*: I guess it depends on your taste, whether you find it easier to
make many small edits or to use an interpreter.

== Full Accuracy I/O

*Bob*: So where are we?  We have successfully defined a class +Body+,
and have learned to give it an initial value and to print it out.
Now we have to provide an input routine as well.

*Alice*: And remember our plan to chain our integrator?  If we use the
output of one invocation as the input for the next one, we don't want
to use accuracy.  If we use 64-bit floating point accuracy, often
called "double precision" for historical reasons, we'd better print
everything out with enought digits.

*Bob*: Good point.  Let's see.  If we use 15 digits, at roughly 10 bits
for every 3 digits, we cover 50 bits, which may be good enough.

*Alice*: And since this type of I/O is meant for machine-to-machine
handshaking, I suggest we output all seven numbers on one line, so that
it will be easy for the input routine to read particles in, having to
read exactly one line per particle.

*Bob*: Simple enough: let's call them <tt>simple_read</tt> and
<tt>simple_print</tt>.  The first one is simple, once you are familiar
with the C language.  For the second one we can use the +split+ method
that by default splits a string into space separated pieces, each of
which are loaded successively into an array.

*Alice*: That sounds like a good strategy.  But now we have to tell Ruby
that we live in a three-dimensional world, otherwise there would be no
easy way of knowing where <tt>pos[]</tt> ends and <tt>vel[]</tt> begins.

*Bob*: Of course we could count the number of fields in one line,
subtract one, divide by two and take that to be the dimensionality of
our problem.  But that is not very elegant.  And besides, different
lines might have erroneously different numbers of input values.  But
let us not worry about error handling for now.

*Alice*: Yes, we have enough on our hands to get things going.  Okay,
let's define the dimensionality as <tt>NDIM = 3</tt>.  This will make
it easy to deal with, say, two-dimensional systems such as a two-body
orbit, or a planetary system in a planar approximation, later on.

*Bob*: Since I don't like global variables, let's encapsulate +NDIM+ as
a class constant, with the +Body+ class:

 :inccode: .leap.rb+write
 :inccode: .leap.rb+read

Here is a test file <tt>test.rb</tt>:

 :inccode: .body-simple-io-demo.rb-demo

*Alice*: Let's see.  To run this, we now have to put in the values by hand:

 :command: cp -f body-simple-io-demo.rb test.rb
 :commandinputoutput: ruby test.rb END
3
0.1 0.2 0.3
4 5 6
END  
 :command: rm -f test.rb

*Bob*: Now let's chain the commands, by invoking it twice, piping the
values from the output of the first invocation to the input for the
second one.

 :command: cp -f body-simple-io-demo.rb test.rb
 :commandinputoutput: ruby test.rb | ruby test.rb END
3
0.1 0.2 0.3
4 5 6
END
 :command: rm -f test.rb

*Bob*: Congratulations!  This is what a mathematician would call a fixed point,
if we would view the operation <tt>ruby body05.rb</tt> as a mapping.

*Alice*: Which would be appropriate: when we finish our integrator, it
will transform initial conditions to final conditions after a certain
time +t+.  In that way, the integrator will act as a propagator,
mapping initial conditions onto final conditions.

*Bob*: I have noticed that you like to use terms from elementary
particle physics.  Well, I guess that our point particles are about as
elementary as they come, so it does make sense ;>).

== Looking Back

*Alice*: That was great, to be able to do such rapid prototyping in a
language we hardly knew.  I can see the advantages of an interpreted
typeless computer language.  If we had tried to do this in C++, it
would have taken quite a bit more time, and we would have had to write
quite a bit more lines of code.

*Bob*: Yes.  Defining a class, getting it to behave, providing I/O, and
letting it chain, all that is a nontrivial beginning.  This is
encouraging!  Let's move on, to see how much we have to add before we
can let the integrator integrate.

== Moving right along

<i>[I'm only putting in a few words for now; when we agree upon the
story, I will flesh it out and provide a complete dialogue --
Piet]</i>

Next our friends write an +Nbody+ class:

 :inccode: .leap.rb-nbody

Then they write a ruby script to generate files with sample initial conditions:

 :inccode: sample_init.rb

*Alice*: Time for a demo:

 :inccode: .mk_binary-demo.rb-demo

*Bob*: Let's run it:

 :command: cp -f mk_binary-demo.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*Alice*: Here is another one:

 :inccode: .mk_triple-demo.rb-demo

*Bob*: Here goes:

 :command: cp -f mk_triple-demo.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*Alice*: We can also test the reading and writing, with another file
<tt>test-io</tt>:

 :inccode: .nbody-simple-io-demo.rb-demo

*Alice*: Now we can pipe the triple output through this last script to
check whether we can the same result:

 :command: cp -f mk_triple-demo.rb       test.rb
 :command: cp -f nbody-simple-io-demo.rb test-io.rb
 :commandoutput: ruby test.rb | ruby test-io.rb
 :command: rm -f test.rb test-io.rb

*Bob*: Or to make inspection even easier:

 :command: cp -f mk_triple-demo.rb test.rb
 :command: cp -f nbody-simple-io-demo.rb test-io.rb
 :command: rm -f test.out test-io.out
 :commandoutput: (ruby test.rb > test.out)
 :commandoutput: (ruby test.rb | ruby test-io.rb > test-io.out)
 :commandoutput: diff test.out test-io.out
 :command: rm -f test.out test-io.out
 :command: rm -f test.rb  test-io.rb


