#
# Makefile.inc : common part for all volumes
#
# This file requires TARGET and SRC defined in the file which
# includes this file.
#
ifdef DVIPDF
   alldepends =  $(TARGET).ps.gz   $(TARGET).pdf
else
   alldepends =  $(TARGET).ps.gz
endif
ACSDOCOPTIONS = --reuseoutput --keep-dot-files
ACSDOC = ruby $(ACSROOT)/src/coherent_programming/acsdoc.rb $(ACSDOCOPTIONS)
GZIP = gzip --force
LOCALDIR = kali

all:  $(alldepends) scripts libs localscripts locallibs
$(TARGET).ps.gz: $(TARGET).dvi
	dvips -o $(TARGET).ps $(TARGET)
	$(GZIP) $(TARGET).ps 
$(TARGET).pdf: $(TARGET).dvi
	$(DVIPDF)  $(TARGET)
$(TARGET).dvi: $(TARGET).tex
	latex $(TARGET)
	latex $(TARGET)
	latex $(TARGET)
$(TARGET).tex: doc $(SRC) .acsdocinitrc
	cat $(SRC) > $(TARGET).cp
	$(ACSDOC) --tolatex $(TARGET).cp
doc : $(SRC) .acsdocinitrc 
	$(ACSDOC)  $(SRC) [a-z,A-Z]*.rb
	touch doc

scripts : $(SCRIPTSRCS)
	ruby -S check_and_install.rb ACSSCRIPTS $(SCRIPTSRCS)
	
libs : $(LIBSRCS)
	ruby -S check_and_install.rb ACSLIBS $(LIBSRCS)

localscripts : $(LOCALSCRIPTSRCS) $(KALI_SCRIPTSRCS)
	ruby -S check_and_install.rb $(ACSSCRIPTS)/$(LOCALDIR) $(LOCALSCRIPTSRCS)
	
locallibs : $(LOCALLIBSRCS) $(KALI_LIBSRCS)
	ruby -S check_and_install.rb $(ACSLIBS)/$(LOCALDIR) $(LOCALLIBSRCS)

clean : 
	-rm -r doc .imgs .acsdoc.commandoutdir
	-rm .[a-z]*\.rb[-+]* .[a-z]*\.rb .acsdoc.command-* 
	-rm .[0-9]*\.rb[-+]* .[o-9]*\.rb .acsdoc.command-* 
	-rm $(TARGET).tex $(TARGET).aux $(TARGET).cp $(TARGET).dvi \
	    $(TARGET).log $(TARGET).ps $(TARGET).toc
	-rm tmp.tex tmp.aux tmp.dvi tmp.log tmp.ps tmp.toc tmp.jpeg*

%.tex : %.ok
	    $(ACSDOC)  $< [a-z,A-Z]*.rb
	$(ACSDOC)  --tolatex  $<
%.dvi : %.tex
	latex  $< 
	latex  $< 
	latex  $< 
%.ps : %.dvi
	dvips  -o $@ $< 


