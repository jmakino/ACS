= A Scratch Pad

== Extra Information

*Bob*: Our first to-do time was to implement a scratch pad, to contain
unrecognized data that can be read in, and to reproduce those safely
at the time of output.  I will copy the file <tt>iobody2.rb</tt>, with
which we have been working so far, into a new file <tt>iobody3.rb</tt>.

*Alice*: Let's call the scratch pad +rest+, since it will contain the
rest of all that we read in, whatever doesn't fit our expectations.
We can make it one big string, to which we keep adding whatever input
line that we don't recognize.

I don't think we should put any <i>a priori</i> limitations on
what such lines could contain.  Obvious choices are lines like

    acceleration = 0.1 -0.3
    density = 345.18

for physical quantities, or something like

    neighbors = 15 18 23

for a list of nearest neighbors, with particles identified by their numbers,
or in any other way for that matter, for example:

    neighbors = star5  star7  GMC3  triple8

As long as other appropriate programs can handle that format, the I/O routines
for the +Body+ class don't have to worry about them.

*Bob*: But how do you know which line makes sense?  You don't have the
information to know what all the other programs can handle.  What if someone
was planning to write an email, reporting on a run, and somehow by mistake
got the text mixed up with the input for a +Body+ instance.  Surely it would
be an error to read in:

    velocity = 1 0
    acceleration = 0.1 -0.3
    Hi Joe!  Look what a cool run I just produced.
    This AGB star is out of control!  Man, it's
    evolving with a time step much less than a year,
    and shrinking.  This way we'll never reach the
    horizontal branch.  Meanwhile, wanna come over
    for a beer?

*Alice*: Actually, this does not have to count as an error at all.  What if
the writer is planning to send Joe a data set, and would want to communicate
to Joe what the data are all about?  It would be much safer to add this
message to the actual data file, rather than sending it in a separate email.
We all know how many emails we get every day, and how difficult it is to
retrieve the right one, after weeks or months.

*Bob*: But there is no way of knowing the form that a human narrative
can take.  People might write <tt>!@#*!!</tt> if they are in a bad
mood.  By letting people write what they want you would allow
literally any line!

== Two Possibilities

*Alice*: Why not?

*Bob*: I know a good reason why not: next thing you know is to see
someone type <tt>begin position</tt>, for example the last three lines
above could have equally well been:

    and shrinking.  This way we'll never reach the
    begin position of the horizontal branch.  Meanwhile,
    wanna come over for a beer?

Remember that we allowed both

    position = 1 2

and also

    begin position
      1 2
    end

So the above version of the email chat would be very dangerous.

