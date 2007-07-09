#ifndef __PhoenixAVLTree_h__
#define __PhoenixAVLTree_h__
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
    namespace AVLTree
    {
      template< int Key, typename Value > 
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
	inline const Value & GetValue() { return m_Value; } 
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
      }
      
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
	/// Node data with key.
	KeyValue m_Data;
	/// Height of this subtree
	int  m_iHeight;
      public:
	////////////////////
	/// Constructor.
	CAVLTreeNode( KeyValue value ) : m_pLeftChild(NULL), 
				m_pRightChild(NULL),
				m_Value(value),
				m_iHeight(0) { }
	////////////////////
	/// For retrieving the value.
	/// \return Value in this node.
	inline KeyValue & GetKeyValue() 
	{
	  return m_Value;
	}
	////////////////////
	/// For setting the value.
	/// \param value A value to be set.
	inline void SetKeyValue( KeyValue &value )
	{
	  m_Value = value;
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
	  if ( tKeyValue.GetKey() > pTemp->GetKeyValue() )
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
	      return 0;
	    }
	  }
	  else if ( kKeyValue < pTemp->GetKeyValue())
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
	      return 0;
	    }
	  }
 
	} // while 
	return -1;
      } // Insert( ...
    }; // namespace AvlTree
  }; // namespace Core
};// namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
