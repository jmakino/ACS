#<i>wrong addition of +to_s+ method in Body class, for educational purpose</i>

include "body.rb"

class Body

#:segment start: to_s
  def to_s
    "  mass = " + @mass.to_s + "\n" +
    "   pos = " + @pos.to_s + "\n" +
    "   vel = " + @vel.to_s + "\n"
  end
#:segment end:

end
