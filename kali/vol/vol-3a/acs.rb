require "new_clop.rb" if $0 != "new_clop.rb"

include Math

class Array
  def one_level_deep_copy
    a = []
    a.push(*self)
  end
end
