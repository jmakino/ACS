include_files = %w{ vector acsio clop }

include_files.each do |f|
  file = f + ".rb"
  require file if $0 != file
end

include Math

VERY_LARGE_NUMBER = 1e30

def min(a, b)
  if a > b
    return b
  else
    return a
  end
end

def max(a, b)
  if a < b
    return b
  else
    return a
  end
end
