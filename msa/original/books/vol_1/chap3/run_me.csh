#!/bin/csh -f
#
# run_me.csh : a script to create all output files, figures etc
# to be included in chapter 3
#
setenv LANG C
g++ -o forward_euler1 forward_euler1.C
forward_euler1 > forward.out
echo "|gravity> forward_euler1 > forward.out" > forward.output
echo "|gravity> head -5 forward.out" >> forward.output
head -5 forward.out >> forward.output
echo "|gravity> tail -5 forward.out" >> forward.output
tail -5 forward.out >> forward.output

echo "|gravity> ./forward_euler1 | tail -2" > forward2.output
forward_euler1 | tail -2 >> forward2.output
cat >forward.gnu <<EOF
set terminal post eps 22
set output "forward1.ps"
set size ratio -1
plot "forward.out"
EOF
gnuplot < forward.gnu

mv forward.out  forward1.out 
cat  > forward2a.output <<EOF
|gravity> g++ -o forward_euler2 forward_euler2.C
|gravity> forward_euler2 > forward2_0.01.out
forward2_0.01.out: File exists.
|gravity> rm forward2_0.01.out
rm: remove \`forward2_0.01.out\'\? y
|gravity> forward_euler2 > forward2_0.01.out
Please provide a value for the time step
0.01
|gravity> tail -2 !\$
tail -2 forward2_0.01.out
EOF
g++ -o forward_euler2 forward_euler2.C
rm forward2_0.01.out
forward_euler2 > forward2_0.01.out <<EOF
0.01
EOF
tail -2 forward2_0.01.out >> forward2a.output
tail -2 forward1.out > forward2b.output

echo "gravity> wc forward1.out  forward2_0.01.out" > wc.output
wc forward1.out  forward2_0.01.out >> wc.output



echo "|gravity> g++ -o forward_euler2b forward_euler2b.C" > forward2b_cerr.output 
echo "|gravity> forward_euler2b > /dev/null" >> forward2b_cerr.output 
g++ -o forward_euler2b forward_euler2b.C
echo 0.01 | (forward_euler2b > /dev/null)  >&  forward2b_cerr.out
head -1 forward2b_cerr.out >>  forward2b_cerr.output
echo 0.01  >>  forward2b_cerr.output
head -5 forward2b_cerr.out | tail -4 >>  forward2b_cerr.output
echo  . . . . . .  >>  forward2b_cerr.output
tail -5  forward2b_cerr.out >>  forward2b_cerr.output

echo "|gravity> g++ -o forward_euler2c forward_euler2c.C" > forward2c_cerr.output 
echo "|gravity> forward_euler2c > /dev/null" >> forward2c_cerr.output 
g++ -o forward_euler2c forward_euler2c.C
echo 0.01 | (forward_euler2c > /dev/null)  >&  forward2c_cerr.out
head -1 forward2c_cerr.out >>  forward2c_cerr.output
echo 0.01  >>  forward2c_cerr.output
head -5 forward2c_cerr.out | tail -4 >>  forward2c_cerr.output
echo  . . . . . .  >>  forward2c_cerr.output
tail -5  forward2c_cerr.out >>  forward2c_cerr.output

cat > csh_sample.output <<EOF
|gravity> g++ -o forward_euler2 forward_euler2.C
|gravity> forward_euler2 > forward2_0.01.out
forward2_0.01.out: File exists.
|gravity> rm !\$
rm: remove \`forward2_0.01.out\'\? y
|gravity> !f
Please provide a value for the time step
0.01
|gravity> tail -2 !\$
tail -2 forward2_0.01.out
EOF
tail -2 forward2_0.01.out >> csh_sample.output

gnuplot <<EOF
set term post eps
set output "forward2_0.01.ps"
plot "forward2_0.01.out"
quit
EOF

cat > forward2c.output <<EOF
|gravity> forward_euler2 > forward2_0.001.out 
Please provide a value for the time step
0.001
|gravity> tail -2 !\$
tail -2 forward2_0.001.out
EOF
echo 0.001 | forward_euler2 > forward2_0.001.out 
tail -2 forward2_0.001.out >>forward2c.output

gnuplot <<EOF
set term post eps
set output "forward2_0.001.ps"
plot "forward2_0.001.out"
quit
EOF

cat > forward3.output <<EOF
|gravity> g++ -o forward_euler3 forward_euler3.C
|gravity> forward_euler3 > forward3_0.0001.out
Please provide a value for the time step
0.0001
|gravity> tail -2 !\$
tail -2 forward3_0.0001.out
EOF
g++ -o forward_euler3 forward_euler3.C
echo 0.0001 | forward_euler3 > forward3_0.0001.out
tail -2 forward3_0.0001.out >> forward3.output
echo "|gravity> ls -al  *.out" >>forward3.output
ls -al forward1.out  forward2_*.out  forward3_0.0001.out  |awk '{   print substr($0,1,16) "carol    students" substr($0,34,999);   }' >>forward3.output

gnuplot <<EOF
set term post eps
set output "forward3_0.0001.ps"
plot "forward3_0.0001.out" notitle
quit
EOF

cat > forward3a.output <<EOF
|gravity> forward_euler3 > forward3_0.00001.out
Please provide a value for the time step
0.00001
|gravity> tail -2 !\$
tail -2 forward3_0.00001.out
EOF
echo 0.00001 | forward_euler3 > forward3_0.00001.out
tail -2 forward3_0.00001.out >> forward3a.output


gnuplot <<EOF
set term post eps
set output "forward3_0.00001.ps"
plot "forward3_0.00001.out" notitle
quit
EOF


cat > forward3b.output <<EOF
|gravity> forward_euler3 > forward3_0.000001.out
Please provide a value for the time step
0.000001
|gravity> tail -2 !\$
tail -2 forward3_0.000001.out
EOF
echo 0.000001 | forward_euler3 > forward3_0.000001.out
tail -2 forward3_0.000001.out >> forward3b.output


gnuplot <<EOF
set term post eps
set output "forward3_0.000001.ps"
plot "forward3_0.000001.out" notitle
quit
EOF



cat > forward3c.output <<EOF
|gravity> forward_euler3 > forward3_0.0000001.out
Please provide a value for the time step
0.0000001
|gravity> tail -2 !\$
tail -2 forward3_0.0000001.out
EOF
echo 0.0000001 | forward_euler3 > forward3_0.0000001.out
tail -2 forward3_0.0000001.out >> forward3c.output


gnuplot <<EOF
set term post eps
set output "forward3_0.0000001.ps"
plot "forward3_0.0000001.out" notitle
quit
EOF


g++ -o forward_euler4 forward_euler4.C

cat > forward4.output <<EOF
|gravity> g++ -o forward_euler4 forward_euler4.C
|gravity> forward_euler4 > forward4_0.01.out
Please provide a value for the time step
0.01
EOF
(echo 0.01 | forward_euler4 > forward4_0.01.out)>&forward4.output

