#!/usr/local/bin/ruby -w

require "nbody.rb"

class Body

  def simple_read
    @mass = gets.to_f
    @pos = gets.split.map{|x| x.to_f}.to_v
    @vel = gets.split.map{|x| x.to_f}.to_v
  end

end

class NBody

  def simple_read
    n = gets.to_i
    @time = gets.to_f
    for i in 0...n
      @body[i] = Body.new
      @body[i].simple_read
    end
  end

end

options_text= <<-END

  Description: Conversion from simple N-body format to ACS data format
  Long description:
    This program takes an N-body snapshot in the following simple format,
    where n is the number of particles; t is the time; and for each body i
    there is a mass mi, position ri and velocity vi, divided over separate
    lines as follows:

                  n
                  t
                  m1 r1_x r1_y r1_z v1_x v1_y v1_z
                  m2 r2_x r2_y r2_z v2_x v2_y v2_z
                  ...
                  mn rn_x rn_y rn_z vn_x vn_y vn_z

    The output is given in standard ACS format, as follows:

                  ACS
                    NBody 
                      Array body
                        Body body[0]
                          Float mass
                               m1
                          Vector pos
                               r1_x  r1_y  r1_z
                          Vector vel
                               v1_x  v1_y  v1_z
                        Body body[1]
                        ...
                        Body body[n]
                          Float mass
                               m1
                          Vector pos
                               r1_x  r1_y  r1_z
                          Vector vel
                               v1_x  v1_y  v1_z
                      Float time
                           t
                  SCA

    (c) 2005, Piet Hut and Jun Makino; see ACS at www.artcompsi.org

    example:
    kali #{$0} -p 2 -i 5 < cube1_sim.in


  END

c = parse_command_line(options_text)

include Math

nb = NBody.new
nb.simple_read
nb.acs_write($stdout, false, c.precision, c.add_indent)
