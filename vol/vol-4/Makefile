SRC =  title.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp  ch05.cp  ch06.cp  ch07.cp  ch08.cp \
       ch09.cp  ch10.cp

volume4.tex: $(SRC) .acsdocinitrc 
	cat $(SRC) > volume4.cp
	ruby ../../src/coherent_programming/acsdoc.rb --tolatex volume4.cp

doc : $(SRC) .acsdocinitrc 
	ruby ../../src/coherent_programming/acsdoc.rb $(SRC) *.rb

clean : 
	-rm -r doc .imgs
	-rm .[a-z]*\.rb[-+]* .[a-z]*\.rb .acsdoc.command-*
	-rm volume4.tex volume4.aux volume4.cp volume4.dvi volume4.log \
	volume4.ps volume4.toc
