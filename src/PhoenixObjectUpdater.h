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
      void Update( unsigned int nTimeMS = 0 );
      ////////////////////
      /// Updates all objects assigned to updater.
      /// \param rAdapter Adapter object which does the actual updating.
      /// \param nTimeMS Passed time in milliseconds since last Update().
      template<class ADAPTER> void Update( ADAPTER &rAdapter, unsigned int nTimeMS = 0 );
      ////////////////////
      /// Begins managing given object.
      /// \param hResource Handle to an object which will be updated.
      void Manage( const Phoenix::Core::CHandle<TYPE> & hResource);
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
  Phoenix::Core::CResourceManager<TYPE, Phoenix::Core::CHandle<TYPE> > *pManager = Phoenix::Core::CResourceManager<TYPE, Phoenix::Core::CHandle<TYPE> >::GetInstance();
  for(size_t n=0;n<m_vecUpdateables.size();n++)
  {
    pManager->Release(m_vecUpdateables[n]);
  }
  m_vecUpdateables.clear();
}
/////////////////////////////////////////////////////////////////
template<class TYPE>
inline void
Phoenix::Core::CObjectUpdater<TYPE>::Manage( const Phoenix::Core::CHandle<TYPE> &hResource )
{
  m_vecUpdateables.push_back( Phoenix::Core::CHandle<TYPE>() );
  Phoenix::Core::CResourceManager<TYPE, Phoenix::Core::CHandle<TYPE> >::GetInstance()->DuplicateHandle( hResource, m_vecUpdateables.back() );
  //assert( !m_vecUpdateables.back().IsNull());
}
/////////////////////////////////////////////////////////////////
template<class TYPE>
template<class ADAPTER> 
void 
Phoenix::Core::CObjectUpdater<TYPE>::Update( ADAPTER &rAdapter, unsigned int nTimeMS )
{
  for(size_t n=0;n<m_vecUpdateables.size();n++)
  {
    TYPE *pTemp = Phoenix::Core::CResourceManager<TYPE, Phoenix::Core::CHandle<TYPE> >::GetInstance()->GetResource(m_vecUpdateables[n]);
    rAdapter.Update( pTemp, nTimeMS );
  } 
}
/////////////////////////////////////////////////////////////////
#endif
