all: volindex index.html
	$(MAKE) -C vol
	$(MAKE) -C pub
acsdoc:
	$(MAKE) -C vol acsdoc
	$(MAKE) -C pub acsdoc
clean:
	-rm index.html
	$(MAKE) -C vol clean
	$(MAKE) -C pub clean
cleandoc:
	$(MAKE) -C vol cleandoc
	$(MAKE) -C pub cleandoc
acscode:
	$(MAKE) -C vol acscode
	$(MAKE) -C pub acscode
tar:
	tar czf ../kali.tar.gz .
volindex: development.html
	ruby -S ../src/coherent_programming/acsindextovolnumber.rb development.html

index.html: index.yaml   index.yhtml
	kali yacsdoc.rb -y  index.yaml -t index.yhtml > index.html
