require 'rbconfig'
require 'find'
require 'ftools'

include Config

sitedir = CONFIG["sitelibdir"]
unless sitedir
  version = CONFIG["MAJOR"]+"."+CONFIG["MINOR"]
  libdir = File.join(CONFIG["libdir"], "ruby", version)
  sitedir = $:.find {|x| x =~ /site_ruby/}
  if !sitedir
    sitedir = File.join(libdir, "site_ruby")
  elsif sitedir !~ Regexp.quote(version)
    sitedir = File.join(sitedir, version)
  end
end

if (destdir = ENV['DESTDIR'])
  sitedir = destdir + sitedir
  File::makedirs(sitedir)
end

if !File.directory?(sitedir)
  $stderr.puts "Cannot find sitedir #{sitedir}"
  exit 1
end

dest = File.join(sitedir, "markup")

File.mkpath(dest, true)

Find.find("simple_markup.rb", "simple_markup") do |fname|
  if File.directory?(fname)
    next if fname =~ /CVS/
    File.mkpath(File.join(dest, fname), true)
  else
    next unless fname =~ /\.rb$/
    next if fname =~ /install.rb$/
    File.install(fname, File.join(dest, fname), 0444, true)
  end
end

