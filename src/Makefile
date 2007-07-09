#/////////////////////////////////////////////////////////////////
# file: Makefile
# desc: The Makefile for GSE library
#/////////////////////////////////////////////////////////////////
# The native linux c++ compiler
LINUX_CC=g++
DEBUG_FLAGS=#-DDEBUG #-DDEBUG_PRINT_INSANE 
INCLUDE_DIR=-I/home/entity/workdir/phoenixentities/src
LINUX_CFLAGS=-Wall -c `sdl-config --cflags` \
	`freetype-config --cflags` -O3 -finline-functions -funroll-loops \
	$(INCLUDE_DIR) -fPIC $(DEBUG_FLAGS) \
	-mmmx -m3dnow -msse -mfpmath=387
LDFLAGS_LINUX=-Wall -lGLU -lGLee -lSDL_image \
		`sdl-config --libs` `freetype-config --libs` \
		-lavformat -lavcodec  -l3ds 

#/////////////////////////////////////////////////////////////////
SRC=$(OBJ:.o=.cpp)
OBJ=PhoenixQuaternion.o PhoenixMath.o PhoenixSpatial.o PhoenixCore.o PhoenixSDLScreen.o PhoenixOGLRenderer.o PhoenixTGAImage.o 
#/////////////////////////////////////////////////////////////////
all:  linux

linux:
	make libs CC=$(LINUX_CC) CFLAGS="$(LINUX_CFLAGS)" 

.PHONY: clean 
clean:
	rm -f *.o *.so a.out *.exe ../bin/Phoenix.dll ../bin/libPhoenix.so ../bin/libPhoenix.a *~
.cpp.o: $@.cpp $@.h
	$(CC) $(CFLAGS) $< -o $@
libs: $(OBJ)
	$(CC) -shared $(OBJ) -o ../bin/libPhoenix.so $(LDFLAGS_LINUX)

