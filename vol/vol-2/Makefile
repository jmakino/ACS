#SRC = title.cp pref.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp  ch05.cp \
#              ch06.cp  ch07.cp  ch08.cp  ch09.cp ch10.cp
SRC =  ch01.cp 

vol2.tex: $(SRC) .acsdocinitrc
	cat $(SRC) > vol2.cp
	ruby ../../src/coherent_programming/acsdoc.rb --tolatex vol2.cp
doc : $(SRC) .acsdocinitrc 
	ruby ../../src/coherent_programming/acsdoc.rb --main .title.cp $(SRC) 
