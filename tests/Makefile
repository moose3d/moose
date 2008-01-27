UNIT_OBJ=PhoenixVectorTest2.o PhoenixVectorTest3.o PhoenixVectorTest4.o PhoenixQuaternionTest.o PhoenixMatrix3x3Test.o PhoenixMathTest.o PhoenixAVLTreeTest.o PhoenixTest.o PhoenixHashTableTest.o PhoenixResourceManagerTest.o PhoenixCameraTest.o PhoenixCollisionTest.o PhoenixOctreeTest.o PhoenixTransformTest.o PhoenixTimeStampTest.o
# helpful targets for unit test compiling
all: unit
	g++ -g -Wall $(UNIT_OBJ) -o PhoenixTest -lUnitTest++ -lPhoenix
unit: $(UNIT_OBJ)
omegademo:
	g++ -Wall -DOMEGA -I../src `sdl-config --cflags` PhoenixDemo.cpp -lPhoenix `sdl-config --libs`
demo:
	g++ -Wall -I../src `sdl-config --cflags` PhoenixDemo.cpp -lPhoenix `sdl-config --libs`
simpledemo:
	g++ -Wall -I../src `sdl-config --cflags` PhoenixDemoSimple.cpp -lPhoenix `sdl-config --libs`
volumeline:
	g++ -Wall -I../src `sdl-config --cflags` PhoenixVolumeLineDemo.cpp -lPhoenix `sdl-config --libs`
octreedemo:
	 g++ -Wall -I../src `sdl-config --cflags` PhoenixOctreeDemo.cpp -lPhoenix `sdl-config --libs`
gamedemo:
	 g++ -Wall -g -I../src `sdl-config --cflags` PhoenixGameDemo.cpp -lPhoenix `sdl-config --libs` -ltcl
particledemo:
	 g++ -Wall -I../src `sdl-config --cflags` PhoenixParticleSystemDemo.cpp -lPhoenix `sdl-config --libs`
galaxydemo:
	 g++ -Wall -I../src `sdl-config --cflags` PhoenixGalaxyDemo.cpp -lPhoenix `sdl-config --libs`
messagedemo:
	 g++ -Wall -g -I../src `sdl-config --cflags` PhoenixMessageDemo.cpp -lPhoenix `sdl-config --libs` -ltcl
statemachinedemo:
	 g++ -Wall -g -I../src `sdl-config --cflags` PhoenixStateMachineDemo.cpp -lPhoenix `sdl-config --libs` -ltcl
framebufferdemo:
	 g++ -Wall -g -I../src `sdl-config --cflags` PhoenixFrameBufferDemo.cpp -lPhoenix `sdl-config --libs` -ltcl
.cpp.o: $@.cpp 
	g++ -c -g -Wall -I../src $(@:.o=.cpp) -o $@ 
.PHONY: clean
clean:
	rm -f *.o a.out $(UNIT_OBJ)

