#
# Makefile.inc : common part for all toolbox documents
#
# This file requires TARGET and SRC defined in the file which
# includes this file.
#
all: web
web:  index.html
	$(MAKE) -C acsdoc
	$(MAKE) -C acsio
	$(MAKE) -C clop
	$(MAKE) -C graphics
	$(MAKE) -C management
	$(MAKE) -C visualization
clean:  
	$(MAKE) -C acsdoc clean
	$(MAKE) -C acsio clean
	$(MAKE) -C clop clean
	$(MAKE) -C graphics clean
	$(MAKE) -C management clean
	$(MAKE) -C visualization clean

index.html: index.yaml ../kali/index.yhtml
	kali yacsdoc.rb -y  index.yaml -t ../kali/index.yhtml > index.html
