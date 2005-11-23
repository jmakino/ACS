all: index.html
	$(MAKE) -C vol
acsdoc:
	$(MAKE) -C vol acsdoc
acscode:
	$(MAKE) -C vol acscode
clean:
	$(MAKE) -C vol clean
cleandoc:
	$(MAKE) -C vol cleandoc

index.html: index.yaml   ../kali/index.yhtml
	kali yacsdoc.rb -y  index.yaml -t ../kali/index.yhtml > index.html
