SRC =  title.cp pref.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp \
       ch05.cp  ch06.cp  ch07.cp  ch08.cp  ch09.cp  ch10.cp

volume5.tex: doc $(SRC) .acsdocinitrc 
	cat $(SRC) > volume5.cp
	ruby ../../src/coherent_programming/acsdoc.rb --tolatex volume5.cp

doc : $(SRC) .acsdocinitrc 
	ruby ../../src/coherent_programming/acsdoc.rb $(SRC) *.rb
	touch doc

clean : 
	-rm -r doc .imgs
	-rm .[a-z]*\.rb[-+]* .[a-z]*\.rb .acsdoc.command-*
	-rm volume5.tex volume5.aux volume5.cp volume5.dvi volume5.log \
	volume5.ps volume5.toc
