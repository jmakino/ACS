include_files = %w{ vector clop }

include_files.each do |f|
  file = f + ".rb"
  require file if $0 != file
end

include Math

VERY_LARGE_NUMBER = 1e30

class Array
  def one_level_deep_copy
    a = []
    a.push(*self)
  end
end
