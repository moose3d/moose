/*
 * PhoenixHashItem.h
 *
 *  Created on: Dec 13, 2009
 *      Author: entity
 */

#ifndef __PhoenixHashTable_h__
#define __PhoenixHashTable_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
	/////////////////////////////////////////////////////////////////
    /// The base object for hash table storing.
    template<typename KEYTYPE, typename OBJECTTYPE>
    class PHOENIX_API CHashItem
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
        class PHOENIX_API CHashTable
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
#endif /* PHOENIXHASHITEM_H_ */
