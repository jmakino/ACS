= Pretty Printing

== Tinkering with the Output

*Bob*: So far we've gotten output as a side effect, with the interpreter
echoing the value of everything it evaluates.  Let's see whether we
can get specific output.  Ruby seems to have a general command "+p+"
for printing the contents of any object.  Let's see what that gives us:

    |gravity> irb -r body2.rb
    irb(main):001:0>

*Alice*: Before you continue, is there a way to make the +irb+ prompt
shorter?  All this "<tt>irb(main):</tt>" part at the beginning of each
line distracts me from the real information.  With Ruby being so
flexible, I bet it would be easy to customize, and to leave that out.

*Bob*: As you may expect under a unix system, there is an +irb+
configuration file called <tt>.irbrc</tt> and I'm sure we can give
you a nicer prompt.  What does the manual say?  Aha, here is the
default definition of the prompt.  Let me copy that as a comment
in the configuration file, for comparison, followed by the new and
shorter version; a <tt>#</tt> sign in Ruby indicates the start of a
comment, like <tt>//</tt> in C++.  And let me make the shorter version
optional, since you may well want to go back to the longer standard
version when you get into more complicated situations.  So let me
create an extra option for invoking +irb+,
as <tt>irb --prompt short_prompt</tt>, which calls
the new variation of the prompt.  Give me a few minutes . . .

. . .  Done!  Here is our first customized <tt>.irbrc</tt> file:

    #  .irbrc

    # This is the definition of the default prompt:
    # 
    #    IRB.conf[:PROMPT_MODE][:DEFAULT] = {
    #          :PROMPT_I => "%N(%m):%03n:%i> ",
    #          :PROMPT_S => "%N(%m):%03n:%i%l ",
    #          :PROMPT_C => "%N(%m):%03n:%i* ",
    #          :RETURN => "%s\n"
    #    }
    #
    # This short version leaves out the first two parts.
    #
    # usage: irb --prompt short_prompt

    IRB.conf[:PROMPT][:SHORT_PROMPT] = {
          :PROMPT_I => "%03n:%i> ",
          :PROMPT_S => "%03n:%i%l ",
          :PROMPT_C => "%03n:%i* ",
          :RETURN => "%s\n"
    }

*Alice*: You're amazing.  That would have taken me a few hours to figure
out.  It looks impressive.  But does it work?

*Bob*: I hope so.  Let's start again:

    |gravity> irb --prompt short_prompt -r body2.rb
    001:0> 

*Alice*: Now I'm really impressed!

*Bob*: Your wish is my command.

*Alice*: Oh yeah?  Then let's make _everything_ modular.

*Bob*: Not that wish; don't push it!  Where were we?  Oh yes, Ruby seems
to have a general command "+p+" for printing the contents of any object.
Let's see what that gives us:

    001:0> b1 = Body.new(0.1, [1.3, 0, 0], [0, 0.5, 0])
    #<Body:0x400d11a4 @mass=0.1, @vel=[0, 0.5, 0], @pos=[1.3,
      0, 0]>
    002:0> p b1
    #<Body:0x400d11a4 @mass=0.1, @vel=[0, 0.5, 0], @pos=[1.3,
       0, 0]>
    nil

*Alice*: I guess your +p+ command is effectively a dump command that
simply dumps the contents of an object.  That doesn't get us much
further, since +irb+ already does that when printing the value of an
object.  The only difference is that the +p+ command itself does
not have a value, it returns +nil+ after doing its dumping.

*Bob*: Right you are.  Notice by the way that you got even more
shortening than you asked for: remember that +irb+ used to give us
this <tt>=></tt> at the beginning of each response, presumably to
draw our attention with a little arrow?  That's gone now too, though
I'm not sure why.

*Alice*: I'm much obliged, but let's not get side tracked.  There must
be better print commands than the +p+ dump method.

*Bob*: There seems to be a generic +print+ command:

    003:0> print b1
    #<Body:0x400d11a4>nil

*Alice*: Are you teasing me by making everything shorter: the prompt,
the response, and now even the dump?

*Bob*: No, I'm only puzzled.  What is that little +nil+ doing there at
the end?  Ah, I see.  This +print+ does not give a new line, unlike +p+.
Both methods return +nil+, but with +print+ it shows up on the same line.

But my Ruby book told me that +print+ is the standard way to get the
contents from an object printed out.  Let's check the book again.

== A Body-to-String Converter

*Bob*: Ah, here is a hint.  The manual page for +print+ states: "Objects
that aren't strings will be converted by calling their +to_s+ method."

