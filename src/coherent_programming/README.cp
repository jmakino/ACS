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

  One can do inline image by saying 'link: filename' (no space allowed
  between link: and filename). Note that the
  link is relative to the place the index.html file is put, which is
  normally the "doc" directory under the directory rdoc source files
  live. In other words, you need to add additional "../" to any file
  name... Hmm, a bit inconvenient?  Anyway, here is a sample inline
  image: 

  link:sample.gif

* added a pair of directives, ":segment start:" and ":segment end:".
  texts between  ":segment start: segment_name" and corresponding
  ":segment end:" will be stored the file .foo.rb-segment.name, where
  foo.rb is the source file. Here is a sample code segment included:

 :inccode: .acsdoc.rb-prep_rb
