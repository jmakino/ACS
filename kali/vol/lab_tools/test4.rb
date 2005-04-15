#!/usr/local/bin/ruby -w

require "acs.rb"

options_text= <<-END

  Description: Gathers statistics for binary formation times
  Long description:
    This program runs a number of simulations, starting with a Plummer model,
    and reports some statistics concerning the times of first binary formation.

    (c) 2005, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:

        kali #{$0} -n 3 -x 1.0 -s 42


  Short name:		-r
  Long name:            --n_runs
  Value type:           int
  Default value:        1
  Variable name:        n_runs
  Print name:           N_runs
  Description:          Number of runs
  Long description:
    Number of runs, where each run is continued only to the point
    where the first binary is formed.


  Short name:		-n
  Long name:            --n_particles
  Value type:           int
  Default value:        1
  Variable name:        n
  Print name:           N
  Description:          Number of particles
  Long description:
    Number of particles in a realization of Plummer's Model.

    Each particles is drawn at random from the Plummer distribution,
    and therefore there are no correlations between the particles.

    Standard Units are used in which G = M = 1 and E = -1/4, where
      G is the gravitational constant
      M is the total mass of the N-body system
      E is the total energy of the N-body system


  Short name:           -s
  Long name:            --seed
  Value type:           int
  Default value:        0
  Description:          pseudorandom number seed given
  Print name:           
  Variable name:      seed
  Long description:
    Seed for the pseudorandom number generator.  If a seed is given with
    value zero, a preudorandom number is chosen as the value of the seed.
    The seed value used is echoed separately from the seed value given,
    to allow the possibility to repeat the creation of an N-body realization.

      Example:

        |gravity> kali mkplummer1.rb -n 42 -s 0
        . . .
        pseudorandom number seed given	: 0
                     actual seed used	: 1087616341
        . . .
        |gravity> kali mkplummer1.rb -n 42 -s 1087616341
        . . .
        pseudorandom number seed given	: 1087616341
                     actual seed used	: 1087616341
        . . .


  Short name: 		-x
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


  Long name:            --run_verbosity
  Value type:           int
  Default value:        0
  Description:          Run Output Verbosity Level
  Variable name:        run_verbosity
  Long description:
    The run verbosity level determines how much information from the
    individual runs are printed out.  The value of the run verbosity level
    is used to specify the verbosity of the program performing the run.


  END

c = parse_command_line(options_text)

times = []
run_seed = c.seed
c.n_runs.times do
  times.push `kali mkplummer.rb -n #{c.n} -s #{run_seed} --verbosity 0 | kali world4.rb -t 1000 -x #{c.max_semi_major_axis} --verbosity #{c.run_verbosity} | kali test2.rb --verbosity 0`.chomp
  run_seed += 1
end
print "times = #{times.join(", ")}\n"
print "t_average = #{times.map{|t| t.to_f}.inject{|n,dn| n+dn}/times.length}\n"