*Alice*: But our +Body+ class does not have an +to_s+ method.  Does that
mean that we have to write one?

*Bob*: I bet we do.  And this book gives us some examples, so it will
be easy to adapt that to our purpose.  This one here.  Aha.  Okay, I
see how it works.  Let me write it directly into our +Body+ class
definition, in file <tt>body3.rb</tt>:

 :inccode: body3.rb

*Alice*: Why the <tt>+</tt> signs?

*Bob*: Ruby has overloaded operators.  When the plus sign is applied to
numbers, it causes an addition, as you would expect.  But when a plus
sign occurs between two strings, the strings are concatenated, which is
the most plausible thing to do when you want to add strings.  Principle
of minimal surprise!

*Alice*: I like that.  And the <tt>"\n"</tt> is a new line, just like in
the C language?

*Bob*: You guessed it.  Some things never change, it seems.  Note that
we don't have to use the +mass+ method to access the internal variable
<tt>@mass</tt>: since our <tt>to_s</tt> method lives inside the +Body+
class definition, it automatically has access to its internal variables.

*Alice*: But what are those three <tt>to_s</tt>'s doing inside the
definition of our own <tt>to_s</tt>?  Is there something recursive going on?

*Bob*: In a way, at least in name, though the actual methods are quite
different.  You see, to convert the value of whole object to a string,
we have to convert the values of its internal variables to strings as
well.  Those three internal variables will quickly become floating point
variables, as soon as we initialize them properly, since they represent
physical quantities.  Fortunately, Ruby knows how to convert floating point
variables, or integers for that matter, to strings.  The method that does
this is called +to_s+, consistent in name with what we are trying to
write now.

*Alice*: This +to_s+ seems to be the opposite of +atof+ in C; what does
+a+ that stand for, perhaps ASCII to floating point?

*Bob*: Yes, but you don't have the reverse, an +ftoa+, in C, let alone
a generic version +toa+ that can convert any object to ASCII so that
it can be printed out.  In Ruby you do have such a generic tool, and
instead of +a+ for ASCII, Ruby uses the more appropriate +s+ for string,
in naming it +to_s+.

*Alice*: With generic, you really mean that <tt>to_s</tt> can convert
_any_ object to a string?

*Bob*: Yes, but only if you provide the appropriate <tt>to_s</tt> method
to the class that defines that object, since Ruby has no way of
guessing beforehand how a new object should be printed out.  That is
our responsibility, and I have just done that.  Ruby's task is to sow
it altogether seemlessly.

*Alice*: Which it can do well, because it is designed in such a modular
way: there is a nice hierarchy of separate +to_s+'s that are calling
each other down the chain of command.

== Taming the Arrays

