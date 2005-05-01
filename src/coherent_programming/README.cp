
= README

:tableofcontents:

== Overview

This is the readme file for the core of the cp system. The system
itself is implemented as rather simple and minor modification of rdoc
system, which is intended to be used as automatic HTML generation from
ruby programs and its enbedded document.

The file acsdoc.rb performs the minor transformations we made:

The changes we made are

* added a new directive, ":inccode:", which works essentially the same
  way as the normal ":include:", but adds horizontal lines before and
  after the included files.

  note: any other format replacing ":inccode:" by ":in.*code:" (arbitrary
  number of arbitrary characters inserted between "in" and "code") is
  fine too; for example "include code" also works, with the added
  advantage that the file does not become an illegal rdoc document,
  since rdoc does not recognize ":xxxx:" if "xxxx" includes white spaces.

* One can do inline image by saying 'link: filename' (no space allowed
  between link: and filename). Here is a sample inline
  image: 

  link:sample.gif

  This is made with "link: sample.gif" but without space after link:

* added a pair of directives, ":segment start:" and ":segment end:".
  texts between  ":segment start: segment name" and corresponding
  ":segment end:" will be stored the file .foo.rb-segment.name, where
  foo.rb is the source file. Here is a sample code segment included:

 :inccode: .acsdoc.rb-prep_rb


 :inccode: .acsdoc.rb-acsdoc

Now, by using "." + filename + "+" methodname, you can show a method
defined in a file: The following is an example, using
": inccode: .acsdoc.rb+prep_rb" (no space after the first ":")

 :inccode: .acsdoc.rb+prep_rb

* added special form of comment, ##number (like ##1, ##2 etc), which
  write the line with that comment to the file named .foo.rb-1 if
  number is 1 and filename is foo.rb, but removing the ##1 itself. 

* it creates the file .foo.rb, from foo.rb. The created file  is an
  stripped out  version of the input file, with segment directives and
  the above ##number directives removed.

* <b>This process need to be applied to the partial files!  </b>

Here is an example of ":output:" with "echo foo" as arguments

 :output: echo foo


Here is an example of ":commandoutput:" with "setenv LANG C ; date"
as arguments

 
 :commandoutput: setenv LANG C ; date

 :command: echo test

Here is an example of   ":commandinputoutput:" with "cat END".
In this example, all lines before "END" will be sent to the cat
command. 
  :commandinputoutput: cat END
aaa
bbb
ccc
END
  :commandinput: cat > aho END
aaa
bbb
ccc
END

  :commandoutput: ls -al aho; pwd

  :commandoutput: cat aho
  :command: rm aho

Thus, you can use

* ":output:"     produces output only
* ":command:"    no output, but command is executed
* ":commandoutput:"  both command line and output
* ":commandnooutput:"  command line only


I changed the prompt to "yebisu>" using ":prompt:" directive here.

:prompt: yebisu>
 
    

* In  .cp files, lines start with "#" (only at the begining of the
  line. No white space or whatever before "#" would suppress this
  function) are ignored by acsdoc.rb (not passed to rdoc).

# This line is not shown

 # This line is shown

* You can embed arbitrary TeX source code fragment, by saying
  \<tex> some tex expression </tex>. For example, 
  \<tex> $x=y$ </tex> gives

  <tex> $x=y$ </tex>

* It seems to fail in a strange way in some cases. This is apparently
  a problem with rdoc. For example, <tex> This should be okay</tex>
  but I suspect after \<tex> this </tex>, the next thing will fail
  <tex> This will fail </tex> for unknown reason.
  
  

* Known BUG in latex generation

  If you try to do unnumbered list and numbered list at the same level
  without any intervening text, the generated tex source might have
  wrong nesting of begin{itemize} etc.

== Numbered figures and equations

Now it is possible to write numbered figures and eqations, similar to
latex but in more simple form.

The following:

   : equation :
   \label{equation1}
   a = b

(blanck line is recognized as the end of equation) is essentially the
same as writing

  < tex >
  \begin{equation}
  \label{equation1}
  a = b
  \end{equation}
  < /tex >

Which is processed to:

   :equation:
   \label{equation1}
   a = b


but equation number is maintained within acsdoc.rb. To refer to
an equation, use ref(equation1)  (ref ( equation1 ), with no space). equation label must be composed of
alphanumerics and ":". "_" or any other symbol is currently illegal.

You should be able to refer to equations in other .cp/.ok file, when
they are processed together. Equation (ref(newtonfull)) is in file
sample2.cp.

One should also be able to refer to equations in other
volumes as Equation (ref(../coherent_programming#newtonfull)), but not
sure...



I also added numbered figures, in the form

  : figure : name_of_the_figure_file size label
  caption text for the figure.


== Table of contents

The : tableofcontents : directive will create the table of content at
that location. In latex conversion, it is simply replaced by latex
\tableofcontents. In rdoc-html conversion, all headers (= name )
automatically appear in nested list.

The : nosectionnumber : directive will suppress the numbering of the
header (chapter, section or whatever) immediately after the
directive. 

== Links to other URL

One can add links to URL by using the specal form
\<web>url|linktext</web>. For example,

\<web>http: www. artcompsci.org|ACS homepage</web>

creates: 
<web>http:www.artcompsci.org|ACS homepage</web>.


== Section numbers

By default (unless specified in some way), acsdoc.rb add
chapter/section numbers in between "=*" and real title.


== Changes made to rdoc itself

We try as hard as possible not to change rdoc itself, but some things
are hardcoded in rdoc which can only be changed by tweaking the source
code of rdoc. However, few changes are necessary, and they are
currently done in the directory   <tt>$ACSROOT/rdoc-0.9.0acs1/</tt>. Current
changes are

* new directive to make HTML name tags, by \!word! and
  \<name>word</name>

* adding 'ALIGN="middle"' to all IMG links generated by link
  directive.

* new directives to generate in-page links, ntaga, ntagb, and ntagc

* capability to generate link from "(link text)[url]" form.
  
If you newly installed ACS software, you need to go to the above
directory and run "ruby install.rb" as superuser, after you installed
rdoc 0.9.0. If your rdoc is newer... well, currently you might want to
downgrade...

== Initialization file

Initialization file is searched in the order of $ACSDOCINITRC,
./.acsdocinitrc, and ~/.acsdocinitrc. The things you can write in the
initialization file is Ruby statement. A typical way to use is
something like:

   # acsdoc initialization file
   print "Loading the initialization file for ascdoc\n"
   @@addtional_preambles_for_inline_tex = "\\usepackage{epsf}"
   @@addtional_commands_for_inline_tex = "\\input macros"
  
This one allows the use of epsf package (style file), and macros.tex
is included after \begin{document}.

