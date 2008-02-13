#/////////////////////////////////////////////////////////////////
# file: Makefile
# desc: The Makefile for Phoenix library
#/////////////////////////////////////////////////////////////////
# The native linux c++ compiler
LINUX_CC=g++
WIN_CC=i686-mingw32-g++
DEBUG_FLAGS=#-DDEBUG #-DDEBUG_PRINT_INSANE 
INCLUDE_DIR=-I/home/entity/workdir/phoenix/src -I../../tristripper/include/
LINUX_CFLAGS=-Wall -c `sdl-config --cflags` \
	`freetype-config --cflags` -O3 -finline-functions -funroll-loops \
	$(INCLUDE_DIR) -fPIC $(DEBUG_FLAGS) \
	-mmmx -msse2 -mfpmath=sse
WIN_CFLAGS=-Wall -c `/usr/i686-mingw32/usr/bin/sdl-config --cflags` \
	-I/usr/i686-mingw32/usr/include/freetype2 \
	-O3 -finline-functions -funroll-loops \
	$(INCLUDE_DIR) $(DEBUG_FLAGS) \
	-mmmx -msse2 -mfpmath=sse
LDFLAGS_LINUX=-Wall -lGLU -lGL -lGLee \
		`sdl-config --libs` `freetype-config --libs` \
		-L../../tristripper/lib -ltristripper
LDFLAGS_WIN=-Wall -lglu32 -lGLee -lopengl32 \
		`/usr/i686-mingw32/usr/bin/sdl-config --libs` \
		-lfreetype -lz
#/////////////////////////////////////////////////////////////////
# These wait for implemenation in Phoenix
# -lavformat -lavcodec  
#/////////////////////////////////////////////////////////////////
SRC=$(OBJ:.o=.cpp)
OBJ=PhoenixQuaternion.o PhoenixMath.o PhoenixSpatial.o PhoenixCore.o \
 PhoenixSDLScreen.o PhoenixOGLRenderer.o PhoenixTGAImage.o \
 PhoenixVertexDescriptor.o PhoenixVolume.o PhoenixFrustum.o \
 PhoenixCamera.o PhoenixMathGeometry.o PhoenixTexture.o PhoenixModel.o \
 PhoenixShader.o PhoenixCollision.o PhoenixMilkshapeLoader.o PhoenixSkybox.o \
 PhoenixTransform.o PhoenixFrameBuffer.o
#/////////////////////////////////////////////////////////////////
all:  linux

linux:
	make libs CC=$(LINUX_CC) CFLAGS="$(LINUX_CFLAGS)" 
win32:
	make winlibs CC=$(WIN_CC) CFLAGS="$(WIN_CFLAGS)"

.PHONY: clean 
clean:
	rm -f *.o *.so a.out *.exe ../bin/Phoenix.dll ../bin/libPhoenix.so ../bin/libPhoenix.a *~
.cpp.o: $@.cpp $@.h
	$(CC) $(CFLAGS) $< -o $@
libs: $(OBJ)
	$(CC) -shared $(OBJ) -o ../bin/libPhoenix.so $(LDFLAGS_LINUX)

winlibs: $(OBJ)
	$(CC) -shared $(OBJ) -o ../bin/Phoenix.dll $(LDFLAGS_WIN)
