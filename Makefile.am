SUBDIRS = src/Moose src/Moose/Audio src/Moose/AI src/Moose/Core src/Moose/Math src/Moose/Graphics src/Moose/Scene src/Moose/Spatial src/Moose/Volume
AUTOMAKE_OPTIONS = foreign
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = phoenix.pc
noinst_PROGRAMS = postbuild
postbuild_SOURCES =
postbuild: 
	@echo "Generating tcl SWIG interface module..."
	./mooseswig.sh
	touch postbuild




