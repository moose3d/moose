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
    class CTransformNode : public Phoenix::Core::CGraphNode
    {
      friend class Phoenix::Core::CGraph;
    public:
      ////////////////////
      /// Returns pointer to a transformable object that is affected by this transformnode.
      virtual Phoenix::Math::CTransformable * GetTransformable() = 0;
    };
  } // namespace Scene
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif

