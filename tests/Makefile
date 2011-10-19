UNIT_OBJ=MooseVectorTest2.o MooseVectorTest3.o MooseVectorTest4.o MooseQuaternionTest.o MooseMatrix3x3Test.o MooseMathTest.o MooseAVLTreeTest.o MooseTest.o MooseHashTableTest.o MooseResourceManagerTest.o MooseCameraTest.o MooseCollisionTest.o MooseOctreeTest.o MooseTransformTest.o MooseTimeStampTest.o MooseEventHandlerTest.o MooseObjLoaderTest.o MooseLightTest.o MooseSpatialGraphTest.o MooseAssetTest.o

# helpful targets for unit test compiling
all: unit
	g++ -g -Wall $(UNIT_OBJ) -o MooseTest -lUnitTest++ `pkg-config --libs moose` -ltcl -logg -lvorbis -lvorbisfile
unit: $(UNIT_OBJ) 
.cpp.o: $@.cpp 
	g++ -c -g -Wall `pkg-config --cflags moose`  $(@:.o=.cpp) -o $@ 
.PHONY: clean
clean:
	rm -f *.o a.out $(UNIT_OBJ)

