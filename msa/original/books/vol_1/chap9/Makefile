####  Time-stamp: <2003-09-26 19:46:08 piet>
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
TOOLFILES = sphere1 sphere1a sphere

all: $(TOOLFILES)

sphere1 : sphere1.C
	g++ ${CFLAGS} -o sphere1 sphere1.C

sphere1a : sphere1a.C
	g++ ${CFLAGS} -o sphere1a sphere1a.C

sphere : sphere.C
	g++ ${CFLAGS} -o sphere sphere.C

notmp:
	\rm tmp*out.*

clean:
	\rm $(TOOLFILES)
