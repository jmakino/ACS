#!/usr/local/bin/ruby -w

require "acs.rb"

options_text= <<-END

  Description: Find the first reported binary, print its story and exit
  Long description:
    This program accepts a stream of Nbody snapshots, and checks the story of
    each one until it finds the first reported binary.  It then prints that
    story on the standard output, and exits.

    (c) 2005, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:

        kali mkplummer.rb -n 4 -s 1 | kali world4.rb -t 1000 -x 0.25 | kali #{$0}


  END

clop = parse_command_line(options_text)

while nb = ACS_IO.acs_read
  raise "class #{nb.class} is not NBody" unless nb.class == NBody
  nb.story.each_line do |line|
    if line =~ / :  a = /
      print nb.story
      exit
    end
  end
end
