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

*Alice*: Can you remind me what a class is?

*Bob*: I thought you were going to tell me, while pointing out how
important they are for your obsession with modular programming!

*Alice*: Well, yes, I certainly know the general idea, but I must
admit, I haven't really worked with object oriented languages very
much.  At first I myself was stuck with some existing big codes that
were written in rather arcane styles.  Later, when I got to supervise
my own students and postdocs, I would have liked to let them get a
better start.  However, I realized that they had to work within rather
strict time limits, within which to learn everything: the background
science, the idea of doing independent research, learning from your
mistakes, and so on.

The main problem for my students has been that there is hardly any
literature that is both interesting for astrophysicists and inspiring
in terms of a really modern programming attitude.  When students are
pressed for time and eager to learn their own field, they are not
likely to spend a long time delving in books on computer science,
which will strike them as equally arcane, for different reasons, as
the astrophysical legacy codes.

Given that there was no middle ground, I did not want them to focus
too much on computational techniques, because that would have just
taken too much time.  My hope is, frankly, that our toy model approach
will bridge the huge gap between the arcane and the arcane.

*Bob*: You always find a way to introduce a world wide vision for
every small task that you encounter.  As for me, I'm happy to just
build a toy model, and if students will find it helpful, I'd be happy
too.  But just to answer your question, defining a class is just a way
to bundle a number of variables and functions together.  Just like a
number of scalar values can be grouped together in one array, which
can stand for a physical vector for example, you can group a more
heterogeneous bunch of variables together.  You do this mainly for
bookkeeping reasons, and to keep your program simpler and more robust.

In practice, a good guide to choosing the appropriate class structure
for a given problem is to start with the physical structures that
occur naturally, but that may not always be the best option, and
certainly not the only one.

=== A Body Class

*Bob*: For example, a single particle has as a minimum a mass,
a position and velocity.  Whenever you deal with a particle, you
would like to have all three variables at hand.  You can't put them in
a single array, because mass is a scalar, and the other two variables
are vectors, so you have to come up with a more general form of
bundling.

The basic idea of this kind of programming is called object-oriented
programming.  In many older computer languages, you can pass variables
around, where each variable can contain a number or an array of numbers;
or you may pass pointers to such variables or arrays.  In an
object-oriented programming language, you pass bundles of information
around: for example all the information pertaining to a single
particle, or even to a whole N-body system.  This provides convenient
handles on the information.

If you look in a computer science book, you will read that the
glorious reason for object-oriented programming is the ability to make
your life arbitrarily difficult by hiding any and all information
within those objects, but I don't particularly care for that aspect.

*Alice*: What exactly can be an object?  Do you always need to have
a bunch of variables, or can you add functions as well?

*Bob*: To take the specific case of Ruby, a typical class contains both.
For a class to be useful, you have at least to be able to create an
instance of a class, so you need an initializer, roughly what is
called a constructor in C++.  This function can either be explicitly
be present or be hidden, but it is always there.

However, there are other objects in Ruby, besides classes.  Sometimes
you have a group of functions that are either similar or just work
together well, and you may want to pass them around as a bundle.  In
Ruby, such bundles of functions are called modules.  But to get started,
it is easier to stick to classes for now.

Note as a matter of terminology that what I have called a function, or
what would be called a subroutine in Fortran, is called a method in Ruby.

*Alice*: Ah, that is nice!  Does this mean that we can define an
integration algorithm as a module, independent of the particular
variables in the classes that define a body or an N-body system?  I
mean, can you write a leapfrog module that can propagate particles,
independently of there type?  You could have point particles in either
a two-dimensional of a three-dimensional world.  Or you could have
particles with a finite radius, that stick together when they collide;
as long as they are not too close, they could be propagated by the
same leapfrog module.

*Bob*: You really have an interesting way of approaching a problem.
We haven't even defined a single particle, and you are already
thinking about



Can you show me the syntax for creating a class that
contains those variables?

*Bob*: I think this is how we introduce a minimal class for a single
particle:

 :inccode: body1.rb

*Alice*: That is remarkably short and simple!  In fact, it seems too
simple.  I know that Ruby is an interpreted language and that is has
no types [explain more???].  However, I am surprised that we do not
have to declare the internal variables.  In other languages that I am
familiar with, it is essential that you tell the computer which memory
places to set aside before naming them.

*Bob*: I am not used to a untyped language either.  In a typed language,
you have to declare [right word?] each variable, but I guess that it
would be possible to allocate memory on the fly, as it is needed, from
context.

*Alice*: It seems so, yes.  So here there is only one function, starting
with +def+ and ending with the inner +end+, correct?

*Bob*: Indeed.  And the last +end+ is the end of the class definition, that
starts with <tt>class Body</tt>.  Note the convention that the name of
a class such as +Body+ always starts with a capital letter.  The names
of normal variables, in contrast, start with a lower-case letters:
we have three such variables, +mass+, +pos+, and +vel+.  All three are
given here as possible parameters to the initialization function
+initialize+.

*Alice*: So how do you create a particle?

*Bob*: According to the book I read, you can simply type
<tt>b = Body.new</tt>, to get a new particle with the default values,
all zero in this case.  Or when you type <tt>b = Body.new(1)</tt> you
give the particle mass an intial value 1, while keeping the other
values 0, and so on.

*Alice*: And so on?  How do you give a value to a vector?

*Bob*: That I'm not sure yet.  We should experiment and try it out.

*Alice*: Before doing that, what are the funny <tt>@</tt> signs?

*Bob*: Those indicate the internal variables associated with a particle.
In Ruby such variables are called <i>instance variables</i>.  The name
+Body+ applies to a class, namely the one defined above.  A command
like <tt>b = Body.new</tt> creates an instance of this class, a
specific object, for which we use the name +b+.  That is the reason
that the variables <tt>@mass</tt>, <tt>@pos</tt> and <tt>@vel</tt> are
called instance variables.  You see that the only thing the
initializer does is to copy the values of the parameters of the class
initialization to the instance variables.

*Alice*: All this reminds me of <tt>C++</tt>.

*Bob*: Indeed, the logical structure of <tt>C++</tt> class definitions
is very similar.

*Alice*: The main difference is that a <tt>C++</tt> class definition is
quite a bit longer.  I'm curious how much longer.  Do you remember how
to write a similar particle class in <tt>C++</tt>?

*Bob*: Let's see.  That shouldn't be too hard.  Always easiest to look at

Here is the <tt>C++</tt> version:

 :inccode: body1.C

*Alice*: xxx

*Bob*: Let's see whether it behaves as we think it should.  We can use the
interactive Ruby shell +irb+, and we can start it up with the
<tt>-r</tt> option that specifies a file to be loaded:

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
