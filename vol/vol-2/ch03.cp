= Particle Input

== How to Read

*Alice*: Hi, Bob, time to write some input routines?

*Bob*: I already got started.  However, I realize that input is quite a bit
more complicated than output, because the input routine has to recognize
hierarchical structures properly.

*Alice*: In general, the problem with input is that you have no control over
what you might find.  While doing output you can decide to just write
positions and velocities, for example, but while reading in an
existing file, you may find that someone wrote accelerations as well.

*Bob*: Good point.  And if we pipe the data from one program to the other,
we don't want to loose any data.  So it would be best to let the
input routine read in everything, and to let the output routine simply
echo whatever it did not understand.

*Alice*: I agree.  We'll have to add that to the <tt>to_s</tt> method
that we just wrote.  But let us first write the input method, so that we
can see what form this blind data handling will take.  What have you
written so far?

*Bob*: I started a new file <tt>iobody2.rb</tt>, in which I first copied
what we had done already in <tt>iobody1.rb</tt>.  The last thing we
did there was to write a +write+ function, for output to a file or to
the standard output.  The natural thing to do next was to write a
+read+ function.  As before, the default input choice would be the
standard input, but you can also give a file name instead, to read
data from a file.

Remember that our +write+ method looked like this:

 :include: .iobody1.rb-to_s_def
   . . .
   end

So my first thought was that the +read+ method should have
the following simple form:

   def read(file = $stdin)
   . . .
   end

since precision and indentation will be already provided by the data
that are being read in.

*Alice*: Sounds plausible.

== Passing the Type

*Bob*: However, I realized that we need an extra argument.  It took me a while
to figure this out, and I saw it only when I tried to imagine how the
method +read+ will be called.  At some higher level, a command will be
given to read a whole data file, which will start as something like

 begin ACS
   begin DSS
     begin particle globular_cluster
     . . .
       begin particle star giant AGB
         mass = 0.1
         position = 1.0 0.0 0.5
         velocity = 0.0 1.0 0.0
       end
     . . .
     end
   end
 end

On that level, the first two lines will be read and discarded, since
they only function as a safety check, guaranteeing that we are dealing
with the right type of data file.  This higher-level method will then
read in the line

     begin particle globular_cluster

and only at that point it becomes clear that the <tt>read</tt> method
of the +Body+ class has to be invoked.  But at that point the extra
information containing the type of the particle has already been read
in.  The third line tells us that the particle stands for the center
of mass of a globular cluster, which contains many other particles.
And the next line that starts with <tt>begin particle</tt> contains
our previous friend, the asymptotic giant branch star.

*Alice*: I see what you mean.  In the case of that last star, the method
<tt>read</tt> for the star will be asked to start reading the next input
lines, and it will only see

         mass = 0.1
         position = 1.0 0.0 0.5
         velocity = 0.0 1.0 0.0
       end

It can hand control back to the calling function when it encounters
the first +end+, but indeed it will never know that it was an AGB star.

*Bob*: Exactly.  So that additional information has to be passed to the
star.  The easiest and most general way is to pass the whole last line
that was read in by the calling function.
I therefore decided that the +read+ method should have the form

 :include: .iobody2.rb-1
   . . .
   end

where in the case of our star the variable +header+ contains the whole
line:

       begin particle star giant AGB

*Alice*: That makes sense.

*Bob*: By the way, you mentioned that control can be handed back to the
calling function when the first +end+ is encountered, but that is not
quite true.  As we saw above, a single +particle+ block can contain
several other +particle+ subblocks.  For example, when the calling
function asks the first particle, of type <tt>globular_cluster</tt>,
to read in its data, that particle will encounter the stars that
belong to the globular cluster.  For each star, it will call the
+read+ method for the appropriate +Body+ instance, and continue after
that method returns.  But since that star's +read+ method has gobbled
up its own +end+ line, the globular cluster +read+ method will never
see those embedded +end+ lines.  Indeed, the first +end+ it encounters
will be its own proper +end+.

*Alice*: So I was right, but for the wrong reason.

