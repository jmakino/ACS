#!/bin/awk -f
#
# split_snapshot1.awk
#
# This awk script takes the output of an N-body integrator, in the form of
# a stream of snapshots that have been output at successive time intervals.
# It cuts the stream up into individual snapshots, and write each snapshot
# into an individual file.  If the snapshot stream resides in a file named
# "snapshots", then each file is given a name "snapshots.number" where
# "number" starts with the value 0 for the first snapshot (typically the
# initial condition at time 0), and then counts the subsequent snapshots
# as 0,1,2,...
#
# usage: awk -f split_snapshot1.awk snapshots
# However, this script is not really intended to be used standalone.
# Typically, use "makemovie.csh" which invokes this script.
#
# where "snapshots" is the name of the file containing the stream of snapshots.
#
BEGIN{
    isnap = 0;
    ip=0;
    print ARGC, ARGV[1];
    filename = ARGV[1];
}
{
    n[isnap]=$1;
    getline;
    t[isnap]=$1;
    fname = "tmp_" filename "." isnap
    for(i=0;i<n[isnap];i++){
        getline;
        print $0 > fname;
    }
    isnap++;
}
