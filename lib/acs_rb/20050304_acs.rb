include_files = %w{ vector acsio clop }

require "acsrequire"

include_files.each do |f|
  file = f + ".rb"
  acsreq f if $0 != file
end

include Math

VERY_LARGE_NUMBER = 1e30

class Object
  def deep_copy
    Marshal.load(Marshal.dump(self))
  end
end

# for debugging, to see the value of a variable name:
#
#   acsp "some_name"
#
# will produce
#
#   some_name = {its value}

def acsp(s)
  STDERR.print s, " = ", eval(s), "\n"
end


