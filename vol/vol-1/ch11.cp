= Introducing the Players

== Creating a Body

*Alice*: So how do you create a particle?

*Bob*: According to the book I read, you can simply type
<tt>b = Body.new</tt>, to get a new particle with the default values,
all zero in this case.

*Alice*: Wait a minute.  According to the rules you just told me, it
should be <tt>b = Body.initialize</tt>, since <tt>initialize</tt> is
the name of the one method that we have defined in our +Body+ class.

*Bob*: All I can tell you is what I read in the manual.  Let's try it
both!

    irb(main):012:0> b = Body.new
    => #<Body:0x4008a1b0 @mass=0, @vel=[0, 0, 0], @pos=[0, 0,
       0]>
    irb(main):013:0> c = Body.initialize
    NoMethodError: private method `initialize' called for Bod
      y:Class
            from (irb):13

*Alice*: I guess the writer of Ruby decided that typing +new+ is both
shorter and more natural than typing +initialize+.

*Bob*: No, it is more subtle than that.  I now see what is going on.
There is a fundamental distinction between <i>class variables</i> and
<i>instance variables</i> and similarly between <i>class methods</i> and
<i>instance methods</i>.  Here is the idea.

The method +new+ is a <i>class method</i>.  There is only one way to
create a new particle.  But as soon as you have done that, and
identified that new particle with the variable +b+ as a handle, then
you can use the <i>instance method</i> +initialize+ to give initial
values to that new particle.  Let us say that we create two particles.
Another way to say this is that we create two different objects of
the same class +Body+.  Yet another way to describe this: we create
two <i>instances</i> of the one <i>class</i> +Body+.  The process of
creating the two instances is the same.  But once an instance has come
into being as a separate particle, then it can be given individual
values for its mass, position and velocity.  This is why the <i>class
method</i> +new+ calls the <i>instance method</i> +initialize+ to give
each particle its proper internal values.

Note also the hierarchy.  When you issue the command +new+, then in
turn +new+ invokes the method +initialize+, in a way that is hidden
from the user.  Note that the error message mentioned a <tt>private
method `initialize'</tt>.  This means that the method initialize is
there alright, it just is not publicly avaible -- which means in turn
that you cannot use the function from outside the scope of the class
definition.  The method <tt>Body.new</tt>, however, does have access;
it is defined to be part of the class +Body+, so it does have access
to all the methods of +Body+, even those that are private.

Finally, at the danger of confusing you, but to make the explanation
complete, note that methods are by default always public.  This makes
sense, since in most cases you define methods as ways to interact with
particles.  It is relatively rare to define private methods, and you
do that only if you have a good reason: generally because you have
only one specific use for it, and a use that is strictly internal to
the module.  Clearly, the details of <i>how</i> to initialize a module
is something that is nobody else's business; the only thing the user
needs to do is to specify <i>what</i> the values will be with which
the particle will be initialized.  Now the only place where it is
reasonable to specify initial values is . . . initially!  And the only
method that you invoke to make a new +Body+ is when you give the
command <tt>Body.new</tt>.  This makes it crystal clear that +new+,
and no other method but +new+, should be allowed to pass initial
values to +initialize+.  So +new+ really does three things: it (1)
creates a standard empty version of a +Body+; (2) gives it a unique
+id+, thereby turning it effectively into an instance of the class
+Body+; (3) hands over the initial values to +initialize+ which in
turn assigns those values to the <i>instance variables</i>
<tt>@mass</tt>, <tt>@pos</tt>, and <tt>@vel</tt>.

*Alice*: I am glad you spoke slowly, since that was quite a stack of
ideas to keep track of, but it makes sense.  And I had to smile seeing
you praising the wisdom of encapsulation.  But what you explained is a
clear and consistent approach, once you see what is going on.  

== Initializing a Body

*Bob*: Phew, yes, that was a long explanation, but I'm very glad you
asked, and that we actually typed the wrong thing, since now I finally
understand completely what I read yesterday in the introduction to
Ruby.  And I also understand now why their are two types of internal
variables within a class.  Just as there are <i>class methods</i> and
<i>instance methods</i>, there are similarly <i>class variables</i>
and <i>instance variables</i>.  Instance variables always start with a
single <tt>@</tt> sign, while class variables always start with a
<tt>@@</tt> sign.  In our +Body+ definition we have introduced only
instance variables.  That makes sense, since different particles may
have different masses, likely have different velocities, and certainly
will have different positions.

*Alice*: What would be an example of a class variable?

*Bob*: Any variable that has a value that is shared by all instances of
that class.  In the case of a +Body+, we could introduce a class variable
<tt>@@time</tt> that gives us the current time for all bodies in the system.
If we have an integration scheme with a shared time step, it might be
a good idea to encapsulate the time within the +Body+ class.  However,
if we then switch to an individual time step scheme, we have to give
each particle its own time of last update.  In that case we should
specify it as <tt>@time</tt>, to make it an instance variable.

*Alice*: I see.  Yes, that seems like a good example.  So now I feel
completely comfortable in using <tt>Body.new</tt>.  It's time to
inspect the value that was echoed upon creation of a new body.  It
looked quite complex.

    irb(main):012:0> b = Body.new
    => #<Body:0x4008a1b0 @mass=0, @vel=[0, 0, 0], @pos=[0, 0,
       0]>

I recognize the values of the mass, and the components of position and
velocity, which are all zero by default.  But what is that hexadecimal
number doing there on the left?

*Bob*: After giving the class name of the object, it prints the id of
the object, a unique integer that is associated with the specific object,
which we called an instance of the class that it belongs to.
Useful for debugging perhaps, but for now we can ignore that.

*Alice*: A while ago you said that +mass+, +pos+, and +vel+ are possible
parameters.  What do you mean with `possible'?

*Bob*: I meant that you don't have to use them.  When you leave all of
them out, you get the default values.  When you specify one or more of
them, you have to specify them from left to right.  For example, when
you type <tt>b = Body.new(1)</tt> you give the particle mass an intial
value 1, rather than the default value 0, while keeping the other
values 0.  At least that's what I read.

*Alice*: easy enough to try:

    irb(main):014:0> c = Body.new(1)
    => #<Body:0x401018b8 @mass=1, @vel=[0, 0, 0], @pos=[0, 0,
       0]>

Good!  The mass is indeed 1, and this particle +c+ is distinct
from our previous particle +b+, since it has a different +id+.
But now how do you give a value to a vector?

*Bob*: That I'm not sure yet.  We should experiment and try it out.

*Alice*: that's what I like about software.  You can break things
without hurting and destroying something.  Okay, here are some
non-zero values for some of the position and velocity components:

    irb(main):015:0> d = Body.new(1, 0.5, 0, 0, 0, 0.7, 0)
    ArgumentError: wrong # of arguments(7 for 3)</tt>
	    from (irb):15:in `initialize'
	    from (irb):15:in `new'
	    from (irb):15

*B*: How nice to get such clear instructions!  Quite a bit more helpful
than <i>segmentation fault</i> or something cryptic like that.  The
debugger works its way out from the innermost nesting, back to its
caller function +initialize+ that is in turn called by +new+ -- just
as I told you!

*Alice*: Indeed.  And yes, I can guess now what went wrong.  I bet I should
have presented the positions and velocities as arrays, making three
arguments in total.

    irb(main):016:0> d = Body.new(1, [0.5, 0, 0], [0, 0.7, 0]
      )
    => #<Body:0x400df1f0 @mass=1, @vel=[0, 0.7, 0], @pos=[0.5
      , 0, 0]>

So there.  It worked!

== Assigning New Values

  xxxx

  xxxx

  xxxx


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
