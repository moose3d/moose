#include "PhoenixTransformGraph.h"
///////////////////////////////////////////////////////////////////////////////
namespace prefix = Phoenix::Scene;
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
  //Phoenix::Core::TravelDF<CTransformUpdater>( static_cast<CGraphNode *>(GetRoot()), &m_TransformUpdater );
  TravelDF( GetRoot() );
}
///////////////////////////////////////////////////////////////////////////////
prefix::CTransformable::CTransformable() : m_pTransformNode(NULL)
{
  SetChanged(true);
}
/////////////////////////////////////////////////////////////////
Phoenix::Math::CTransform &
prefix::CTransformable::GetLocalTransform()
{
  return m_LocalTransform;
}
/////////////////////////////////////////////////////////////////
const Phoenix::Math::CTransform &
prefix::CTransformable::GetLocalTransform() const
{
  return m_LocalTransform;
}
/////////////////////////////////////////////////////////////////
Phoenix::Math::CTransform &
prefix::CTransformable::GetWorldTransform()
{
  return m_WorldTransform;
}
/////////////////////////////////////////////////////////////////
const Phoenix::Math::CTransform &
prefix::CTransformable::GetWorldTransform() const
{
  return m_WorldTransform;
}
/////////////////////////////////////////////////////////////////
void
prefix::CTransformable::SetLocalTransform( const Phoenix::Math::CTransform & rTransform )
{
  m_LocalTransform = rTransform;
  SetChanged(true);
}
/////////////////////////////////////////////////////////////////
void
prefix::CTransformable::SetWorldTransform( const Phoenix::Math::CTransform & rTransform )
{
  m_WorldTransform = rTransform;
  SetChanged(true);
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Math::CTransform::IsChanged() const
{
  return m_bChanged;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::CTransform::SetChanged(bool bFlag )
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
prefix::CTransformable::SetTransformNode( Phoenix::Scene::CTransformNode *pNode)
{
	m_pTransformNode = pNode;
}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CTransformNode *
prefix::CTransformable::GetTransformNode()
{
	return m_pTransformNode;
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CTransformable::Reparent( CTransformable *pNewParent )
{
	assert( pNewParent->GetTransformNode() != NULL && "Reparent: Transformable must have a TransformNode!" );
	assert( GetTransformNode() != NULL && "Reparent: current node must have TransformNode!");
	GetTransformNode()->Reparent( pNewParent->GetTransformNode() );
}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Core::TGraphEdge<Phoenix::Scene::CTransformNode> *
prefix::CTransformNode::AddEdge( CTransformNode *pTo )
{
	return m_pGraph->AddEdge( this, pTo );
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CTransformNode::Reparent( CTransformNode * pNewParent )
{

	CTransformNode *pCurrentParent = GetArrivingEdges().front()->GetFromNode();
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
	Phoenix::Scene::CTransformable *pThis   = GetTransformable();
	Phoenix::Scene::CTransformable *pParent = NULL;

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
	Phoenix::Scene::CTransformable *pTmp = GetTransformable();
	if( pTmp != NULL ) pTmp->SetChanged(false);
}
///////////////////////////////////////////////////////////////////////////////
