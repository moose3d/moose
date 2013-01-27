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

/////////////////////////////////////////////////////////////////
#ifndef __MooseRadixSort_h__
#define __MooseRadixSort_h__
/////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <cassert>
#include <vector>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
    ////////////////////
    /// Example key retriever for int data.
    struct RadixIntKeyAccessorOp
    {
      int operator()( int & object )
      {
        return object;
      }
    };
    ////////////////////////
    /// Class for sorting stuff using radix sort.
    /// TYPE is the template parameter for actual type of objects to be sorted.
    /// KEYACCESSOR is functor that accepts TYPE reference and returns an (short) integer. Override with your own.
    /// KEY_BITS is number of bits to be used from key for one pass. 
    /// KEY_BUCKETS is number of buckets reserved (related to KEY_BITS)
	template < class TYPE, class KEYACCESSOR = RadixIntKeyAccessorOp, size_t KEY_BITS = 8, size_t KEY_BUCKETS = 256>
    class CRadixSorter
    {
    protected:
      size_t            m_nInstances[KEY_BUCKETS];
      unsigned short    mask;
      KEYACCESSOR      m_KeyRetriever;
    public:

      CRadixSorter() : mask( (1 << KEY_BITS)-1 )
      {
        
      }
      
      void Sort( std::vector<TYPE> *pData,  std::vector<TYPE> * pTmpArray, const size_t SIZE )
      {
        for( size_t b=0;b<2;++b)
        {
          unsigned int shift = KEY_BITS * b;
          mask = ((1 << KEY_BITS)-1) << shift;
          // for readability
          std::vector<TYPE> &vecData = *pData;
          std::vector<TYPE> &vecTmp = *pTmpArray;
          // zero instance counts
          for( size_t t=0;t<KEY_BUCKETS;++t)   m_nInstances[t] = 0;

          // count instances
          for( size_t i=0;i<SIZE;i++)
            ++m_nInstances[ (m_KeyRetriever(vecData[i]) & mask) >> shift ];

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
            // key value for or element
            int key = (m_KeyRetriever(vecData[i]) & mask) >> shift;
            // in which index should it go
            int idx = m_nInstances[key];
            // store it to that index
            vecTmp[ idx ] = vecData[i];
            // increase index value so next element will be properly placed
            ++m_nInstances[key];
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
      unsigned short GetKey( unsigned short iValue, unsigned int nWhich )
      {
        unsigned int shift = (KEY_BITS*nWhich);
        return ((iValue & (mask << shift )) >> shift);
      }
    };

  } // namespace Core
} // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
