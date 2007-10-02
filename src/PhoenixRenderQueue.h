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
				   const Phoenix::Spatial::COctree<TYPE> &octree);
      ////////////////////
      /// Renders current queue using renderer.
      /// \param renderer OpenGL renderer.
      void Render( Phoenix::Graphics::COglRenderer &renderer );
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
Phoenix::Graphics::CRenderQueue::CRenderQueue()
{
  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Graphics::CRenderQueue::~CRenderQueue()
{
  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
unsigned int 
Phoenix::Graphics::CRenderQueue::CollectObjects( const Phoenix::Graphics::CCamera &camera, 
						 const Phoenix::Spatial::COctree<TYPE> &octree)
{
  std::list< Phoenix::Spatial::COctree<TYPE> *> lstNodePtrs;
  lstNodePtrs.push_back(octree.GetRoot());
  unsigned int nObjCount = 0;

  while(!lstNodePtrs.empty())
  {
    // Pop first node from list
    Phoenix::Spatial::COctreeNode<TYPE> *pNode = lstNodePtrs.front();
    lstNodePtrs.pop_front();

    // Check does cube intersect frustum
    if ( camera.Frustum().IntersectsCube(*pNode))
    {
      // insert objects from this node into list
      nObjCount += pNode->GetObjects().size();
      typename std::list<TYPE>::iterator it = pNode->GetObjects().begin();
      for( ; it!=pNode->GetObjects().end();it++)
      {
	m_lstObjects.push_back( *it );
      }
      // If there's objects left in children, push them into nod
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
Phoenix::Graphics::CRenderQueue::Render( Phoenix::Graphics::COglRenderer &renderer )
{
  typename std::list<TYPE>::iterator it = m_lstObjects.begin();
  for( ; it != m_lstObjects.end(); it++)
  {
    // determine type and call proper command from renderer.
  }
}
/////////////////////////////////////////////////////////////////
#endif
