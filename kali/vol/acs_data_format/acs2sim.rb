require "vector.rb"
require "clop.rb"
require "acsio.rb"

class Body

  attr_accessor :mass, :pos, :vel

  def initialize(mass = 0, pos = Vector[0,0,0], vel = Vector[0,0,0])
    @mass, @pos, @vel = mass, pos, vel
  end

  def simple_print(precision, no_exp_flag)
    if no_exp_flag
      printf("%#{precision+8}.#{precision}f\n", @mass)
      @pos.each{|x| printf("%#{precision+8}.#{precision}f", x)}; print "\n"
      @vel.each{|x| printf("%#{precision+8}.#{precision}f", x)}; print "\n"
    else
      printf("%#{precision+8}.#{precision}e\n", @mass)
      @pos.each{|x| printf("%#{precision+8}.#{precision}e", x)}; print "\n"
      @vel.each{|x| printf("%#{precision+8}.#{precision}e", x)}; print "\n"
    end
  end

end

class Nbody

  attr_accessor :time, :body

  def initialize
    @body = []
  end

  def simple_print(precision, no_exp_flag)
    print @body.size, "\n"
    if no_exp_flag
      printf("%#{precision+8}.#{precision}f\n", @time)
    else
      printf("%#{precision+8}.#{precision}e\n", @time)
    end
    @body.each{|b| b.simple_print(precision, no_exp_flag)}
  end

end

options_text= <<-END

  Description: Conversion from ACS data format to simple N-body format
  Long description:

    This program takes an N-body snapshot in the standard ACS format:

                  ACS
                    Nbody 
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

    The output is given in the following simple format, where n is the
    number of particles; t is the time; and for each body i there is a
    mass mi, position ri and velocity vi, divided over separate lines as:

                  n
                  t
                  m1 r1_x r1_y r1_z v1_x v1_y v1_z
                  m2 r2_x r2_y r2_z v2_x v2_y v2_z
                  ...
                  mn rn_x rn_y rn_z vn_x vn_y vn_z

    (c) 2004, Piet Hut and Jun Makino; see ACS at www.artcompsi.org

    example:
    ruby #{$0} < cube1_acs.in


  Short name:           -p
  Long name:            --precision
  Value type:           int
  Default value:        16
  Description:          Floating point precision
  Variable name:        precision
  Long description:
    The precision with which floating point numbers are printed in the output.
    The default precision is comparable to double precision accuracy.


  Short name:           -f
  Long name:            --no_exponents
  Value type:           bool
  Description:          Floating point notation without exponents
  Variable name:        no_exponents
  Long description:
    If this flag is set to true, floating point numbers will be printed
    out in decimal notation without the use of exponents (as in the %f
    format of output in C notation, instead of the default %e format).


  END

c = parse_command_line(options_text, true)

include Math

nb = ACS_IO.acs_read(Nbody)
nb.simple_print(c.precision, c.no_exponents)