*Bob*: It's always nice to be lucky!

*Alice*: I'd rather be right, though.  Can you show me your +read+
implementation?

== Handling the Header

*Bob*: Here it is:

 :inccode: .iobody2.rb+read

*Alice*: I see.  First you process the +header+ which tells you what type
of particle you are dealing with and then you enter into a loop called +loop+
that processes subsequent lines from the file or input stream, until
you encounter a final +end+.

*Bob*: Yes, and the first thing to do is to check whether the +header+,
which is really an echo of the last line read, really starts with the
words <tt>begin particle</tt>.  In general there will be white space
in front, because of the identation, and one or more pieces of white
space in between +begin+ and +particle+ but everything else is only
allowed to follow those two words.

What you see at the right hand side of the first line of the method is
the +raise+ command, which raises and error if the +header+ does not
have the correct form.  In Ruby, +unless+ is the opposite of +if+, a
nice feature, since otherwise we would have needed to say ``if not of
the correct form'' and ``unless of the correct form'' is more natural.

The combination <tt>=~</tt> compares a string on the left with a
regular expression on the right, and returns a +nil+ value if the
string does not correspond to the regular expression, with is
surrounded by two slashes.

*Alice*: Let me try to remember my regular expressions.  The first
up arrow <tt>^</tt> means that it matches the beginning of the
expression.  Then <tt>\s</tt> stands for a white space, a blank or a
tab, followed by a <tt>*</tt> which tells us that we can expect zero
or one or more of those white spaces, while the <tt>+</tt> after the
second <tt>\s</tt> tells us that there should be at least one, and
possible more, white spaces between +begin+ and +particle+.

So for a string to match this regular expression, it should start with
zero of more white spaces, followed by the word +begin+, followed by
one or more white spaces, followed by the word +particle+, and optionally
followed by whatever else you like.

*Bob*: Indeed.  Now if that test is passed successfully, the header is
split of by the +String+ method +split+ into an array of words, as we
have seen before.  If there are only the two words +begin+ and
+particle+ no further action is taken.  But if there is at least one
more word, then all those extra words are glued together with +join+,
to form the type of the particle, stored in the instance variable
<tt>@type</tt>.  We have to give +join+ an argument to separate the
individual words, and a single blank space is the most natural choice.

== Handling Other Lines

*Alice*: So now we enter a loop, I presume.  What does +loop+ mean?

*Bob*: It stands for an endless loop.  In C you would write <tt>while(1)</tt>
or <tt>for{;;}</tt> depending on your taste, but Ruby has a more clean and
direct construct.  The only way to leave the loop +loop+ is to break
out explicity: you break with +break+.

*Alice*: Ruby seems to do exactly what you tell it to do!  The +case+
statement reminds me of the +switch+ statement in C and C++.

*Bob*: Yes.  In this case, +case+ is used to compare the first word of
each new line with the three legal choices we have so far for the names
of input and output data: mass, position and velocity.  When one of these
choices is encountered, the appropriate helper method kicks in, a simple
translator to floating point for the mass, and a translator to a
floating point vector for position and velocity.  Here is the first one:

 :inccode: .iobody2.rb+s_to_f

This time we give +split+ an explicit argument.  The default has been
to consider white space as the separator between words, but now we use
an equal sign <tt>=</tt> as a separator.  The name of the variable, in
our case the string +"mass"+ is returned as the value of
<tt>s.split("=")[0]</tt>.  Everything to the right of the equal sign,
in our case the value of the mass, is returned as <tt>s.split("=")[0]</tt>,
and promptly converted to floating point format with <tt>.to_f</tt>.

*Alice*: Everything, unless there are more equal signs, in which case the
following pieces would wind up in <tt>s.split("=")[2]</tt>, etc.

*Bob*: True.  Normally there should not be one and exactly one equal sign,
and I could have checked that by checking and raising an error condition
in case <tt>s.split("=").size != 2</tt>.  However, at this stage I'm happy
to live a bit dangerously.  And besides, the extra stuf will be ignored,
since it is not used.

