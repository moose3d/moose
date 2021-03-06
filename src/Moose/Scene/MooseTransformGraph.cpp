/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "MooseTransformGraph.h"
#include "MooseGameObject.h"
///////////////////////////////////////////////////////////////////////////////
namespace prefix = Moose::Scene;
///////////////////////////////////////////////////////////////////////////////
prefix::CTransformGraph::CTransformGraph()
{
  m_pRoot = CreateNode< CPlainTransform >();
}
///////////////////////////////////////////////////////////////////////////////
prefix::CTransformGraph::~CTransformGraph() { }
///////////////////////////////////////////////////////////////////////////////
prefix::CTransformNode *
prefix::CTransformGraph::GetRoot()
{
  return m_pRoot;
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CTransformGraph::Update()
{
  //Moose::Core::TravelDF<CTransformUpdater>( static_cast<CGraphNode *>(GetRoot()), &m_TransformUpdater );
  TravelDF( GetRoot() );
}
///////////////////////////////////////////////////////////////////////////////
prefix::CTransformable::CTransformable() : m_pTransformNode(NULL)
{
  SetChanged(true);
}
/////////////////////////////////////////////////////////////////
prefix::CTransformable::~CTransformable()
{

}
/////////////////////////////////////////////////////////////////
Moose::Math::CTransform &
prefix::CTransformable::GetLocalTransform()
{
  return m_LocalTransform;
}
/////////////////////////////////////////////////////////////////
const Moose::Math::CTransform &
prefix::CTransformable::GetLocalTransform() const
{
  return m_LocalTransform;
}
/////////////////////////////////////////////////////////////////
Moose::Math::CTransform &
prefix::CTransformable::GetWorldTransform()
{
  return m_WorldTransform;
}
/////////////////////////////////////////////////////////////////
const Moose::Math::CTransform &
prefix::CTransformable::GetWorldTransform() const
{
  return m_WorldTransform;
}
/////////////////////////////////////////////////////////////////
void
prefix::CTransformable::SetLocalTransform( const Moose::Math::CTransform & rTransform )
{
  m_LocalTransform = rTransform;
  SetChanged(true);
}
/////////////////////////////////////////////////////////////////
void
prefix::CTransformable::SetWorldTransform( const Moose::Math::CTransform & rTransform )
{
  m_WorldTransform = rTransform;
  SetChanged(true);
}
/////////////////////////////////////////////////////////////////
bool
Moose::Math::CTransform::IsChanged() const
{
  return m_bChanged;
}
/////////////////////////////////////////////////////////////////
void
Moose::Math::CTransform::SetChanged(bool bFlag )
{
  m_bChanged = bFlag;
}
/////////////////////////////////////////////////////////////////
bool
prefix::CTransformable::IsChanged() const
{
  return m_bChanged;
}
/////////////////////////////////////////////////////////////////
void
prefix::CTransformable::SetChanged(bool bFlag )
{
  m_bChanged = bFlag;
  if ( !m_bChanged ) PostTransformUpdate();
}
/////////////////////////////////////////////////////////////////
void
prefix::CTransformable::PostTransformUpdate()
{
	// NOP, override in appropriate classes. (mainly in CGameObject)
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CTransformable::SetTransformNode( Moose::Scene::CTransformNode *pNode)
{
	m_pTransformNode = pNode;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CTransformNode *
prefix::CTransformable::GetTransformNode()
{
	return m_pTransformNode;
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CTransformable::Reparent( CTransformable *pNewParent )
{
    if ( pNewParent == NULL )
    {
        // Assigns root of transform graph as parent in this case.
        GetTransformNode()->Reparent( NULL );
        GetLocalTransform().SetChanged(true);
    }
    else
    {
        assert( pNewParent->GetTransformNode() != NULL && "Reparent: new parent must have a TransformNode!" );
        assert( GetTransformNode() != NULL && "Reparent: current node must have TransformNode!");
        GetTransformNode()->Reparent( pNewParent->GetTransformNode() );
        // So changes will be reflected on next Update().
        GetLocalTransform().SetChanged(true);
   }
}
///////////////////////////////////////////////////////////////////////////////
prefix::CTransformNode::~CTransformNode()
{
	TGraphNodeDestroy();
}
///////////////////////////////////////////////////////////////////////////////
Moose::Core::TGraphEdge<Moose::Scene::CTransformNode> *
prefix::CTransformNode::AddEdge( CTransformNode *pTo )
{
	return m_pGraph->AddEdge( this, pTo );
}
///////////////////////////////////////////////////////////////////////////////
prefix::CTransformNode *
prefix::CTransformNode::GetParentTransformNode()
{
    if ( GetArrivingEdges().empty() ) return NULL;
	// Returns first (hopefully the ONLY) node where edge is leading to this.
	return GetArrivingEdges().front()->GetFromNode();
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CTransformNode::Reparent( CTransformNode * pNewParent )
{
    if ( pNewParent == NULL )
    {
        pNewParent = dynamic_cast<CTransformGraph *>(this->m_pGraph)->GetRoot();
    }
	//CTransformNode *pCurrentParent = GetParentTransformNode();
	// Makes node parentless
	RemoveArrivingEdges();
	assert( GetArrivingEdges().empty() && "Not empty!");
	// Remove possibly existing edges between new parent and this
	pNewParent->DeleteAllEdgesTo(this);
	// Add edges between new parent and this
	pNewParent->AddEdge( this );
	assert( GetArrivingEdges().size() == 1 );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CTransformNode::Enter()
{
	Moose::Scene::CTransformable *pThis   = GetTransformable();
	Moose::Scene::CTransformable *pParent = NULL;

	if ( pThis == NULL)
	{
		std::cerr << "Transformable == NULL" << std::endl;
		return 0;
	}

	if ( HasArrivingEdges() )
	{
		pParent = GetArrivingEdges().front()->GetFromNode()->GetTransformable();

		// If there is no ship, skip handling and use local transform.
		if ( pParent == NULL )
		{
			if ( pThis->IsChanged() )
				pThis->SetWorldTransform( pThis->GetLocalTransform());
			return 0;
		}

		if ( pParent->IsChanged() || pThis->IsChanged() )
		{
			pThis->SetChanged(true);
			Multiply( pParent->GetWorldTransform(), pThis->GetLocalTransform(), pThis->GetWorldTransform() );
		}
	}
	else
	{
		if ( pThis->IsChanged() ) pThis->SetWorldTransform( pThis->GetLocalTransform());
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CTransformNode::Leave()
{
	Moose::Scene::CTransformable *pTmp = GetTransformable();
	if( pTmp != NULL ) pTmp->SetChanged(false);
}
///////////////////////////////////////////////////////////////////////////////
prefix::CObjectTransform *
prefix::CTransformGraph::Insert( CGameObject * pObject )
{

	CObjectTransform *pTR = new CObjectTransform();
	RegisterNode(pTR);
	pObject->SetTransformNode(pTR);
	assert ( !pObject->GetObjectHandle().IsNull() &&
			"Object handle is null, this makes adding to transform graph"
			"via HANDLE pretty difficult. Attach handle to object via "
			"ResourceManager after creation immediately "
			"ResourceManager::Create( Object, name, HANDLE)");
	pTR->GetObjectHandle() = pObject->GetObjectHandle();
	GetRoot()->AddEdge( pTR );
	return pTR;
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CTransformGraph::RemoveSubtree( CTransformable *pTransformable )
{
	TransformableList lstObjects;
	CollectSubtree( pTransformable, lstObjects );
	TransformableList::iterator it = lstObjects.begin();

	while( lstObjects.empty() == false )
	{
		CTransformable *pTmp = lstObjects.front();
		lstObjects.pop_front();
		delete pTmp->GetTransformNode();
		delete pTmp;
	}
}
///////////////////////////////////////////////////////////////////////////////////
void
prefix::CTransformGraph::CollectSubtree( CTransformable *pSubRoot, TransformableList & subtree )
{
	subtree.clear(); // remove previous, if any

	TransformableList tmpList;
	tmpList.push_back( pSubRoot );
	while( tmpList.empty() == false)
	{
		CTransformable *pTr = tmpList.front();
		tmpList.pop_front();
		// add node to subtree node list
		subtree.push_back( pTr );

		std::list< Moose::Core::TGraphEdge<CTransformNode> * >::iterator it;

		for( it = pTr->GetTransformNode()->GetLeavingEdges().begin();
				 it != pTr->GetTransformNode()->GetLeavingEdges().end();
				 ++it)
		{
			tmpList.push_back( (*it)->GetToNode()->GetTransformable() );
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CTransformable *
prefix::CObjectTransform::GetTransformable()
{
	return *(this->GetObjectHandle());
}
///////////////////////////////////////////////////////////////////////////////
