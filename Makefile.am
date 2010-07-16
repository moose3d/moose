ACLOCAL_AMFLAGS=-I m4
SUBDIRS = src/Moose src/Moose/Audio src/Moose/AI src/Moose/Core src/Moose/Math src/Moose/Graphics src/Moose/Scene src/Moose/Spatial src/Moose/Volume
AUTOMAKE_OPTIONS = foreign
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = moose.pc

install-data-hook:
# create swig lib and install.
	if ! [ -e ./postbuild ]; then \
	./mooseswig.sh && ./tclpackage.tcl && cp -Rf ./tcl/Moose $(DESTDIR)/$(libdir) && \
	touch ./postbuild; fi

clean-local:
	rm -f ./postbuild



