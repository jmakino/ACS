= Particle Output

== Getting Started

*Bob*: Let's get some actual work done, after all our talking, last time.
Shall we code up an I/O implementation of our ACS data format we designed
yesterday?

*Alice*: Yes.  We definitely need to see a working model, before we can go
any further.  Let's start with the <tt>body.rb</tt> file we created while
we were experimenting with higher-order integrators, strip off everything
but the I/O part, and see whether we can rewrite that into our new format.

*Bob*: Okay.  Let's call it <tt>iobody.rb</tt>.  And since we will keep
adding and changing things, it is probably a good idea to keep a number
of versions around, so that we can always go back to see what we did
earlier.  We can start with a file called <tt>iobody1.rb</tt>, and after
we have some rudimentary functionality we just freeze it, and don't modify
it anymore.  From then on we will work on <tt>iobody2.rb</tt>, and so on.

*Alice*: That's a good idea, to keep a trail of previous versions.

*Bob*: So here is our starting point, a copy of what we did before, which
we can call <tt>iobody0.rb</tt>:

 :inccode: .iobody0.rb

== A Single Write Method

*Alice*: Ah, look, we started with two different ways of outputting our
data: we had a pretty way to list the data, using the +pp+ command,
for pretty printing, and we had a raw way to dump all the significant
digits, using <tt>simple_print</tt>.

*Bob*: Yes, it's all coming back now.  And given that we have introduced
a self-describing format, I guess we don't need two different methods
anymore.

*Alice*: Even so, it would be nice to control the number of digits.
A human reader may want to see only a few of the most significant digits,
whereas you need full double precision when you want to pipe data in and
out of programs read by the computer, so that you don't loose accuracy.

*Bob*: But instead of writing different methods, it would be better to
have only one method, with the number of digits as an argument.  Also,
let us follow the idea shown above with <tt>to_s</tt>, let us write the
output data first onto a string.  We can then use another method to
print out that string, or to write it to a file, as the case may be.

*Alice*: Good idea.  And by called that method <tt>to_s</tt>, we can
type <tt>print b</tt> for a particle +b+, since the Ruby command +print+
by default looks for a member function <tt>to_s</tt>.

*Bob*: Let us recall how we want a single particle to appear in the output.
We had decided on:

 begin ACS
   begin DSS
     begin particle star giant AGB
       mass = 0.1
       position = 1.0 0.0 0.5
       velocity = 0.0 1.0 0.0
     end
   end
 end

Let's not worry about how to print the first two lines; those will be
taken care of by a higher-level function.  What our <tt>to_s</tt>
should do is just write the middle five lines, the contents of a
+Body+ instance.

*Alice*: But with the proper indentation, which will depend on information
that is only available from outside the <tt>iobody.rb</tt> context.  The
calling function should provide the base amount of indentation, to start
with.

*Bob*: Indeed.  And while we're giving the users the freedom to specify
the numbers of digits of precision, we may as well allow them to specify
the incremental indentation between the <tt>begin particle</tt> line
and the subsequent lines.  How about something like

 :include: .iobody1.rb-to_s_def
   . . .
   end

*Alice*: Good.  That makes it clear that we intend to give 16 digits of
precision by default, enough to cover double-precision notation.  You're
quick at figuring out how to implement this, why don't you fill in the dots?

== Writing <tt>to_s</tt>

*Bob*: How about this?  As we discussed, I've put this now in file
<tt>iobody1.rb</tt>:

 :inccode: .iobody1.rb+to_s

*Alice*: I see that you return the string with all the output information
in the last logical line, which is actually wrapped over the last five
lines before the end.

*Bob*: Yes.  I start by adding <tt>base_indentation</tt> number of blank
spaces.  Ruby, with the principle of least surprise, lets you do that by
typing <tt>" " * base_indentation</tt>.

*Alice*: I find it actually quite surprising that you can simply multiply
a string with a number in such a simple way, because I'm not used to that
convenience in other languages.  But you're right, it does look very natural.

*Bob*: I then have to provide the main tag +particle+, which I have encoded
as a +Body+ class constant, by adding the following line to the +Body+ class:

 :inccode: .iobody1.rb-1

The rest of the tag, in our example <tt>star giant AGB</tt>, which I call
the type, in our case the type of particle, I assume will be stored in
an instance variable <tt>@type</tt>.  By default, when you create a
vanilla flavor +Body+ instance, there is no extra type information,
so I have added the following line to the +initialize+ method:

 :inccode: .iobody1.rb-3

and I added <tt>@type</tt> to the list of accessor macros:

 :inccode: .iobody1.rb-2

If a type is specified, then the string <tt>@type</tt> is inserted after
the string +TAG+, with a space in between, as you can see in the +if+ clause;
the +else+ clause does not add anything.

*Alice*: I must admit, that first line in <tt>to_s</tt> is a bit confusing,
but I guess I can make sense of it.  What appears to the right of the
<tt>=</tt> sign is a normal if-else construction, but without the
usual indentation.

*Bob*: Yes, it seemed a bit wastful of space to use five lines for what
can be easily written in half a line.  But note that I added the word
+then+, which you don't use when you write it over several lines.
Ruby insists on using +then+ for inline constructs like this, since
otherwise it would not know how to separate the condition from the
resulting action.

