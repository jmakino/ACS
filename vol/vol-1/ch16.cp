= Debugging

== A Vector Class

*Bob*: Now that we can integrate, let me come back to these lines in
the code that you were not so happy with, containing a +k+ for the
components, remember?

*Alice*: Yes, do you have a way of avoiding any component notation,
even on the code level?

*Bob*: I think I do.  In a way, it is pure syntactic sugar, but then
again, you may say that a compiler or intepretar is one giant heap of
syntactic sugar on top of the machine language.  If it makes the code
much easier to read, I'm happy with it.

The trick must be to make sure that you can add vectors the way you
expect to add them in physics.

*Alice*: But according to the principle of least surprise, I would have
guessed that Ruby would be able to add two vectors, in array notation,
at least if they contain numerical entrees, no?

*Bob*: One person's expectation is another person's surprise!  Not
everyone approaches arrays with your physicist's view of the world.
Here, this is a good question for which to go back to +irb+.

    |gravity> irb --prompt short_prompt
    001:0> a = [1, 2]
    [1, 2]
    002:0> b = [3, 3]
    [3, 3]
    003:0> a + b
    [1, 2, 3, 3]

*Alice*: Hey, that's not fair!  Making a four-dimensional vector out
of two two-dimensional ones, how did that happen??

*Bob*: Ah, but see, if you were a computer scientist, or anyone not
used to working frequently with vectors, what could it possibly mean
to add two arrays?  You have an ordered collections of components, and
another such collection.  The components can be apples and oranges.

*Alice*: Or cats.

*Bob*: Yes.  Or numbers.  And the most sensible way to add those
collections is to make one large ordered collection, in the order
in which they were added.  Note that there is something interesting
hore for a mathematician: addition suddenly has become noncommutative.

*Alice*: I see what you mean.  Least surprise for most people.  How was
that again, about striving for the largest happiness for the largest
amount of people?  But still, I sure would like to have my vector addition.

*Bob*: Now the good news, in Ruby, is: you can, and rather easily!  All
you need to do is to introduce a new class, let us call it +Vector+,
and implement it as an array for which the addition operator <tt>+</tt>
does what you want it to do.  Let's try it right away.  I will put the 
definition of the +Vector+ class in the file <tt>vector.rb</tt>

 :inccode: vector1.rb

== Debugging

Before explaining what all this means, let us first see whether it works.

    |gravity> irb --prompt short_prompt -r vector.rb
    001:0> a = Vector[1, 2, 3]
    [1, 2, 3]
    002:0> b = Vector[0.1, 0.01, 0.001]
    [0.1, 0.01, 0.001]
    003:0> a += b
    [1.1, 2.01, 3.001]
    004:0> p a
    [1.1, 2.01, 3.001]
    nil
    005:0> a = b*2
    [0.2, 0.02, 0.002]
    006:0> a*b
    TypeError: cannot convert Vector into Integer
    	from (irb):6:in `*'
    	from (irb):6

*Alice*: Ah, what a pity.  We had vector addition, and also scalar
multiplication of a vector, but the inner product of two vectors
didn't work.

*Bob*: That is puzzling.  Hmmm.  Cannot convert Vector into Integer.
And all that somewhere in the definition of <tt>*</tt>.  There are
only three vectors involved, within the definition of <tt>*(a)</tt>.
There is the instance of the vector class itself, called +self+,
there is the other vector +a+ with which it can be multiplied in a
inner product, and there is the temporary vector <tt>product[]</tt>.
Neither of the first two are threatened anywhere with conversion
into an integer.  Nor doe the third one.  Hmmm.

But wait.  I have tried to use the same name <tt>product[]</tt> for a
vector, in the +else+ clause, and for a scaler, in the +if+ clause.
I thought that Ruby was so clever that you could do that.  Because a
type is determined only at run time, I thought that you could wait to
see which branch of the +if+ statement would be actually traversed, to
see which type Ruby is giving to +product+ array or scalar value.
In either case, that is what should be returned.

I guess I was wrong.  After all, Ruby is doing a lot of clever thinking
behind the scenes, in order to give us this nice behavior of minimum
surprise.  So I guess that Ruby noticed that product is being defined
as an array in the +else+ clause, while the value 0 is assigned to it
in the +if+ clause just above.  Could that be the reason that +irb+ is
complaining that a +Vector+ cannot be converted to an +Integer+?

If my theory is right, I should be able to resolve it by giving two
different names to the product, +iproduct+ for inner product, and
+sproduct+ for scaler product.  Let me try that:

 :inccode: vector2.rb

We have seen that addition and scalar multiplication all went fine.
Let me see whether we can get the inner product to work now.

    |gravity> irb --prompt short_prompt -r vector.rb
    001:0> a = Vector[1, 2, 3]
    [1, 2, 3]
    002:0> b = Vector[0.1, 0.01, 0.001]
    [0.1, 0.01, 0.001]
    003:0> a*b
    0.123

Ha!  That was it.  Interesting.  But at least it now works.

== An Extra Safety Check

*Alice*: I heard you talking to yourself, but I must admit, I couldn't
follow all that.  We'll have to step through the class definition a lot
slower, so that I can catch up.  But before doing that, are you _sure_
that it now works, and more importantly, that your understanding why it
went wrong is correct, so that you can avoid that error, whatever it was,
in the future.

*Bob*: Well, I must be right!  I reasoned my way through, starting from
the error message, and _voila_, the correct result appeared.

*Alice*: Well, you _may_ be right, but just to make sure, why don't you
repeat the exact same sequence of commands that you give before, all six
of them.

*Bob*: What good would that do?  They worked, and I didn't change anything
about the way they worked!

*Alice*: Just to make me feel better.

*Bob*: Oh, well, it's only six lines typing.  Here you are:

    |gravity> irb --prompt short_prompt -r vector.rb
    001:0> a = Vector[1, 2, 3]
    [1, 2, 3]
    002:0> b = Vector[0.1, 0.01, 0.001]
    [0.1, 0.01, 0.001]
    003:0> a += b
    [1.1, 2.01, 3.001]
    004:0> p a
    [1.1, 2.01, 3.001]
    nil
    005:0> a = b*2
    [0.2, 0.02, 0.002]
    006:0> a*b
    TypeError: cannot convert Vector into Integer
	    from (irb):6:in `*'
	    from (irb):6

