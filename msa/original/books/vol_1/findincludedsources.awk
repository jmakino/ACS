#!/bin/awk -f
#
# findincudedsources.awk
#
{
    if ( $1 == "code" ){
	i = NF;
	im = NF - 1;
	dir = $im;
        filename = $i;
	len = index(filename, "C");
	totallen = length(filename);
	filename = substr(filename, 1, len);
	if ((len > 0) && (totallen > len)) {
	    print dir, filename;
	}
    }
}
