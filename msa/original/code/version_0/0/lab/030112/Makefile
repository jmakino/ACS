####  Time-stamp: <2003-01-14 10:30:46 piet>
#------------------------------------------------------------------------------
#
# Makefile for gravitylab
#
#   December 2001   Piet Hut
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

#CFLAGS = -Wall -W -Werror

#CXX = icc
CXX = g++

TOOLFILES = mk_plummer.00 mk_plummer.01 mk_plummer \
            nbody_sh1.00 nbody_sh1.01 nbody_sh1.02 nbody_sh1 \
	    lagrangian_radii density

all: $(TOOLFILES)

nbody_sh1.00 : nbody_sh1.00.C
	${CXX} ${CFLAGS} -o nbody_sh1.00 nbody_sh1.00.C

nbody_sh1.01 : nbody_sh1.01.C
	${CXX} ${CFLAGS} -o nbody_sh1.01 nbody_sh1.01.C

nbody_sh1.02 : nbody_sh1.02.C
	${CXX} ${CFLAGS} -o nbody_sh1.02 nbody_sh1.02.C

nbody_sh1 : nbody_sh1.C
	${CXX} ${CFLAGS} -o nbody_sh1 nbody_sh1.C

mk_plummer : mk_plummer.C
	${CXX} ${CFLAGS} -o mk_plummer mk_plummer.C

lagrangian_radii : lagrangian_radii.C
	${CXX} ${CFLAGS} -o lagrangian_radii lagrangian_radii.C

density : density.C
	${CXX} ${CFLAGS} -o density density.C

clean:
	\rm $(TOOLFILES)
