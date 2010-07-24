#ifndef __MooseSpatialGraph_h__
#define __MooseSpatialGraph_h__
/////////////////////////////////////////////////////////////////
//#include "MooseGameObject.h"
#include "MooseGameObject.h"
#include "MooseOctree.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Scene
  {
    /////////////////////////////////////////////////////////////////
    /// Structure for storing objects and retrieving them according
    /// to intersection volume.
    class CSpatialGraph : public Moose::Spatial::COctree<Moose::Scene::CGameObject *>
    {
    public:
      ////////////////////
      /// Constructs spatial structure using an octree.
      /// \param nNumLevels Depth of octree.
      /// \param fWorldSize Maximum size of world cube side.
      CSpatialGraph( unsigned int nNumLevels, float fWorldSize ) : Moose::Spatial::COctree<Moose::Scene::CGameObject *>(nNumLevels,fWorldSize){}
      ////////////////////
      /// Inserts object into spatial graph according to world position and bounding sphere size.
      /// \param pGameObject Object to be inserted.
      void InsertObject( Moose::Scene::CGameObject *pGameObject )
      {
        assert(pGameObject!=NULL);

        unsigned int nSpatialIndex = Moose::Spatial::COctree<CGameObject *>::InsertObject(
                                                                                            pGameObject, pGameObject->GetWorldBoundingSphere());
        assert( nSpatialIndex < this->GetNodeCount() );
        pGameObject->SetSpatialIndex( nSpatialIndex );

      }
      ////////////////////
      /// Removes object from spatial graph.
      /// \param pGameObject Object to be removed.
      void DeleteObject( CGameObject *pGameObject )
      {
        Moose::Spatial::COctreeNode<CGameObject *> *pNode = this->GetNodeAt( pGameObject->GetSpatialIndex());
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
        Moose::Math::CVector3<float> vTmp = pGameObject->GetWorldTransform().GetTranslation();
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
      size_t CollectObjects( const Moose::Graphics::CCamera &camera, GameObjPtrContainer & list, Moose::Core::TAG tag = 0, Moose::Core::TagCompare compare = Moose::Core::NOT_USED ) const;
      ////////////////////
      /// Collects objects from Octree into list of objects.
      /// \param frustum Frustumused in culling.
      /// \param list List where objects are inserted.
      /// \param tag What TAG is used in comparison.
      /// \param compare How TAG value is used in comparison. By default, it is not used.
      /// \returns Number of collected objects.
      template<class GameObjPtrContainer>
      size_t CollectObjects( const Moose::Graphics::CFrustum &frustum, GameObjPtrContainer & list, Moose::Core::TAG tag = 0, Moose::Core::TagCompare compare = Moose::Core::NOT_USED ) const;
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
      size_t CollectObjects( const Moose::Volume::CSphere &sphere, GameObjPtrContainer & list, Moose::Core::TAG tag = 0, Moose::Core::TagCompare compare = Moose::Core::NOT_USED ) const;
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
      size_t CollectObjects( const Moose::Math::CRay &ray, GameObjPtrContainer & list, Moose::Core::TAG tag = 0, Moose::Core::TagCompare compare = Moose::Core::NOT_USED ) const;

    };
  } // Scene
}// Moose
////////////////////////////////////////////////////////////////////////////////
#define INSERT( OBJ ) { \
  if ( pNode->GetChild( OBJ ) != NULL ) \
    lstNodePtrs.push_back(pNode->GetChild( OBJ ));\
}
/////////////////////////////////////////////////////////////////
inline bool CheckTagMatch( Moose::Core::TAG tagObj, Moose::Core::TAG tag, Moose::Core::TagCompare compare )
{
	using namespace Moose::Core;
	switch ( compare )
	{
	case NOT_USED:
		break;
	case EQUAL:
		if ( tagObj != tag ) return false;
		break;
	case NOT_EQUAL:
		if ( tagObj == tag ) return false;
		break;
	case AND:
		if ( !(tagObj & tag) ) return false;
		break;
	case NOT_AND:
		if ( (tagObj & tag) ) return false;
		break;
	}
	// if we get here, tag is matching.
	return true;
}
/////////////////////////////////////////////////////////////////
template<class GameObjPtrContainer>
size_t
Moose::Scene::CSpatialGraph::CollectObjects( const Moose::Graphics::CFrustum &frustum, GameObjPtrContainer & list, Moose::Core::TAG tag, Moose::Core::TagCompare compare ) const
{
  using namespace Moose::Spatial;
	std::list< const Moose::Spatial::COctreeNode<CGameObject *> *> lstNodePtrs;
	lstNodePtrs.push_back(GetRoot());

	const Moose::Spatial::COctreeNode<CGameObject *> *pNode = NULL;
	Moose::Volume::CSphere sphere;
	std::list<CGameObject *>::const_iterator it;

	while(!lstNodePtrs.empty())
	{
		// Pop first node from list
		pNode = lstNodePtrs.front();
		lstNodePtrs.pop_front();

		// Check does cube intersect frustum
		if ( Moose::Collision::AABBIntersectsPolytope( *pNode, frustum) )
		{
			// Check do objects intersect frustum and if so,
			// insert them into list
			it = pNode->GetObjects().begin();
			for( ; it!=pNode->GetObjects().end();it++)
			{
				if ( ! CheckTagMatch( (*it)->GetTag(), tag, compare) ) continue;

				// Then compare bounding volumes
				sphere = (*it)->GetWorldBoundingSphere();

				if ( Moose::Collision::SphereIntersectsPolytope( sphere , frustum ))
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
Moose::Scene::CSpatialGraph::CollectObjects( const Moose::Graphics::CCamera &camera, GameObjPtrContainer & list, Moose::Core::TAG tag, Moose::Core::TagCompare compare ) const
{
  using namespace Moose::Spatial;
	// TODO place for optimization; create cache for octree nodes ie. Prefetch( camera ) etc. Then use prefetch values in consequent queries.
  std::list< const Moose::Spatial::COctreeNode<CGameObject *> *> lstNodePtrs;
  lstNodePtrs.push_back(GetRoot());

  const Moose::Spatial::COctreeNode<CGameObject *> *pNode = NULL;
  Moose::Volume::CSphere sphere;
  std::list<CGameObject *>::const_iterator it;

  while(!lstNodePtrs.empty())
  {
    // Pop first node from list
    pNode = lstNodePtrs.front();
    lstNodePtrs.pop_front();

    // Check does cube intersect frustum
    if ( Moose::Collision::AABBIntersectsPolytope( *pNode, camera.Frustum()) )
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

    	  if ( Moose::Collision::SphereIntersectsSphere( sphere, camera.FrustumSphere()) &&
    		   Moose::Collision::SphereIntersectsPolytope( sphere , camera.Frustum() ))
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
Moose::Scene::CSpatialGraph::CollectObjects( const Moose::Volume::CSphere &cullSphere, GameObjPtrContainer & list, Moose::Core::TAG tag, Moose::Core::TagCompare compare ) const
{
  using namespace Moose::Spatial;
  std::list< const Moose::Spatial::COctreeNode<CGameObject*> *> lstNodePtrs;
  lstNodePtrs.push_back(GetRoot());

  const Moose::Spatial::COctreeNode<CGameObject *> *pNode = NULL;
  Moose::Volume::CSphere sphere;
  std::list<CGameObject*>::const_iterator it;

  while(!lstNodePtrs.empty())
  {
    // Pop first node from list
    pNode = lstNodePtrs.front();
    lstNodePtrs.pop_front();

    // Check does cube intersect sphere
    if ( Moose::Collision::SphereIntersectsAACube(cullSphere, *pNode))
    {
      // Check do objects intersect sphere and if so,
      // insert them into list
      it = pNode->GetObjects().begin();
      for( ; it!=pNode->GetObjects().end();it++)
      {

    	  if ( ! CheckTagMatch( (*it)->GetTag(), tag, compare) ) continue;

    	  sphere = (*it)->GetBoundingSphere();
    	  sphere.Move( (*it)->GetWorldTransform().GetTranslation() );

    	  if ( Moose::Collision::SphereIntersectsSphere( sphere, cullSphere) )
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
Moose::Scene::CSpatialGraph::CollectObjects( const Moose::Math::CRay &ray, GameObjPtrContainer & list, Moose::Core::TAG tag, Moose::Core::TagCompare compare ) const
{
  using namespace Moose::Spatial;
  std::list< const Moose::Spatial::COctreeNode<CGameObject *> *> lstNodePtrs;
  lstNodePtrs.push_back(GetRoot());

  const Moose::Spatial::COctreeNode<CGameObject *> *pNode = NULL;
  std::list<CGameObject *>::const_iterator it;

  while(!lstNodePtrs.empty())
  {
    // Pop first node from list
    pNode = lstNodePtrs.front();
    lstNodePtrs.pop_front();

    // Check does cube intersect ray
    // Write collision check in Engine using AABB stuff.
    if ( Moose::Collision::RayIntersectsAACube( ray, *pNode, NULL ))
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
