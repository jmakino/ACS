require "nbody.rb"

class Vector

  def cross(other)                   # outer product
    if other.size == 2
      return self[0]*other[1] - self[1]*other[0]
    elsif other.size == 3
      result = Vector.new
      result[0] = self[1]*other[2] - self[2]*other[1]
      result[1] = self[2]*other[0] - self[0]*other[2]
      result[2] = self[0]*other[1] - self[1]*other[0]
      return result
    else
      raise "dimension = #{other.size} not supported"
    end
  end

end

class Nbody

  def report_binaries(max_semi_major_axis, precision)
    print_time_flag = true
    @body.each_index do |i|
      @body.each_index do |j|
        if j > i
          b = Binary.new(@body[i], @body[j])
          if b.energy < 0 and b.semi_major_axis <= max_semi_major_axis
            if @time
              if print_time_flag
                STDERR.printf("  time = %8.3f :", @time)   # to be improved <==
                print_time_flag = false
              else
                STDERR.print "                   "
              end
            end
            STDERR.print "  [", i, ",", j, "] :  a = "
            STDERR.printf("%.#{precision}f", b.semi_major_axis)
            STDERR.print " ; e = "
            STDERR.printf("%.#{precision}f\n", b.eccentricity)
          end
        end
      end
    end
  end

end

class Binary

  def initialize(body1, body2)
    @b1 = body1
    @b2 = body2
    @total_mass = @b1.mass + @b2.mass
    @reduced_mass = ( @b1.mass * @b2.mass ) / ( @b1.mass + @b2.mass )
    @pos = @b2.pos - @b1.pos
    @vel = @b2.vel - @b1.vel
  end

  def kinetic_energy
    0.5 * @reduced_mass * @vel * @vel
  end

  def potential_energy
    -( @b1.mass * @b2.mass / sqrt( @pos * @pos ) )
  end

  def energy
    kinetic_energy + potential_energy
  end

  def angular_momentum_squared
    r_cross_v = @pos.cross(@vel)
    @reduced_mass**2 * r_cross_v * r_cross_v
  end

  def semi_major_axis
    -( @b1.mass * @b2.mass ) / ( 2 * energy )
  end

  def eccentricity
    e_sq = 1 - angular_momentum_squared /
                 ( @reduced_mass * @b1.mass * @b2.mass * semi_major_axis )
    e_sq = 0.0 if e_sq < 0.0  # to avoid round-off to slightly negative numbers
    sqrt(e_sq)
  end

end

options_text= <<-END

  Description: Find and report gravitationally bound pairs of stars
  Long description:
    This program accepts Nbody snapshots, and returns information about the
    binaries stars (gravitationally bound pairs of stars) on the stderr output
    channel.  It also echoes each original snapshot on the stdout output
    channel, so that it will be available for another diagnostics program.

    (c) 2004, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:
               ruby mkplummer.rb -n 3 | ruby #{$0}


  Short name: 		-a
  Long name:            --max_semi_major_axis
  Value type:           float
  Default value:        #{VERY_LARGE_NUMBER}
  Description:          Maximum value of semi major axis
  Variable name:        max_semi_major_axis
  Long description:
    This option allows the user to limit the number of binaries detected
    by discarding binaries with a semi-major axis larger than the specified
    number.  This is useful in situation such as the initial state for a cold
    collapse situation, where every star is formally bound to every other star.


  Short name:           -p
  Long name:            --precision
  Value type:           int
  Default value:        4
  Description:          Floating point precision
  Variable name:        precision
  Long description:
    The precision with which floating point numbers are printed in the output.


  END

clop = parse_command_line(options_text)

nb = ACS_IO.acs_read
while nb
  raise "class #{nb.class} is not Nbody" unless nb.class == Nbody
  nb.report_binaries(clop.max_semi_major_axis, clop.precision)
  nb.acs_write
  nb = ACS_IO.acs_read
end
