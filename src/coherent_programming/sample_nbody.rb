#<em> Nbody models an N-body system </em>

class Nbody

  def initialize(n)
    @body = Array.new
    for i in 0...n
      @body[i] = Body.new
    end
  end

  def to_s
    @body.each do |b| b.to_s end
  end

  def pp            # pretty print
    @body.each do |b| b.pp end
  end

end
