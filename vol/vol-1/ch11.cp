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
turn +new+ invokes the _instance_ method +initialize+, in a way that
is hidden from the user.  Note that the error message mentioned a
<tt>private method `initialize'</tt> of the class +Body+.  This means
that the _class_ method +initialize+ is there alright, it just is not
publicly avaible -- which means in turn that you cannot use the
function from outside the scope of the class definition.  The method
<tt>Body.new</tt>, however, does have access; it is defined to be part
of the class +Body+, so it does have access to all the methods of
+Body+, even those that are private.

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

*Bob*: Now that you have successfully created and initialized a particle,
I bet you would like to change some of its internal state.

*Alice*: If we are going to integrate the orbit of a particle, we'll
certainly have to update its position and velocity.  But let me try
first with the simplest case, the scalar value of the mass.  How about
a bit of mass loss?

    irb(main):017:0> Body.@mass = 0.9
    SyntaxError: compile error
    (irb):17: syntax error
    Body.@mass = 0.9
              ^
            from (irb):17
            from ^C:0

*Bob*: That's what you get for not reading the manual!  In Ruby, the
internal variables of a class are all private.  There is no way that
you can access them from outside, either for reading or for writing.
If I would have designed a language, I wouldn't have been so strict,
but that's the way it is.

*Alice*: I actually think that is a _very_ good feature of Ruby.  If
you change some piece of code you or someone else has written a long
time ago, it is good to have a clear protocol about how to access
internal data.  A house has walls, an animal has a skin, a cell has
a cell wall, and there are good reasons for that!

*Bob*: Rather than getting into a modularity argument again, the good
news is that Ruby makes both of us happy: you have your encapsulation,
and I can almost pretend that it wasn't there, since it is very easy
to set up a mechanism to get around this cellular approach, with a
very natural syntax.  Let me show you how to do this in the most
straightforward way once, but then I'll move on to a much better
shortcut.

In order to change the mass we have to add the following line to the
class definition.

    irb(main):018:0> class Body
    irb(main):019:1>   def mass
    irb(main):020:2>     @mass
    irb(main):021:2>   end
    irb(main):022:1>   def mass=(m)
    irb(main):023:2>     @mass = m
    irb(main):024:2>   end
    irb(main):025:1> end
    => nil

*Alice*: Wait a minute!  You are now giving a new definition of the class
+Body+.  Doesn't that override the older definition that we have give
before?

*Bob*: Ah, but that's the beauty of Ruby, one of its many beauties, that
you can always add new features to a class, whenever you want!  Each time
you define features of a class or a module that already exists, Ruby adds
those features to whatever is already there.

*Alice*: It does look as if Ruby will make both of us happy!  You don't
feel encapsulated, because you can add anything any time, and I still
feel modular, since I know it all winds up internally inside one class
definition.  As for the two methods you introduced, they are effectively
`get' and `set' functions, I presume?

*Bob*: Yes.  The first method echoes the value of the internal variable
<tt>@mass</tt>.  While the variable <tt>@mass</tt> itself is private,
the methode <tt>mass</tt> is public by default, so this gives you the
simple way I promised, to access data that would be hidden otherwise.

The second method allows you to change the internal state of an object.
Note that the equal sign is part of the name.  The method is called
<tt>mass=</tt>, with one parameter +m+.  And the effect of calling
this method is to assign the value of its parameter to the internal
variable <tt>@mass</tt>.

*Alice*: The syntax <tt>mass=(m)</tt> looks rather odd, if you ask me,
this combination of an equal sign and parentheses.  Anyway, let's see
whether I understand it correctly.

    irb(main):026:0> d.mass
    => 1
    irb(main):027:0> d.mass=(2)
    => 2
    irb(main):028:0> d.mass
    => 2

Great!  It works as advertised.  Of course there was no need to type
the third line, but I just wanted to be sure that everything was
consistent.  However, I still don't like the syntax of <tt>=()</tt>.

*Bob*: Ah, but here is where Ruby's freedom of expression helps out:
those parentheses are optionally.  You needed them in the method
definition, to tell the Ruby interpreter that you were dealing with a
parameter that was an argument of a method.  But once defined, you can
leave them out when you give a command.  And you can even introduce a
space between <tt>mass</tt> and <tt>=</tt> if you like.  In general,
of course you can introduce spaces in the middle of a name, but in the
case of a method name ending on <tt>=</tt>, this is allowed.  This is
one of the many places where Ruby caters to the pleasure of the user,
and not to the pleasure of someone with a rigid logical bend.  Here
are a couple examples.

    irb(main):029:0> d.mass = 3
    => 3
    irb(main):030:0> d.mass=4
    => 4

*Alice*: Much better!  I like the pragmatic compromise between clarity
and ease of use.

