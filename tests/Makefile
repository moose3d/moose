UNIT_OBJ=PhoenixVectorTest2.o PhoenixVectorTest3.o PhoenixVectorTest4.o PhoenixQuaternionTest.o PhoenixMatrix3x3Test.o PhoenixMathTest.o PhoenixAVLTreeTest.o PhoenixTest.o PhoenixHashTableTest.o PhoenixResourceManagerTest.o
# helpful targets for unit test compiling
all: unit
	g++ -Wall $(UNIT_OBJ) -o PhoenixTest -lUnitTest++ -lPhoenix
unit: $(UNIT_OBJ)
.cpp.o: $@.cpp 
	g++ -c -Wall -I../src $(@:.o=.cpp) -o $@ 
.PHONY: clean
clean:
	rm -f *.o a.out $(UNIT_OBJ)

