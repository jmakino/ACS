
# foreach type (c b s )
# blockstep-$type > shortout-${type}.dat <<EOF
# 0.005  1000 0.1
# 1.9 0 0 .22941573387056176590
#EOF
#end
 foreach type (c b s )
 blockstep-$type > shortout-${type}.dat <<EOF
 0.01  1000 0.1
 1.99 0 0 .07088812050083359002
EOF
end

foreach type (c b s )
blockstep-$type > midout-${type}.dat <<EOF
0.01  10000 0.1
 1.99 0 0 .07088812050083359002
EOF
end

foreach type (c b s )
blockstep-$type > midout2-${type}.dat <<EOF
0.01  100000 1
 1.99 0 0 .07088812050083359002
EOF
end

foreach type (c )
blockstep-$type > longout-${type}.dat <<EOF
0.01  10000000 1
 1.99 0 0 .07088812050083359002
EOF
end
