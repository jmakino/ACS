####  Time-stamp: <2003-01-15 12:42:53 piet>
#------------------------------------------------------------------------------
#
# Makefile for gravitylab
#
#   January 2001   Piet Hut
#
#------------------------------------------------------------------------------
#
#  During program development, to get really clean code, it is a good idea to
#  add the following three options to the g++ command:
#
#     -Wall	to get many more warning messages than usual
#     -W	to get yet even more warning messages
#     -Werror   to prevent compilation if any warnings are present
#		note: this last option may not cause the old executable
#                     to be removed, so don't assume that the presence
#                     of an executable indicates succesful compilation

CFLAGS = -Wall -W -Werror
TOOLFILES = plummer nbody_diff

#TOOLFILES = mk_fig8_triple mk_circ_binary mk_binary_binary plummer mk_pyth\
#            nbody_leap1 nbody_leap2a nbody_leap2b nbody_leap3a nbody_leap3b \
#            nbody_leap_coll1 nbody_diff

all: $(TOOLFILES)

nbody_diff : nbody_diff.C
	g++ ${CFLAGS} -o nbody_diff nbody_diff.C

nbody_leap_coll1 : nbody_leap_coll1.C
	g++ ${CFLAGS} -o nbody_leap_coll1 nbody_leap_coll1.C

nbody_leap1 : nbody_leap1.C
	g++ ${CFLAGS} -o nbody_leap1 nbody_leap1.C

nbody_leap2a : nbody_leap2a.C
	g++ ${CFLAGS} -o nbody_leap2a nbody_leap2a.C

nbody_leap2b : nbody_leap2b.C
	g++ ${CFLAGS} -o nbody_leap2b nbody_leap2b.C

nbody_leap3a : nbody_leap3a.C
	g++ ${CFLAGS} -o nbody_leap3a nbody_leap3a.C

nbody_leap3b : nbody_leap3b.C
	g++ ${CFLAGS} -o nbody_leap3b nbody_leap3b.C

mk_fig8_triple : mk_fig8_triple.C
	g++ ${CFLAGS} -o mk_fig8_triple mk_fig8_triple.C

mk_circ_binary : mk_circ_binary.C
	g++ ${CFLAGS} -o mk_circ_binary mk_circ_binary.C

mk_binary_binary : mk_binary_binary.C
	g++ ${CFLAGS} -o mk_binary_binary mk_binary_binary.C

plummer : plummer.C
	g++ ${CFLAGS} -o plummer plummer.C

mk_pyth : mk_pyth.C
	g++ ${CFLAGS} -o mk_pyth mk_pyth.C

#nbody_leap1 : nbody_leap1.C
#	g++ ${CFLAGS} -o $@ $@.C

clean:
	\rm $(TOOLFILES)
