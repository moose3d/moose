#ifndef __PhoenixGuiSystem_h__
#define __PhoenixGuiSystem_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixSpatial.h>
#include <PhoenixMath.h>
#include <PhoenixCore.h>
#include <PhoenixTransform.h>
#include <PhoenixGraph.h>
#include <PhoenixTransformGraph.h>
#include <string>
/////////////////////////////////////////////////////////////////
namespace Phoenix 
{
  namespace Gui
  {
    /// Types of elements that are allowed in GUI.
    enum GUI_ELEMENT_TYPE
    {
      GUI_WINDOW = 0,
      GUI_LABEL,
      GUI_BUTTON,
      GUI_MULTILABEL
    };
    /// Message types for GUI elements.
    enum GUI_MESSAGE_TYPES 
    {
      GUI_MSG_MOUSE_ENTER = 0,
      GUI_MSG_MOUSE_LEAVE,
      GUI_MSG_MOUSE_CLICK,
      GUI_NUM_OF_MESSAGE_TYPES
    };

#define GUI_ELEMENT_TNODE_TYPE Phoenix::Scene::CTransformNode<GUI_ELEMENT_TYPE, Phoenix::Gui::CGuiElement, Phoenix::Core::CHandle<Phoenix::Gui::CGuiElement> >
    class CGuiSystem;
    ////////////////////
    /// Base class for every GUI object.
    class CGuiElement : public Phoenix::Spatial::CDimensional2D, 
			public CTypeBase<GUI_ELEMENT_TYPE>,
			public Phoenix::Math::CTransformable
    {
    protected:
      /// Is this element visible.
      int	m_bVisible;
      /// Does this element have current focus.
      int	m_bHasFocus;
      /// Transform node for this element.
      GUI_ELEMENT_TNODE_TYPE *m_pTransformNode;
      ////////////////////
      /// Constructor.
      CGuiElement( GUI_ELEMENT_TNODE_TYPE *pNode ) : m_bVisible(0), 
						     m_bHasFocus(0), 
						     m_pTransformNode(pNode) { }
    public:
      ////////////////////
      /// Destructor.
      virtual ~CGuiElement() {}
      ////////////////////
      /// Checks is element visible.
      /// \returns non-zero if visible, zero otherwise.
      int IsVisible() const	      {	return m_bVisible;   }
      ////////////////////
      /// Sets element visibility.
      /// \param bFlag Non-zero if visible, zero otherwise.
      void SetVisible( int bFlag ) {	m_bVisible = bFlag;  }
      ////////////////////
      /// Checks is this element focused.
      /// \returns Non-zero on focus, zero otherwise.
      int HasFocus() const { return m_bHasFocus;  }
      ////////////////////
      /// Sets focus flag.
      /// \param bFlag Non-zero for focus on, 0 for unfocus.
      void SetFocus( int bFlag ) { m_bHasFocus = bFlag; }
      ////////////////////
      /// Returns transform node.
      /// \returns Pointer to transform node.
      GUI_ELEMENT_TNODE_TYPE *GetTransformNode() { return m_pTransformNode; }
    };
    ////////////////////
    /// Class for window. Window contains other objects.
    class CWindow : public Phoenix::Gui::CGuiElement
    {
      friend class CGuiSystem;
    protected:
      ////////////////////
      /// Constructor.
      CWindow( GUI_ELEMENT_TNODE_TYPE *pNode ) : Phoenix::Gui::CGuiElement( pNode )
      {
	SetType( Phoenix::Gui::GUI_WINDOW );
      }
      
    };
    ////////////////////
    /// Simple one-line label class.
    class CLabel : public Phoenix::Gui::CGuiElement 
    {
      friend class CGuiSystem;
    protected:
      ////////////////////
      /// Constructor.
      CLabel(  GUI_ELEMENT_TNODE_TYPE *pNode ) : Phoenix::Gui::CGuiElement( pNode )
      {
	SetType( Phoenix::Gui::GUI_LABEL );
      }
    };
    ////////////////////
    /// Very simple button class.
    class CButton : public Phoenix::Gui::CGuiElement
    {
      friend class CGuiSystem;
    protected:
      ////////////////////
      /// Constructor.
      CButton( GUI_ELEMENT_TNODE_TYPE *pNode ) : Phoenix::Gui::CGuiElement( pNode )
      {
	SetType( Phoenix::Gui::GUI_BUTTON );
      }
    };
    ////////////////////
    /// Label with multiple lines. Convinient for displaying logs etc.
    class CMultiLineLabel : public Phoenix::Gui::CGuiElement
    {
      friend class CGuiSystem;
    protected:
      ////////////////////
      /// Constructor.
      CMultiLineLabel( GUI_ELEMENT_TNODE_TYPE *pNode ) : Phoenix::Gui::CGuiElement( pNode )
      {
	SetType( Phoenix::Gui::GUI_MULTILABEL );
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Update adapter for GUI element graph.
    class CGuiUpdateAdapter
    {
    public:
      ////////////////////
      /// Handles entering to node while traversing
      int Enter( CGraphNode<GUI_ELEMENT_TYPE> *pNode )
      {
	HandleTransform( static_cast<GUI_ELEMENT_TNODE_TYPE *>(pNode));
	// not culling
	return 0;
      }
      ////////////////////  
      /// Handles exiting  node while traversing
      void Leave( CGraphNode<GUI_ELEMENT_TYPE> *pNode )
      {
	//NOP
      }
    protected:
      ////////////////////
      /// Reflects change from parent to child.
      /// \param pNode Node to be processed.
      void HandleTransform( GUI_ELEMENT_TNODE_TYPE * pNode )
      {
	
	Phoenix::Math::CTransform *pParentWorldTransform = NULL;
	
	// If this node is root
	if ( pNode->GetArrivingEdges().empty())
	{
	  pNode->GetResource()->SetWorldTransform( pNode->GetResource()->GetLocalTransform() );
	}
	else 
	{
	  pParentWorldTransform = 
	  &static_cast<GUI_ELEMENT_TNODE_TYPE *>( pNode->GetArrivingEdges().front()->GetFromNode() )->GetResource()->GetWorldTransform();
	  Multiply( *pParentWorldTransform, 
		    pNode->GetResource()->GetLocalTransform(), 
		    pNode->GetResource()->GetWorldTransform());
      
	}
      }
    };
    /////////////////////////////////////////////////////////////////
    class CGuiSystem : private Phoenix::Core::CGraph<GUI_ELEMENT_TYPE>
    {
    protected:
      /// pointer to root window.
      Phoenix::Gui::CWindow *m_pBaseWindow;
      /// Updater adapter.
      Phoenix::Gui::CGuiUpdateAdapter      m_updaterAdapter;
    public:
      ////////////////////
      /// Constructor.
      CGuiSystem()
      {
	m_pBaseWindow = Create<Phoenix::Gui::CWindow>("main");
	assert( m_pBaseWindow != NULL && "GUI: Basewindow creation failed! Out of memory?" );
      }
      ////////////////////
      /// Returns reference to root window:
      Phoenix::Gui::CWindow & GetRoot()
      {
	return *m_pBaseWindow;
      }
      ////////////////////
      /// Updates gui locations.
      void Update()
      {
	TravelDF<CGuiUpdateAdapter, GUI_ELEMENT_TYPE, std::string, int>( 
									static_cast<CGraphNode<GUI_ELEMENT_TYPE> *>(m_pBaseWindow->GetTransformNode()), 
									&m_updaterAdapter );
      }
      ////////////////////
      /// Factory method for creating GUI elements.
      template< typename ELEMENT_TYPE > ELEMENT_TYPE * Create( const char *szResourceName)
      {
	// create node for transformation graph.
	GUI_ELEMENT_TNODE_TYPE *pNode = Phoenix::Core::CGraph<GUI_ELEMENT_TYPE>::CreateNode<GUI_ELEMENT_TNODE_TYPE>();
	// Create actual button, and pass proper pointer to transform node
	ELEMENT_TYPE *pElement = new ELEMENT_TYPE(pNode);

	// Make element managed by gui element resourcemanager
	assert( (Phoenix::Core::CResourceManager< Phoenix::Gui::CGuiElement, 
		Phoenix::Core::CHandle<Phoenix::Gui::CGuiElement> >::GetInstance()->
		Create( pElement, szResourceName, pNode->GetHandle() ) == 0) && "GUI System: Conflicting element name or Out Of Memory!");
	return pElement;
      }
      ////////////////////
      /// Deletes given element from GUI.
      /// \param pPtr Pointer to an element to be deleted.
      void Delete( Phoenix::Gui::CGuiElement *pPtr )
      {
	assert( pPtr != NULL && "GuiSystem: delete failed, ptr NULL");
	
	// A hack; THIS SHOULD NEVER BE DONE IN THIS WAY.
	// But we use only the index of handle to destroy object; 
	// handle gets destroyed when transform node is destroyed, so we can't really use it directly.
	// Also the handle is never registered to resource manager and never used outside this scope,
	// so we're on the safe side.
	Phoenix::Core::CHandle<Phoenix::Gui::CGuiElement> hTmp = pPtr->GetTransformNode()->GetHandle();
	// Delete transform node from graph.
        Phoenix::Core::CGraph<GUI_ELEMENT_TYPE>::DeleteNode( static_cast< CGraphNode<GUI_ELEMENT_TYPE> *>(pPtr->GetTransformNode()));
	// Delete actual object from manager using temp handle.
        Phoenix::Core::CResourceManager< Phoenix::Gui::CGuiElement, 
					 Phoenix::Core::CHandle<Phoenix::Gui::CGuiElement> >::GetInstance()->Destroy( hTmp );

      }
    };
  } // namespace Gui
} // namespace Phoenix
#undef GUI_ELEMENT_TNODE_TYPE
/////////////////////////////////////////////////////////////////
#endif

