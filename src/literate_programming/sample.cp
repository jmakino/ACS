= Coherent Programming

In this chapter, we introduce our idea of `coherent programming'.
Briefly, this is a software system designed to make it easy and
inviting to keep the text of related files up-to-date and consistent.

== History

When writing a series of books, it is important that the text for the
book chapters, the corresponding computer codes and their documentation,
as well as scripts of various kinds will all be modified in the proper
way as soon as one of these files is modified.  For example, adding a
few lines of code should reflect in slight modifications in manual pages
and book text, where appropriate.

In the ninety-seventies, Donald Knuth invented the notion of `literate
programming', where he solved the above stated problem by maintaining
only one source file, in which both book text and code fragments can
be found.  Special programs are then emplied to extract both the book,
for human consumption, and the source code, to be run on a computer.

Our notion of `coherent programming' is similar in spirit, but quite
different in implementation.  In the twenty-first century, it is easy
to load multiple files in a coherent way into a screen editor with
multiple windows, and to program the editor in such as way as to make
it hard or impossible to modify one file without also modifying the
corresponding places in related files.  This stands in stark contrast
to the much more primitive editors that Knuth had to deal with.  In
addition, Knuth wrote his original TeX system as one large Pascal file,
which made the process of code extraction rather straightforward.  In
any modern system, we need to deal with a large variety of shorter
code files in different languages, scripts, make files, and so on.
Since our implementation is so different from that chosen by Knuth, we
decided not to stretch his term `literate programming' to include our
version.

== An Example

We start by giving a simple example.  Here is the definition of a
celestial body, as a class Body in Ruby:

class Body

  NDIM = 3

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = [0,0,0], vel = [0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.join(", ") + "\n" +
    "   vel = " + @vel.join(", ") + "\n"
  end

  def pp            # pretty print
    print to_s
  end

  def simple_print
    printf("%22.15e", @mass)
    @pos.each do |x| printf("%23.15e", x) end
    @vel.each do |x| printf("%23.15e", x) end
    print "\n"
  end

  def simple_read
    s = gets
    a = s.split
    a.collect! { |x| x.to_f }
    @mass = a[0]
    @pos = a[1..NDIM]
    @vel = a[(NDIM+1)..(NDIM*2)]
  end

end

== Sample Session

And here is a sample session with the `irb' Ruby interpreter (one way
to get the information about our new Body class to irb is to include
the text of the Body definition above in the irb startup file `~/.irbrc').

irb(main):001:0> b = Body.new
=> #<Body:0x401c666c @mass=0, @vel=[0, 0, 0], @pos=[0, 0, 0]>
irb(main):002:0> b.pp
  mass = 0
   pos = 0, 0, 0
   vel = 0, 0, 0
=> nil
irb(main):003:0> c = Body.new(1, [1, 0, 0], [0, 1, 0])
=> #<Body:0x4023a0d4 @mass=1, @vel=[0, 1, 0], @pos=[1, 0, 0]>
irb(main):004:0> c.pp
  mass = 1
   pos = 1, 0, 0
   vel = 0, 1, 0
=> nil
irb(main):005:0> c.simple_print
 1.000000000000000e+00  1.000000000000000e+00  0.000000000000000e+00 \ 
 0.000000000000000e+00  0.000000000000000e+00  1.000000000000000e+00 \ 
 0.000000000000000e+00
=> nil

Note that the last line is wrapped here to present the results in a
more orderly way; in the actual irb session all seven numbers appeared
on a single line.

== Labnote Session

xxxxxx
xxxxxx
