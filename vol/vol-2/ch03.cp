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

However, I realized that we need an extra argument.  It took me a while
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

== 

*Bob*: Here it is:

 :inccode: .iobody2.rb+read






