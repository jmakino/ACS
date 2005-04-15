#!/usr/local/bin/ruby -w

require "acs.rb"

options_text= <<-END

  Description: Find when the first binary froms, print that time and exit
  Long description:
    This program accepts a stream of Nbody snapshots, and checks the story of
    each one until it finds the first reported binary.  It then prints the
    time at which that binary was first found, on the standard output, and
    exits.

    (c) 2005, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:

        kali mkplummer.rb -n 8 | kali world4.rb -t 1000 -x 0.1 | kali #{$0}


  END

class NBody
  attr_reader :time
end

clop = parse_command_line(options_text)

while nb = ACS_IO.acs_read
  raise "class #{nb.class} is not NBody" unless nb.class == NBody
  nb.story.each_line do |line|
    if line =~ / :  a = /
      print nb.time, "\n"
      exit
    end
  end
end
