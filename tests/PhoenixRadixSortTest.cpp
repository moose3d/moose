#include "PhoenixRadixSort.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
using namespace Phoenix::Core;
template <typename Type> 
Type GetRandom( Type min, Type max )
{
  Type val = max - min ;
  if ( min == max)
    return min;
  return  min + (Type)( val *(rand() / (RAND_MAX+1.0f)));
}
int main( int argc, char **argv)
{


  CRadixSorter<int> sort;

  size_t nFigures = 100;

  int *data = new int[nFigures];
  for( size_t i=0;i<nFigures;i++)
  {
    data[i] = GetRandom( 0, 9000000);
  }
  
  int *helper = new int[nFigures];
  sort.Sort( data,helper,nFigures);
  
  delete [] data;
  delete [] helper;
  
   // int which = 5;
//    int keysize = 4;
//    int val = 0xabcdef;
//    cout << hex << val << endl;
//    int mask = ((1 << keysize)-1) << keysize*which;
//    cout << hex << ((mask & val) >> keysize*which) << endl;

  
  return 0;
}
