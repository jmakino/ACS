SRC =  title.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp  ch05.cp  ch06.cp  ch07.cp  ch08.cp \
       ch09.cp  ch10.cp

volume9.tex: $(SRC) .acsdocinitrc 
	cat $(SRC) > volume9.cp
	ruby ../../src/coherent_programming/acsdoc.rb --tolatex volume9.cp

doc : $(SRC) .acsdocinitrc 
	ruby ../../src/coherent_programming/acsdoc.rb $(SRC) *.rb

clean : 
	rm -r doc .imgs
	rm volume9.tex volume9.aux volume9.cp volume9.dvi volume9.log \
	volume9.ps volume9.toc
