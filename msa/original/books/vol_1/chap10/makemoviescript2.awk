#!/bin/awk -f
#
# makemoviescript2.awk
#
# This awk script takes a list of filenames, each containing one N-body
# snapshot.  The files are produced by the awk script split_snapshot1.awk
# from an N-body snapshot stream.  Commands are then generated for gnuplot
# to show a movie, at a rate of one frame per second, where successive
# frames show successive snapshots.
#
# usage: this script is not intended to be used standalone.
# instead, use "makemovie.csh" which invokes this script.
#
BEGIN{
    print "set size ratio 1";
    print "set xrange [-3:3]";
    print "set yrange [-3:3]";
}
{
    print "plot \"" $1 "\" using 2:3 notitle pointtype 1 pointsize 2";
    print "pause 1";
    lastfile  = $1;
}
END{
    print "set terminal post eps 22";
    print "set output \"movie.ps\"";
    print "plot \"" lastfile "\" using 2:3 notitle pointtype 1 pointsize 2";
    print "pause -1 \"Hit return to exit\"";
}
