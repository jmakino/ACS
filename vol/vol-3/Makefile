#SRC = title.cp pref.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp  ch05.cp \
#              ch06.cp  ch07.cp  ch08.cp  ch09.cp ch10.cp
SRC =  ch01.cp ch02.cp ch03.cp  ch04.cp  ch05.cp \
               ch06.cp  ch07.cp  ch08.cp  ch09.cp ch10.cp
vol3.tex: $(SRC) .acsdocinitrc
	cat $(SRC) > vol3.cp
	ruby ../../src/coherent_programming/acsdoc.rb --tolatex vol3.cp
doc : $(SRC) .acsdocinitrc 
	ruby ../../src/coherent_programming/acsdoc.rb  $(SRC) 

clean : 
	rm  vol3.tex vol3.aux vol3.cp vol3.dvi vol3.log vol3.ps vol3.toc
