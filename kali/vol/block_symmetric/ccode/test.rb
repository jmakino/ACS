f = open("midout2-b.dat", "r")
xa=[]
ya=[]
i=0
while s=f.gets
  if s =~ /T=/
    i+= 1
    if  true
      a=s.split
      xa.push(a[1].to_f)
      ya.push(a[5].to_f)
    end
  end
end
p xa.max
p xa.min
p ya.max
p ya.min

xdata xa
ydata ya
term
limit 
era
box
conn
