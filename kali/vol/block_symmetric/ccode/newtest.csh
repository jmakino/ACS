 foreach type ( m)
 blockstep-$type > shortout-${type}.dat <<EOF
 0.01  1000 0.1
 1.99 0 0 .07088812050083359002
EOF
end

foreach type (m )
blockstep-$type > midout-${type}.dat <<EOF
0.01  10000 0.1
 1.99 0 0 .07088812050083359002
EOF
end

foreach type (m )
blockstep-$type > midout2-${type}.dat <<EOF
0.01  100000 1
 1.99 0 0 .07088812050083359002
EOF
end

foreach type (m )
blockstep-$type > longout-${type}.dat <<EOF
0.01  10000000 1
 1.99 0 0 .07088812050083359002
EOF
end
