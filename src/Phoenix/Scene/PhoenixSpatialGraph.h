#ifndef __PhoenixSpatialGraph_h__
#define __PhoenixSpatialGraph_h__
/////////////////////////////////////////////////////////////////
//#include "PhoenixGameObject.h"
#include "PhoenixGameObject.h"
#include "PhoenixOctree.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Scene
  {
    /////////////////////////////////////////////////////////////////
    /// Structure for storing objects and retrieving them according
    /// to intersection volume.
    class CSpatialGraph : public Phoenix::Spatial::COctree<Phoenix::Scene::CGameObject *>
    {
    public:
      ////////////////////
      /// Constructs spatial structure using an octree.
      /// \param nNumLevels Depth of octree.
      /// \param fWorldSize Maximum size of world cube side.
      CSpatialGraph( unsigned int nNumLevels, float fWorldSize ) : Phoenix::Spatial::COctree<Phoenix::Scene::CGameObject *>(nNumLevels,fWorldSize){}
      ////////////////////
      /// Inserts object into spatial graph according to world position and bounding sphere size.
      /// \param pGameObject Object to be inserted.
      void InsertObject( Phoenix::Scene::CGameObject *pGameObject )
      {
        assert(pGameObject!=NULL);

        unsigned int nSpatialIndex = Phoenix::Spatial::COctree<CGameObject *>::InsertObject(
                                                                                            pGameObject, pGameObject->GetWorldBoundingSphere());
        assert( nSpatialIndex < this->GetNodeCount() );
        pGameObject->SetSpatialIndex( nSpatialIndex );

      }
      ////////////////////
      /// Removes object from spatial graph.
      /// \param pGameObject Object to be removed.
      void DeleteObject( CGameObject *pGameObject )
      {
        Phoenix::Spatial::COctreeNode<CGameObject *> *pNode = this->GetNodeAt( pGameObject->GetSpatialIndex());
        if ( pNode != NULL )
        {
          pNode->DeleteObject( pGameObject );
        }
      }
      ////////////////////
      /// Updates spatial location of gameobject.
      /// \param pGameObject Object to be updated.
      void Update( CGameObject *pGameObject )
      {
        Phoenix::Math::CVector3<float> vTmp = pGameObject->GetWorldTransform().GetTranslation();
        vTmp += pGameObject->GetBoundingSphere().GetPosition();

        unsigned int nLevel = this->GetObjectDepth( pGameObject->GetBoundingSphere().GetRadius() );
        unsigned int nIndex = this->GetIndex1D(nLevel, vTmp[0], vTmp[1], vTmp[2]);

        // If object has changed spatial location enough
        if ( pGameObject->GetSpatialIndex() != nIndex)
        {
          assert( pGameObject->GetSpatialIndex() < this->GetNodeCount());
          // Remove from  previous location
          this->GetNodeAt( pGameObject->GetSpatialIndex())->DeleteObject( pGameObject );
          // Insert into new
          this->GetNodeAt( nIndex )->AddObject( pGameObject );
          // Update child statuses up to root.
          this-> PropagateChildrenStatus( this->GetNodeAt( nIndex ));
          // Store new spatial index.
          pGameObject->SetSpatialIndex( nIndex );
        }
      }

      ////////////////////
      /// Collects objects from Octree into list of objects.
      /// \param camera Camera used in culling.
      /// \param list List where objects are inserted.
      /// \param tag What TAG is used in comparison.
      /// \param compare How TAG value is used in comparison. By default, it is not used.
      /// \returns Number of collected objects.
      template<class GameObjPtrContainer>
      size_t CollectObjects( const Phoenix::Graphics::CCamera &camera, GameObjPtrContainer & list, Phoenix::Core::TAG tag = 0, Phoenix::Core::CTagged::TagCompare compare = Phoenix::Core::CTagged::NOT_USED ) const;
      ////////////////////
      /// Collects objects from Octree into list of objects.
      /// \param frustum Frustumused in culling.
      /// \param list List where objects are inserted.
      /// \param tag What TAG is used in comparison.
      /// \param compare How TAG value is used in comparison. By default, it is not used.
      /// \returns Number of collected objects.
      template<class GameObjPtrContainer>
      size_t CollectObjects( const Phoenix::Graphics::CFrustum &frustum, GameObjPtrContainer & list, Phoenix::Core::TAG tag = 0, Phoenix::Core::CTagged::TagCompare compare = Phoenix::Core::CTagged::NOT_USED ) const;
      ////////////////////
      /// Collects objects from Octree into list of objects.
      /// All objects must should be inherited from CGameObject or provide similar interface
      /// ( GetBoundingSphere, GetWorldTransform() ).
      /// \param sphere Objects must intersect this in order to be included.
      /// \param list List where objects are inserted.
      /// \param tag What TAG is used in comparison.
      /// \param compare How TAG value is used in comparison. By default, it is not used.
      /// \returns Number of collected objects.
      template<class GameObjPtrContainer>
      size_t CollectObjects( const Phoenix::Volume::CSphere &sphere, GameObjPtrContainer & list, Phoenix::Core::TAG tag = 0, Phoenix::Core::CTagged::TagCompare compare = Phoenix::Core::CTagged::NOT_USED ) const;
      ////////////////////
      /// Collects objects from Octree into list of objects.
      /// All objects must should be inherited from CGameObject or provide similar interface
      /// ( GetBoundingSphere, GetWorldTransform() ).
      /// \param sphere Objects must intersect this in order to be included.
      /// \param list List where objects are inserted.
      /// \param tag What TAG is used in comparison.
      /// \param compare How TAG value is used in comparison. By default, it is not used.
      /// \returns Number of collected objects.
      template<class GameObjPtrContainer>
      size_t CollectObjects( const Phoenix::Math::CRay &ray, GameObjPtrContainer & list, Phoenix::Core::TAG tag = 0, Phoenix::Core::CTagged::TagCompare compare = Phoenix::Core::CTagged::NOT_USED ) const;

    };
  } // Scene
}// Phoenix
////////////////////////////////////////////////////////////////////////////////
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
template<class GameObjPtrContainer>
size_t
Phoenix::Scene::CSpatialGraph::CollectObjects( const Phoenix::Graphics::CFrustum &frustum, GameObjPtrContainer & list, Phoenix::Core::TAG tag, Phoenix::Core::CTagged::TagCompare compare ) const
{
  using namespace Phoenix::Spatial;
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
		if ( Phoenix::Collision::AABBIntersectsPolytope( *pNode, frustum) )
		{
			// Check do objects intersect frustum and if so,
			// insert them into list
			it = pNode->GetObjects().begin();
			for( ; it!=pNode->GetObjects().end();it++)
			{
				if ( ! CheckTagMatch( (*it)->GetTag(), tag, compare) ) continue;

				// Then compare bounding volumes
				sphere = (*it)->GetWorldBoundingSphere();

				if ( Phoenix::Collision::SphereIntersectsPolytope( sphere , frustum ))
				{
					list.push_back( *it );
				}
			}
			// If there's objects left in children, push them into nodeptr list
			if ( pNode->ChildrenContainObjects())
			{
		INSERT( O_TOP_LEFT_FRONT );
		INSERT( O_TOP_LEFT_BACK );
		INSERT( O_TOP_RIGHT_FRONT );
		INSERT( O_TOP_RIGHT_BACK );
		INSERT( O_BOTTOM_LEFT_FRONT );
		INSERT( O_BOTTOM_LEFT_BACK );
		INSERT( O_BOTTOM_RIGHT_FRONT );
		INSERT( O_BOTTOM_RIGHT_BACK );
			}
		}
	}
	// return number of currently visible objects.
	return list.size();
}
/////////////////////////////////////////////////////////////////
template<class GameObjPtrContainer>
size_t
Phoenix::Scene::CSpatialGraph::CollectObjects( const Phoenix::Graphics::CCamera &camera, GameObjPtrContainer & list, Phoenix::Core::TAG tag, Phoenix::Core::CTagged::TagCompare compare ) const
{
  using namespace Phoenix::Spatial;
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
		INSERT( O_TOP_LEFT_FRONT );
		INSERT( O_TOP_LEFT_BACK );
		INSERT( O_TOP_RIGHT_FRONT );
		INSERT( O_TOP_RIGHT_BACK );
		INSERT( O_BOTTOM_LEFT_FRONT );
		INSERT( O_BOTTOM_LEFT_BACK );
		INSERT( O_BOTTOM_RIGHT_FRONT );
		INSERT( O_BOTTOM_RIGHT_BACK );
      }
    }
  }
  // return number of currently visible objects.
  return list.size();
}
/////////////////////////////////////////////////////////////////
template<class GameObjPtrContainer>
size_t
Phoenix::Scene::CSpatialGraph::CollectObjects( const Phoenix::Volume::CSphere &cullSphere, GameObjPtrContainer & list, Phoenix::Core::TAG tag, Phoenix::Core::CTagged::TagCompare compare ) const
{
  using namespace Phoenix::Spatial;
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
	INSERT( O_TOP_LEFT_FRONT );
	INSERT( O_TOP_LEFT_BACK );
	INSERT( O_TOP_RIGHT_FRONT );
	INSERT( O_TOP_RIGHT_BACK );
	INSERT( O_BOTTOM_LEFT_FRONT );
	INSERT( O_BOTTOM_LEFT_BACK );
	INSERT( O_BOTTOM_RIGHT_FRONT );
	INSERT( O_BOTTOM_RIGHT_BACK );
      }
    }
  }
  // return number of currently visible objects.
  return list.size();
}
////////////////////////////////////////////////////////////////////////////////
template<class GameObjPtrContainer>
size_t
Phoenix::Scene::CSpatialGraph::CollectObjects( const Phoenix::Math::CRay &ray, GameObjPtrContainer & list, Phoenix::Core::TAG tag, Phoenix::Core::CTagged::TagCompare compare ) const
{
  using namespace Phoenix::Spatial;
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
	INSERT( O_TOP_LEFT_FRONT );
	INSERT( O_TOP_LEFT_BACK );
	INSERT( O_TOP_RIGHT_FRONT );
	INSERT( O_TOP_RIGHT_BACK );
	INSERT( O_BOTTOM_LEFT_FRONT );
	INSERT( O_BOTTOM_LEFT_BACK );
	INSERT( O_BOTTOM_RIGHT_FRONT );
	INSERT( O_BOTTOM_RIGHT_BACK );
      }
    }
  }
  // return number of currently visible objects.
  return list.size();
}
#undef INSERT


#endif
