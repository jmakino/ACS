include_files = %w{ vector clop acsio }
 
require "acsrequire"

include_files.each do |f|
  acsrequire f if File.basename($0,".rb") != f
end

include Math

VERY_LARGE_NUMBER = 1e30
VERY_SMALL_NUMBER = 1.0/VERY_LARGE_NUMBER

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
