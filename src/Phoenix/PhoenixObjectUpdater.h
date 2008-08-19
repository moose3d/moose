#ifndef __PhoenixObjectUpdater_h__
#define __PhoenixObjectUpdater_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixResourceManager.h"
#include "PhoenixAPI.h"
#include <vector>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
    class PHOENIX_API IUpdateable
    {
    public:
      virtual ~IUpdateable() {};
      virtual void Update( size_t nPassedTimeMS ) = 0;
    };
    class PHOENIX_API IHandlerBase
    {
    public:
      virtual ~IHandlerBase() {};
      virtual void Update( size_t nPassedTimeMS ) = 0;
      virtual bool IsNull() const = 0;
    };
    /////////////////////////////////////////////////////////////////
    template<class TYPE>
    class PHOENIX_API CUpdateableObjectHandler : public IHandlerBase
    {
    private:
      Phoenix::Core::CHandle<TYPE> m_hThis;
    public:
      ////////////////////
      /// Just to make sure that object actually has the Update-method.
      CUpdateableObjectHandler( const Phoenix::Core::CHandle<TYPE> & hHandle )
      {
	m_hThis = hHandle;
      }
      
      void Update( size_t nPassedTimeMS ) 
      {
	static_cast<IUpdateable *>(*m_hThis)->Update( nPassedTimeMS );
      }
      
      bool IsNull() const { return m_hThis.IsNull(); }
    };
    /////////////////////////////////////////////////////////////////
    /// Class for updater object. 
    class PHOENIX_API CObjectUpdater
    {
    protected:
      typedef std::vector< IHandlerBase * > UpdateableVector;
      /// Holds all handles to updateable objects
      UpdateableVector m_vecUpdateables;
    public:
      
      ////////////////////
      /// Destructor.
      virtual ~CObjectUpdater();
      ////////////////////
      void Update( size_t nTimeMS = 0 );
      ////////////////////
      /// Updates all objects assigned to updater.
      /// \param rAdapter Adapter object which does the actual updating.
      /// \param nTimeMS Passed time in milliseconds since last Update().
      //template<class ADAPTER> void Update( ADAPTER &rAdapter, unsigned int nTimeMS = 0 );
      ////////////////////
      /// Begins managing given object.
      /// \param hResource Handle to an object which will be updated.
      template<class TYPE>
      void Manage( const Phoenix::Core::CHandle<TYPE> & hResource);
    };
  }
};
/////////////////////////////////////////////////////////////////
template<class TYPE>
inline void
Phoenix::Core::CObjectUpdater::Manage( const Phoenix::Core::CHandle<TYPE> &hResource )
{
  m_vecUpdateables.push_back( new CUpdateableObjectHandler<TYPE>(hResource) );
}
/////////////////////////////////////////////////////////////////
#endif
