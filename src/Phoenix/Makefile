#/////////////////////////////////////////////////////////////////
# file: Makefile
# desc: The Makefile for Phoenix library
#/////////////////////////////////////////////////////////////////
# The native linux c++ compiler
CC=g++
DEBUG_FLAGS=#-DDEBUG
INCLUDE_DIR=-I/home/entity/workdir/phoenix/src -I../../tristripper/include/
CFLAGS=-Wall -c -g `sdl-config --cflags` \
	`freetype-config --cflags` -O3 -finline-functions -funroll-loops \
	$(INCLUDE_DIR) -fPIC $(DEBUG_FLAGS) \
	-mmmx -msse2 -mfpmath=sse
LDFLAGS=-Wall -lGLU -lGL -lGLee \
	`sdl-config --libs` `freetype-config --libs` \
	-L../../tristripper/lib -ltristripper
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
 PhoenixTransform.o PhoenixFrameBuffer.o PhoenixModelHelper.o PhoenixDDSImage.o 
#/////////////////////////////////////////////////////////////////
all:  linux

linux:
	make libs 
linuxstatic:
	make libsstatic 
.PHONY: clean 
clean:
	rm -f *.o *.so a.out *.exe 
.cpp.o: $@.cpp $@.h
	$(CC) $(CFLAGS) $< -o $@
libs: $(OBJ)
	$(CC) -shared $(OBJ) -o ../bin/libPhoenix.so $(LDFLAGS)
libsstatic: $(OBJ)
	ar rcs ../bin/libPhoenix.a $(OBJ)  
