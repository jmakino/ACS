all: volindex index.html
	$(MAKE) -C vol
acsdoc:
	$(MAKE) -C vol acsdoc
clean:
	$(MAKE) -C vol clean
cleandoc:
	$(MAKE) -C vol cleandoc
acscode:
	$(MAKE) -C vol acscode
tar:
	tar czf ../kali.tar.gz .
volindex: development.html
	ruby -S ../src/coherent_programming/acsindextovolnumber.rb development.html

index.html: index.yaml   index.yhtml
	kali yacsdoc.rb -y  index.yaml -t index.yhtml > index.html
