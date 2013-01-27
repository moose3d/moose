/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "MooseRadixSort.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
using namespace Moose::Core;
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
