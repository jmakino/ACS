require 'rdoc/rdoc'

begin
  r = RDoc::RDoc.new
  r.document(ARGV)
rescue RDoc::RDocError => e
  $stderr.puts e.message
  exit(1)
end
