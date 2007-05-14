wall:
	$(MAKE) -C ok
	$(MAKE) -C kali
	$(MAKE) -C toolbox
web:
	$(MAKE) -C toolbox web
acsdoc:
	$(MAKE) -C ok   acsdoc
	$(MAKE) -C kali acsdoc
acscode:
	$(MAKE) -C ok   acscode
	$(MAKE) -C kali acscode
clean:
	$(MAKE) -C ok   clean
	$(MAKE) -C kali clean
	$(MAKE) -C toolbox clean

cleandoc:
	$(MAKE) -C kali cleandoc
newversion: all
	ruby src/utilities/acsmaketar.rb
newversion_install: 
	ruby src/utilities/acsmaketar.rb -w
