# This class is written just to try out the correct way of using <=> in Ruby.
#
# Piet Hut and Murat Kaplan, September 2004
#
# note:
# there is within Ruby already a file rational.rb, with class Rational
# We did not know this, and first called this file "rational.rb" and our
# class "Rational", and got strange error messages.  After we realized
# the name clash, we renamed our new class "Rat".

class Rat

  include Comparable

  attr_accessor :numerator, :denominator

  def initialize(p, q)
    @numerator = p.to_i
    @denominator = q.to_i
  end

  def <=>(a)
    @numerator * a.denominator <=> @denominator * a.numerator
  end

  def to_s
    @numerator.to_s + "/" + @denominator.to_s + "\n"
  end

end
