#
# Makefile.inc : common part for all toolbox documents
#
# This file requires TARGET and SRC defined in the file which
# includes this file.
#
all:  index.html
index.html: index.yaml ../../kali/index.yhtml
	kali yacsdoc.rb -y  index.yaml -t ../../kali/index.yhtml > index.html
clean:
	-rm index.html
