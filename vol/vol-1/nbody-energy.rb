#<i>extends Nbody class in order to provide pot/kin/tot energy diagnostics</i>

require "nbody.rb"
require "body-energy.rb"

class Nbody

  attr_accessor :einit, :ekin, :epot

  def clear_ekin
    @ekin = 0
  end

  def clear_epot
    @epot = 0
  end

  def update_ekin
    clear_ekin
    @body.each do |b|
      @ekin += b.single_kin
    end
  end

  def update_epot
    clear_epot
    i = 0
    while (i < @body.size) 
      j = i+1
      while (j < @body.size)
	@epot += @body[i].pairwise_pot(@body[j])
	j += 1
      end
      i += 1
    end
  end

  def initialize_energy
    update_ekin
    update_epot
    @einit = @ekin + @epot
  end

  def report_energy
    etot = @ekin+@epot
    STDERR.print "  E_kin = ", @ekin, " , E_pot = ", @epot,
      " , E_tot = ", etot, "\n                ",
      "absolute energy error: E_tot - E_init = ", etot-@einit,
      "\n                ",
      "relative energy error: (E_tot - E_init) / E_init = ",
               (etot - @einit) / @einit, "\n"
  end

end
