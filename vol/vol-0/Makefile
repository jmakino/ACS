SRC = title.cp pref.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp  ch05.cp \
              ch06.cp  ch07.cp  ch08.cp  ch09.cp ch10.cp

volume0.tex: doc $(SRC) .acsdocinitrc
	cat $(SRC) > volume0.cp
	ruby ../../src/coherent_programming/acsdoc.rb --tolatex volume0.cp
doc : $(SRC) .acsdocinitrc 
	ruby ../../src/coherent_programming/acsdoc.rb  $(SRC) 

clean :
	-rm -r doc .imgs
	-rm .[a-z]*\.rb[-+]* .[a-z]*\.rb .acsdoc.command-*
	-rm volume0.tex volume0.aux volume0.cp volume0.dvi volume0.log \
	volume0.ps volume0.toc
