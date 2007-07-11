#ifndef __PhoenixObjectCounter_h__
#define __PhoenixObjectCounter_h__
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
    template <typename CountedType> 
    class CObjectCounter { 
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
}; // Phoenix
///////////////////////////////////////////////////////////////// 
// initialize counter with zero .
template <typename CountedType> 
size_t Phoenix::Core::CObjectCounter<CountedType>::m_nCount = 0; 
/////////////////////////////////////////////////////////////////
#endif
