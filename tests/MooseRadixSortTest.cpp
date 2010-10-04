#include "MooseRadixSort.h"
#include "MooseRenderableModel.h"
#include "MooseRenderState.h"
#include "MooseRenderQueue.h"
#include "MooseTimer.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;
using namespace Moose::Core;
using namespace Moose::Graphics;
template <typename Type> 
Type GetRandom( Type min, Type max )
{
  Type val = max - min ;
  if ( min == max)
    return min;
  return  min + (Type)( val *(rand() / (RAND_MAX+1.0f)));
}





inline bool docmp( CRenderable *pA, CRenderable *pB) 
{
  return pA->GetRenderState().GetRenderLayer() < pB->GetRenderState().GetRenderLayer();
}


std::list<CRenderable *> buckets[256];

struct BucketSorterByte
{
  static bool cmp( CRenderable *a, CRenderable *b )
  {
    return (a->GetRenderState().GetRenderLayer() < b->GetRenderState().GetRenderLayer());
  }
  
  void operator()( std::list<CRenderable *>  & lst )
  {
    
    int mask = 0;
    
    mask = 0xff;
    
    for( std::list<CRenderable *>::iterator it = lst.begin(); it != lst.end(); it++)
    {
      CRenderable *pR = *it;
      buckets[ pR->GetRenderState().GetRenderLayer() & mask ].push_back(pR);
    }
    lst.clear();
    // merge into original (maintain relative order)
    for(int i=0;i<256;i++)
    {
      lst.splice(lst.end(),buckets[i]);
    }
    mask = 0xff00;
    for( std::list<CRenderable *>::iterator it = lst.begin(); it != lst.end(); it++)
    {
      CRenderable *pR = *it;
      buckets[ (pR->GetRenderState().GetRenderLayer() & mask) >> 8 ].push_back(pR);
    }
    lst.clear();
    // merge into original (maintain relative order)
    for(int i=0;i<256;i++)
    {
      lst.splice(lst.end(),buckets[i]);
    }
  }

};
////////////////////////////////////////////////////////////////////////////////
struct KeyPtr_t 
{
  unsigned short        key;
  CRenderable *         renderable;
};
KeyPtr_t pData[2000000];
////////////////////////////////////////////////////////////////////////////////
struct RadixSorterByte 
{
#define BUCKETS 256

  size_t count[BUCKETS];  
  
  void Radix( unsigned short byteOffset, size_t numElements, 
                     KeyPtr_t * source,         KeyPtr_t *dest )
  {
    KeyPtr_t *srcPtr;
    size_t *countPtr, sum, c;
    unsigned char *bytePtr;
    countPtr = count;
    // zero count
    for(size_t i=0;i<BUCKETS;i++) *countPtr++ = 0;

    // grab pointer to proper part of key and compute instances
    bytePtr = (unsigned char *)(&source->key) + byteOffset;
    for(size_t i=0;i<numElements; i++, bytePtr+=sizeof(KeyPtr_t))
    {
      ++count[*bytePtr];
    }
    // compute zero-index sum
    sum = 0;
    countPtr = count;
    for( size_t i=0;i<BUCKETS;i++, ++countPtr)
    {
      c = *countPtr;
      *countPtr = sum;
      sum += c;
    }
    // fill dest appropriately
    bytePtr = ((unsigned char *)source)+ byteOffset;
    srcPtr = source;
    for(int i=0;i<numElements;i++, bytePtr+=sizeof(KeyPtr_t), ++srcPtr)
    {
      dest[ count[*bytePtr]++ ] = *srcPtr; 
    }
  }

  void operator()( std::list<CRenderable *> & lst )
  {

    // Allocate data for radix + its helper memory
    //KeyPtr_t *pData = new KeyPtr_t[lst.size()*2];
    size_t numElements = lst.size();
    std::list<CRenderable *>::iterator it = lst.begin();
    for( int i=0; it != lst.end(); it++, i++)
    {
      pData[i].key = (*it)->GetRenderState().GetRenderLayer();
      pData[i].renderable = *it;
    }
    lst.clear();
    Radix( 0, numElements, pData, pData+numElements);
    Radix( 1, numElements, pData+numElements, pData );

    // copy elements back to list
    for( int i=0; i<numElements; i++)
    {
      lst.push_back(pData[i].renderable);
    }
    
    //delete [] pData;
  }
};


int main( int argc, char **argv)
{
  CRenderQueue<CRenderable *> q,q2;
  size_t nFigures = 10000;
  list<CRenderable *> data;
  for ( int i=0;i<nFigures;i++)
  {
    CRenderableModel *pModel = new CRenderableModel();
    pModel->GetRenderState().SetRenderLayer( GetRandom(0,65535));
    data.push_back( pModel);
  }


  q.GetObjectList().insert(q.GetObjectList().begin(), data.begin(), data.end());
  CTimer time;
  time.Reset();
  q.GetObjectList().sort( docmp);
  time.Update();
  cout << "Sorting time (stl): " << time.GetPassedTime() << "\n";

  std::list<CRenderable *>::iterator it = q.GetObjectList().begin();
  unsigned short prevValue = (*it)->GetRenderState().GetRenderLayer();
  unsigned short curValue = 0;
  it++;
  for( ; it != q.GetObjectList().end(); it++)
  {
    curValue = (*it)->GetRenderState().GetRenderLayer();
    if ( prevValue > curValue ) 
    {
      cerr << "stl sorting failed\n";
      break;
    }
    prevValue = curValue;
  }
  q.GetObjectList().clear();
  // Sort using bucket sort
  q.GetObjectList().insert(q.GetObjectList().begin(), data.begin(), data.end());
  cerr << "bucket sorting begins...\n";
  BucketSorterByte s;
  time.Reset();
  q.Sort<BucketSorterByte >(s  );
  time.Update();
  cout << "Sorting time (bucket): " <<  time.GetPassedTime() << "\n";
  
  it = q.GetObjectList().begin();
  prevValue = (*it)->GetRenderState().GetRenderLayer();
  curValue = 0;
  it++;
  for( ; it != q.GetObjectList().end(); it++)
  {
    curValue = (*it)->GetRenderState().GetRenderLayer();
    if ( prevValue > curValue ) 
    {
      cerr << "bucket sorting failed\n";
      break;
    }
    prevValue = curValue;
  }
  q.GetObjectList().clear();
  q.GetObjectList().insert(q.GetObjectList().begin(), data.begin(), data.end());
  cerr << "radix sorting begins...\n";
  RadixSorterByte rs;
  time.Reset();
  q.Sort<RadixSorterByte >(rs  );
  time.Update();
  cout << "Sorting time (radix): " <<  time.GetPassedTime() << "\n";
  
  it = q.GetObjectList().begin();
  prevValue = (*it)->GetRenderState().GetRenderLayer();
   curValue = 0;
  it++;
  for( ; it != q.GetObjectList().end(); it++)
  {
    curValue = (*it)->GetRenderState().GetRenderLayer();
    if ( prevValue > curValue ) 
    {
      cerr << "radix sorting failed\n";
      break;
    }
    prevValue = curValue;
  }
  q.GetObjectList().clear();
  
   // int which = 5;
//    int keysize = 4;
//    int val = 0xabcdef;
//    cout << hex << val << endl;
//    int mask = ((1 << keysize)-1) << keysize*which;
//    cout << hex << ((mask & val) >> keysize*which) << endl;

  
  return 0;
}