== Syntactic Sugar

*Bob*: You may guess how we can give `get' and `set' functions for the
other internal variables.  Here is how we deal with the position

    irb(main):031:0> class Body
    irb(main):032:1>   def pos
    irb(main):033:2>     @pos
    irb(main):034:2>   end
    irb(main):035:1>   def pos=(p)
    irb(main):036:2>     @pos = p
    irb(main):037:2>   end
    irb(main):038:1> end
    => nil
    irb(main):039:0> d.pos
    => [0.5, 0, 0]
    irb(main):040:0> d.pos = [0.5, 0, 0.1]
    => [0.5, 0, 0.1]

*Alice*: The way you use the `set' function is different, in the sense
that for the position you now provide a vector, rather than a scalar.
But everything else is the same, in particular the definition.  It is
nice that Ruby is so homogeneous in its notion: it doesn't care at the
level of the definition whether a variable describes a scalar or a vector.

*Bob*: That is a nice aspect of dynamic typing.  And it also invites a
more compact notion.  Rather than writing everything all over again for
+vel+ instead of +pos+ above, you can use a convenient shorthand
notation, a piece of syntactic sugar as these are sometimes called.
The six lines above that define the two methods to read and write
+pos+ values can be replaced by the following single line:

    attr_accessor :pos

You can even add more than one variable on one line.  This line:

    attr_accessor :mass, :pos, :vel

replaces no less than eighteen lines of code written out in full.

*Alice*: Why the cryptic name?

*Bob*: Because there are two more elementary commands:

    attr_reader :pos

replaces the first method definition above, and

    attr_writer :pos

replaces the second method definition.  The word +accessor+ is meant
to suggest that you can both read and write, <i>i.e.</i> you have
two-way access to the variables, from outside.

*Alice*: I certainly prefer this compact notation.  But if we now add
that to our class definition, I may get confused, with bits and pieces
of the class definitions spread here and there throughout our +irb+
session.  Is it possible to put everything in a file, and somehow let
+irb+ have access to the definitions in that file?

*Bob*: Yes, that can be done.  First we put all the definitions in the
file <tt>body2.rb</tt>, where <tt>.rb</tt> is the standard ending for
a file name that contains Ruby code.  I added the <tt>2</tt> here because
this is our second attempt to define a +Body+ class, and I'm sure
there will be more.  I'll type it straight into the file, since it's
so short.  Here it is, <tt>body2.rb</tt>:

 :inccode: body2.rb

Now we can start a new +irb+ session by giving the name of a file that
will be loaded when +irb+ starts up, as follows:

    |gravity> irb -r body2.rb
    irb(main):001:0> b = Body.new
    => #<Body:0x400d4930 @pos=[0, 0, 0], @mass=0, @vel=[0, 0,
       0]>
    irb(main):002:0> b.mass
    => 0

*Alice*: That is much more convenient, to start a session with the
previous knowledge already in place.  Let me try something new

    irb(main):003:0> b.pos[1]
    => 0

Ah, that works.  So you can select a component of a vector and use
that directly in a reader function, and presumably also in a writer:

    irb(main):004:0> b.pos[1] = 0.5
    => 0.5
    irb(main):005:0> b
    => #<Body:0x400d4930 @pos=[0, 0.5, 0], @mass=0, @vel=[0, 
      0, 0]>

As expected.  And an array index in Ruby obviously start with a 0,
as in C and C++, rather than with a 1, as in Fortran.  <tt>b.pos[1]</tt>
is the second element of the array, while <tt>b.pos[0]</tt> is the
first element.

*Bob*: I saw you hesitating when you typed line 4.  I would have
thought you would type something like:

    irb(main):006:0> b.vel = [0.1, 0, 0]
    => [0.1, 0, 0]
    irb(main):007:0> b
    => #<Body:0x400d4930 @pos=[0, 0.5, 0], @mass=0, @vel=[0.1
      , 0, 0]>
    irb(main):008:0> 

which is an alternative but more clumsy way to change the element in
an array.  When you want to change more than one value, it is of course
easier to use array notation:

    irb(main):008:0> b.vel = [1, 2, 3]
    => [1, 2, 3]
    irb(main):009:0> b
    => #<Body:0x400d4930 @pos=[0, 0.5, 0], @mass=0, @vel=[1, 
      2, 3]>

*Alice*: Yes, you read my mind.  I had understood that "<tt>b.pos =</tt>"
is parsed by Ruby as an assignment operator "<tt>pos=</tt>"
associated with +b+ and frankly I did not expect that I could throw in
the component selector "<tt>[1]</tt>" without complaints from the
interpreter.

*Bob*: but it did the right thing!  This must be what they mean when
they say that Ruby is based on the principle of minimum surprise.
