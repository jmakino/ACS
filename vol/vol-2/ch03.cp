= Particle Input

== 

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

*Bob*: I started with a file <tt>iobody2.rb</tt>, 
