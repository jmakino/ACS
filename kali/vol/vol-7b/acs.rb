include_files = %w{ vector acsio clop }

include_files.each do |f|
  file = f + ".rb"
  require file if $0 != file
end

include Math

VERY_LARGE_NUMBER = 1e30

class Object
  def deep_copy
    Marshal.load(Marshal.dump(self))
  end
end

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

# for debugging, to see the value of a variable name:
#
#   acsp "some_name"
#
# will produce
#
#   some_name = {it's value}

def acsp(s)
  STDERR.print s, " = ", eval(s), "\n"
end


