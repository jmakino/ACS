def datain(fname)
f = open(fname, "r")
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
  f.close
  [xa,ya]
end

def draw(fname)
  xa, ya=datain(fname)
  xdata xa
  ydata ya
  conn
end

def lmdraw(fname)
  xa, ya=datain(fname)
  xa, ya=findlms(xa,ya)
  xdata xa
  ydata ya
  conn
end

def findlms(xa, ya)
  nx=[]
  ny=[]
  (1..(xa.size-2)).each{|i|
    if ya[i] < ya[i-1] &&  ya[i] < ya[i+1] 
      nx.push(xa[i])
      ny.push(ya[i])
    end
  }
  [nx, ny]
end

  
unless File.exist?("fig1.ps")
  printer fig1.ps/vcps
  square
  viewport 0.2 1.1 0.2 1.1
  limit 0 10000 -1e-3 1e-2
  expand 1.4
  lw 2
  era
  box
  ["s", "b", "c"].each{|x|
    draw("midout-#{x}.dat")
  }
  limit 0 1 0 1
  box 1 1
  reloc 0.45 -0.15
  expand 2
  label T
  reloc -0.25 0.6
  label \\gDE
  pgend
  psfix
end
unless File.exist?("fig2.ps")
  printer fig2.ps/vcps
  square
  viewport 0.2 1.1 0.2 1.1
  limit 0 10000 -1e-3 1e-2
  expand 1.4
  lw 2
  era
  box
  ["s", "b", "c"].each{|x|
    lmdraw("midout-#{x}.dat")
  }
  limit 0 1 0 1
  box 1 1
  reloc 0.45 -0.15
  expand 2
  label T
  reloc -0.25 0.6
  label \\gDE
  pgend
  psfix
end

unless File.exist?("fig3.ps")
  printer fig3.ps/vcps
  square
  viewport 0.2 1.1 0.2 1.1
  limit 0 100000 -1e-3 1e-2
  expand 1.4
  lw 2
  era
  box
  ["s", "b", "c"].each{|x|
    lmdraw("midout2-#{x}.dat")
  }
  limit 0 1 0 1
  box 1 1
  reloc 0.45 -0.15
  expand 2
  label T
  reloc -0.25 0.6
  label \\gDE
  pgend
  psfix
end
