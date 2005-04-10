all: volindex
	$(MAKE) -C vol
clean:
	$(MAKE) -C vol clean
cleandoc:
	$(MAKE) -C vol cleandoc
tar:
	tar czf ../kali.tar.gz .
volindex: index.html
	ruby -S ../src/coherent_programming/acsindextovolnumber.rb index.html