#ifndef __PhoenixAVLTree_h__
#define __PhoenixAVLTree_h__
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
    ////////////////////
    /// Binary tree data structure.
    template< typename TYPE >
    class CBinTreeNode
    {
    protected:
      /// Left child.
      CBinTreeNode *m_pLeftChild; 
      /// Right child.
      CBinTreeNode *m_pRightChild;
      /// Node data.
      TYPE m_Value;
    public:
      ////////////////////
      /// Constructor.
      CBinTreeNode( TYPE value ) : m_Value(value), 
				   m_pLeftChild(NULL), 
				   m_pRightChild(NULL) { }
      ~CBinTreeNode()
      {
      }
      ////////////////////
      /// For retrieving the value.
      /// \return Value in this node.
      inline const TYPE & GetValue() const
      {
	return m_Value;
      }
      ////////////////////
      /// For setting the value.
      /// \param value A value to be set.
      inline void SetValue( const TYPE &value )
      {
	m_Value = value;
      }
      ////////////////////
      /// Returns right child.
      /// \returns A pointer to right child.
      inline CBinTreeNode * GetRightChild() const
      {
	return m_pRightChild;
      }
      ////////////////////
      /// Returns left child.
      /// \returns A pointer to left child.
      inline CBinTreeNode * GetLeftChild() const
      {
	return m_pLeftChild;
      }
      ////////////////////
      /// Assigns right child.
      /// \param pRightChild New right child node.
      inline void SetRightChild( CBinTreeNode *pRightChild) 
      {
	m_pRightChild = pRightChild;
      }
      ////////////////////
      /// Assigns right child.
      /// \param pLeftChild New right child node.
      inline void SetLeftChild( CBinTreeNode *pLeftChild) 
      {
	m_pLeftChild = pLeftChild;
      }
    }; // CBinTree
  }; // namespace Core
};// namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
