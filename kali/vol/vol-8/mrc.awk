BEGIN{
  t = 0;
  rh = 0;
  nbc=0;
    }
{if ($1=="LAGR:"&&t==0) {
  rh = exp($9*log(10.0));
}
}
{if ($1=="rlag:"&&t==0) {
  rh = $12;
}
}
{if ($1=="T") {
  t = $3;
  n = $6;
  nb = $12;
if (t==0) {
  n0 = n;
}
}
}
# find energies
{if ($1=="ENERGIES") {
  eint = $4;
   }
}

# find number of init binaries
{if ($1=="BINARIES:") {
  nb0 = $4;
   }
}
# number of binaries in the core
{if ($1=="OR") {
  nbc = $21;
   }
}
# log of half mass radius for binaries
{if ($1=="LAGR_BIN:") {
  lrhb = $9;
   }
}
# log of half mass radius for singles
{if ($1=="LAGR_sing:") {
  lrhs = $9;
   }
}
{if ($1=="#1") {
  if (t==0&&rh==0){
    rh = $2;
  }
  trh = 0.138*n0*rh*sqrt(rh)/log(0.11*n0);
  print t/trh,$5,$2,(n-nb)/(n0-nb0),nb/nb0,($6-nbc),nbc,lrhs,lrhb,eint,0.
    }
}
# in the output file the following quantities are given:
# 1   time (in initial half mass relaxation time units)
# 2   core radius
# 3   half mass radius
# 4   ratio of singles to the initial number of singles
# 5   ratio of binaries to the initial number of binaries
# 6   number of singles in the core
# 7   number of binaries in the core
# 8   log of half mass radius for singles
# 9   log of half mass radius for binaries
# 10  external energy of bound stars
# 11 unused column 


