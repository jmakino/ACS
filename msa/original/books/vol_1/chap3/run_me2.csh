g++ -o forward_euler4 forward_euler4.C

cat > forward4.output <<EOF
|gravity> g++ -o forward_euler4 forward_euler4.C
|gravity> forward_euler4 > forward4_0.01.out
Please provide a value for the time step
0.01
EOF
(echo 0.01 | forward_euler4 > forward4_0.01.out) >& forward4a.output
tail +2 forward4a.output >>  forward4.output

echo 4 end

cat > forward4a.output<<EOF
|gravity> tail -2 forward4_0.01.out
EOF
tail -2 forward4_0.01.out >>  forward4a.output

echo 4a end


cat > forward4b.output <<EOF
|gravity> forward_euler4 > /dev/null
Please provide a value for the time step
0.001
EOF
(echo 0.001 | forward_euler4 > /dev/null ) >& forward4b2.output
tail +2 forward4b2.output >>  forward4b.output

foreach dt ( 0.0001 0.00001 0.000001 )
echo dt = $dt

cat >> forward4b.output <<EOF
|gravity> !!
Please provide a value for the time step
$dt
EOF
(echo $dt | forward_euler4 > /dev/null ) >& forward4b2.output
tail +2 forward4b2.output >>  forward4b.output

end