*Alice*: You have a point there.  So we have to be a bit more careful.
How about allowing only two types of `rest' lines: either such a line
should have an equal sign in it, to indicate that it is of the form
<tt>name = value</tt>, or its first word should be +begin+, in which
case we read in everything until we encounter the next same-level +end+.

So the above data could take as a legal form:

    velocity = 1 0
    acceleration = 0.1 -0.3
    begin story
      Hi Joe!  Look what a cool run I just produced.
      This AGB star is out of control!  Man, it's
      evolving with a time step much less than a year,
      and shrinking.  This way we'll never reach the
      begin position of the horizontal branch.  Meanwhile,
      wanna come over for a beer?
    end

*Bob*: I like the idea of extending the notion of self-describing data.
Your story idea will introduce self-chatting data!

*Alice*: I prefer self-narrating data.  Good!  Shall we introduce a +Story+
class?

*Bob*: Huh?  Why?  We had just decided that we will put all the <i>rest</i>
lines on a big pile, and store that in a string called +rest+.  Why
would you suddenly want to give extra structure to that string??

*Alice*: Because we have to distinguish the story structure above from
other unrecognized structures.  For example, imagine that some other program
includes information about a few multipoles for the internal structure
of our star.  That might take a form such as:

    begin multipoles
      begin monopole
        1.5
      end
      begin quadrupole
        0.3
      end
    end

Now there are two possibilities.  Here is the first one.  You can read
in the <i>rest</i> data in a hierarchical way, in which you keep track
of how many levels deep you go with the +begin+ and +end+ statements.
In that case you can read the complete multipole information, even if
your program has not the foggiest idea of what multipoles are, just by
counting levels and stopping when you encounter the first +end+ on the
same level as the +begin+ that was associated with +multipoles+.

Using the first possibility, however, will let your program crash when
you try to read in the narrative above, starting with <tt>begin story</tt>.
The input mechanism sees <tt>begin position</tt> later on, and it will presume
that the last +end+ belongs to the same level of <tt>begin position</tt>, so
it will keep searching for the extra +end+ that it expects to correspond
to <tt>begin story</tt>.

The second possibility is to ignore any +begin+ statement at the start
of a line, and just to keep reading on till you find an +end+, all alone
on a line.  That would solve the problem for the story above.  And in
practice, even if you would write a single "end" at the end of a sentence,
most likely you would put a period or question mark or exclamation mark
after the end.

So the second possibility is pretty safe, although not completely safe,
as far as a story goes.  But what is much worse, this second method fails
miserably when reading in the multipoles.  It would stop at the first
+end+, then assume that <tt>begin quadrupole</tt> would be a new item,
read that in until its proper +end+, and then it would encounter the
final +end+.  At that point it would think it had read the whole particle
structure, which is not the case.

== More Possibilities

*Bob*: I see.  Hmmm.  That is tricky.  And you were thinking to solve that
by introducing a special +Story+ class, for which you use the second solution,
whereas you use the first solution for all other cases?

*Alice*: Yes, that was my first thought.  But now that I have put it all
on the table, I'm not so sure whether that would be a good solution.  For
one thing, it is not completely fail-safe: an email, say, can indeed
contain the word <tt>"end"</tt> instead of <tt>"end."</tt>.  Many
young people these days seem to completely ignore punctuation.

*Bob*: And many old people can leave out a period, as a typo, especially
when they forget to bring their reading glasses.

*Alice*: Just you wait!  Before too long you'll have to choose between
bifocals and reading glasses.  But I guess we both agree that my initial
thought is not safe enough.  Hmmm.

*Bob*: There is another problem with your initial thought.  What if you
want to keep a log of previous commands, as in the Unix history mechanism?
You might want to include that as

  begin history
    make_binary -M 2 -m 3
    integrate -t 10
    find_orbital_elements
  end

It is possible that such a list of commands would include a command called
+begin+.  Why not?  So it is not only the <tt>begin story</tt> that would
need a +Story+ class.  you would need a +History+ class, and so on,
one class each for each different type of application.  I don't like that.

*Alice*: Well, what else do you propose?

*Bob*: One possibility would be to check indentation.  If you encounter
an +end+ on the same level of indentation as the rest of the lines within
a block, it does not mean that the block ends; it only counts as a
real end if it is indented by one or more spaces less.  Similarly, a
+begin+ should only signify the beginning of a new block if the next line
is indented by one or more spaces, compared to the line starting with
+begin+.

*Alice*: But then the higher-level program should pass more than just the
header, as we have implemented above.  It should read one more input line
before it can decide that <tt>begin particle</tt> really meant the start
of a new particle structure, or whether it was part of a chatty email
as we saw above.  But once it had read in that line, it has to pass it
to the +read+ function of +Body+.  So we would get something like

  def read(header, next_line, file = $stdin)
  . . .
  end

And what is worse, instead of going directly into the loop +loop+, you
would have to first process this <tt>next_line</tt> before you can pick
up more lines with <tt>file.gets</tt>

*Bob*: I agree, that will make things ugly.  Hmmm again.  Well, perhaps
we can invent more complex words that +begin+ and +end+.  If we write
+acs_begin+ or even <tt>!@#*!!_acs_begin</tt> and similarly
<tt>!@#*!!_acs_end</tt> we would be safe enough.  What is the chance
that someone would type those combination of characters by chance in a
chatty email?

*Alice*: Not in an normal email, no.  But if someone will include a
piece of natural language text to explain what a bunch of data represent,
and if the data are in our acs format, chances are that that person may
also explain how to read and write those data.  And, guess what, that
person will have to write <i>exactly</i> the expressions
<tt>!@#*!!_acs_begin</tt> and <tt>!@#*!!_acs_end</tt> . . .

*Bob*: Yes, that is a catch.  I don't see an way around that.  But hey,
wait, there is a way!  We can ask this person to write something like
<tt>\!@#*!!_acs_begin</tt>, and provide a way to translate that into the
proper <tt>\!@#*!!_acs_begin</tt> when the story is being processed for
a human reader.

But the more I think about that, the more I dislike the idea.  Who
would want to look at files that have <tt>\!@#*!!_acs_begin</tt> and
<tt>\!@#*!!_acs_end</tt> everywhere in them?  ACS will get a bloody
bad reputation with what looks like curse words sprinkled in everywhere.

== A Box

*Alice*: It seems that we're running out of options.

*Bob*: And yet we have to solve it, at least if we want to allow self-narrating
data.  I must see, I got warmed to the idea, and I don't like to give
that up, just because we have some difficulty figuring out how to implement
it.

*Alice*: You said you didn't like to make a whole slew of exceptional cases,
for <tt>begin story</tt> and <tt>begin history</tt> and what not.
Here is an alternative.  Let us protect the content of a story or a history
or whatever by somehow putting it into a safe box, wrapping it up in
something . . .

*Bob*: . . . by putting four lines around the text as in a children's
drawing?  I wish we could do that.

*Alice*: A line!

*Bob*: A line?

*Alice*: You found the solution!  Or more accurately, one quarter of what
you just found is the solution.  We need to put a vertical line in
front of the text, at the left-hand margin.  In other words, a comment
symbol in front of each line.

*Bob*: Ah, of course, like you use a <tt>#</tt> in Ruby or a <tt>C</tt>
in Fortran or a <tt>//</tt> in C++ or a percent sign in Latex.  Yes, I
like that.  In that way we can allow any part of a story or history or
anything else to be commented out, so to speak, making both +begin+
and +end+ invisible for the I/O routines.

*Alice*: So our example for the <i>rest</i> data could become

    velocity = 1 0
    acceleration = 0.1 -0.3
    begin story
      |Hi Joe!  Look what a cool run I just produced.
      |This AGB star is out of control!  Man, it's
      |evolving with a time step much less than a year,
      |and shrinking.  This way we'll never reach the
      |begin position of the horizontal branch.  Meanwhile,
      |wanna come over for a beer?
    end

You could it even put in a real box, as a children's drawing, if you want:

    velocity = 1 0
    acceleration = 0.1 -0.3
    begin story
      +----------------------------------------------------+
      |Hi Joe!  Look what a cool run I just produced.      |
      |This AGB star is out of control!  Man, it's         |
      |evolving with a time step much less than a year,    |
      |and shrinking.  This way we'll never reach the      |
      |begin position of the horizontal branch.  Meanwhile,|
      |wanna come over for a beer?                         |
      +----------------------------------------------------+
    end

*Bob*: Very funny.  But yes, you could use any symbol you like.
The least obtrusive would be a period, just as the Unix system
does for files that are normally invisible:

    velocity = 1 0
    acceleration = 0.1 -0.3
    begin story
      .Hi Joe!  Look what a cool run I just produced.
      .This AGB star is out of control!  Man, it's
      .evolving with a time step much less than a year,
      .and shrinking.  This way we'll never reach the
      .begin position of the horizontal branch.  Meanwhile,
      .wanna come over for a beer?
    end

*Alice*: And whatever symbol you use, nothing will match +begin+ and +end+
anywhere.  I think we have found a fail-safe solution!  A nice surprise,
after we both thought that we were stuck.

== Onward

*Bob*: Isn't it interesting?  You can use comments in a program for many years
and never give it much thought.  But when you have to design a special data
format, as we are doing for ACS, you are in fact designing a kind of
mini-language.  So we have just reinvented the wheel!  Now I can appreciate
much better the role of comment conventions in computer languages.

*Alice*: Of course, anyone using the data will still have to find a way
to strip the comment symbols of, if they want to work with clean text.

*Bob*: However, it is less urgent.  In this last example, the leading periods
are almost invisible.

*Alice*: For some purposes, yes, but for other applications I'm sure that
you may want to implement a way to get rid of the comment characters.

*Bob*: If you like.  But first onward to get to graphics.  Any good
software project can be stalled completely by implementing a surplus
of features before you really need them -- and most of those turn out
not to be want you want anyway, when you later look back on them.
I've seen that happening.

*Alice*: I agree.  Where were we?  We decided to create a scratch pad
named +rest+ for all the rest of the lines that +Body+ could not understand.

*Bob*: How about this: we can add an instance variable <tt>@rest</tt>
for the +Body+ class, in the form of one big string.  Initially
each +Body+ will be created with an empty string:

 :inccode: .iobody3.rb+initialize

The only line that we need to change in the +read+ method, is to
replace the old

 :inccode: .iobody2.rb-2

in <tt>iobody2.rb</tt> by

 :inccode: .iobody3.rb-1

in our new <tt>iobody3.rb</tt>

The first line tests whether the unknown line has an equal sign in it.
If so, the whole line is appended to the <tt>@rest</tt> string.  If
not, it really is an error.

*Alice*: Simple indeed!  But we have to make a change in our output
mechanism as well.

*Bob*: Yes, and that is simple too:

 :inccode: .iobody3.rb+to_s

 :inccode: .iobody3.rb+rest_to_s

*Bob*: Let's check this, by writing a test
file <tt>test.rb</tt>:

 :commandinput: cat > test.rb END
 require "iobody3.rb"

 b = Body.new
 b.read("begin particle star giant AGB")
 b.write
END

 :output: cat test.rb

And here is the result:

 :commandinputoutput: ruby test.rb END
      mass =      1
      nearest_neighbor = 365
      position =  2  3
      velocity =  4.5  6.7
      density = 3.2e-07
    end
END

*Bob*: And now with bad indentation, to see whether it will get corrected?

 :commandinputoutput: ruby test.rb END
      mass =      1
nearest_neighbor = 365
               position =  2  3
  velocity =  4.5  6.7
        density = 3.2e-07
end
END

*Alice*: Let's try to give a story line:

 :commandinput: cat > test.rb END
 require "iobody3.rb"

 b = Body.new
 b.read("begin particle star giant AGB")
 b.write
END

 :output: cat test.rb

And here is the result:

 :commandinputoutput: ruby test.rb END
      mass =      1
      nearest_neighbor = 365
      position =  2  3
      velocity =  4.5  6.7
      this is a rather large star
      density = 3.2e-07
    end
END

TODO: HIGHER LEVEL ACS READ-IN, HASH TABLE.
