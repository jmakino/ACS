#!/bin/csh -f
#
# makemovie2.csh
#
# This csh script takes the output of an N-body integrator, in the form of
# a stream of snapshots that have been output at successive time intervals.
# As a result of running this script, a gnuplot movie will run at a rate of
# one frame per second, where successive frames show successive snapshots.
#
# In addition, this script produces a ps file of the last frame
#
# Usage: makemovie2.csh snapshots
# where "snapshots" is the name of the file containing the stream of snapshots.
#
set snapfile = $1
rm  tmp_${snapfile}.*
awk -f split_snapshot1.awk $snapfile
set snapno = 0
set tmpsnap = tmp_${snapfile}
rm tmp1
while ( -e  ${tmpsnap}.${snapno} )
echo  ${tmpsnap}.${snapno}  >> tmp1
@ snapno = $snapno + 1
echo snapno = $snapno
end
awk -f makemoviescript2.awk tmp1 > tmp2.gnu
gnuplot  tmp2.gnu
