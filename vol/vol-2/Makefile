#SRC = title.cp pref.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp  ch05.cp \
#              ch06.cp  ch07.cp  ch08.cp  ch09.cp ch10.cp
SRC =  title.cp ch01.cp ch02.cp ch03.cp  ch04.cp  ch05.cp \
               ch06.cp  ch07.cp  ch08.cp  ch09.cp ch10.cp
volume2.tex: doc $(SRC) .acsdocinitrc
	cat $(SRC) > volume2.cp
	ruby ../../src/coherent_programming/acsdoc.rb --tolatex volume2.cp
doc : $(SRC) .acsdocinitrc 
	ruby ../../src/coherent_programming/acsdoc.rb  $(SRC) *.rb

clean : 
	-rm -r doc .imgs
	-rm .[a-z]*\.rb[-+]* .[a-z]*\.rb .acsdoc.command-*
	-rm volume2.tex volume2.aux volume2.cp volume2.dvi volume2.log \
           volume2.ps volume2.toc