*Bob*: the M word again!  At some point you have to define what _modular_
exactly means.  If you call every good idea that works `modular', you
have a tautology.  Anyway, I'm curious whether it will come out better
now, when we type +print+.

    |gravity> irb --prompt short_prompt -r body3.rb
    001:0> b1 = Body.new(0.1, [1.3, 0, 0], [0, 0.5, 0])
    #<Body:0x400d0cf4 @mass=0.1, @vel=[0, 0.5, 0], @pos=[1.3,
       0, 0]>
    002:0> print b1
      mass = 0.1
       pos = 1.300
       vel = 00.50
    nil

*Alice*: I looks nicer all right, but what happened to poor +pos+
and +vel+?  The mass came out fine though.

*Bob*: Ah, of course, look, the elements of each array are all
concatenated: <tt>[1.3, 0, 0]</tt> has become <tt>1.300</tt>.
Remember what I said about strings?  In that case addition means
concatenation.  So all the characters of the array elements are
strung together.

*Alice*: That must be why they call it a string!  But how do we
separate the fields?

*Bob*: Guess what, we use a field separator.  The book tells me
that we can define our own version.  Here it is: the method +join+
converts an array to a string, and you can give a separator as an
arguments.  Let's try.  Here is <tt>body4.rb</tt>:

 :inccode: body4.rb

I'll do the same exercise:

    |gravity> irb --prompt short_prompt -r body4.rb
    001:0> b1 = Body.new(0.1, [1.3, 0, 0], [0, 0.5, 0])
    #<Body:0x400d0c7c @mass=0.1, @vel=[0, 0.5, 0], @pos=[1.3,
       0, 0]>
    002:0> print b1
      mass = 0.1
       pos = 1.3, 0, 0
       vel = 0, 0.5, 0
    nil

*Alice*: Well done!  But I don't understand why +pos+ and +vel+ suddenly
have acquired this nifty method +join+, as what in C++ would be called
a member function.  Who gave that to them?

*Bob*: The manual says that +join+ is an instance method of the class
+Array+.  And since +pos+ and +vel+ are each arrays, they both have
access to the +join+ method.  It was given to them the moment they
were created as arrays.  So whenever you create an array, you immediately
have a whole arsenal of useful methods right at your finger tips.  Look,
isn't this an impressive list of methods?

*Alice*: I guess so, but let's see how useful they will turn out to be.
You mentioned that +join+ is an _instance_ method.  What would be an example
of a _class_ method?

*Bob*: Here they are listed, in the Ruby documentation pages, under the
heading <tt>class Array</tt>.  There are only two: <tt>[]</tt> and +new+.

*Alice*: We have seen +new+; you told me that that is a generic method for
any class.  But what is <tt>[]</tt>?

*Bob*: Typing <tt>Array.new</tt> gives you an empty array, to which you can
add elements afterwards.  Typing <tt>Array[](1,a,3)</tt> gives you an array
that already is initialized with some elements, as specified in the arguments
of this method; in this case it will return the array <tt>[1, a, 3]</tt>.

*Alice*: I see.  If you look at it that way, it makes sense.  The only thing
different is that there is no dot between +Array+ and <tt>[]</tt>.

*Bob*: However, if would still be a bit cumbersome to initialize an array
by having to type the expression <tt>Array[]</tt> each time.  There are two
better alternatives.  You can simply type <tt>Array[1,a,3]</tt>, which
gives you the exact same thing.  Or you can even type <tt>[1,a,3]</tt>, leaving
out the word +Array+ altogether, and you still get <tt>[1, a, 3]</tt>.

So the class method <tt>[]</tt> is a versatile and somewhat slippery thing.
Yet it really is a class method, and the various ways of invoking it are
again forms of syntactic sugar, making your life a lot easier.

*Alice*: Ruby must be a really sweet language!  I guess I'd better
study the manual.  There must be a lot of tools as well as notations,
ready to be used.

== Executing Ruby Files

*Bob*: So now our +Body+ class has grown up: it can be printed out
correctly with +print+.

*Alice*: I wonder though, can't we give our +Body+ its own pretty
printing method, +pp+ say?  In such a way that we can type
<tt>b1.pp</tt>, and it will give us this nicely formatted output?

*Bob*: Easy.  Here is <tt>body5.rb</tt>.  Note that I put a comment
in, since otherwise I would never remember what +pp+ meant.

 :inccode: body5.rb

*Alice*: Ah, that's because you are not a Lisp programmer.  But you
are right.  That was very easy.  Let's check.

    |gravity> irb --prompt short_prompt -r body5.rb
    001:0> b1 = Body.new(0.1, [1.3, 0, 0], [0, 0.5, 0])
    #<Body:0x400d0ad8 @mass=0.1, @vel=[0, 0.5, 0], @pos=[1.3,
       0, 0]>
    002:0> b1.pp
      mass = 0.1
       pos = 1.3, 0, 0
       vel = 0, 0.5, 0
    nil

And it works!  But I'm getting pretty tired from retyping that whole
initialization line for our single body.  An interactive interpreter
is fine for small jobs, but perhaps it makes more sense to start
putting both the class definition and the commands all in one file.
We can then keep that file in an editor, and each time we modify
either the class or the command, we can just run the file.

*Bob*: That's an excellent idea.  In fact, I was getting tired of
creating that whole series of files from <tt>body1.rb</tt> up to
<tt>body5.rb</tt>.  Let us just take one file and call it simply
<tt>test.rb</tt>.  Here is the first version, and from now on we
can add and change whatever we want.  You see, at the end I have
included our two commands, the first to create and initialize a
particular particle, and the second to print it out.

 :inccode: body6.rb

*Alice*: But how do we run this file, in order to execute those
two commands that you added at the end?

*Bob*: The simplest way is to type <tt>ruby</tt> followed by the
file name:

 :command: cp -f body6.rb test.rb
 :commandoutput: ruby test.rb
 :command: rm -f test.rb

*Alice*: Much easier indeed!  No more retyping.  And no more +nil+
at the end either.  Ah, of course.  The interactive interpreter echoes
the value of each line that you type.  But when you execute a file,
you only get the results of each command that you give.

*Bob*: Yes, that's the difference, and that's why this looks cleaner.
Of course, for debugging purposes you may want to have an echo of
all that passes in front of you, but I'm happy to work with a file,
for the time being.
