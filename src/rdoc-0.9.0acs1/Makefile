# -*- Makefile -*-
# Release must have VERSION variable set
#
#    make VERSION=0.1 release
#

release:	clean stamp tar

stamp:
		ruby -pi -e 'sub!(/".*?"/, %{"$(VERSION)"}) if /RDOC_VERSION/' rdoc/rdoc.rb
		cvs commit
		cvs rtag `echo V$(VERSION) | sed s/\\\\./_/g` rdoc

tar:
		-rm ../rdoc-$(VERSION)
		ln -fs rdoc ../rdoc-$(VERSION)
		tar 	--directory=..			\
			--create			\
			--dereference			\
			--file=rdoc-$(VERSION).tgz 	\
			--compress 			\
			`ruby -pe 'print "rdoc-$(VERSION)/"' ../rdoc-$(VERSION)/MANIFEST`
		rm ../rdoc-$(VERSION)

clean:
		@-find . -name '*~' | xargs rm
		@-find . -name doc -type d | xargs rm -rf
