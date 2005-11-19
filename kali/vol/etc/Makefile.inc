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
#ACSDOC = ruby $(ACSROOT)/src/coherent_programming/acsdoc.rb $(ACSDOCOPTIONS)
ACSDOC = ruby $(ACSROOT)/kali/vol/documentation/acsdoc2.rb $(ACSDOCOPTIONS)
ACSGZIP = gzip --force
LOCALDIR = kali
LOCALLIBDIR=$(ACSROOT)/$(LOCALDIR)/lib
LOCALBINDIR=$(ACSROOT)/$(LOCALDIR)/bin
       
TMPPS = $(SRC:.ok=.ps)
TMPLOG = $(SRC:.ok=.log)
TMPAUX = $(SRC:.ok=.aux)
TMPDVI = $(SRC:.ok=.dvi)
TMPTEX = $(SRC:.ok=.tex)

all:  acsdoc acscode
acsdoc:  $(alldepends)
acscode: scripts libs localscripts locallibs	
$(TARGET).ps.gz: $(TARGET).dvi
	dvips -o $(TARGET).ps $(TARGET)
	$(ACSGZIP) $(TARGET).ps 
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
	$(ACSDOC)  [a-z,A-Z]*.rb  $(SRC)
	touch doc

scripts : $(SCRIPTSRCS)
	ruby $(ACSROOT)/bin/check_and_install.rb ACSSCRIPTS $(SCRIPTSRCS)
	
libs : $(LIBSRCS)
	ruby $(ACSROOT)/bin/check_and_install.rb ACSLIBS $(LIBSRCS)

localscripts : $(LOCALSCRIPTSRCS) $(KALI_SCRIPTSRCS)
	ruby $(ACSROOT)/bin/check_and_install.rb $(LOCALBINDIR) $(LOCALSCRIPTSRCS)
	
locallibs : $(LOCALLIBSRCS) $(KALI_LIBSRCS)
	ruby $(ACSROOT)/bin/check_and_install.rb $(LOCALLIBDIR) $(LOCALLIBSRCS)

clean : 
	-rm -r doc .imgs .acsdoc.commandoutdir >&/dev/null
	-rm  .[a-z]*\.ok .[a-z]*\.rb[-+]* .[a-z]*\.rb .acsdoc.aux .acsdoc.command-* >&/dev/null
	-rm .[0-9]*\.rb[-+]* .[0-9]*\.rb .acsdoc.command-* >&/dev/null
	-rm $(TARGET).tex $(TARGET).aux $(TARGET).cp $(TARGET).dvi \
	    $(TARGET).log $(TARGET).ps $(TARGET).pdf $(TARGET).ps.gz \
	    $(TARGET).toc >&/dev/null   
	-rm tmp.tex tmp.aux tmp.dvi tmp.log tmp.ps tmp.toc tmp.jpeg* >&/dev/null	   
	-rm $(TMPPS) $(TMPLOG) $(TMPAUX) $(TMPDVI) $(TMPTEX) >&/dev/null
	    
 cleandoc : 
	-rm -r doc  $(TARGET).tex   $(TARGET).ps $(TARGET).ps.gz  $(TARGET).toc
	-rm tmp.tex tmp.aux tmp.dvi tmp.log tmp.ps tmp.toc tmp.jpeg* >&/dev/null

%.tex : %.ok
	    $(ACSDOC)  [a-z,A-Z]*.rb  $<
	$(ACSDOC)  --tolatex  $<
%.dvi : %.tex
	latex  $< 
	latex  $< 
	latex  $< 
%.ps : %.dvi
	dvips  -o $@ $< 


