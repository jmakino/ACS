#
# acsrequire.rb
#


class AcsRequire

  attr_accessor :verbose

  LIBDIR = ENV["ACSLIBS"]
  def addzeros(n,s="")
    ("0" * (n - s.length))+s
  end
  def datestring
    addzeros(4,Time.now.year.to_s)+ addzeros(2,Time.now.month.to_s)+
      addzeros(2,Time.now.day.to_s)
  end

  def appropriate_filename(name, datestr, libdir)
    if File.exist?(name+".rb") then
      finalname=name
    else
      subdir= libdir+"/"+name+ "_rb"
      fname=nil
      Dir.entries(subdir).sort.each{|f| 
	if f[0...(datestr.length)] <= datestr and f.index(name)
	  fname=f
	end
      }
      if fname==nil
	finalname =  name 
      else
	finalname =subdir+"/"+fname
      end
    end
    print "Acsrequire for arg #{name}\n file=#{finalname} \n" if verbose
    finalname
  end
    
    
  def acsrequire(name)
    $acsdate = datestring    if not defined? $acsdate
    raise "Env. var ACSLIB need to be defined " if LIBDIR == nil
    require appropriate_filename(name, $acsdate, LIBDIR)
  end
  
end


def acsreq(name)
  $acsrequire.acsrequire(name)
end

$acsrequire = AcsRequire.new if not defined? $acsrequire
$acsrequire.verbose=$acsreqverbosemode  if defined? $acsreqverbosemode 

