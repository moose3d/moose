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
    ////////////////////
    /// Interface for all updateables.
    class PHOENIX_API IUpdateable
    {
    public:
      virtual ~IUpdateable() {};
      virtual void Update( float fSecondsPassed ) = 0;
    };
    ////////////////////
    /// Handler base.
    class PHOENIX_API IHandlerBase
    {
    public:
      virtual ~IHandlerBase() {};
      virtual void Update( float fSecondsPassed ) = 0;
      virtual bool IsNull() const = 0;
    };
    /////////////////////////////////////////////////////////////////
    /// Handler for updateable objects.
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
      ////////////////////
      /// Calls update on object to which handle points.
      /// \param fSecondsPassed Time passed in seconds since last update.
      void Update( float fSecondsPassed ) 
      {
	static_cast<IUpdateable *>(*m_hThis)->Update( fSecondsPassed );
      }
      ////////////////////
      /// Checks is this object handle NULL.
      /// \returns true if is null, false otherwise.
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
      void Update( float fSecondsPassed );
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
