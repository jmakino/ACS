#
# mrc.rb
#

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

include Math

t = 0
rh = 0
nbc = 0

while (s = gets)
  a = s.sub(/^\s+/,"").split
  case a[0]
    when "LAGR:"
      rh = exp(a[8].to_f * log(10.0)) if t == 0
    when "rlag:"
      rh = a[11].to_f if t == 0
    when "T"
      t = a[2].to_f
      n = a[5].to_i
      nb = a[11].to_i
      n0 = n if t == 0
    when "ENERGIES"           # find energies
      eint = a[3].to_f
    when "BINARIES:"          # find number of init binaries
      nb0 = a[3].to_i
    when "OR"                 # number of binaries in the core
      nbc = a[20].to_i
    when "LAGR_BIN:"          # log of half mass radius for binaries
      lrhb = a[8].to_f
    when "LAGR_sing:"         # log of half mass radius for singles
      lrhs = a[8].to_f
    when "#1"
      rh = a[1].to_f if t == 0 and rh == 0
      trh = 0.138 * n0 * rh * sqrt(rh) / log(0.11 * n0)
      print t/trh, a[4], a[1], (n-nb)/(n0-nb0), nb/nb0,
            (a[5]-nbc), nbc, lrhs, lrhb, eint, "0.", "\n"
  end
end
