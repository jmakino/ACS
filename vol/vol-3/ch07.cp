= A Final Version

== Clarity

*Bob*: It was fun to play with so many different versions, but I'm
beginning to get a little confused as to which version did what.
Maybe we should pick just one version, and use that while we add more
features and move toward specific applications.

*Alice*: I agree.  And since we decided not to worry, for now at least,
about performance, let us concentrate on clarity of expression.  I must
say, I like the last one best, in file <tt>rknbody6.rb</tt>, where
everything fits on one line.  However, the version where we gave the
+Body+ class explicit helper variables, in file <tt>rknbody4.rb</tt>,
was even shorter.

*Bob*: Yeah, but I really did not like the idea of giving this poor
+Body+ class all possible helper variables for all possible application.
I much prefered your suggestion to send a command string to be
evaluated dynamical, thereby generating the proper helper variables.
Just-time-delivery, much better!

*Alice*: But when clarity is really the criterion, I am not sure whether
the last version is really clearer.  Let us compare the forward Euler
algorithm in both cases, in <tt>rknbody4.rb</tt>:

 :inccode: .rknbody4.rb+forward

and in <tt>rknbody6.rb</tt>

 :inccode: .rknbody6.rb+forward

*Bob*: The last one is ever so slightly shorter.

*Alice*: That I don't mind so much.  I'm just not happy with the fact that
it is not clear, for a casual reader, what those variables <tt>@body</tt>
and +dt+ are doing, as the first two arguments of +calc+, and the appearence
of +ba+ is also a mystery; there is no indication here that +ba+ stands for
`body array' and will get its value from <tt>@body</tt>.  In contrast,
the earlier version has nothing hidden: the <tt>@body.each{|b| . . . }</tt>
construct is vanilla flavor for someone familiar with Ruby.

== Brevity

*Bob*: Perhaps we can improve the +calc+ method of +Nbody+ further.
How about redefining it in such a way that we can leave out the first two
arguments altogether?

*Alice*: Ah, that's a good idea.  It is also a logical next step, after
introducing the short cut notion of sending a string in the first place.
Once we do something that is somewhat dirty and not so self-explanatory,
we might as well go all the way.

*Bob*: I suppose that we would have to introduce an extra instance variable
<tt>@dt</tt> for +Nbody+.  Otherwise it will not be possible to leave
out the second argument +dt+ out of the current +calc+.  In fact, that would
make the definition even shorter.  So it would look, very cleanly,
like this:

 :inccode: .rknbody7.rb+forward

*Alice*: That is short and sweet, indeed.  And we have to modify +calc+
on the +Nbody+ level from what we had before:

 :inccode: .rknbody6.rb+calc+Nbody

to a version with only one parameter, namely the command string:

 :inccode: .rknbody7.rb+calc+Nbody

Since the other two parameters to the +calc+ method of +Body+ are now
both instance variables, their value is known here.

*Bob*: What else do we have to change?  We have to set the value of the
new variable <tt>@dt</tt> in the +evolve+ method, and we have to leave
out the +dt+ argument when invoking the integration methods.  Two
small changes, which leaves us with +evolve+ looking like this:

 :inccode: .rknbody7.rb+evolve

== Correctness

*Alice*: And the only other changes, besides the change in the forward Euler
algorithm we already saw, are the simplified readings of the three
other integrators.  They now become:

 :inccode: .rknbody7.rb+leapfrog

 :include: .rknbody7.rb+rk2

 :inccode: .rknbody7.rb+rk4

*Bob*: Time to check whether the new code does the same thing as all the
older ones:

 :commandoutput: ruby rknbody7a_driver.rb < figure8.in

*Alice*: And so it does.  Good!  I think we can stick with this version
as our work horse, for a while at least.

== xxx
