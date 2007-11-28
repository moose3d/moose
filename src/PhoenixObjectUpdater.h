#ifndef __PhoenixObjectUpdater_h__
#define __PhoenixObjectUpdater_h__
/////////////////////////////////////////////////////////////////
#include <vector>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
    /////////////////////////////////////////////////////////////////
    /// Template class for updater object. 
    template<class TYPE>
    class CObjectUpdater
    {
    protected:
      /// Holds all handles to updateable objects
      std::vector<CHandle<TYPE> > m_vecUpdateables;
    public:
      ////////////////////
      /// Constructor.
      CObjectUpdater();
      ////////////////////
      /// Destructor.
      ~CObjectUpdater();
      ////////////////////
      /// Updates all objects assigned to updater.
      /// /param nTimeMS Passed time in milliseconds since last Update().
      void Update( unsigned int nTimeMS );
      ////////////////////
      /// Assigns given object as updateable.
      void AssignObject( const Phoenix::Core::CHandle<TYPE> & hResource);
      
    };
  }
};
/////////////////////////////////////////////////////////////////
template<class TYPE>
Phoenix::Core::CObjectUpdater<TYPE>::CObjectUpdater()
{
  
}
/////////////////////////////////////////////////////////////////
template<class TYPE>
Phoenix::Core::CObjectUpdater<TYPE>::~CObjectUpdater()
{
  for(size_t n=0;n<m_vecUpdateables.size();n++)
  {
    Phoenix::Core::CResourceManager<TYPE, Phoenix::Core::CHandle<TYPE> >::Release(m_vecUpdateables[n]);
  }
  m_vecUpdateables.clear();
}
/////////////////////////////////////////////////////////////////
template<class TYPE>
void
Phoenix::Core::CObjectUpdater<TYPE>::Update( unsigned int nTimeMS )
{
  for(size_t n=0;n<m_vecUpdateables.size();n++)
  {
    TYPE *pTemp = Phoenix::Core::CResourceManager<TYPE, Phoenix::Core::CHandle<TYPE> >::GetResource(m_vecUpdateables[n]);
    pTemp->Update( nTimeMS );
  }
}
/////////////////////////////////////////////////////////////////
template<class TYPE>
inline void
Phoenix::Core::CObjectUpdater<TYPE>::AssignObject( const Phoenix::Core::CHandle<TYPE> &hResource )
{
  m_vecUpdateables.push_back( Phoenix::Core::CHandle<TYPE>() );
  Phoenix::Core::CResourceManager<TYPE, Phoenix::Core::CHandle<TYPE> >::AttachHandle(
				  Phoenix::Core::CResourceManager<TYPE, Phoenix::Core::CHandle<TYPE> >::GetResourceName( hResource ),
				  m_vecUpdateables.back());
  
}
/////////////////////////////////////////////////////////////////
#endif
