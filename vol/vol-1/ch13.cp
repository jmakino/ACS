= Choosing an I/O Format

== Full Precision I/O

*Alice*: So where are we?  We have successfully defined a class +Body+,
and have learned to give it an initial value and to print it out.
Now we have to provide an input routine as well.

*Bob*: And remember our plan to chain our integrator?  If we use the
output of one invocation as the input for the next one, we don't want
to use accuracy.  If we use 64-bit floating point accuracy, often
called "double precision" for historical reasons, we'd better print
everything out with enought digits.

*Alice*: Good point.  Let's see.  If we use 15 digits, at roughly 10 bits
for every 3 digits, we cover 50 bits, which may be good enough.

*Bob*: And since this type of I/O is meant for machine-to-machine
handshaking, I suggest we output all seven numbers on one line, so
that it will be easy for the input routine to read particles in,
having to read exactly one line per particle.  Let's call them
<tt>simple_read</tt> and <tt>simple_print</tt>.  The first one is
simple, once you are familiar with the C language.  For the second one
we can use the +split+ method that by default splits a string into
space separated pieces, each of which are loaded successively into an
array.
