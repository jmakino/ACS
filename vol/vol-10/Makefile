SRC =  title.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp  ch05.cp  ch06.cp  ch07.cp  ch08.cp \
       ch09.cp  ch10.cp

volume10.tex: doc $(SRC) .acsdocinitrc 
	cat $(SRC) > volume10.cp
	ruby ../../src/coherent_programming/acsdoc.rb --tolatex volume10.cp

doc : $(SRC) .acsdocinitrc 
	ruby ../../src/coherent_programming/acsdoc.rb $(SRC) *.rb

clean : 
	-rm -r doc .imgs
	-rm .[a-z]*\.rb[-+]* .[a-z]*\.rb .acsdoc.command-*
	-rm volume10.tex volume10.aux volume10.cp volume10.dvi volume10.log \
	volume10.ps volume10.toc
