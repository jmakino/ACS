all: volindex
	$(MAKE) -C vol
acsdoc:
	$(MAKE) -C vol acsdoc
clean:
	$(MAKE) -C vol clean
cleandoc:
	$(MAKE) -C vol cleandoc
tar:
	tar czf ../kali.tar.gz .
volindex: index.html
	ruby -S ../src/coherent_programming/acsindextovolnumber.rb index.html
