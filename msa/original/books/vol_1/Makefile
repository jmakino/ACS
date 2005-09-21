#
# Makefile
#
# most entries are for developer's use
#
# Do not try make, at least with this version of makefile
#
# To install the updated set of document, say "make install"
# Just to compile the updated set of document and put them
# at "working" directory, say "make weball"
#
# The difference between weball and install is:
# a) weball put files to $(TARGETDIR), while install to $(INSTALLDIR)
# b) install create the backup version under $(INSTALLDIR)/version-date
#
# To use these entries, directories  $(TARGETDIR) and $(INSTALLDIR) must
# exist and defined outside this makefile. Normal use is to define these
# in "Makefile.local", since these directories are site-specific and outside
# 
#
#
include Makefile.local
warning:
	echo This makefile is not designed to be used by anybody
	echo other than developpers.

weball: .weball
	ls -al $(TARGETDIR)/*{ps.gz,pdf}

.weball: .web  .web_ps  .web_pdf
	touch .weball
install: .weball acscodes.tar
	-mkdir $(INSTALLDIR)/version-`date +%Y%m%d%H%M`
	mv  $(INSTALLDIR)/v1_web* $(INSTALLDIR)/version-`date +%Y%m%d%H%M`
	cp -rp  $(TARGETDIR)/v1_web* $(INSTALLDIR)
	cp acscodes.tar $(INSTALLDIR)
.web: v1_web.dvi
	latex2web-withlists.csh v1_web $(TARGETDIR)
	touch .web
	cd $(TARGETDIR) ; tar cvzf v1_web_html.tgz v1_web
.web_ps:v1_web.dvi
	dvips v1_web
	gzip v1_web.ps
	mv v1_web.ps.gz $(TARGETDIR)
	touch .web_ps
.web_pdf:v1_web.dvi
	dvipdfmx v1_web
	mv v1_web.pdf $(TARGETDIR)
	touch .web_pdf
v1_web.tex: v1.tex
	cp -pi v1.tex v1_web.tex
v1_web.dvi: v1_web.tex
	cd chap3
	make
	cd ..
	csh -f makecutfiles.csh
	latex v1_web
	latex v1_web
acscodes.tar: v1_web.tex Makefile
	tar cvf acscodes.tar */*.C  */*.awk  */*.csh
clean:
	-rm v1_web.dvi v1_web.tex
