#!/usr/local/bin/ruby -w

require "acs"

options_text = <<-END

  Description: Creates a document from YAML input using a template 
  Long description:
    This program creates a document from YAML input using a template.

    (c) 2005, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:
    kali yacsdoc.rb -n 4 -s 1 | kali #{$0} -t 1


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
  Default value:	0.01
  Variable name:	dt_param
  Description:		Determines the time step size
  Long description:
    This option sets the step size control parameter dt_param << 1.  Before
    each new time step, we first calculate the time scale t_scale on which
    changes are expected to happen, such as close encounters or significant
    changes in velocity.  The new time step is then given as the product
    t_scale * dt_param << t_scale.


  Short name: 		-f
  Long name:		--init_timescale_factor
  Value type:		float
  Default value:	0.01
  Variable name:	init_timescale_factor
  Description:		Initial timescale factor
  Long description:
    This option allows the user to determine how extra small the initial
    timesteps are, for all particles.  In order to allow a safe startup
    for high-order multistep methods, all particles are forced to start
    their integration with a time scale that is significantly smaller
    than what they normally would be, by a factor "init_timescale_factor".


  Short name: 		-e
  Long name:		--era_length
  Value type:		float
  Default value:	0.0078125
  Variable name:	dt_era
  Description:		Duration of an era
  Long description:
    This option sets the time interval between begin and end of an era,
    which is the period in time that contains a bundle of world lines,
    all of which are guaranteed to extend beyond the era boundaries with
    by at least one world point in either direction.  In other words, each
    world line has an earliest world point before the beginning of the era,
    and a latest world point past the end of the era.  This guarantees
    accurate interpolation at each time within an era.


  Short name: 		-m
  Long name:		--max_timestep_param
  Value type:		float
  Default value:	1
  Variable name:	dt_max_param
  Description:		Maximum time step (units dt_era)
  Long description:
    This option sets an upper limit to the size dt of a time step,
    as the product of the duration of an era and this parameter:
    dt <= dt_max = dt_era * dt_max_param .


  Short name: 		-d
  Long name:		--diagnostics_interval
  Value type:		float
  Default value:	1
  Variable name:	dt_dia
  Description:		Diagnostics output interval
  Long description:
    The time interval between successive diagnostics output.
    The diagnostics include the kinetic and potential energy,
    and the absolute and relative drift of total energy, since
    the beginning of the integration.
        These diagnostics appear on the standard error stream.


  Short name: 		-o
  Long name:		--output_interval
  Value type:		float
  Default value:	1
  Variable name:	dt_out
  Description:		Snapshot output interval
  Long description:
    This option sets the time interval between output of a snapshot
    of the whole N-body system, which which will appear on the
    standard output channel.

    The snapshot contains the mass, position, and velocity values
    for all particles in an N-body system, in ACS format


  Short name: 		-y
  Long name:		--pruned_dump
  Value type:		int
  Default value:	0
  Variable name:	prune_factor
  Description:		Prune Factor
  Long description:
    If this option is invoked with a positive argument k = 1, then
    the full information for a particle is printed as soon as it
    makes a step.  If the prune factor is set to a value k > 1,
    the information is printed only for 1 out of every k steps.
    The output appears in ACS format on the standard output channel.
    It is guaranteed that for each particle the full information will
    be printed before the first step and after the last step.  
    The resulting stream of outputs contains information for different
    particles at different times, but within each worldline, the
    world points are time ordered.

    If this option is not invoked, or if it is invoked with the default
    value k = 0, no such action will be undertaken.  This option, when
    invoked with k > 0, overrides the normal output options (a specified
    value for the normal output interval will be ignored). 


  Short name: 		-t
  Long name:		--time_period
  Value type:		float
  Default value:	10
  Variable name:	dt_end
  Print name:		t
  Description:		Duration of the integration
  Long description:
    This option sets the duration t of the integration, the time period
    after which the integration will halt.  If the initial snapshot is
    marked to be at time t_init, the integration will halt at time
    t_final = t_init + t.


  Short name: 		-u
  Long name:		--cpu_time_max
  Value type:		int
  Default value:	60
  Variable name:	cpu_time_max
  Description:		Max cputime diagnost. interval
  Long description:
    This option sets the maximum cpu time interval between diagnostics output,
    in seconds.


  Short name:		-i
  Long name:  		--init_out
  Value type:  		bool
  Variable name:	output_at_startup_flag
  Description:		Output the initial snapshot
  Long description:
    If this flag is set to true, the initial snapshot will be output
    on the standard output channel, before integration is started.


  Short name:		-r
  Long name:  		--world_output
  Value type:  		bool
  Variable name:	world_output_flag
  Description:		World output format, instead of snapshot
  Long description:
    If this flag is set to true, each output will take the form of a
    full world dump, instead of a snapshot (the default).  Reading in
    such an world again will allow a fully accurate restart of the
    integration,  since no information is lost in the process of writing
    out and reading in, in terms of world format.


  Short name:		-a
  Long name:  		--shared_timesteps
  Value type:  		bool
  Variable name:	shared_flag
  Description:		All particles share the same time step
  Long description:
    If this flag is set to true, all particles will march in lock step,
    all sharing the same time step.


  END

clop = parse_command_line(options_text)

World.admit($stdin, clop).evolve(clop)
