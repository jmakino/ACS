all:
	$(MAKE) -C ok
	$(MAKE) -C kali
clean:
	$(MAKE) -C ok   clean
	$(MAKE) -C kali clean

