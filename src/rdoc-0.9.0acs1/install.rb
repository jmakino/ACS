require 'rbconfig'
require 'find'
require 'ftools'

include Config

$ruby = CONFIG['ruby_install_name']

##
# Install a binary file. We patch in on the way through to
# insert a #! line. If this is a Unix install, we name
# the command (for example) 'rdoc' and let the shebang line
# handle running it. Under windows, we add a '.rb' extension
# and let file associations to their stuff
#

def installBIN(from, opfile)

  tmp_dir = nil
  for t in [".", "/tmp", "c:/temp", $bindir]
    stat = File.stat(t) rescue next
    if stat.directory? and stat.writable?
      tmp_dir = t
      break
    end
  end

  fail "Cannot find a temporary directory" unless tmp_dir
  tmp_file = File.join(tmp_dir, "_tmp")
    
    
  File.open(from) do |ip|
    File.open(tmp_file, "w") do |op|
      ruby = File.join($realbindir, $ruby)
      op.puts "#!#{ruby}"
      op.write ip.read
    end
  end

  opfile += ".rb" if CONFIG["target_os"] =~ /mswin/i
  File::install(tmp_file, File.join($bindir, opfile), 0755, true)
  File::unlink(tmp_file)
end





$sitedir = CONFIG["sitelibdir"]
unless $sitedir
  version = CONFIG["MAJOR"]+"."+CONFIG["MINOR"]
  $libdir = File.join(CONFIG["libdir"], "ruby", version)
  $sitedir = $:.find {|x| x =~ /site_ruby/}
  if !$sitedir
    $sitedir = File.join($libdir, "site_ruby")
  elsif $sitedir !~ Regexp.quote(version)
    $sitedir = File.join($sitedir, version)
  end
end

$bindir =  CONFIG["bindir"]

$realbindir = $bindir

bindir = CONFIG["bindir"]
if (destdir = ENV['DESTDIR'])
  $bindir  = destdir + $bindir
  $sitedir = destdir + $sitedir
  
  File::makedirs($bindir)
  File::makedirs($sitedir)
end

rdoc_dest = File.join($sitedir, "rdoc")
dot_dest  = File.join($sitedir, "dot")
rdoc_generators = File.join(rdoc_dest, "generators")
rdoc_templates  = File.join(rdoc_generators, "template")
rdoc_parsers    = File.join(rdoc_dest, "parsers")

File::makedirs(rdoc_dest,
               dot_dest, 
               rdoc_generators, 
               rdoc_templates, 
               rdoc_parsers,
               true)

File::chmod(0755, rdoc_dest)

# The library files
files = %w{
 rdoc/code_objects.rb
 rdoc/generators/*_generator.rb
 rdoc/options.rb
 rdoc/parsers/parserfactory.rb  
 rdoc/parsers/parse_*.rb  
 rdoc/template.rb
 rdoc/tokenstream.rb
 rdoc/diagram.rb
 rdoc/rdoc.rb
 dot/dot.rb
}.collect {|f| Dir.glob(f)}.flatten

for template in ["chm", "html", "xml"]
  File::makedirs(File.join(rdoc_templates, template), true)
  files.concat Dir.glob("rdoc/generators/template/#{template}/*.rb")
end

for aFile in files
  File::install(aFile, File.join($sitedir, aFile), 0644, true)
end

# and the executable

installBIN("rdoc.rb", "rdoc")

# Temporary - we used to install html_generator in the rdoc
# directory, but now it's moved

File.unlink(File.join(rdoc_dest, "html_generator.rb")) rescue 1;

# and we used to have the templates under html_template
template = File.join(rdoc_dest, "generators", "html_template")
File.unlink(File.join(template, "standard.rb")) rescue 1;
File.unlink(File.join(template, "kilmer.rb")) rescue 1;

# and then they were in template/ ...
template = File.join(rdoc_dest, "generators", "template")
File.unlink(File.join(template, "standard.rb")) rescue 1;
File.unlink(File.join(template, "kilmer.rb")) rescue 1;
File.unlink(File.join(template, "xml.rb")) rescue 1;
File.unlink(File.join(template, "rdf.rb")) rescue 1;


# 'Markup' will eventually be a separate package, but
# for now we'll install it automatically 

Dir.chdir("markup") && system("#$ruby install.rb")
