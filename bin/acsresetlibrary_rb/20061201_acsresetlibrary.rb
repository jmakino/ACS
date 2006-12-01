#
# acsresetlibrary.rb
#
# just remove the file $ACSLIB/.sourcenames/foo.rb,
# when argument "foo" was supplied
#
while name=ARGV.shift
  pathname = ENV["ACSLIBS"]+"/.sourcenames/"+name
  print "removing the library managemnent file for #{name}: #{pathname}\n"
  system "rm #{pathname}"
end

