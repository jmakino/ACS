#SRC = title.cp pref.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp  ch05.cp \
#              ch06.cp  ch07.cp  ch08.cp  ch09.cp ch10.cp
SRC =  title.cp ch01.cp ch02.cp ch03.cp  ch04.cp  ch05.cp \
               ch06.cp  ch07.cp  ch08.cp  ch09.cp ch10.cp
volume3.tex: $(SRC) .acsdocinitrc
	cat $(SRC) > volume3.cp
	ruby ../../src/coherent_programming/acsdoc.rb --tolatex volume3.cp
doc : $(SRC) .acsdocinitrc 
	ruby ../../src/coherent_programming/acsdoc.rb  $(SRC)  *.rb

clean : 
	-rm -r doc .imgs
	-rm .[a-z]*\.rb[-+]* .[a-z]*\.rb .acsdoc.command-*
	-rm volume3.tex volume3.aux volume3.cp volume3.dvi volume3.log \
	volume3.ps volume3.toc
