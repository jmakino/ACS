= Two More Versions

== Extra +Body+ Variables

*Bob*: Now that we've begun to modify my original N-body code,
I would like to write a third version, where I keep local copies of
the auxiliary variables, such as <tt>old_acc</tt>, <tt>half_vel</tt>,
and the like, as instance variables of the +Body+ class.  That should
make the notation within the integrator methods a lot simpler.  I don't
like the notion of proliferating instance variables, though.

*Alice*: I don't think that would be such a bad thing. I could argue
that in object-oriented programming, it is appropriate to modify the
object if you use it for a different purpose.  And especially in Ruby,
such a modification will be easy.  You can always add a few lines in a
new class definition, and as you know, those new lines will be
directly added to the existing definitions.

*Bob*: Yeah, well, I still prefer to be parsimonious.  But I'm curious
to see how much simpler the integrators will become.  The first step
will be to add those auxiliary variables to the +Body+ class.  In the
first version of the code, I had:

 :include: .rknbody2.rb-1

In the alternative version, we left out the backward link:

 :include: .rknbody3.rb-1

And now let me put in the extra variables:

 :include: .rknbody4.rb-1

Ah, this is the only modification that we have to make to the +Body+ class,
so you're right, it is not as bad as I thought.  All other changes happen
only in the +Nbody+ class!

*Alice*: Let's start again with the forward Euler method, listing now all three
versions in order: your original one; the one we got by including an explicit
parameter in the +acc+ call; and the new version you're writing now with extra
variables in the +Body+ class:

 :include: .rknbody2.rb+forward

 :incode: .rknbody3.rb+forward

 :include: .rknbody4.rb+forward

== Clean Code

*Bob*: I must say, it is very gratifying to see how much cleaner this last
version looks.  Let me rewrite the other three methods as well, and call
this file <tt>rknbody4.rb</tt>.  Here they all are:

 :include: .rknbody4.rb+leapfrog

 :incode: .rknbody4.rb+rk2

 :include: .rknbody4.rb+rk4

*Alice*: A lot easier to read.  A great improvement over the previous
two versions, though not _quite_ as clean as the two-body version.  Let's
put up the +rk4+ method for the old two-body code:

 :include: .rkbody.rb+rk4

*Bob*: The difference is that in our latest version we still have to indicate
which body +b+ it is that gets the instructions, hence the "<tt>b.</tt>"
in front of each +pos+, +vel+, _etc_., call.

== Sending a String

*Alice*: Well, I have an idea.  Perhaps we can get rid of the "<tt>b.</tt>"
in front of each physical variable, after all.

*Bob*: How?  I mean, you have to loop over each particle!  I can't see how
you can get rid of that.

*Alice*: You can't get rid of that, I agree, but you don't have to repeat
the fact that you are looping many times in one line, as we are doing now.
I think we can express the idea of a loop just once in each line.

*Bob*: I still don't see how you can do that.  In the old two-body code,
we could get away with writing <tt>pos += vel*dt</tt> because there was
only one +pos+ and one +vel+, but here we have little choice but writing
<tt>b.pos += b.vel*dt</tt>, at a minimum.  And I already chose the shortest
name I could think of, +b+ for the body whose +pos+ gets updated!

*Alice*: My idea is to let the +Nbody+ class give a command to the +Body+
class, specifying directly to do <tt>pos += vel*dt</tt> for particle +b+,
without repeating the +b+ presence separately for +pos+ and +vel+.

*Bob*: That would be nice, yes, but it still looks impossible to do,
since it would involve something sending that whole line to the +Body+
class!

*Alice*: Exactly!

*Bob*: I beg your pardon?

*Alice*: You got it!  Let us send that line to the +Body+ class!  We
can ask the integration methods in +Nbody+ to specify what needs to be
done, but instead of making the actual command calls, these methods can
write the commands into a string, and then pass that string down to
the +Body+ class.

*Bob*: Hey, that is a great idea!  I would never have thought about that.
Can you really do that?  Well, of course you can.  I guess I'm still
thinking too much in Fortran and C terms.

*Alice*: Yes, Ruby invites a different way of thinking.  Even if you could
find a way to do such a trick in C++, in some convolved way, perhaps using
templates, the point is that it would not occur to you to do so, since the
language does not invite that way of thinking.  But when I browsed through
some Ruby code on the web, I came across something similar as what I just
suggested, and then I realized that that would be a very natural way of
passing instructions between classes.

*Bob*: Let's try it!  I'll create yet another file, <tt>rknbody5.rb</tt>,
and see how we can implement your idea.

*Alice*: This time, I suggest we start on the level of the +Nbody+ class.
In a wishful thinking sort of way, let us assume that we can tell a +Body+
named +b+ to calculate something, by issuing a command <tt>b.calc(s)</tt>,
where +s+ is a string that will get executed by +b+.  The forward Euler
method would then look like this:

 :include: .rknbody5.rb+forward

*Bob*: And indeed, with no mention of +b+ anymore within the string that is
passed as the third argument of +calc+.

*Alice*: The first two arguments are needed, because otherwise the +Body+
class will not know what to do with the string: it knows about its own
instance variables such as <tt>@pos</tt>, and it knows about the _method_
<tt>acc</tt>, but not about the _argument_ to <tt>acc</tt>, which has to
be specified explicitly.

*Bob*: I see.  What does +ba+ stand for?

*Alice*: Body array.  When +calc+ executes the call, it will replace +ba+
by its first argument, <tt>@body</tt>.  Similarly, it will replace +dt+
by +dt+.  I'll show you in a moment.  I hope it will all work.  And I'm
pretty sure it will.

== Implementation

*Bob*: You used the +calc+ method in the middle line of +forward+ as well,
even though you could have used the simpler statement

 :inccode: .rknbody4.rb-2

which we used before, in the previous version.

*Alice*: Yes, but my intention was to not break the symmetry between the
lines.  By treating all of them in the same way, your eye can be guided
to what is different on each line, forgetting the left half of each line,
which is the same in all cases.  Here, let me write the other three methods
as well, and then it will become more clear how the actual strings
that contain the commands will stand out:

 :include: .rknbody5.rb+leapfrog

 :incode: .rknbody5.rb+rk2

 :include: .rknbody5.rb+rk4

*Bob*: It's an improvement over the first two versions, which used
an index _i_.  In the previous version, we could leave out that obnoxious
_i_ index at the expense of introducing extra variables on the +Body+ level.
But now we can have our cake and eat it: no more _i_ and no more extra
variable with +Body+, I presume.

*Alice*: Correct!  The content of the string effectively
will declare the extra variables for us when the string is evaluated in
the +Body+ class.  Here is how I would write the +calc+ method for the
+Body+ class:

 :include: .rknbody5.rb+calc

*Bob*: Simplicity itself.  I see now what you meant, when you described
the way the two parameters +ba+ and +dt+ were going to be substituted
in an actual call to +calc+.

*Alice*: Almost too simple to be true.

== xxx

*Bob*: No matter how clever your approach is,
