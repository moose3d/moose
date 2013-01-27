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

/*
 * MooseHashItem.h
 *
 *  Created on: Dec 13, 2009
 *      Author: entity
 */

#ifndef __MooseHashTable_h__
#define __MooseHashTable_h__
/////////////////////////////////////////////////////////////////
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
	/////////////////////////////////////////////////////////////////
    /// The base object for hash table storing.
    template<typename KEYTYPE, typename OBJECTTYPE>
    class MOOSE_API CHashItem
    {
    protected:
      /// The identification number for the object.
      KEYTYPE			m_Key;
      /// Object itself.
      OBJECTTYPE		m_Object;
    public:
      ////////////////////
      /// Constructor.
      CHashItem() { }
      ////////////////////
      ///
      inline void SetObject( OBJECTTYPE & object )
      {
	m_Object = object;
      }
      ////////////////////
      /// Sets item key.
      /// \param nKey Item key.
      inline void SetKey( const KEYTYPE & key )
      {
	m_Key = key;
      }
      ////////////////////
      /// Returns the key for this object.
      /// \returns Hash key.
      inline KEYTYPE GetKey() const
      {
	return m_Key;
      }
      ////////////////////
      /// Returns reference to object.
      inline OBJECTTYPE & GetObject()
      {
	return m_Object;
      }
      ////////////////////
      /// Returns reference to object.
      inline const OBJECTTYPE & GetObject() const
      {
	return m_Object;
      }
      ////////////////////
      /// Comparison operator.
      inline bool operator==( const CHashItem & item ) const
      {
	return (item.GetKey() == GetKey());
      }
      ////////////////////
      /// Assignment operator.
      inline void operator=(const CHashItem &item)
      {
	m_Key    = item.GetKey();
	m_Object = item.GetObject();
      }
    };
    ////////////////////
	/// Creates hash of string. Uses djb2 algorithm.
	struct CreateHash
	{
	  size_t operator()( const std::string &str, size_t nSlots )
	  {
	unsigned int nHash = 5381;
	int c;
	const char *pStr = str.c_str();
	while ( (c = *pStr++) )
	  nHash = ((nHash << 5) + nHash) + c; /* hash * 33 + c */

	return nHash % nSlots;
	  }
	};
        /////////////////////////////////////////////////////////////////
        /// Hash table class for several types.
        template <typename KEYTYPE, typename OBJECTTYPE, typename HashFunc=CreateHash>
        class MOOSE_API CHashTable
        {
        protected:
          /// vector of vector of objects.
          std::vector< CHashItem<KEYTYPE,OBJECTTYPE> > *m_pTable;
          size_t		   m_nSize;
        public:
          ////////////////////
          /// Default constructor.
          CHashTable( size_t nSize ) : m_nSize(nSize)
          {
    	if ( m_nSize < 1 ) m_nSize = 1;
    	m_pTable = new std::vector< CHashItem<KEYTYPE,OBJECTTYPE> >[GetSize()];
          }
          ////////////////////
          /// Destructor.
          ~CHashTable()
          {
    	delete [] m_pTable;
          }
          ////////////////////
          /// Inserts new item to hashtable.
          /// \param obj object to be inserted.
          inline void Insert( CHashItem<KEYTYPE,OBJECTTYPE> &obj )
          {
    	size_t nHash = HashFunc()( obj.GetKey(), GetSize() );
    	m_pTable[nHash].push_back(obj);
          }
          ////////////////////
          /// Returns size of this CHashTable
          /// \return number of hash slots.
          inline size_t GetSize() const
          {
    	return m_nSize;
          }
          ////////////////////
          /// Deletes object with key.
          /// \param nKey Key to object.
          void Delete( const KEYTYPE &nKey )
          {
    	CHashItem<KEYTYPE,OBJECTTYPE> item;
    	item.SetKey(nKey);

    	size_t nHash = HashFunc()(nKey, GetSize());
    	std::vector< CHashItem<KEYTYPE,OBJECTTYPE> > *pHashChain = &m_pTable[nHash];
    	typename std::vector< CHashItem<KEYTYPE,OBJECTTYPE> >::iterator it;

    	for( it = pHashChain->begin(); it!= pHashChain->end(); it++)
    	{
    	  if ( *it == item )
    	  {
    	    pHashChain->erase(it);
    	    break;
    	  }
    	}
          }
          ////////////////////
          /// Finds HashItem by key.
          /// \param nKey Key to object.
          /// \returns NULL if not found, pointer to hashitem otherwise.
          CHashItem<KEYTYPE,OBJECTTYPE> * Find( const KEYTYPE &nKey ) const
          {
    	CHashItem<KEYTYPE,OBJECTTYPE> item;
    	item.SetKey(nKey);

    	size_t nHash = HashFunc()(nKey, GetSize());
    	std::vector<CHashItem<KEYTYPE,OBJECTTYPE> > *pHashChain = &m_pTable[nHash];
    	for( size_t i =0; i<pHashChain->size();  i++)
    	{
    	  if ((*pHashChain)[i] == item ) return (&(*pHashChain)[i]);
    	}
    	return NULL;
          }

        };
  }
}
#endif /* MOOSEHASHITEM_H_ */
