#<i>sample_init produces a few sets of initial conditions for Nbody systems</i>
#
#* a file <i>binary.in</i> is created with the initial conditions for a
#  circular binary;
#* a file <i>triple.in</i> is created with the initial conditions for a
#  triple in a figure-8 orbit

 # Jun, where shall we put this next line, in "body.rb" perhaps??

include Math

require "nbody.rb"

def mk_binary
  binary = Nbody.new(2)

  binary.body[0].mass = 1.0
  binary.body[0].pos[0] = 0.5
  binary.body[0].vel[1] = 1/sqrt(2.0)

  binary.body[1].mass = 1.0
  binary.body[1].pos[0] = -0.5
  binary.body[1].vel[1] = -1/sqrt(2.0)

  return binary
end

def mk_triple
  triple = Nbody.new(3)

  triple.body[0].mass = 1.0
  triple.body[0].pos[0] = 0.9700436
  triple.body[0].pos[1] = -0.24308753
  triple.body[0].vel[0] = 0.466203685
  triple.body[0].vel[1] = 0.43236573

  triple.body[1].mass = 1.0
  triple.body[1].pos[0] = -triple.body[0].pos[0]
  triple.body[1].pos[1] = -triple.body[0].pos[1]
  triple.body[1].vel[0] = triple.body[0].vel[0]
  triple.body[1].vel[1] = triple.body[0].vel[1]

  triple.body[2].mass = 1.0
  triple.body[2].vel[0] = -0.93240737
  triple.body[2].vel[1] = -0.86473146

  return triple
end

def mk_sample_init_files
  binary = mk_binary
  binary_file = File.open("binary.in", "w+")
  binary.simple_fprint(binary_file)

  triple = mk_triple
  triple_file = File.open("triple.in", "w+")
  triple.simple_fprint(triple_file)
end
