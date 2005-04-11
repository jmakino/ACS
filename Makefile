all:
	$(MAKE) -C ok
	$(MAKE) -C kali
acsdoc:
	$(MAKE) -C ok   acsdoc
	$(MAKE) -C kali acsdoc
clean:
	$(MAKE) -C ok   clean
	$(MAKE) -C kali clean

cleandoc:
	$(MAKE) -C kali cleandoc
newversion: all
	ruby src/utilities/acsmaketar.rb
newversion_install: 
	ruby src/utilities/acsmaketar.rb -w
