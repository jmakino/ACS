SRC = title.cp pref.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp  ch05.cp \
              ch06.cp  ch07.cp  ch08.cp  ch09.cp ch10.cp

volume1.tex: doc $(SRC) .acsdocinitrc
	cat $(SRC) > volume1.cp
	ruby ../../../src/coherent_programming/acsdoc.rb --tolatex volume1.cp
doc : $(SRC) .acsdocinitrc 
	ruby ../../../src/coherent_programming/acsdoc.rb  $(SRC) 
	touch doc

clean :
	-rm -r doc .imgs
	-rm .[a-z]*\.rb[-+]* .[a-z]*\.rb .acsdoc.command-*
	-rm volume1.tex volume1.aux volume1.cp volume1.dvi volume1.log \
	volume1.ps volume1.toc
