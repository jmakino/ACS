#!/bin/csh -f
#
# forward_euler_and_leapfrog1.csh
#
# run this program in a subdirectory
#
foreach program (forward_euler2 leapfrog1)
    if ( $program == forward_euler2) then
       set fname = for
    else
        set fname = leap
    endif
    foreach refine (1 10 100 1000)
       set fullname = ${fname}${refine}
       rm log
       echo $refine | (../${program} >! ${fullname}.out) >&log
       head -5 log >! ${fullname}.err
       echo ". . . . ." >> ${fullname}.err
       tail -3 log >> ${fullname}.err
       gnuplot <<EOF
set terminal post eps 22
set output "${fullname}.ps"
plot '${fullname}.out'
EOF
    end
end

