#ifndef __PhoenixSpatialGraph_h__
#define __PhoenixSpatialGraph_h__
/////////////////////////////////////////////////////////////////
//#include "PhoenixGameObject.h"
#include "PhoenixGameObject.h"
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
		};
	} // Scene
}// Phoenix
/////////////////////////////////////////////////////////////////
#endif
