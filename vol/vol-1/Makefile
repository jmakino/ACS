SRC =  title.cp pref.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp \
       ch05.cp  ch06.cp  ch07.cp  ch08.cp  ch09.cp  ch10.cp  ch11.cp  ch12.cp

TARGET = volume1

include ../etc/Makefile.inc

all: twopfig.gif  twopfig2.gif
	ls -al $<
twopfig.gif: twopfig.eps
	convert twopfig.eps twopfig.gif
twopfig2.gif: twopfig2.eps
	convert twopfig2.eps twopfig2.gif

