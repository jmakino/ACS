#SRC =  ch01.cp  ch02.cp  ch03.cp  ch04.cp  ch05.cp  ch06.cp  ch07.cp  ch08.cp \
#       ch09.cp  ch10.cp  ch11.cp  ch12.cp  ch13.cp  ch14.cp  ch15.cp
SRC =  ch15.cp

vol1.tex: $(SRC) .acsdocinitrc 
	cat $(SRC) > vol1.cp
	ruby ../../src/coherent_programming/acsdoc.rb --tolatex vol1.cp

all: twopfig.gif  twopfig2.gif
	ls -al $<
twopfig.gif: twopfig.eps
	convert twopfig.eps twopfig.gif
twopfig2.gif: twopfig2.eps
	convert twopfig2.eps twopfig2.gif