*Alice*: But I'm surprised that you can just assign the results of the
if-else construction to a variable.

*Bob*: A nice feature of Ruby, which will feel very natural once you have
used it a few times.  Here is what I could have written more explicitly:

    if @type
      subtag = " "+@type
    else
      subtag = ""

In inline-version that would have become

    if @type then subtag = " "+@type else subtag = "" end

But don't you think this is more short and simple:

    subtag = if @type then " "+@type else "" end

*Alice*: Shorter yes, but simpler only once you get used to it.
Okay, I see what is happening in this method.  You have postponed the
real work to the two methods <tt>f_to_s</tt> and <tt>f_v_to_s</tt>.
A nice example of top-down programming!

== Methods f_to_s and f_v_to_s

*Bob*: The real work is actually very simple, since we've done it already
in our previous version.  Here is the first method:

 :inccode: .iobody1.rb+f_to_s

*Alice*: So I guess +f+ stands for floating-point format, and <tt>f_to_s</tt>
indicates a conversion from a floating point number to a string.  That makes
sense, as a first step toward the more general <tt>to_s</tt> with converts
the whole +Body+ content to a string.  In fact <tt>to_s</tt> could be called
<tt>body_to_s</tt>.

*Bob*: Ah, but here is where Ruby's method notation shines: you invoke the
method <tt>to_s</tt> for a particular +Body+ instance +b+ by writing
<tt>b.to_s</tt>, which when you read it aloud sounds like b-to-s, and does
what you expect it to do.

*Alice*: You're right.  It is all very logical and consistent -- and concise
as well.  I like it.

*Bob*: Here is the second method:

 :inccode: .iobody1.rb+f_v_to_s

*Alice*: I see.  Earlier we have used a <tt>to_v</tt> method as an extra
method for the class +Array+, which is in fact a type of a-to-v method,
or array-to-vector.  But as you reminded me, a particular array +a+
will be converted by writing <tt>a.to_v</tt> which sounds just right.
And now you are using the same logic to define a v-to-s method, from
vector to string.

*Bob*: Yes, and I thought it would be more consistent to stress the fact
that we are not dealing with any type of vector, but with a vector that has
floating point values in it.  Hence the name <tt>f_v_to_s</tt>.

*Alice*: But we use vectors exclusively for physical quantities, that are
always represented as floating point variables.  Is it really
necessary to add this <tt>f_</tt> to stress that we are dealing with
floating point numbers?  You could as well write <tt>f_p_n_v_to_s</tt>
for floating-point-number-valued-vectors.

*Bob*: Ah, but look at the definition of the +Vector+ class; you will find
no mention there of floating point variables.  So it does make sense
to add that we are doing an extra conversion.  You can also look at the
<tt>simple_read</tt> input method that we defined before.  The position,
for example, was read in as follows:

 :inccode: .iobody0.rb-pos_f_v

So you see, from that point of view it is natural to make a
combination like <tt>to_f_v</tt>, as we will undoubtedly do later on
in our new read method.  For our write method this means that <tt>f_v_to</tt>
is natural.

*Alice*: I see your point.  But how about making it a bit more compact,
like <tt>fv_to_s</tt>?  I would prefer that, it is visually more pleasing.

*Bob*: But logically less correct, I would say.

*Alice*: Hmm, I don't think so.  But you wrote it, and it's not that
important, so let's do it your way.

== Testing

*Alice*: We still need a method to do the actual output.
Let me try something.  How about this:

 :inccode: .iobody1.rb+write

*Bob*: Yes, that should work.  By default this will print to the standard
output, and if you provide a file name, the output will be stored in that
file.

*Alice*: Let's test it.  Here is a test file <tt>test.rb</tt>

 :commandinput: cat > test.rb END
require "iobody1.rb"

b = Body.new(1, [2,3], [4.5, 6.7])
b.write
END

 :output: cat test.rb

And here is the result:

 :commandoutput: ruby test.rb

*Bob*: Looks good!  Let's give it a more modest accuracy.  Given the
order of the arguments to +write+, this means that we now have to
explicitly supply the file name +stdout+:

 :commandinput: cat > test.rb END
require "iobody1.rb"

b = Body.new(1, [2,3], [4.5, 6.7])
b.write($stdout, 4)
END

 :output: cat test.rb

Let's test it:

 :commandoutput: ruby test.rb

*Alice*: Just what it should be.  Let's see whether the indentation works:

 :commandinput: cat > test.rb END
require "iobody1.rb"

b = Body.new(1, [2,3], [4.5, 6.7])
b.write($stdout, 4, 16, 4)
END

 :output: cat test.rb

 :commandoutput: ruby test.rb

*Bob*: Perfect.  I think we've done enough writing now.  Time to start
reading in our new data format!

*Alice*: I agree.  But just to see the whole landscape,
can you show me what the file <tt>iobody1.rb</tt> looks like now?

== The File <tt>iobody1.rb</tt>

*Bob*: My pleasure:

 :inccode: .iobody1.rb
