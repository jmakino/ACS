SRC = pref.cp ch01.cp  ch02.cp  ch03.cp  ch04.cp  ch05.cp \
              ch06.cp  ch07.cp  ch08.cp  ch09.cp
vol0.tex: $(SRC) .acsdocinitrc
	cat $(SRC) > vol0.cp
	acsdoc.rb --tolatex vol0.cp
