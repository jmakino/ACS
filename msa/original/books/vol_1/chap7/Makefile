####  Time-stamp: <2003-09-23 12:38:42 piet>
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

%CFLAGS = -Wall -W -Werror
CFLAGS = -g
TOOLFILES = hermite3 hermite4 hermite5 hermite6 mk_pyth

all: $(TOOLFILES)

hermite3 : hermite3.C
	g++ ${CFLAGS} -o hermite3 hermite3.C

hermite4 : hermite4.C
	g++ ${CFLAGS} -o hermite4 hermite4.C

hermite5 : hermite5.C
	g++ ${CFLAGS} -o hermite5 hermite5.C

hermite6 : hermite6.C
	g++ ${CFLAGS} -o hermite6 hermite6.C

mk_pyth : mk_pyth.C
	g++ ${CFLAGS} -o mk_pyth mk_pyth.C

clean:
	\rm $(TOOLFILES)
