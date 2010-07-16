#ifndef __MooseObjectCounter_h__
#define __MooseObjectCounter_h__
/////////////////////////////////////////////////////////////////
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
    template <typename CountedType> 
    class MOOSE_API CObjectCounter { 
    private: 
      static size_t m_nCount;  // number of existing objects 
    protected: 
      ////////////////////
      /// Default constructor.
      CObjectCounter() 
      { 
	++CObjectCounter<CountedType>::m_nCount; 
      } 
      ////////////////////
      /// Copy constructor .
      CObjectCounter (CObjectCounter<CountedType> const&) 
      { 
	++CObjectCounter<CountedType>::m_nCount; 
      } 
      ////////////////////
      /// Destructor.
      ~CObjectCounter() 
      { 
	--CObjectCounter<CountedType>::m_nCount; 
      } 
    public: 
      ////////////////////
      /// Return number of existing objects.
      /// \returns amount of created objects with this type.
      static size_t GetAliveCount() 
      { 
	return CObjectCounter<CountedType>::m_nCount; 
      } 
    }; // CObjectCounter
  }; // Core
}; // Moose
///////////////////////////////////////////////////////////////// 
// initialize counter with zero .
template <typename CountedType> 
size_t Moose::Core::CObjectCounter<CountedType>::m_nCount = 0; 
/////////////////////////////////////////////////////////////////
#endif
