UNIT_OBJ=PhoenixVectorTest2.o PhoenixVectorTest3.o PhoenixVectorTest4.o PhoenixQuaternionTest.o PhoenixMatrix3x3Test.o PhoenixMathTest.o PhoenixAVLTreeTest.o PhoenixTest.o PhoenixHashTableTest.o PhoenixResourceManagerTest.o PhoenixCameraTest.o PhoenixCollisionTest.o
# helpful targets for unit test compiling
all: unit
	g++ -g -Wall $(UNIT_OBJ) -o PhoenixTest -lUnitTest++ -lPhoenix
unit: $(UNIT_OBJ)
demo:
	g++ -Wall -I../src `sdl-config --cflags` PhoenixDemo.cpp -lPhoenix `sdl-config --libs`
.cpp.o: $@.cpp 
	g++ -c -g -Wall -I../src $(@:.o=.cpp) -o $@ 
.PHONY: clean
clean:
	rm -f *.o a.out $(UNIT_OBJ)

