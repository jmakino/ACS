SRC =  title.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp  ch05.cp  ch06.cp  ch07.cp  ch08.cp \
       ch09.cp  ch10.cp

volume7.tex: $(SRC) .acsdocinitrc 
	cat $(SRC) > volume7.cp
	ruby ../../src/coherent_programming/acsdoc.rb --tolatex volume7.cp

doc : $(SRC) .acsdocinitrc 
	ruby ../../src/coherent_programming/acsdoc.rb $(SRC) *.rb

clean : 
	rm -r doc .imgs
	rm volume7.tex volume7.aux volume7.cp volume7.dvi volume7.log \
	volume7.ps volume7.toc
