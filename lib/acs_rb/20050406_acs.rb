include_files = %w{ vector acsio clop }

require "acsrequire"

include_files.each do |f|
  file = f + ".rb"
  acsrequire f if $0 != file
end

include Math

VERY_LARGE_NUMBER = 1e30

class Object
  def deep_copy
    Marshal.load(Marshal.dump(self))
  end
end

# to print the value of a variable some_name, one would normally write:
# 
#    print "some_name = ", some_name, "\n"
#
# we present as an alternative:
#
#    acsp(:some_name, binding)
#
# note: use "" for printing out a function with (an) argument(s):
#
#    acsp("some_function(arg1, arg2)", binding)

def acsp(x, b)
  x = x.to_s
  e = eval(x,b)
  e = "[" + e.join(", ") + "]" if e.is_a? Array
  print x, " = ", e, "\n"
end
