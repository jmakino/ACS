require "acs"

class Body

# naming convention for instance variables in class Body:
#
# @mass    : mass 
# @pos     : position
# @vel     : velocity
# @idn     : identification number of a body, typically used when
#            numbering the bodies consecutively within an N-body system

end

class Nbody

  ACS_OUTPUT_NAME = self.to_s

  attr_accessor :body               # an array with elements of class Body

  def initialize
    @body = []
  end

end
