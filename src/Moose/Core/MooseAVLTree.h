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

#ifndef __MooseAVLTree_h__
#define __MooseAVLTree_h__
#include <stdlib.h>
#include <iostream>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
    namespace AVLTree
    {
      using namespace Moose::Core::AVLTree;
      /// Node classification values (in respect to parent node)
      enum NODE_TYPE { NODE_ROOT = 0, NODE_RIGHT_CHILD, NODE_LEFT_CHILD };
      ////////////////////
      /// Data container for each node.
      template< typename Value > 
      class MOOSE_API KeyValue
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
      /// A self-balancing binary tree data structure node (AVL-tree node).
      template< class KeyValue >
      class MOOSE_API CAVLTreeNode
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
	/// Destructor.
	~CAVLTreeNode() 
	{
	  DeleteThis();
	}
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
	/// Balancing method, performs rotation to right. 
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
	    pNewRoot->SetParent(NULL);
	    break;
	  case NODE_RIGHT_CHILD:
	    pParentOfThis->SetRightChild( pNewRoot );
	    pNewRoot->SetParent( pParentOfThis );
	    break;
	  case NODE_LEFT_CHILD:
	    pParentOfThis->SetLeftChild( pNewRoot ); 
	    pNewRoot->SetParent( pParentOfThis );
	    break;
	  }
	  SetBalance(0);
	  pNewRoot->SetBalance(0);

	  CalculateHeight();
	  pNewRoot->CalculateHeight();

	  
	}
	////////////////////
	/// Balancing method, performs rotation to left. 
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
	/// Balancing method, performs double rotation to right. 
	void DoubleRotateRight()
	{

	  NODE_TYPE     kType	       = ClassifyNode();
	  CAVLTreeNode *pNewRoot       = GetLeftChild()->GetRightChild();
	  CAVLTreeNode *pNewLeftChild  = GetLeftChild();
	  CAVLTreeNode *pParentOfThis  = GetParent();
	  
	  pNewLeftChild->SetRightChild( pNewRoot->GetLeftChild() );
	  if ( pNewRoot->GetLeftChild() != NULL ) 
	    pNewRoot->GetLeftChild()->SetParent( pNewLeftChild );
	  
	  this->SetLeftChild( pNewRoot->GetRightChild() );
	  if ( pNewRoot->GetRightChild() != NULL ) 
	  {
	    pNewRoot->GetRightChild()->SetParent( this );
	  }
	  
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
	  CalculateHeight();
	  if ( pNewLeftChild != NULL )
	    pNewLeftChild->CalculateHeight();
	  pNewRoot->CalculateHeight();

	}
	////////////////////
	/// Balancing method, performs double rotation to left. 
	void DoubleRotateLeft()
	{

	  NODE_TYPE     kType	       = ClassifyNode();
	  CAVLTreeNode *pNewRoot       = GetRightChild()->GetLeftChild();
	  CAVLTreeNode *pNewRightChild = GetRightChild();

	  CAVLTreeNode *pParentOfThis  = GetParent();
	  
	  pNewRightChild->SetLeftChild( pNewRoot->GetRightChild() );
	  if( pNewRoot->GetRightChild() != NULL ) 
	    pNewRoot->GetRightChild()->SetParent( pNewRightChild );
	  
	  this->SetRightChild( pNewRoot->GetLeftChild() );
	  if ( pNewRoot->GetLeftChild() != NULL ) 
	  {
	    pNewRoot->GetLeftChild()->SetParent( this );
	  }

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

	  CalculateHeight();
	  if ( pNewRightChild != NULL )
	    pNewRightChild->CalculateHeight();
	  pNewRoot->CalculateHeight();
	}
	////////////////////
	/// Calculates height for this node.
	/// height = max(height of left child, height of right child) + 1.
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
	/// Travels towards root node from this node, updates heights and balances.
	void PropagateHeight()
	{
	  
	  CAVLTreeNode *pNode = this->GetParent();
	  if ( pNode == NULL ) return;
	  ////////////////////
	  /// Do Until we reach root ( no parent node ).
	  while ( pNode->GetParent() != NULL )
	  {
	    pNode->CalculateHeight();
	    pNode->CheckBalance();
	    // Go up towards root
	    pNode = pNode->GetParent();
	  }
	  ////////////////////
	  /// Update also root node height.
	  pNode->CalculateHeight();
	  pNode->CheckBalance();
	  
	}
	////////////////////
	/// Checks for imbalanced nodes and corrects them.
	inline void CheckBalance()
	{
	  if ( GetBalance() == 2 )
	  {
	    if ( GetRightChild()->GetBalance() == 1 )
	    {
	      RotateLeft();
	    }
	    else
	    {
	      DoubleRotateLeft();
	    }
	  }
	  if ( GetBalance() == -2 )
	  {
	    if ( GetLeftChild()->GetBalance() == -1 )
	    {
	      RotateRight();
	    }
	    else
	    {
	      DoubleRotateRight();
	    }
	  }
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
	/// Returns tree that this node belongs to.
	/// \returns Pointer to tree.
	inline CAVLTree<KeyValue> * GetTree() 
	{
	  return m_pTree;
	}
      protected:
	////////////////////
	/// Updates tree so that removal of this node leaves correct order.
	void DeleteThis()
	{
	  // Lift largest of smallest children (LOS) of this node in place of this.
	  // The left child of LOS becomes the right child of this->leftChild.
	  // The right child of this becomes right child of LOS.
	  NODE_TYPE tType = ClassifyNode();
	  CAVLTreeNode *pLargestOfSmallest = SeekLargestOfSmallest();

	  // If there isn't smaller nodes
	  if ( pLargestOfSmallest == NULL )
	  {
	    switch( tType )
	    {
	    case NODE_ROOT:

	      GetTree()->SetRoot( GetRightChild() );
	      if ( GetRightChild() != NULL )
	      {
		GetRightChild()->SetParent(NULL);
	      }
	      /// in case the node was last, root has become NULL.
	      if ( GetTree()->GetRoot() != NULL )
	      {
		// Take care of balancing
		GetTree()->GetRoot()->CalculateHeight();
		GetTree()->GetRoot()->CheckBalance();
	      }
	      break;
	    case NODE_LEFT_CHILD:
	      GetParent()->SetLeftChild( GetRightChild() );
	      if ( GetRightChild() != NULL )
		GetRightChild()->SetParent( GetParent() );
	      // Take care of balancing
	      GetParent()->CalculateHeight();
	      GetParent()->CheckBalance();
	      GetParent()->PropagateHeight();
	      break;
	    case NODE_RIGHT_CHILD:

	      GetParent()->SetRightChild( GetRightChild() );
	      if ( GetRightChild() != NULL )
		GetRightChild()->SetParent( GetParent() );
	      // Take care of balancing
	      GetParent()->CalculateHeight();
	      GetParent()->CheckBalance();
	      GetParent()->PropagateHeight();
	      break;
	    }
	  }
	  else
	  {
	    // If LOS isn't the left child.
	    if ( GetLeftChild() != pLargestOfSmallest )
	    {
	      GetLeftChild()->SetRightChild( pLargestOfSmallest->GetLeftChild());
	      if ( pLargestOfSmallest->GetLeftChild() != NULL )
		pLargestOfSmallest->GetLeftChild()->SetParent( GetLeftChild() );
	      
	      pLargestOfSmallest->SetLeftChild( this->GetLeftChild() );
	      if( this->GetLeftChild() != NULL ) 
		this->GetLeftChild()->SetParent( pLargestOfSmallest );
	    }
	    // Right child of LOS will be this->rightchild
	    pLargestOfSmallest->SetRightChild( this->GetRightChild());
	    if ( this->GetRightChild() != NULL )
	      this->GetRightChild()->SetParent( pLargestOfSmallest);
	    
	    switch( tType )
	    {
	    case NODE_ROOT:
	      GetTree()->SetRoot( pLargestOfSmallest );
	      pLargestOfSmallest->SetParent( NULL );
	      break;
	    case NODE_LEFT_CHILD:
	      this->GetParent()->SetLeftChild( pLargestOfSmallest);
	      pLargestOfSmallest->SetParent( this->GetParent());
	      break;
	    case NODE_RIGHT_CHILD:
	      this->GetParent()->SetRightChild( pLargestOfSmallest);
	      pLargestOfSmallest->SetParent( this->GetParent());
	      break;
	    }
	    // Take care of balancing.
	    if ( pLargestOfSmallest->GetLeftChild() != NULL )
	    {
	      pLargestOfSmallest->GetLeftChild()->CalculateHeight();
	      pLargestOfSmallest->GetLeftChild()->CheckBalance();
	      pLargestOfSmallest->GetLeftChild()->PropagateHeight();
	    }
	    else
	    {
	      pLargestOfSmallest->CalculateHeight();
	      pLargestOfSmallest->CheckBalance();
	      pLargestOfSmallest->PropagateHeight();
	    }
	  }
	}
	////////////////////
	/// Seeks the largest of smallest child nodes.
	/// \returns Pointer to such node if one exists. Otherwise NULL.
	CAVLTreeNode * SeekLargestOfSmallest() const
	{
	  CAVLTreeNode *pTemp = GetLeftChild();
	  if ( pTemp == NULL ) return NULL;
	  while( pTemp->GetRightChild() != NULL ) 
	  {
	    pTemp = pTemp->GetRightChild();
	  }
	  return pTemp;
	}
      }; // CBinTree
      ////////////////////
      /// AVL-tree structure.
      template< class KeyValue >
      class MOOSE_API CAVLTree
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
	inline CAVLTreeNode<KeyValue> *GetRoot()  const
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
	    GetRoot()->CalculateHeight();
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
	    else
	    {
	      // Duplicate key found, Cannot insert.
	      bFound = 1;
	    }
 
	  } // while 
	  return -1;
	} // Insert( ...
	////////////////////
	/// Seeks key from binary tree.
	/// \param iKey Key to seek.
	/// \returns KeyValue object.
	inline KeyValue * Find( int iKey ) const
	{
	  CAVLTreeNode<KeyValue> *pTemp = FindNode( iKey );
	  if ( pTemp != NULL ) 
	    return &pTemp->GetKeyValue();
	  return NULL;
	} // Find( ...
	////////////////////
	/// Seeks node from binary tree.
	/// \param iKey Key to seek.
	/// \returns Pointer to node.
	CAVLTreeNode<KeyValue> *FindNode( int iKey ) const
	{
	  if ( GetRoot() == NULL ) return NULL;
	  
	  CAVLTreeNode<KeyValue> *pTemp = GetRoot();

	  while ( pTemp != NULL  )
	  {
	    if ( iKey < pTemp->GetKeyValue().GetKey() )
	    {
	      pTemp = pTemp->GetLeftChild();
	    }
	    else if ( iKey > pTemp->GetKeyValue().GetKey() )
	    {
	      pTemp = pTemp->GetRightChild();
	    }
	    else return pTemp;
	  } // while
	  return NULL;
	} // FindNode
	////////////////////
	/// Deletes node with key.
	/// \param iKey Key to node.
	/// \returns zero if success, non-zero on failure.
	int Delete( int iKey )
	{
	  CAVLTreeNode<KeyValue> *pTemp = FindNode( iKey );
	  if ( pTemp != NULL ) 
	  {
	    delete pTemp;
	    return 0;
	  }
	  return 1;
	}
	////////////////////
	/// Returns height of the tree.
	/// \returns height of root node, if it exists, 0 otherwise.
	inline unsigned int GetHeight() const
	{
	  if ( GetRoot() == NULL ) return 0;
	  else return GetRoot()->GetHeight();
	}
	////////////////////
	/// Travels through tree in Pre-Order.
	template< class Handler >
	void ProcessPreOrder( Handler &handler, CAVLTreeNode<KeyValue> *pNode ) 
	{
	  if ( pNode == NULL ) return;
	  handler.Handle( *pNode );
	  if ( pNode->GetLeftChild() != NULL) ProcessPreOrder( handler, pNode->GetLeftChild());
	  if ( pNode->GetRightChild() != NULL ) ProcessPreOrder( handler, pNode->GetRightChild());
	}
	////////////////////
	/// Travels through tree in In-Order.
	template< class Handler >
	void ProcessInOrder( Handler &handler, CAVLTreeNode<KeyValue> *pNode ) 
	{
	  if ( pNode == NULL ) return;
	  if ( pNode->GetLeftChild() != NULL) ProcessInOrder(handler, pNode->GetLeftChild());
	  handler.Handle( *pNode );
	  if ( pNode->GetRightChild() != NULL ) ProcessInOrder(handler, pNode->GetRightChild());
	}
	////////////////////
	/// Travels through tree in Post-Order.
	template< class Handler >
	void ProcessPostOrder( Handler &handler, CAVLTreeNode<KeyValue> *pNode ) 
	{
	  if ( pNode == NULL ) return;
	  if ( pNode->GetLeftChild() != NULL) ProcessPostOrder( handler, pNode->GetLeftChild());
	  if ( pNode->GetRightChild() != NULL ) ProcessPostOrder( handler, pNode->GetRightChild());
	  handler.Handle( *pNode );
	}
      }; // CAVLTree
    }; // namespace AvlTree
  }; // namespace Core
};// namespace Moose
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