*Alice*: Even so, I wouldn't want to continue if there is was an second
equal sing in the input line, since that would mean that there would be
something very seriously wrong.  Let's get back to error handling soon;
we really should teach the students some defensive programming, instead
of just assuming that the world is a paradise, and that nobody will hand
you wrong data.

*Bob*: I agree.  But first things first: let's get things to work.  Here
is my vector version:

 :inccode: .iobody2.rb+s_to_f_v

As before, it uses the +map+ method to convert each component of the vector
from a string to a floating point number.

*Alice*: And when you encounter a new +begin+ before you have reached the
+end+ of the current particle data input, you interpret that as a new
particle that is contained in the current particle, in the same way that
a star is contained in a star cluster.

*Bob*: Indeed.  Such a new particle should normally be indented one level
further, since it lies one level deeper in the hierarchy of particles, but
I don't check for that, since I don't want to insist on a particular style
of indenting.  Style is nice for human readers, but computers should
be able to handle anything logically reasonable.

*Alice*: And +subread+ is the method that handles those particles deeper in
the hierarchy, under the current particle.  What does it do?

*Bob*: I don't know yet.  I just put it there as a stub, a place holder to
remind us that we still had some work to do there.  Hmm.  Now that you ask
me, I guess it should just be +read+ again, a recursive invocation of the
same method that we are currently executing.

*Alice*: Yes, but you first have to know where to put the date you read
for the daughter particle.  I think you will have to create such a
particle, to start with, and then hand execution to that particle.

*Bob*: That sounds right.  Let's do that in a moment.

*Alice*: Well done!  This is clean and elegant, but I'm still worrying about
handling unusual lines.  If somebody hands you a line with

 acceleration = 0.1

your program will raise an error.  But there is nothing wrong with
providing extra information.  How about creating a little scratch pad
where you store all the lines for which you did not know what to do
with them.  You can just keep this scratch pad lying around, and by
the time you do an output, you echo its contents so that the information
is passed on correctly to the next function, which may know what to do
with it.

*Bob*: That is a good idea too.  Let's do that in a moment too, in another
moment.

== Testing

*Alice*: Did you test your code?

*Bob*: Not yet, but it's high time.  Let's try a write-read cycle.

*Alice*: You mean, you will output the content of one +Body+ instance
with its +write+ method and then read it in again into another +Body+
instance using that instances +read+ function?

*Bob*: Conceptually, yes, but in practice it is more complicated.
Remember that we haven't yet written the higher level function that
handles the top level lines <tt>begin ACS</tt> and all that.  That
function gobbles up not only those higher levels lines, but also the
header of the particle data.  Let's see.

*Alice*: Ah, of course, you're right.  This means that we will have
to pass the header by hand, for now, in the +read+ command, after which
we can feed in the remaining lines from the standard input, or from a
file, if we wish.

*Bob*: Exactly.  Let's check it, by writing another test
file <tt>test.rb</tt>:

 :commandinput: cat > test.rb END
 require "iobody2.rb"

 b = Body.new
 b.read("begin particle star giant AGB")
 b.write
END

 :output: cat test.rb

And here is the result:

 :commandinputoutput: ruby test.rb END
   mass =      1
   position =  2  3
   velocity =  4.5  6.7
   end
END

*Alice*: Just what it should be.  After typing in those four lines,
with whatever indentation, <tt>b.write</tt> generates the right
output.  Congratulations!

*Bob*: Let's see what happens if we give a wrong input line.

*Alice*: Okay.  How about giving it a supersymmetric particle,
for a change?

 :commandinput: cat > test.rb END
 require "iobody2.rb"

 b = Body.new
 b.read("begin sparticle")
 b.write
END

 :output: cat test.rb

And here is the result:

 :commandoutput: ruby test.rb

*Bob*: It works, but I admit, this type of error message is less
than helpful.

*Alice*: A third thing to do in yet another moment.  So here is our
to-do list:

* implement a scratch pad

* write the +subread+ method

* improve the error handling

*Bob*: And then of course we have to write the higher level read functions
to.  Okay, one thing at a time!
