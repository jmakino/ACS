= ACS I/O

== The First Implementation

*Bob*: Let's get some actual work done, after all our talking, last time.
Shall we code up an I/O implementation of our ACS data format we designed
yesterday?

*Alice*: Yes.  We definitely need to see a working model, before we can go
any further.  Let's start with the <tt>body.rb</tt> file we created while
we were experimenting with higher-order integrators, strip off everything
but the I/O part, and see whether we can rewrite that into our new format.

*Bob*: Okay.  Let's call it <tt>iobody.rb</tt>.  Here it is:

 :inccode: init_iobody.rb

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

  def to_s(precision = 16, base_indentation = 0, additional_indentation = 2)
  . . .
  end

*Alice*: Good.  That makes it clear that we intend to give 16 digits of
precision by default, enough to cover double-precision notation.  You're
quick at figuring out how to implement this, why don't you fill in the dots?

== Writing <tt>to_s</tt>

*Bob*: How about this?

 :inccode: .iobody.rb+to_s

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

  TAG = "particle"

The rest of the tag, in our example <tt>star giant AGB</tt>, which I call
the type, in our case the type of particle, I assume will be stored in
an instance variable <tt>@type</tt>.  By default, when you create a
vanilla flavor +Body+ instance, there is not extra type information,
so I have added the following line to the +initialize+ method:

    @type = nil

If a type is specified, then the string <tt>@type</tt> is inserted after
the string +TAG+, with a space in between, as you can see in the +if+ clause;
the +else+ clause does not add anything.

*Alice*: I must admit, that first line is a bit confusing, but I guess I
can make sense of it.  What appears to the right of the <tt>=</tt> sign
is a normal if-else construction, but without the usual indentation.

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
real work to the two methods <tt>real_to_s</tt> and <tt>vector_to_s</tt>.
A nice example of top-down programming!

*Bob*: The real work is actually very simple, since we've done it already
in our previous version.  Here are the methods:

 :inccode: .iobody.rb+real_to_s

 :inccode: .iobody.rb+vector_to_s

*Alice*: Why did you not call the first one <tt>scalar_to_s</tt>?

*Bob*: Since it handles just a single number, it seemed sufficient to


== xx

*Alice*: We still need a method to do the actual output.
Let me try something.  How about this:

 :inccode: .iobody.rb+write

*Bob*: Yes, that should work.  By default this will print to the standard
output, and if you provide a file name, the output will be stored in that
file.

*Alice*: Let's test it.  Here is a test file <tt>test.rb</tt>

 :inccode: iobody_driver1.rb

And here is the result:

 :command: cp -f iobody_driver1.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*Bob*: Looks good!  Let's give it a more modest accuracy.  Given the
order of the arguments to +write+, this means that we now have to
explicitly supply the file name +stdout+:

 :inccode: .iobody_driver2.rb-barebones

Let's test it:

 :command: cp -f iobody_driver2.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb





