#--------------------------------------------------
#
# Makefile for complete GSE system
#
#--------------------------------------------------
all: linux
#	@echo "Type \"make linux\" for shared library (.so)"
#	@echo "Type \"make win32\" for dynamically linked library (.dll)"
#	@echo "type \"make docs\" to create API documentation with doxygen."

linux:
	cd src && $(MAKE) linux
linuxstatic:
	cd src && $(MAKE) linuxstatic
win32:
	cd src && $(MAKE) win32
clean:
	cd src && $(MAKE) clean
docs:
	doxygen doc/doxy.conf	