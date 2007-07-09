#ifndef __PhoenixAVLTree_h__
#define __PhoenixAVLTree_h__
#include <stdlib.h>
#include <iostream>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
    namespace AVLTree
    {
      using namespace Phoenix::Core::AVLTree;
      enum NODE_TYPE { NODE_ROOT = 0, NODE_RIGHT_CHILD, NODE_LEFT_CHILD };
      template< typename Value > 
      class KeyValue
      {
      private:
	int m_iKey;
	Value m_Value;
      public:
	////////////////////
	/// Constructor.
	KeyValue( int iKey, Value kValue ) : m_iKey(iKey), m_Value(kValue) {}
	////////////////////
	/// Returns key.
	/// \return key value.
	inline int GetKey() const { return m_iKey; }
	////////////////////
	/// Sets key.
	/// \param iKey Key value.
	inline void SetKey( int iKey ) { m_iKey = iKey; }
	////////////////////
	/// Returns value.
	/// \returns Value object.
	inline Value & GetValue() { return m_Value; } 
	////////////////////
	/// Assigns value.
	/// \param value Value object.
	inline void SetValue( const Value &value ) { m_Value = value; }
	////////////////////
	/// Comparison operator.
	/// \param tKeyValue KeyValue compared against.
	/// \returns boolean true if this is smaller than other.
	inline bool operator<(const KeyValue &tKeyValue ) const
	{
	  return GetKey() < tKeyValue.GetKey();
	}
	////////////////////
	/// Comparison operator.
	/// \param tKeyValue KeyValue compared against.
	/// \returns boolean true if this is greater than other.
	inline bool operator>(const KeyValue &tKeyValue ) const
	{
	  return GetKey() > tKeyValue.GetKey();
	}
	////////////////////
	/// Comparison operator.
	/// \param tKeyValue KeyValue compared against.
	/// \returns boolean true if this is greater than other.
	inline bool operator==(const KeyValue &tKeyValue ) const
	{
	  return GetKey() == tKeyValue.GetKey();
	}
      };
      template <class KeyValue > class CAVLTree;
      ////////////////////
      /// Binary tree data structure.
      template< class KeyValue >
      class CAVLTreeNode
      {
      protected:
	/// Left child.
	CAVLTreeNode *m_pLeftChild; 
	/// Right child.
	CAVLTreeNode *m_pRightChild;
	/// Parent node.
	CAVLTreeNode *m_pParent;
	/// Node data with key.
	KeyValue m_KeyValue;
	/// Height of this subtree
	int  m_iHeight;
	/// balance of this node.
	int  m_iBalance;
	CAVLTree<KeyValue> *m_pTree;

      public:
	////////////////////
	/// Constructor.
	CAVLTreeNode( KeyValue value, CAVLTree<KeyValue> *pTree ) : m_pLeftChild(NULL), 
	m_pRightChild(NULL),
	m_pParent(NULL),
	m_KeyValue(value),
	m_iHeight(1),
	m_iBalance(0),
	m_pTree(pTree) { }
	////////////////////
	/// For retrieving the value.
	/// \return Value in this node.
	inline KeyValue & GetKeyValue() 
	{
	  return m_KeyValue;
	}
	////////////////////
	/// For setting the value.
	/// \param value A value to be set.
	inline void SetKeyValue( KeyValue &value )
	{
	  m_KeyValue = value;
	}
	////////////////////
	/// Returns right child.
	/// \returns A pointer to right child.
	inline CAVLTreeNode * GetRightChild() const
	{
	  return m_pRightChild;
	}
	////////////////////
	/// Returns left child.
	/// \returns A pointer to left child.
	inline CAVLTreeNode * GetLeftChild() const
	{
	  return m_pLeftChild;
	}
	////////////////////
	/// Assigns right child.
	/// \param pRightChild New right child node.
	inline void SetRightChild( CAVLTreeNode *pRightChild) 
	{
	  m_pRightChild = pRightChild;
	}
	////////////////////
	/// Assigns right child.
	/// \param pLeftChild New right child node.
	inline void SetLeftChild( CAVLTreeNode *pLeftChild) 
	{
	  m_pLeftChild = pLeftChild;
	}
	////////////////////
	/// Returns parent.
	/// \returns parent node.
	inline CAVLTreeNode * GetParent()  const
	{
	  return m_pParent;
	}
	////////////////////
	/// Assigns parent.
	/// \param pNode Node to be assigned as parent.
	inline void SetParent( CAVLTreeNode *pNode ) 
	{
	  m_pParent = pNode;
	}
	////////////////////
	/// Returns height.
	/// \returns height of tree.
	inline int GetHeight() const
	{
	  return m_iHeight;
	}
	////////////////////
	/// Returns height.
	/// \returns height of tree.
	inline void SetHeight( int iHeight) 
	{
	  m_iHeight = iHeight;
	}
	////////////////////
	/// Sets balance
	/// \param iBalance new balance.
	inline void SetBalance( int iBalance )
	{
	  m_iBalance = iBalance;
	}
	////////////////////
	/// Returns balance.
	/// \returns balance of node.
	inline int GetBalance() const
	{
	  return m_iBalance;
	}
	////////////////////
	void RotateRight()
	{
	  NODE_TYPE kType              = ClassifyNode();
	  CAVLTreeNode *pParentOfThis  = GetParent();
	  CAVLTreeNode *pNewRoot       = GetLeftChild();
	  CAVLTreeNode *pNewLeftChild  = GetLeftChild()->GetRightChild();
	  
	  SetLeftChild( pNewLeftChild );
	  if ( pNewLeftChild != NULL )
	    pNewLeftChild->SetParent(this);

	  pNewRoot->SetRightChild( this );
	  this->SetParent( pNewRoot );
	  switch( kType )
	  {
	  case NODE_ROOT:
	    GetTree()->SetRoot( pNewRoot );	           
	    break;
	  case NODE_RIGHT_CHILD:
	    pParentOfThis->SetRightChild( pNewRoot );
	    break;
	  case NODE_LEFT_CHILD:
	    pParentOfThis->SetLeftChild( pNewRoot ); 
	    break;
	  }
	  SetBalance(0);
	  pNewRoot->SetBalance(0);

	  CalculateHeight();
	  pNewRoot->CalculateHeight();
	  
	}
	////////////////////
	void RotateLeft()
	{
	  NODE_TYPE kType = ClassifyNode();
	  CAVLTreeNode *pParentOfThis  = GetParent();
	  CAVLTreeNode *pNewRoot       = GetRightChild();
	  CAVLTreeNode *pNewRightChild = GetRightChild()->GetLeftChild();
	  
	  SetRightChild( pNewRightChild );
	  if ( pNewRightChild != NULL )
	    pNewRightChild->SetParent(this);

	  pNewRoot->SetLeftChild( this );
	  this->SetParent(pNewRoot);
	  switch( kType )
	  {
	  case NODE_ROOT:
	    GetTree()->SetRoot( pNewRoot ); 
	    pNewRoot->SetParent(NULL); 
	    break;
	  case NODE_LEFT_CHILD:
	    pParentOfThis->SetLeftChild( pNewRoot ); 
	    pNewRoot->SetParent(pParentOfThis); 
	    break;
	  case NODE_RIGHT_CHILD:
	    pParentOfThis->SetRightChild( pNewRoot ); 
	    pNewRoot->SetParent(pParentOfThis); 
	    break;
	  }
	  
	  SetBalance(0);
	  pNewRoot->SetBalance(0);
	  
	  CalculateHeight();
	  pNewRoot->CalculateHeight();
	}
	////////////////////
	void DoubleRotateRight()
	{
	  NODE_TYPE     kType	       = ClassifyNode();
	  CAVLTreeNode *pNewRoot       = GetLeftChild()->GetRightChild();
	  CAVLTreeNode *pNewLeftChild  = GetLeftChild();
	  CAVLTreeNode *pParentOfThis  = GetParent();
	  
	  pNewLeftChild->SetRightChild( pNewRoot->GetLeftChild() );
	  pNewRoot->GetLeftChild()->SetParent( pNewLeftChild );
	  
	  this->SetLeftChild( pNewRoot->GetRightChild() );
	  pNewRoot->GetRightChild()->SetParent( this );
	  
	  pNewRoot->SetRightChild( this );
	  this->SetParent( pNewRoot );

	  pNewRoot->SetLeftChild(pNewLeftChild);
	  pNewLeftChild->SetParent(pNewRoot);
	  switch ( kType )
	  {
	  case NODE_ROOT:
	    GetTree()->SetRoot( pNewRoot );
	    pNewRoot->SetParent(NULL);
	    break;
	  case NODE_LEFT_CHILD:
	    pParentOfThis->SetLeftChild( pNewRoot );
	    pNewRoot->SetParent( pParentOfThis );
	    break;
	  case NODE_RIGHT_CHILD:
	    pParentOfThis->SetRightChild( pNewRoot );
	    pNewRoot->SetParent( pParentOfThis );
	    break;
	  }
	  pNewRoot->CalculateHeight();
	  CalculateHeight();
	  if ( pNewLeftChild != NULL )
	    pNewLeftChild->CalculateHeight();
	}
	////////////////////
	void DoubleRotateLeft()
	{
	  NODE_TYPE     kType	       = ClassifyNode();
	  CAVLTreeNode *pNewRoot       = GetRightChild()->GetLeftChild();
	  CAVLTreeNode *pNewRightChild = GetRightChild();

	  CAVLTreeNode *pParentOfThis  = GetParent();
	  
	  pNewRightChild->SetLeftChild( pNewRoot->GetRightChild() );
	  pNewRoot->GetRightChild()->SetParent( pNewRightChild );
	  
	  this->SetRightChild( pNewRoot->GetLeftChild() );
	  pNewRoot->GetLeftChild()->SetParent( this );
	  
	  pNewRoot->SetLeftChild( this );
	  this->SetParent( pNewRoot );

	  pNewRoot->SetRightChild(pNewRightChild);
	  pNewRightChild->SetParent(pNewRoot);
	  switch ( kType )
	  {
	  case NODE_ROOT:
	    GetTree()->SetRoot( pNewRoot );
	    pNewRoot->SetParent(NULL);
	    break;
	  case NODE_LEFT_CHILD:
	    pParentOfThis->SetLeftChild( pNewRoot );
	    pNewRoot->SetParent( pParentOfThis );
	    break;
	  case NODE_RIGHT_CHILD:
	    pParentOfThis->SetRightChild( pNewRoot );
	    pNewRoot->SetParent( pParentOfThis );
	    break;
	  }
	  pNewRoot->CalculateHeight();
	  CalculateHeight();
	  if ( pNewRightChild != NULL )
	    pNewRightChild->CalculateHeight();
	}

	void CalculateHeight()
	{
	  
	  CAVLTreeNode  *pLeft = GetLeftChild();
	  CAVLTreeNode  *pRight = GetRightChild();

	  if ( pLeft == NULL && pRight == NULL ) // no children at all
	  {
	    SetHeight(1);
	    SetBalance(0);
	  }
	  else if ( pLeft == NULL )  // only right
	  {
	    SetHeight( 1 + pRight->GetHeight() );
	    SetBalance( pRight->GetHeight());
	  }
	  else if ( pRight == NULL ) // only left
	  {
	    SetHeight( 1 + pLeft->GetHeight() );
	    SetBalance( -pLeft->GetHeight());
	  }
	  else // both child exists
	  {
	    if ( pLeft->GetHeight() > pRight->GetHeight() ) SetHeight( 1 + pLeft->GetHeight());
	    else					    SetHeight( 1 + pRight->GetHeight());
	    
	    SetBalance( pRight->GetHeight() - pLeft->GetHeight() );
	  }
	}
	////////////////////
	void PropagateHeight()
	{
	  CAVLTreeNode *pNode = this->GetParent();
	  CAVLTreeNode *pLeft = NULL;
	  CAVLTreeNode *pRight = NULL;
	  ////////////////////
	  // Do Until we reach root ( no parent node )
	  while ( pNode->GetParent() != NULL )
	  {
	    pLeft = pNode->GetLeftChild();
	    pRight = pNode->GetRightChild();
	    pNode->CalculateHeight();

	    if ( pNode->GetBalance() == 2 )
	    {
	      if ( pNode->GetRightChild()->GetBalance() == 1 )
	      {
		pNode->RotateLeft();
	      }
	      else
	      {
		pNode->DoubleRotateLeft();
	      }
	    }
	    if ( pNode->GetBalance() == -2 )
	    {
	      if ( pNode->GetLeftChild()->GetBalance() == -1 )
	      {
		pNode->RotateRight();
	      }
	      else
	      {
		pNode->DoubleRotateRight();
	      }
	    }
	    // Go up towards root
	    pNode = pNode->GetParent();
	  }
	  ////////////////////
	  // Update also root node height.
	  pNode->CalculateHeight();
	}
	////////////////////
	/// Returns NODE_TYPE in respect to parent.
	/// \returns NODE_ROOT, if it has no parent,
	///	     NODE_LEFT_CHILD, if it is the left child.
	///          NODE_RIGHT_CHILD, it it is the right child.
	inline NODE_TYPE ClassifyNode() const
	{
	  if ( GetParent() == NULL )		     
	    return NODE_ROOT;
	  if ( GetParent()->GetLeftChild() == this ) 
	    return NODE_LEFT_CHILD;
	  else
	    return NODE_RIGHT_CHILD;
	}
	////////////////////
	inline CAVLTree<KeyValue> * GetTree() 
	{
	  return m_pTree;
	}
      }; // CBinTree
      
      template< class KeyValue >
      class CAVLTree
      {
      protected:
	CAVLTreeNode<KeyValue> *m_pRoot;
      public:
	////////////////////
	/// Constructor.
	CAVLTree() : m_pRoot(NULL)
	{
	}
	////////////////////
	/// Sets root node.
	/// \param pNode Node to be set as root.
	inline void SetRoot( CAVLTreeNode<KeyValue> *pNode )
	{
	  m_pRoot = pNode;
	}
	////////////////////
	/// Returns root node.
	/// \returns Pointer to root node .
	inline CAVLTreeNode<KeyValue> *GetRoot() 
	{
	  return m_pRoot;
	}
	////////////////////
	/// Inserts into binary tree.
	/// \param tKeyValue Value to be inserted.
	int Insert( const KeyValue & tKeyValue)
	{
	  CAVLTreeNode<KeyValue> *pTemp = GetRoot();
	  int bFound = 0;
	  if ( pTemp == NULL )
	  {
	    CAVLTreeNode<KeyValue> *pNew = new CAVLTreeNode<KeyValue>( tKeyValue, this );
	    SetRoot( pNew );
	    return 0;
	  }
	  while ( !bFound )
	  {
	    if ( tKeyValue > pTemp->GetKeyValue() )
	    {
	      if ( pTemp->GetRightChild() != NULL )
	      {
		pTemp=pTemp->GetRightChild();
		continue;
	      }
	      else
	      {
		CAVLTreeNode<KeyValue> *pNew = new CAVLTreeNode<KeyValue>( tKeyValue, this );
		pTemp->SetRightChild( pNew );
		pNew->SetParent(pTemp);
		pNew->PropagateHeight();
		return 0;
	      }
	    }
	    else if ( tKeyValue < pTemp->GetKeyValue() )
	    {
	      if ( pTemp->GetLeftChild() != NULL )
	      {
		pTemp=pTemp->GetLeftChild();
		continue;
	      }
	      else
	      {
		CAVLTreeNode<KeyValue> *pNew = new CAVLTreeNode<KeyValue>( tKeyValue,  this );
		pTemp->SetLeftChild( pNew );
		pNew->SetParent(pTemp);
		pNew->PropagateHeight();
		return 0;
	      }
	    }
 
	  } // while 
	  return -1;
	} // Insert( ...
	////////////////////
	/// Seeks key from binary tree.
	/// \param iKey Key to seek.
	/// \returns KeyValue object.
	KeyValue * Find( int iKey )
	{
	  if ( GetRoot() == NULL ) return NULL;

	  CAVLTreeNode<KeyValue> *pTemp = GetRoot();
	  int bFound = 0;
	  while ( !bFound )
	  {
	    if ( iKey < pTemp->GetKeyValue().GetKey() )
	    {
	      pTemp = pTemp->GetLeftChild();
	    }
	    else if ( iKey > pTemp->GetKeyValue().GetKey() )
	    {
	      pTemp = pTemp->GetRightChild();
	    }
	    else return &pTemp->GetKeyValue();
	  } // while
	  return NULL;
	} // Find( ...
	
	////////////////////
	/// Travels through tree in Pre-Order.
	template< class Handler >
	void ProcessPreOrder( Handler &handler, CAVLTreeNode<KeyValue> *pNode ) 
	{
	  handler.Handle( *pNode );
	  if ( pNode->GetLeftChild() != NULL) ProcessPreOrder( handler, pNode->GetLeftChild());
	  if ( pNode->GetRightChild() != NULL ) ProcessPreOrder( handler, pNode->GetRightChild());
	}
	////////////////////
	/// Travels through tree in In-Order.
	template< class Handler >
	void ProcessInOrder( Handler &handler, CAVLTreeNode<KeyValue> *pNode ) 
	{
	  if ( pNode->GetLeftChild() != NULL) ProcessInOrder(handler, pNode->GetLeftChild());
	  handler.Handle( *pNode );
	  if ( pNode->GetRightChild() != NULL ) ProcessInOrder(handler, pNode->GetRightChild());
	}
	////////////////////
	/// Travels through tree in Post-Order.
	template< class Handler >
	void ProcessPostOrder( Handler &handler, CAVLTreeNode<KeyValue> *pNode ) 
	{
	  if ( pNode->GetLeftChild() != NULL) ProcessPostOrder( handler, pNode->GetLeftChild());
	  if ( pNode->GetRightChild() != NULL ) ProcessPostOrder( handler, pNode->GetRightChild());
	  handler.Handle( *pNode );
	}
      }; // CAVLTree
    }; // namespace AvlTree
  }; // namespace Core
};// namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
