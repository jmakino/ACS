#<i>extends Body class in order to include Leapfrog module</i>

require "body.rb"
require "leapfrog.rb"

class Body

  include Leapfrog

  attr_accessor :acc, :pot

  def clear_acc
    @acc = [0.0,0.0,0.0]
  end

  def leapfrog_to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.join(", ") + "\n" +
    "   vel = " + @vel.join(", ") + "\n" +
    "   acc = " + @acc.join(", ") + "\n"
  end

  def leapfrog_pp            # pretty print
    print leapfrog_to_s
  end

end
