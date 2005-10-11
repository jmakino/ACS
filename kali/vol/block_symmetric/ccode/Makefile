bins: blockstep-c blockstep-b blockstep-s
blockstep-c: blockstep.c
	cc -DCORRECTED_BLOCKSTEP -O4 -o blockstep-c blockstep.c -lm
blockstep-b: blockstep.c
	cc -DSIMPLE_BLOCKSTEP -O4 -o blockstep-b blockstep.c -lm
blockstep-s: blockstep.c
	cc -DSIMPLE_SYMMETRIC -O4 -o blockstep-s blockstep.c -lm
blockstep-m: blockstep.c
	cc -DMIN_BLOCKSTEP -O4 -o blockstep-m blockstep.c -lm
