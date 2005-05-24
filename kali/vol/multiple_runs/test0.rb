#!/usr/local/bin/ruby -w

require "acs.rb"

options_text= <<-END

  Description: Testing Ruby's way of running Ruby programs
  Long description:
    This program prints out the results of a comparison between three
    different accuracy parameter choices, in otherwise similar integrations.

    (c) 2005, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:

        kali mkplummer.rb -n 8 | kali world4.rb -t 1000 -x 0.1 |
        kali #{$0} -n 4 -s 42 -g leapfrog -c 0.01


  Short name:		-n
  Long name:            --n_particles
  Value type:           int
  Default value:        4
  Variable name:        n
  Print name:           N
  Description:          Number of particles
  Long description:
    Number of particles for the Plummer model realization.


  Short name:           -s
  Long name:            --seed
  Value type:           int
  Default value:        0
  Description:          pseudorandom number seed given
  Print name:           
  Variable name:        seed
  Long description:
    Seed that will provided to generate the Plummer model realization.
    If no seed is provided, the default value 0 will be used.  This
    implies that two subsequent invocations of this program will generate
    different results, since the Plummer model builder translates a value
    0 in a seed value equal to a new pseudorandom number.


  Short name: 		-g
  Long name:		--integration_method
  Value type:		string
  Default value:	hermite
  Variable name:	integration_method
  Description:		Choice of integration method
  Long description:
    This option chooses the integration method.  The user is expected to
    provide a string with the name of the method, for example "leapfrog",
    "hermite".


  Short name: 		-c
  Long name:		--step_size_control
  Value type:		float
  Default value:	0.1
  Variable name:	dt_param
  Description:		Determines the time step size
  Long description:
    This option sets the step size control parameter dt_param for the first
    run.  Subsequent runs are performed with a step size control parameter
    that are ten and one hundred times smaller, respectively.


  END

clop = parse_command_line(options_text)

`kali mkplummer.rb -n #{clop.n} -s #{clop.seed} > tmp.in`
m = clop.integration_method
t = clop.dt_param
`kali nbody_sh1.rb --exact_time -g #{m} -t 1 -c #{t} < tmp.in > tmp1.out`
`kali nbody_sh1.rb --exact_time -g #{m} -t 1 -c #{0.1*t} < tmp.in > tmp2.out`
`kali nbody_sh1.rb --exact_time -g #{m} -t 1 -c #{0.01*t} < tmp.in > tmp3.out`
print `cat tmp1.out tmp2.out | kali nbody_diff.rb`
print `cat tmp2.out tmp3.out | kali nbody_diff.rb`