I'll be darned!!

*Alice*: No comment.

*Bob*: How can that possibly be???  The same error message as before,
with the same operation.  And all the previous five tests were still
okay.  And yet, I just showed you that <tt>a*b</tt> worked, by itself,
with the same vectors no less!  This is spooky.

*Alice*: The same vectors?  But didn't you add +b+ to +a+ in line 3?
And wait, you changed +a+ again in line 5.

*Bob*: Oh yes, true, but what difference can that make?  Hmm.  Forget
I just said that.  All bets are off now, for me.  I will multiply those
last two vector values.  I won't go home before I've traced this bug,
somehow.  This is perplexing.  Okay:

    |gravity> irb --prompt short_prompt -r vector.rb
    001:0> a = Vector[0.2, 0.02, 0.002]
    [0.2, 0.02, 0.002]
    002:0> b = Vector[0.1, 0.01, 0.001]
    [0.1, 0.01, 0.001]
    003:0> a*b
    0.020202

WHAT?!?  I now typed in the exact values, and still it works.  But the
original sequence of six commands above didn't work.

== And Yet It Is

*Alice*: At least now we know what works and what doesn't.  And all
we have to do is to find out why.  Just to put it back to back, let me
repeat all six commands again, and print out the values of +a+ and +b+
before taking their inner product:

    |gravity> irb --prompt short_prompt -r vector.rb
    001:0> a = Vector[1, 2, 3]
    [1, 2, 3]
    002:0> b = Vector[0.1, 0.01, 0.001]
    [0.1, 0.01, 0.001]
    003:0> a += b
    [1.1, 2.01, 3.001]
    004:0> p a
    [1.1, 2.01, 3.001]
    nil
    005:0> a = b*2
    [0.2, 0.02, 0.002]
    006:0> p a
    [0.2, 0.02, 0.002]
    nil
    007:0> p b
    [0.1, 0.01, 0.001]
    nil
    008:0> a*b
    TypeError: cannot convert Vector into Integer
	    from (irb):8:in `*'
	    from (irb):8

*Bob*: This can't be.

*Alice*: And yet it is.  Something must be different.  Something must
have happened.  I wonder, do we know what +a+ and +b+ _really_ are?
They are the only variables in town, in this small session.  And the
session is so short, we should be able to look at them under a
microscope at every step.  And let us try to make our steps as small
as we can, doing only one thing at a time.

*Bob*: Okay, divide and conquer.  That's generally a good method.
Let me start a new +irb+ session.  Above we started by creating a
vector with three components, to which we assigned values.  One
way to split this command in into smaller steps is to separate the
creating and assigning parts.  First I'll create a vector and then
I'll assign its values.  I'll do the same for the second vector,
replacing the second command above by a creation and assignment
statement.

*Alice*: And then I suggest you replace the third command by a simple
addition, rather than the <tt>+=</tt> combination.

*Bob*: Good, more divide and conquer strategy!  Let's see what
sort of surprise we'll run into next.

    |gravity> irb --prompt short_prompt -r vector.rb
    001:0> a = Vector.new
    []
    002:0> a = [1, 2, 3]
    [1, 2, 3]
    003:0> b = Vector.new
    []
    004:0> b = [0.1, 0.01, 0.001]
    [0.1, 0.01, 0.001]
    005:0> a + b
    [1, 2, 3, 0.1, 0.01, 0.001]

AAHHHAAAA!!  There we have a clear smoke signal.  A new trail to follow!
But the trail seems to hang in mid air . . .

== Using a Microscope

*Alice*: Somehow Ruby has fallen back onto its old habit of adding by
concatenation.  But I thought you had taught Ruby to not do that any
longer?

*Bob*: I taught Ruby not to concatenate when adding vectors.  Normal
arrays will still be concatenated by addition.  But +a+ and +b+ are
_not_ normal arrays.  You are my witness that we have created both
+a+ and +b+ as vectors, brand new vectors, right out of the oven!
And yet they don't behave like vectors.  I'm beginning to despair.
There seems to be no room left here for introducing a bug.

Hmmmm.  Maybe we should go over the vector class definition in more
detail.  There just has to be something wrong there.

*Alice*: Before doing that, let us enlarge the magnification of our
microscope just a bit more.  We have taken smaller steps now, but we
haven't yet looked into the results of each step further.  In other
words, we have divided, but not yet conquered.

How about asking +a+ and +b+ themselves what they think is going
on?  For starters, we can ask them their type, what class they think
they are, with the <tt>class</tt> method we have seen before.

*Bob*: That's not a bad idea.  Actually a great idea.  Let's try it.

    |gravity> irb --prompt short_prompt -r vector.rb
    001:0> a = Vector.new
    []
    002:0> a.class
    Vector
    003:0> a = [1, 2, 3]
    [1, 2, 3]
    004:0> a.class
    Array

What the heck . . . !!  How can that possibly be?  We created +a+ as a
vector, it told us that it was a vector, we gave it some values, and
now it thinks it is an ordinary array.

*Alice*: It seems to have forgotten its vectorness.  Growing up in a bad
environment, it seems.

*Bob*: But how bad can this environment be?  Let me think, and stare
at the third line for a while.  Hmmppfff . . .  On the left hand side
we have a variable called +a+ that has just told us that it considers
itself to be a decent citizen of class +Vector+.  Good.  On the right
hand side we have an array with three components.  Good.  We give those
values to the vector and . . . hey, that must be it!

*Alice*: what must be what?

*Bob*: That's the answer!

*Alice*: You sure look happier now, but can you enlighten me?

*Bob*: Dynamic typing!!

*Alice*: Yes, Ruby has dynamic types, how does that solve our problem?

== Following the Trail

*Bob*: Elementary, my dear Watson.  Because Ruby has dynamic typing,
upon assignment of an array to a vector, the vector changes into an
array.  Instantly.  Like a chameleon, changing colors to adapt to its
environment!

*Alice*: I told you +a+ was growing up in a bad environment!  But yes,
now I see what you mean.  How very tricky.

*Bob*: And this explains all the error messages we have seen so far.
As long as we did not feed any of our vectors, and only printed the
results of operations on them, they remained pristine little vectors.
But as soon as we fed them a value, they flipped into the type of that
value, confusing us no end.  It all makes sense now . . . .

*Alice*: I have heard that before.

*Bob*: I know, I know.  But no I can _prove_ to you that we have traced
the bug.  We can easily repair our +Vector+ class.  So let's go back
into the file <tt>vector.rb</tt>.  Ahh, no, it is even easier!

*Alice*: What is?

*Bob*: See, this business with introducing +iproduct+ and +sproduct+,
you know what?  I bet that was not necessary.  I got sidetracked, in
the wrong direction.

*Alice*: Sidetracking usually gets you in the wrong direction allright,
but what was it that was wrong?

*Bob*: Ah, remember?  You said something about hearing me talking to
myself, and not being able to follow all that.  That was when I had
convinced myself that the type problem lie in the use of different
types for the variable +product+ in the +Vector+ class definition.
But my first attempt _was_ right, after all.  You _can_ perfectly
well use the same variable to stand for different types in different
branches of an +if+ statement.  It is _exactly_ because Ruby is a
dynamically typed language that you can do that!

So if I would have _really_ thought through what I was trying at the
very start, while I was writing the +Vector+ class, I would have
realized the cameleon behavior of Ruby variables.  The problem was
_not_ with switching color somewhere in the process, but the problem
was to make sure that the right color was being returned at the end!
Object oriented programming!  Epiphany!

*Alice*: Modularity!

*Bob*: Modularity if you like, right now I'm ready to go along with
anything.  This is wonderful, that a language can be so malleable!
Wow.  I can just begin to see how powerful Ruby is, when you can turn
any type into anything else, whenever and wherever you need it, without
having to declare and define various things at multiple places and then
recompile and link again and so on.  What a delight!
 
*Alice*: I'm glad that you're glad, and I see that you have undone all
your edits in <tt>vector.rb</tt>; how are you going to return the right
color?

*Bob*: Here, at these two places, where I previously wrote
<tt>sum = []</tt> and <tt>product = []</tt>.  By doing so, I dynamically
typed both +product+ and +sum+ to become an array.  At the end of the
<tt>+</tt> method, +sum+ appears, so the value of +sum+ is the value
that is returned by <tt>+</tt>, and it is an array, not a vector!
That is why <tt>a+b</tt> returned the concatenation of two arrays.
Similarly, the <tt>*</tt> method returns the value of +product+, also
an array.

So the remedy is really simple.  We just have to return vectors
instead of arrays.  And we can do that by declaring both +sum+ and
+product+ to be vectors from the start.  That is all!  Here it is:

 :inccode: vector3.rb

== Extreme Programming

*Alice*: Mind if we test it, by going through the _exact_ same moves
as before?

*Bob*: My pleasure!  And a pleasure it will be.  Here you go:

    |gravity> irb --prompt short_prompt -r vector.rb
    001:0> a = Vector[1, 2, 3]
    [1, 2, 3]
    002:0> b = Vector[0.1, 0.01, 0.001]
    [0.1, 0.01, 0.001]
    003:0> a += b
    [1.1, 2.01, 3.001]
    004:0> p a
    [1.1, 2.01, 3.001]
    nil
    005:0> a = b*2
    [0.2, 0.02, 0.002]
    006:0> p a
    [0.2, 0.02, 0.002]
    nil
    007:0> p b
    [0.1, 0.01, 0.001]
    nil
    008:0> a*b
    0.020202

*Alice*: As it should be.  Congratulations!

*Bob*: Well, I couldn't have done it without you.  Once I got my teeth
firmly into the problem, I must admit I felt quite stuck.

*Alice*: I couldn't even have started if you hadn't showed me how to
get going with Ruby.  I guess this idea of pair programming is much
more efficient than people think.  It seems that you can get more
lines of code written when you're alone behind a keyboard, and that
may or may not be true, but at least when chasing bugs, more eyes make
all the difference.

*Bob*: Yeah, I agree.  I've done almost all my coding in the privacy
of my own office, or home, depending; but when I worked with someone
else on a debugging session, it often helped to look at the same
problem from two different directions.

*Alice*: I find it funny to read so much about it these days in
computer productivity literature.  They make it sound as if this whole
notion of pair programming is this wonderful new invention.  When I
was at MIT as a graduate student, I dropped in at the AI department
quite frequently.  And everywhere I saw people coding together.  When
I did a little project there, I also teamed up with one of my fellow
students.  That was an eye opener for me, and ever since I have
prefered to program together with someone else.  Besides, it is more
fun too!

*Bob*: You're right.  Apart from my total annoyance at first with that
recalcitrant bug, it has been fun to explore together.  And I must admit,
even that bug gave me a lot of excitement.  Like watching a horror movie
and then finding that you're part of the movie itself.  But still, it
sounds a bit extreme to do all your programming with somebody sitting
next to you.

*Alice*: That must be why they call it <i>extreme programming</i> these
days, again as if they have just invented something new.

*Bob*: You're kidding.

*Alice*: No, I'm not.  They really call it that!  But enough management
talk.  We have certified our vector class, let's now put it to work for
our integrator.

*Bob*: Certified, he?  You must be born to become a manager.

*Alice*: Go integrate.
