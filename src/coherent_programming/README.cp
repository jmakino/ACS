=README

==Overview

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
  between link: and filename). Note that the
  link is relative to the place the index.html file is put, which is
  normally the "doc" directory under the directory rdoc source files
  live. In other words, you need to add additional "../" to any file
  name... Hmm, a bit inconvenient?  Anyway, here is a sample inline
  image: 

  link:../sample.gif

  This is made with "link: ../sample.gif" but without space after link:

* added a pair of directives, ":segment start:" and ":segment end:".
  texts between  ":segment start: segment_name" and corresponding
  ":segment end:" will be stored the file .foo.rb-segment.name, where
  foo.rb is the source file. Here is a sample code segment included:

:inccode: .acsdoc.rb-prep_rb


:inccode: .acsdoc.rb-acsdoc

Now, by using "." + filename + "+" methodname, you can show a method
defined in a file: The following is an example, using
": inccode: .acsdoc.rb+prep_rb" (no space after the first ":")

:inccode: .acsdoc.rb+prep_rb


Here is an example of ":output:" with "echo foo" as arguments

 :output: echo foo

I changed the prompt to "yebisu>" using ":prompt:" directive here.

:prompt: yebisu>
 

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
  :commandoutput: cat aho
  :command: rm aho
  

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