SRC =  title.cp pref.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp \
       ch05.cp  ch06.cp  ch07.cp  ch08.cp  ch09.cp  ch10.cp

volume8.tex: doc $(SRC) .acsdocinitrc 
	cat $(SRC) > volume8.cp
	ruby ../../src/coherent_programming/acsdoc.rb --tolatex volume8.cp

doc : $(SRC) .acsdocinitrc 
	ruby ../../src/coherent_programming/acsdoc.rb $(SRC) *.rb
	touch doc

clean : 
	-rm -r doc .imgs
	-rm .[a-z]*\.rb[-+]* .[a-z]*\.rb .acsdoc.command-*
	-rm volume8.tex volume8.aux volume8.cp volume8.dvi volume8.log \
	volume8.ps volume8.toc
