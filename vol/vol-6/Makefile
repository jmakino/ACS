SRC =  title.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp  ch05.cp  ch06.cp  ch07.cp  ch08.cp \
       ch09.cp  ch10.cp

volume6.tex: doc $(SRC) .acsdocinitrc 
	cat $(SRC) > volume6.cp
	ruby ../../src/coherent_programming/acsdoc.rb --tolatex volume6.cp

doc : $(SRC) .acsdocinitrc 
	ruby ../../src/coherent_programming/acsdoc.rb $(SRC) *.rb

clean : 
	-rm -r doc .imgs
	-rm .[a-z]*\.rb[-+]* .[a-z]*\.rb .acsdoc.command-*
	-rm volume6.tex volume6.aux volume6.cp volume6.dvi volume6.log \
	volume6.ps volume6.toc
