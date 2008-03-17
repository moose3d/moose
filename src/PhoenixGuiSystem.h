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
    ////////////////////
    /// Base class for every GUI object.
    class CGuiElement : public Phoenix::Spatial::CDimensional2D,
			public Phoenix::Math::CTransformable,
			public Phoenix::Scene::CTransformNode<GUI_ELEMENT_TYPE, int, Phoenix::Core::CHandle<int> >
    {
    protected:
      /// Is this element visible.
      int	m_bVisible;
    public:
      ////////////////////
      /// Checks is element visible.
      /// \returns non-zero if visible, zero otherwise.
      int IsVisible() const	      {	return m_bVisible;   }
      ////////////////////
      /// Sets element visibility.
      /// \param bFlag Non-zero if visible, zero otherwise.
      void SetVisible( int bFlag ) {	m_bVisible = bFlag;  }
    };
    ////////////////////
    /// Class for window. Window contains other objects.
    class CWindow : public Phoenix::Gui::CGuiElement
    {
    public:
      ////////////////////
      /// Constructor.
      CWindow() 
      {
	SetType( Phoenix::Gui::GUI_WINDOW );
      }
    };
    ////////////////////
    /// Simple one-line label class.
    class CLabel : public Phoenix::Gui::CGuiElement 
    {
    public:
      ////////////////////
      /// Constructor.
      CLabel()
      {
	SetType( Phoenix::Gui::GUI_LABEL );
      }
    };
    ////////////////////
    /// Very simple button class.
    class CButton : public Phoenix::Gui::CGuiElement
    {
    public:
      ////////////////////
      /// Constructor.
      CButton() 
      {
	SetType( Phoenix::Gui::GUI_BUTTON );
      }
    };
    ////////////////////
    /// Label with multiple lines. Convinient for displaying logs etc.
    class CMultiLineLabel : public Phoenix::Gui::CGuiElement
    {
    public:
      ////////////////////
      /// Constructor.
      CMultiLineLabel() 
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
	HandleTransform( static_cast<Phoenix::Gui::CGuiElement *>(pNode));
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
      void HandleTransform( Phoenix::Gui::CGuiElement * pNode )
      {
    
	Phoenix::Math::CTransform *pParentWorldTransform = NULL;
	
	// If this node is root
	if ( pNode->GetArrivingEdges().empty())
	{
	  pNode->SetWorldTransform( pNode->GetLocalTransform() );
	}
	else 
	{
	  pParentWorldTransform = 
	  &static_cast<Phoenix::Gui::CGuiElement *>( pNode->GetArrivingEdges().front()->GetFromNode() )->GetWorldTransform();
	  Multiply( *pParentWorldTransform, 
		    pNode->GetLocalTransform(), 
		    pNode->GetWorldTransform());
      
	}
      }
    };
    /////////////////////////////////////////////////////////////////
    class CGuiSystem : public Phoenix::Core::CGraph<GUI_ELEMENT_TYPE>
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
	//m_pBaseWindow = Phoenix::Core::CGraph<GUI_ELEMENT_TYPE>::CreateNode< Phoenix::Gui::CWindow >();
	//assert( m_pBaseWindow != NULL && "GUI: Basewindow creation failed! Out of memory?" );
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
									static_cast<CGraphNode<GUI_ELEMENT_TYPE> *>(m_pBaseWindow), 
									&m_updaterAdapter );
      }
    };
  } // namespace Gui
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif

