#
# Makefile.inc : common part for all volumes
#
# This file requires TARGET and SRC defined in the file which
# includes this file.
# 	       
$(TARGET).tex: doc $(SRC) .acsdocinitrc
	cat $(SRC) > $(TARGET).cp
	ruby ../../../src/coherent_programming/acsdoc.rb --tolatex $(TARGET).cp
doc : $(SRC) .acsdocinitrc 
	ruby ../../../src/coherent_programming/acsdoc.rb  $(SRC) *.rb
	touch doc

clean : 
	-rm -r doc .imgs
	-rm .[a-z]*\.rb[-+]* .[a-z]*\.rb .acsdoc.command-* tmp.*
	-rm $(TARGET).tex $(TARGET).aux $(TARGET).cp $(TARGET).dvi \
	    $(TARGET).log $(TARGET).ps $(TARGET).toc

