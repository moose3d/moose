/////////////////////////////////////////////////////////////////
#ifndef __MooseRadixSort_h__
#define __MooseRadixSort_h__
/////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <cassert>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
	 ////////////////////////
    /// Class for sorting stuff using radix sort.
	template < class TYPE, size_t KEY_BITS = 4, size_t KEY_BUCKETS = 16>
    class CRadixSorter
    {
    public:
      size_t m_nInstances[KEY_BUCKETS];
      int mask;

      CRadixSorter() : mask( (1 << KEY_BITS)-1 )
      {

      }
      void Sort( TYPE *pData, TYPE *pTmpArray, const size_t SIZE )
      {

	for( size_t b=0;b<KEY_BUCKETS;++b)
	{
	  // zero instance counts
	  for( size_t t=0;t<KEY_BUCKETS;++t)   m_nInstances[t] = 0;

	  // count instances
	  for( size_t i=0;i<SIZE;i++)
	    ++m_nInstances[GetKey( pData[i],b)];

	  // zero-start sum
	  int tmp = m_nInstances[0]+m_nInstances[1];
	  int sum = tmp;
	  m_nInstances[1] = m_nInstances[0];
	  m_nInstances[0] = 0;
	  for( size_t i=2;i<KEY_BUCKETS;i++)
	  {
	    tmp = m_nInstances[i];
	    m_nInstances[i] = sum;
	    sum += tmp;
	  }


	  for( size_t i=0;i<SIZE;i++)
	  {
	    pTmpArray[m_nInstances[GetKey( pData[i],b)]] = pData[i];
	    ++m_nInstances[GetKey( pData[i],b)];
	  }
	  // swap arrays
	  std::swap( pData, pTmpArray );
	}
	// if there were odd number of key buckets, swap arrays for final time.
	if ( (KEY_BUCKETS % 2) != 0)
	{
	  // set elements into proper locations
	  for( size_t i=0;i<SIZE;i++)
	  {
	    pData[i] = pTmpArray[i];
	  }
	}
      }
      /// nWhich : 0 .. (n-1).
      inline TYPE GetKey( TYPE iValue, unsigned int nWhich )
      {
	int shift = (KEY_BITS*nWhich);
	return ((iValue & (mask << shift )) >> shift);
      }
    };

  } // namespace Core
} // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
