####  Time-stamp: <2002-04-26 15:12:53 piet>
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

CFLAGS = -Wall -W -Werror
TOOLFILES = nbody_sh1

all: $(TOOLFILES)

nbody_sh1 : nbody_sh1.C
	g++ ${CFLAGS} -o nbody_sh1 nbody_sh1.C

clean:
	\rm $(TOOLFILES)
