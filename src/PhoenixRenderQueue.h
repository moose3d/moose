#ifndef __PhoenixRenderQueue_h__
#define __PhoenixRenderQueue_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixOctree.h"
#include "PhoenixCamera.h"
#include "PhoenixOGLRenderer.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class for collecting and sorting renderable objects.
    template<typename TYPE>
    class CRenderQueue
    {
    protected:
      std::list<TYPE> m_lstObjects;
    public:
      ////////////////////
      /// Constructor.
      CRenderQueue();
      ////////////////////
      /// Destructor.
      ~CRenderQueue();
      ////////////////////
      /// Collects objects from octree into internal list of objects.
      /// \param camera Camera used in culling.
      /// \param octree Octree that contains objects.
      /// \returns Number of collected objects.
      unsigned int CollectObjects( const Phoenix::Graphics::CCamera &camera, 
				   Phoenix::Spatial::COctree<TYPE> &octree);
      ////////////////////
      /// Renders current queue using renderer and given adapter.
      /// \param renderer OpenGL renderer.
      template<class ADAPTER_CLASS> 
      void Render( Phoenix::Graphics::COglRenderer &renderer, ADAPTER_CLASS &rAdapter);
      ////////////////////
      /// Clears current queue.
      void Clear();
      ////////////////////
      /// Performs sorting using adapter class.
      template<class ADAPTER_CLASS> void Sort( ADAPTER_CLASS & rAdapter );
    };
  }
}
/////////////////////////////////////////////////////////////////
#define INSERT( OBJ ) { \
  if ( pNode->GetChild( OBJ ) != NULL ) \
    lstNodePtrs.push_back(pNode->GetChild( OBJ ));\
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Graphics::CRenderQueue<TYPE>::CRenderQueue()
{
  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Graphics::CRenderQueue<TYPE>::~CRenderQueue()
{
  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
unsigned int 
Phoenix::Graphics::CRenderQueue<TYPE>::CollectObjects( const Phoenix::Graphics::CCamera &camera, 
						       Phoenix::Spatial::COctree<TYPE> &octree)
{
  std::list< Phoenix::Spatial::COctreeNode<TYPE> *> lstNodePtrs;
  lstNodePtrs.push_back(octree.GetRoot());
  unsigned int nObjCount = 0;
  Phoenix::Spatial::COctreeNode<TYPE> *pNode = NULL;
  Phoenix::Volume::CSphere sphere;
  typename std::list<TYPE>::iterator it;
  while(!lstNodePtrs.empty())
  {
    // Pop first node from list
    pNode = lstNodePtrs.front();
    lstNodePtrs.pop_front();
    //std::cerr << "processing node : " << pNode << std::endl;
    // Check does cube intersect frustum
    if ( camera.Frustum().IntersectsCube(*pNode))
    {
      //std::cerr << "Camera intersects!" << std::endl;
      
      // insert objects from this node into list
      it = pNode->GetObjects().begin();
      for( ; it!=pNode->GetObjects().end();it++)
      {
	sphere = (*it)->GetBoundingSphere();
	sphere.Move( (*it)->GetWorldTransform().GetTranslation() );

	if ( Phoenix::Collision::SphereIntersectsSphere( sphere, camera.FrustumSphere()) &&
	     camera.Frustum().IntersectsSphere( sphere))
	{
	  m_lstObjects.push_back( *it );
	  nObjCount++;
	}
      }
      // If there's objects left in children, push them into nodeptr list
      if ( pNode->ChildrenContainObjects())
      {
	INSERT( TOP_LEFT_FRONT );
	INSERT( TOP_LEFT_BACK );
	INSERT( TOP_RIGHT_FRONT );
	INSERT( TOP_RIGHT_BACK );
	INSERT( BOTTOM_LEFT_FRONT );
	INSERT( BOTTOM_LEFT_BACK );
	INSERT( BOTTOM_RIGHT_FRONT );
	INSERT( BOTTOM_RIGHT_BACK );
      }
    }
  }
  return nObjCount;  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Phoenix::Graphics::CRenderQueue<TYPE>::Clear()
{
  m_lstObjects.clear();
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
template<class ADAPTER_CLASS> 
void 
Phoenix::Graphics::CRenderQueue<TYPE>::Render( Phoenix::Graphics::COglRenderer &renderer, ADAPTER_CLASS &rAdapter)
{
  typename std::list<TYPE>::iterator it = m_lstObjects.begin();
  for( ; it != m_lstObjects.end(); it++)
  {
    rAdapter.Commit(renderer, *it);
    //ogl.CommitModel( *it );
  }      
}
/////////////////////////////////////////////////////////////////
#undef INSERT
#endif
