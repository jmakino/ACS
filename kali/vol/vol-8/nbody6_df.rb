#
# nbody6_df.rb
#
# August 2004, Piet Hut and Michele Trenti,
#
# This program extracts information from NBODY6 output,
# in order to construct the distribution function f(E).
# For more information, ask Michele; email: "trenti@sns.it"
#
# in the output file the following quantities are given:
# 1  energy array
# 2  f(E) array at time 0
# 3  f(E) array at time 10
# 4  f(E) array at time 50
# 5  f(E) array at time 100

include Math

t = 0
rh = 0
nbc = 0
trh = 1

e = []
df = []

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
      n0 = n if t == 0
    when "#1"
      rh = a[1].to_f if t == 0 and rh == 0
      trh = 0.138 * n0 * rh * sqrt(rh) / log(0.11 * n0)
    when "JMAXS"
      nbin_s = a[3].to_i
    when "EbinS"
      nbin_s.times do |i|
        e[i] = a[2+i].to_f
      end
    when "DFS"
      nbin_s.times do |i|
        df[i] = a[2+i].to_f
      end
      printf("%.5f\n", t/trh)
      nbin_s.times do |i|
        printf("%.5f %.5f\n", e[i],df[i])
      end
  end
end
