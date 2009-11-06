#include "PhoenixSpatialGraph.h"
using namespace Phoenix::Spatial;
/////////////////////////////////////////////////////////////////
#define INSERT( OBJ ) { \
  if ( pNode->GetChild( OBJ ) != NULL ) \
    lstNodePtrs.push_back(pNode->GetChild( OBJ ));\
}
/////////////////////////////////////////////////////////////////
inline bool CheckTagMatch( Phoenix::Core::TAG tagObj, Phoenix::Core::TAG tag, Phoenix::Core::CTagged::TagCompare compare )
{
	using namespace Phoenix::Core;
	switch ( compare )
	{
	case CTagged::NOT_USED:
		break;
	case CTagged::EQUAL:
		if ( tagObj != tag ) return false;
		break;
	case CTagged::NOT_EQUAL:
		if ( tagObj == tag ) return false;
		break;
	case CTagged::AND:
		if ( !(tagObj & tag) ) return false;
		break;
	case CTagged::NOT_AND:
		if ( (tagObj & tag) ) return false;
		break;
	}
	// if we get here, tag is matching.
	return true;
}
/////////////////////////////////////////////////////////////////
size_t
Phoenix::Scene::CSpatialGraph::CollectObjects( const Phoenix::Graphics::CCamera &camera, std::list<CGameObject *> & list, Phoenix::Core::TAG tag, Phoenix::Core::CTagged::TagCompare compare ) const
{
	// TODO place for optimization; create cache for octree nodes ie. Prefetch( camera ) etc. Then use prefetch values in consequent queries.
  std::list< const Phoenix::Spatial::COctreeNode<CGameObject *> *> lstNodePtrs;
  lstNodePtrs.push_back(GetRoot());

  const Phoenix::Spatial::COctreeNode<CGameObject *> *pNode = NULL;
  Phoenix::Volume::CSphere sphere;
  std::list<CGameObject *>::const_iterator it;

  while(!lstNodePtrs.empty())
  {
    // Pop first node from list
    pNode = lstNodePtrs.front();
    lstNodePtrs.pop_front();

    // Check does cube intersect frustum
    if ( Phoenix::Collision::AABBIntersectsPolytope( *pNode, camera.Frustum()) )
    {
      // Check do objects intersect frustum and if so,
      // insert them into list
      it = pNode->GetObjects().begin();
      for( ; it!=pNode->GetObjects().end();it++)
      {
    	  if ( ! CheckTagMatch( (*it)->GetTag(), tag, compare) ) continue;

    	  // Then compare bounding volumes
    	  sphere = (*it)->GetBoundingSphere();
    	  sphere.Move( (*it)->GetWorldTransform().GetTranslation() );

    	  if ( Phoenix::Collision::SphereIntersectsSphere( sphere, camera.FrustumSphere()) &&
    		   Phoenix::Collision::SphereIntersectsPolytope( sphere , camera.Frustum() ))
    	  {
    		  list.push_back( *it );
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
  // return number of currently visible objects.
  return list.size();
}
/////////////////////////////////////////////////////////////////
size_t
Phoenix::Scene::CSpatialGraph::CollectObjects( const Phoenix::Volume::CSphere &cullSphere, std::list<CGameObject*> & list, Phoenix::Core::TAG tag, Phoenix::Core::CTagged::TagCompare compare ) const
{
  std::list< const Phoenix::Spatial::COctreeNode<CGameObject*> *> lstNodePtrs;
  lstNodePtrs.push_back(GetRoot());

  const Phoenix::Spatial::COctreeNode<CGameObject *> *pNode = NULL;
  Phoenix::Volume::CSphere sphere;
  std::list<CGameObject*>::const_iterator it;

  while(!lstNodePtrs.empty())
  {
    // Pop first node from list
    pNode = lstNodePtrs.front();
    lstNodePtrs.pop_front();

    // Check does cube intersect sphere
    if ( Phoenix::Collision::SphereIntersectsAACube(cullSphere, *pNode))
    {
      // Check do objects intersect sphere and if so,
      // insert them into list
      it = pNode->GetObjects().begin();
      for( ; it!=pNode->GetObjects().end();it++)
      {

    	  if ( ! CheckTagMatch( (*it)->GetTag(), tag, compare) ) continue;

    	  sphere = (*it)->GetBoundingSphere();
    	  sphere.Move( (*it)->GetWorldTransform().GetTranslation() );

    	  if ( Phoenix::Collision::SphereIntersectsSphere( sphere, cullSphere) )
    	  {
    		  list.push_back( *it );
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
  // return number of currently visible objects.
  return list.size();
}
size_t
Phoenix::Scene::CSpatialGraph::CollectObjects( const Phoenix::Math::CRay &ray, std::list<CGameObject *> & list, Phoenix::Core::TAG tag, Phoenix::Core::CTagged::TagCompare compare ) const
{
  std::list< const Phoenix::Spatial::COctreeNode<CGameObject *> *> lstNodePtrs;
  lstNodePtrs.push_back(GetRoot());

  const Phoenix::Spatial::COctreeNode<CGameObject *> *pNode = NULL;
  std::list<CGameObject *>::const_iterator it;

  while(!lstNodePtrs.empty())
  {
    // Pop first node from list
    pNode = lstNodePtrs.front();
    lstNodePtrs.pop_front();

    // Check does cube intersect ray
    // Write collision check in Engine using AABB stuff.
    if ( Phoenix::Collision::RayIntersectsAACube( ray, *pNode, NULL ))
    {
      // Check do objects intersect ray and if so,
      // insert them into list
      it = pNode->GetObjects().begin();
      for( ; it!=pNode->GetObjects().end();it++)
      {
    	  if ( ! CheckTagMatch( (*it)->GetTag(), tag, compare) ) continue;

    	  if ( (*it)->Intersects(ray) )
    	  {
    		  list.push_back( *it );
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
  // return number of currently visible objects.
  return list.size();
}
#undef INSERT
