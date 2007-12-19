#ifndef __TransformGraph_h__
#define __TransformGraph_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixTransform.h>
#include <PhoenixGraph.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Scene
  {
    /////////////////////////////////////////////////////////////////
    /// Transform node template, allows several different objects to be 
    /// attached to each other via typing mechanism.
    /// \warning Objects MUST BE managed by respective ResourceManager!
    template <typename TYPE, typename RESOURCE_TYPE, typename HANDLE>
    class CTransformNode : public Phoenix::Core::CGraphNode<TYPE>
    {
      friend class Phoenix::Core::CGraph<TYPE>;
    protected:
      /// Handle to transformable object
      HANDLE m_hTransformableObject;
    protected:
      ////////////////////
      /// Constructor, called only via graph itself.
      CTransformNode();
      ////////////////////
      /// Destructor, called only via graph itself.
      ~CTransformNode();
      ////////////////////
      /// Releases handle.
      void ReleaseHandle();
    public:
      ////////////////////
      /// Returns handle pointing to transformable object.
      /// \returns Reference to a handle.
      HANDLE &	GetHandle();
      ////////////////////
      /// Returns handle pointing to transformable object.
      /// \returns Reference to a handle.
      const HANDLE &	GetHandle() const;
    };
  } // namespace Scene
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
template <typename TYPE, typename RESOURCE_TYPE, typename HANDLE>
Phoenix::Scene::CTransformNode<TYPE,RESOURCE_TYPE,HANDLE>::CTransformNode()
{
  
}
/////////////////////////////////////////////////////////////////
template <typename TYPE, typename RESOURCE_TYPE, typename HANDLE>
Phoenix::Scene::CTransformNode<TYPE,RESOURCE_TYPE,HANDLE>::~CTransformNode()
{
  ReleaseHandle();
}
/////////////////////////////////////////////////////////////////
template <typename TYPE, typename RESOURCE_TYPE, typename HANDLE>
HANDLE &
Phoenix::Scene::CTransformNode<TYPE,RESOURCE_TYPE,HANDLE>::GetHandle()
{
  return m_hTransformableObject;
}
/////////////////////////////////////////////////////////////////
template <typename TYPE, typename RESOURCE_TYPE, typename HANDLE>
const HANDLE &
Phoenix::Scene::CTransformNode<TYPE,RESOURCE_TYPE,HANDLE>::GetHandle() const
{
  return m_hTransformableObject;
}
/////////////////////////////////////////////////////////////////
template <typename TYPE, typename RESOURCE_TYPE, typename HANDLE>
void
Phoenix::Scene::CTransformNode<TYPE,RESOURCE_TYPE,HANDLE>::ReleaseHandle()
{
  Phoenix::Core::CResourceManager<RESOURCE_TYPE, HANDLE >::GetInstance()->Release( m_hTransformableObject );
}
/////////////////////////////////////////////////////////////////
#endif

