#
# acsindextovolnumber.rb
#
# create the table of volume numbers from index.html and
# put it to vol/volumetable.dat
#
# assumption: a link to any volume has the form like:
# <a href=vol/vision/doc/files/_/_title_ok.html>0.
list = {}
while s = gets
  if s =~ /^<a href=(.*)html>(.*)$/
    filename = $1
    volname = $2
    a = filename.split("/")
    if a[0] == "vol"
      print  a[1], " ", volname, "\n"
      list[a[1]]=volname 
    end
  end
end
f=open("vol/volumetable.dat","w+")
f.print Marshal.dump(list)




