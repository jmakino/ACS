SRC =  title.ok pref.ok litt.ok ch01.ok  ch02.ok  ch03.ok  ch04.ok \
       ch05.ok  ch06.ok  ch07.ok  ch08.ok  ch09.ok  ch10.ok  ch11.ok  ch12.ok

TARGET = volume1

include ../etc/Makefile.inc

all: twopfig.gif  twopfig2.gif
	ls -al $<
twopfig.gif: twopfig.eps
	convert twopfig.eps twopfig.gif
twopfig2.gif: twopfig2.eps
	convert twopfig2.eps twopfig2.gif

