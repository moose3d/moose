#ifndef __PhoenixAVLTree_h__
#define __PhoenixAVLTree_h__
#include <stdlib.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
    namespace AVLTree
    {
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
      template <class KeyValue > class CAVLTreeNode;
      template< class KeyValue >
      class CAVLTreeNode
      {
      protected:
	CAVLTreeNode<KeyValue> *m_pRoot;
      public:
	////////////////////
	/// Constructor.
	CAVLTreeNode() : m_pRoot(NULL)
	{
	  
	}
      };
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
	CAVLTree *m_pTree;

      public:
	////////////////////
	/// Constructor.
	CAVLTreeNode( KeyValue value, CAVLTree *pTree ) : m_pLeftChild(NULL), 
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
	inline CAVLTreeNode * GetParent() 
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
	  CAVLTreeNode *pParentOfThis  = GetParent();
	  CAVLTreeNode *pNewRoot       = GetLeftChild();
	  CAVLTreeNode *pNewLeftChild = GetLeftChild()->GetRightChild();
	  
	  SetLeftChild( pNewLeftChild );
	  pNewLeftChild->SetParent(this);
	  pNewRoot->SetRightChild( this );

	  if ( IsRoot() )                    { pTree->SetRoot( pNewRoot );	         }
	  else if ( IsRightChildOfParent() )  { pParentOfThis->SetRightChild( pNewRoot );  }
	  else                                { pParentOfThis->SetLeftChild( pNewRoot ); }

	  SetBalance(0);
	  pNewRoot->SetBalance(0);

	  CalculateHeight();
	  pNewRoot->CalculateHeight();
	}
	////////////////////
	void RotateLeft()
	{
	  CAVLTreeNode *pParentOfThis  = GetParent();
	  CAVLTreeNode *pNewRoot       = GetRightChild();
	  CAVLTreeNode *pNewRightChild = GetRightChild()->GetLeftChild();
	  
	  SetRightChild( pNewRightChild );
	  pNewRightChild->SetParent(this);

	  pNewRoot->SetLeftChild( this );
	  this->SetParent(pNewRoot);

	  if ( IsRoot() ) 
	  { 
	    pTree->SetRoot( pNewRoot ); 
	    pNewRoot->SetParent(NULL); 
	  }
	  else if ( IsLeftChildOfParent() )  
	  { 
	    pParentOfThis->SetLeftChild( pNewRoot ); 
	    pNewRoot->SetParent(pParentOfThis); 
	  }
	  else
	  { 
	    pParentOfThis->SetRightChild( pNewRoot ); 
	    pNewRoot->SetParent(pParentOfThis); 
	  }
	  
	  SetBalance(0);
	  pNewRoot->SetBalance(0);

	  CalculateHeight();
	  pNewRoot->CalculateHeight();
	}
	////////////////////
	void DoubleRotateRight()
	{
	  CAVLTreeNode *pNewRoot       = GetRightChild()->GetLeftChild();
	  CAVLTreeNode *pNewRightChild = GetRightChild();
	  CAVLTreeNode *pNewLeftChild  = this;
	  CAVLTreeNode *pParentOfThis  = GetParent();
	  
	  pNewRightChild->SetLeftChild( pNewRoot->GetRightChild() );
	  pNewRoot->GetRightChild()->SetParent( pNewRightChild );
	  
	  this->SetRightChild( pNewRoot->GetLeftChild() );
	  pNewRoot->GetLeftChild()->SetParent( this );
	  
	  pNewRoot->SetLeftChild( this );
	  this->SetParent( pNewRoot );

	  pNewRoot->SetRightChild(pNewRightChild);
	  pNewRightChild->SetParent(pNewRoot);

	  pParentOfThis
	  
	}
	////////////////////
	void DoubleRotateLeft()
	{
	  
	}

	void CalculateHeight()
	{
	  
	  CAVLTreeNode  *pLeft = pNode->GetLeftChild();
	  CAVLTreeNode  *pRight = pNode->GetRightChild();

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
	    pNode = pNode->GetParent();
	  }
	  ////////////////////
	  // Update also root node height.
	  pNode->CalculateHeight();
	}
	////////////////////
	/// Returns true, if this node is left child of parent node.
	/// \returns non-zero if true, zero if false.
	inline int IsLeftChildOfParent() const
	{
	  if ( GetParent() == NULL ) return 0;
	  return (GetParent()->GetLeftChild() == this );
	}
	////////////////////
	/// Returns true, if this node is right child of parent node.
	/// \returns non-zero if true, zero if false.
	inline int IsRightChildOfParent() const
	{
	  if ( GetParent() == NULL ) return 0;
	  return (GetParent()->GetRightChild() == this );
	}
	////////////////////
	/// Returns true, if this node is root node ( no parent )
	/// \returns non-zero if true, zero if false.
	inline int IsRoot() const
	{
	  return ( GetParent() == NULL );
	}
	
      }; // CBinTree
      ////////////////////
      /// Inserts into binary tree.
      /// \param pRoot Pointer to root node. Must be NON-NULL.
      /// \param value 
      template<class KeyValue>
      int Insert( CAVLTreeNode<KeyValue> *pRoot, const KeyValue & tKeyValue)
      {
	CAVLTreeNode<KeyValue> *pTemp = pRoot;
	int bFound = 0;
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
	      CAVLTreeNode<KeyValue> *pNew = new CAVLTreeNode<KeyValue>( tKeyValue );
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
	      CAVLTreeNode<KeyValue> *pNew = new CAVLTreeNode<KeyValue>( tKeyValue );
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
      /// \param pRoot Pointer to root node. Must be NON-NULL.
      /// \param iKey Key to seek.
      /// \returns KeyValue object.
      template<class KeyValue>
      KeyValue * Find( CAVLTreeNode<KeyValue> *pRoot, int iKey )
      {
	CAVLTreeNode<KeyValue> *pTemp = pRoot;
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
      } // Insert( ...
    }; // namespace AvlTree
  }; // namespace Core
};// namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
