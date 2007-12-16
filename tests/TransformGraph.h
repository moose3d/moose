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
    template <class TYPE, class HANDLE>
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
template <class TYPE, class HANDLE>
Phoenix::Scene::CTransformNode<TYPE,HANDLE>::CTransformNode()
{
  
}
/////////////////////////////////////////////////////////////////
template <class TYPE, class HANDLE>
Phoenix::Scene::CTransformNode<TYPE,HANDLE>::~CTransformNode()
{
  ReleaseHandle();
}
/////////////////////////////////////////////////////////////////
template <class TYPE, class HANDLE>
HANDLE &
Phoenix::Scene::CTransformNode<TYPE,HANDLE>::GetHandle()
{
  return m_hTransformableObject;
}
/////////////////////////////////////////////////////////////////
template <class TYPE, class HANDLE>
const HANDLE &
Phoenix::Scene::CTransformNode<TYPE,HANDLE>::GetHandle() const
{
  return m_hTransformableObject;
}
/////////////////////////////////////////////////////////////////
template <class TYPE, class HANDLE>
void
Phoenix::Scene::CTransformNode<TYPE,HANDLE>::ReleaseHandle()
{
  Phoenix::Core::CResourceManager<TYPE, HANDLE >::GetInstance()->Release( m_hTransformableObject );
}
/////////////////////////////////////////////////////////////////
#endif

