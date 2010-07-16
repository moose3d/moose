/*
 * MooseCacheable.h
 *
 *  Created on: Dec 13, 2009
 *      Author: entity
 */

#ifndef MOOSECACHEABLE_H_
#define MOOSECACHEABLE_H_
////////////////////////////////////////////////////////////////
#include "MooseAPI.h"
////////////////////////////////////////////////////////////////
namespace Moose
{
	namespace Core
	{

	////////////////////////////////////////////////////////////////
    /// Which states caches can have.
    enum CACHE_STATE_TYPE
    {
      CACHE_NOCACHE = 0,  /// Cache is not used.
      CACHE_REFRESH = 1,  /// Original data has changed, cache needs refreshing.
      CACHE_UP2DATE = 2   /// Cache matches original data.
    };
    /////////////////////////////////////////////////////////////////
    /// Template class for different cache objects.
    template<typename CACHE_TYPE>
    class MOOSE_API CCacheable
    {
    protected:
      CACHE_STATE_TYPE m_nState;
      CACHE_TYPE       m_nCacheId;
      ////////////////////
      /// Constructor.
      CCacheable() : m_nState(CACHE_NOCACHE) { }
    public:
      ////////////////////
      /// Constructor.
      virtual ~CCacheable() { }
      ////////////////////
      /// Returns reference to cache.
      /// \returns Cache reference.
      CACHE_TYPE & GetCache()
      {
	return m_nCacheId;
      }
      ////////////////////
      /// Returns reference to cache.
      /// \returns Cache reference.
      const CACHE_TYPE & GetCache() const
      {
	return m_nCacheId;
      }
      ////////////////////
      /// Checks is array cached.
      /// \returns non-zero if descriptor is cached.
      /// \returns zero otherwise.
      inline int IsCached() const
      {
	return (GetState()!=CACHE_NOCACHE);
      }
      ////////////////////
      /// Sets cache state.
      /// \param nState Cache state value.
      inline void SetState( CACHE_STATE_TYPE nState )
      {
	m_nState = nState;
      }
      ////////////////////
      /// Checks cache state.
      /// \returns Currently set CACHE_STATE_TYPE.
      inline CACHE_STATE_TYPE GetState() const
      {
	return m_nState;
      }
      };
	}
}

#endif /* MOOSECACHEABLE_H_ */
