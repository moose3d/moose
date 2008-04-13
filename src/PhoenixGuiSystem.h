#ifndef __PhoenixGuiSystem_h__
#define __PhoenixGuiSystem_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixSpatial.h>
#include <PhoenixMath.h>
#include <PhoenixCore.h>
#include <PhoenixTransform.h>
#include <PhoenixGraph.h>
#include <PhoenixTransformGraph.h>
#include <PhoenixStateMachine.h>
#include <string>
/////////////////////////////////////////////////////////////////
namespace Phoenix 
{
  namespace Gui
  {
    /// Types of elements that are allowed in GUI.
    enum GUI_ELEMENT_TYPE
    {
      GUI_ELEMENT = 0
    };
    /// Message types for GUI elements.
    enum GUI_MESSAGE_TYPES 
    {
      GUI_MSG_MOUSE_MOTION = 0,
      GUI_MSG_MOUSE_DOWN,
      GUI_MSG_MOUSE_UP,
      GUI_MSG_MOUSE_CLICK,
      GUI_NUM_OF_MESSAGE_TYPES
    };
    
    /////////////////////////////////////////////////////////////////
    /// Base class for mouse events.
    class CMouseEventBase
    {
    private:
      Phoenix::Math::CVector2<int> m_vCoords;
      int			   m_iButton;
    protected:
      CMouseEventBase( const Phoenix::Math::CVector2<int> &vCoords) : m_vCoords(vCoords), m_iButton(0) {}
      CMouseEventBase( const Phoenix::Math::CVector2<int> &vCoords, int iButton ) : m_vCoords(vCoords), m_iButton(iButton) {}
    public:
      ////////////////////
      /// Returns mouse coordinates.
      /// \returns Mouse coordinates as int 2-vector
      const Phoenix::Math::CVector2<int> & GetCoords() const 
      { 
	return m_vCoords; 
      }
      ////////////////////
      /// Returns button identifier.
      /// \returns Button id.
      int GetButton() const
      {
	return m_iButton;
      }
    };
    template< class BASE_COMPONENT_TYPE > class CGuiElement;
    /////////////////////////////////////////////////////////////////
    /// Mouse event handler class. 
    template <class BASE_COMPONENT_TYPE>
    class CMouseEventHandler 
    {
    public:
      virtual ~CMouseEventHandler(){}
      virtual void HandleMouseDown( const CMouseEventBase & me,   CGuiElement<BASE_COMPONENT_TYPE> & element ) {}
      virtual void HandleMouseUp( const CMouseEventBase & me,     CGuiElement<BASE_COMPONENT_TYPE> & element ) {}
      virtual void HandleMouseClick( const CMouseEventBase & me,  CGuiElement<BASE_COMPONENT_TYPE> & element ) {}
      virtual void HandleMouseMotion( const CMouseEventBase & me, CGuiElement<BASE_COMPONENT_TYPE> & element ) {}
    };
    /////////////////////////////////////////////////////////////////
#define GUI_ELEMENT_TNODE_TYPE Phoenix::Scene::CTransformNode<GUI_ELEMENT_TYPE, BASE_COMPONENT_TYPE, Phoenix::Core::CHandle<BASE_COMPONENT_TYPE > >
    template< class BASE_COMPONENT_TYPE > class CGuiSystem;
    ////////////////////
    /// Base class for every GUI object.
    template< class BASE_COMPONENT_TYPE >
    class CGuiElement : public Phoenix::Spatial::CDimensional2D, 
			public Phoenix::Math::CTransformable,
			public Phoenix::AI::CMessageObject< BASE_COMPONENT_TYPE, GUI_MESSAGE_TYPES >,
			public Phoenix::Core::CFocusable
    {
      friend  class Phoenix::Gui::CGuiSystem<BASE_COMPONENT_TYPE>;
    protected:
      /// Is this element visible.
      int	m_bVisible;
      /// Is element pressed.
      int	m_bPressed;
      /// Element id
      int	m_iActionId;
      /// Transform node for this element.
      GUI_ELEMENT_TNODE_TYPE *m_pTransformNode;
      CMouseEventHandler<BASE_COMPONENT_TYPE> *m_pMouseHandler;
      ////////////////////
      /// Constructor.
      CGuiElement( ) : m_bVisible(0), 
		       m_bPressed(0),
		       m_iActionId(0),
		       m_pTransformNode(NULL),
		       m_pMouseHandler(NULL) { }
    public:
      ////////////////////
      /// Sets current mouse handler.
      /// \param pMouseHandler Pointer to mousehandler.
      void SetMouseHandler( CMouseEventHandler<BASE_COMPONENT_TYPE> *pMouseHandler )
      {
	m_pMouseHandler = pMouseHandler;
      }
      ////////////////////
      /// Returns mouse handler.
      /// \returns Pointer to current mouse handler.
      CMouseEventHandler<BASE_COMPONENT_TYPE> * GetMouseHandler()
      {
	return m_pMouseHandler;
      }
      ////////////////////
      /// Sets transform node.
      /// \param pNode transform node to be set.
      void SetTransformNode( GUI_ELEMENT_TNODE_TYPE *pNode )
      {
	m_pTransformNode = pNode;
      }
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
      /// Checks if element is pressed.
      /// \returns non-zero if pressed, zero otherwise.
      int IsPressed() const { return m_bPressed; }
      ////////////////////
      /// Sets pressed status.
      /// \param bFlag non-zero if pressed, zero otherwise.
      void SetPressed( int bFlag ){ m_bPressed = bFlag; }
      ////////////////////
      /// Returns transform node.
      /// \returns Pointer to transform node.
      GUI_ELEMENT_TNODE_TYPE *GetTransformNode() { return m_pTransformNode; }
      ////////////////////
      /// Checks does mouse click hit this element.
      /// Mouse clicks must be in OpenGL Window format, (0,0) is in left lower corner.
      virtual int MouseCoordinatesInside( const Phoenix::Math::CVector2<int> &vCoords ) 
      {
	const Phoenix::Math::CMatrix4x4<float> & matrix = GetWorldTransform().GetMatrix();
	// Convert coords;
	// first scale, then add translation
	float fX1  = matrix(0,0) * (GetWidth() ) + matrix(0,3);
	float fY1 = matrix(1,1) * (GetHeight() ) + matrix(1,3);
	// Origin is always at (0,0)
	float fX0 =  matrix(0,3);
	float fY0 =  matrix(1,3);
	// actual checking
	return ( (vCoords[0] > (int)fX0)    && 
		 (vCoords[0] < (int)(fX1)) && 
		 (vCoords[1] > (int)fY0)    && 
		 (vCoords[1] < (int)(fY1)) );
      }
      ////////////////////
      /// Adds given elemtn as child of this element.
      /// \param pElement Element to be assigned as child.
      void AddChild( CGuiElement *pElement )
      {
	assert( pElement != NULL && "pElement IS NULL!!!" );
	GetTransformNode()->AddEdge( pElement->GetTransformNode());
      }
      ////////////////////
      /// Returns id.
      /// \returns Id number.
      int GetActionId() const
      {
	return m_iActionId;
      }
      ////////////////////
      /// Sets id.
      /// \param id New id.
      void SetActionId( int id )
      {
	m_iActionId = id;
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Update adapter for GUI element graph.
    template <class BASE_COMPONENT_TYPE>
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
    /// Update adapter for GUI element graph.
    template <class BASE_COMPONENT_TYPE, class EXT_ADAPTER>
    class CGuiRenderAdapter
    {
    private:
      EXT_ADAPTER adapter;
    public:
      ////////////////////
      CGuiRenderAdapter() {}
      ////////////////////
      /// Handles entering to node while traversing
      int Enter( CGraphNode<GUI_ELEMENT_TYPE> *pNode )
      {
	adapter.Process( *static_cast<GUI_ELEMENT_TNODE_TYPE *>(pNode)->GetResource() );
	// add visibility checking here.
	// not culling
	return 0;
      }
      ////////////////////  
      /// Handles exiting  node while traversing
      void Leave( CGraphNode<GUI_ELEMENT_TYPE> *pNode )
      {
	//NOP
      }
      ////////////////////
      /// Returns reference to adapter.
      EXT_ADAPTER & GetAdapter() { return adapter; }
    };
    
    /////////////////////////////////////////////////////////////////
    /// Simple mouse motion event.
    template <class BASE_COMPONENT_TYPE>
    class CMouseMotionEvent : public Phoenix::AI::CMessage<BASE_COMPONENT_TYPE,GUI_MESSAGE_TYPES>,
			      public CMouseEventBase
    
    {
    public:
      CMouseMotionEvent( const Phoenix::Math::CVector2<int> &vCoords, int iButton ) : CMouseEventBase( vCoords, iButton)
      { 
	Phoenix::AI::CMessage<BASE_COMPONENT_TYPE,GUI_MESSAGE_TYPES>::SetType( GUI_MSG_MOUSE_MOTION ); 
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Simple mouse click event.
    template <class BASE_COMPONENT_TYPE>
    class CMouseClickEvent : public Phoenix::AI::CMessage<BASE_COMPONENT_TYPE,GUI_MESSAGE_TYPES>,
			     public CMouseEventBase
    {

    public:
      CMouseClickEvent( const Phoenix::Math::CVector2<int> &vCoords, int iButton ) : CMouseEventBase( vCoords, iButton)
      { 
	Phoenix::AI::CMessage<BASE_COMPONENT_TYPE,GUI_MESSAGE_TYPES>::SetType( GUI_MSG_MOUSE_CLICK ); 
      }
     
    };
    /////////////////////////////////////////////////////////////////
    /// Simple mouse down event.
    template <class BASE_COMPONENT_TYPE>
    class CMouseDownEvent : public Phoenix::AI::CMessage<BASE_COMPONENT_TYPE,GUI_MESSAGE_TYPES>,
			    public CMouseEventBase
    {
    public:
      CMouseDownEvent( const Phoenix::Math::CVector2<int> &vCoords, int iButton ) : CMouseEventBase( vCoords, iButton)
      { 
	Phoenix::AI::CMessage<BASE_COMPONENT_TYPE,GUI_MESSAGE_TYPES>::SetType( GUI_MSG_MOUSE_DOWN ); 
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Simple mouse up event.
    template <class BASE_COMPONENT_TYPE>
    class CMouseUpEvent : public Phoenix::AI::CMessage<BASE_COMPONENT_TYPE,GUI_MESSAGE_TYPES>,
			  public CMouseEventBase
    {
    public:
      CMouseUpEvent( const Phoenix::Math::CVector2<int> &vCoords, int iButton ) : CMouseEventBase( vCoords, iButton)
      { 
	Phoenix::AI::CMessage<BASE_COMPONENT_TYPE,GUI_MESSAGE_TYPES>::SetType( GUI_MSG_MOUSE_UP ); 

      }
    };
    /////////////////////////////////////////////////////////////////
    /// Sorter function for receivers. By using this,
    /// the EvaluateLayout method orders elements from top to bottom, making
    /// "top" elements hide those "under" them when clicking.

    template <class OBJECT_TYPE> 
    struct SorterZ
    {
      inline bool operator()( CHandle<OBJECT_TYPE> * hFirst, CHandle<OBJECT_TYPE> * hSecond )
      {
	if ( hFirst->IsNull() )  return false;
	if ( hSecond->IsNull() ) return true;
	return ( CResourceManager<OBJECT_TYPE, CHandle<OBJECT_TYPE> >::GetInstance()->GetResource(*hFirst)->GetWorldTransform().GetMatrix()(2,3) >
		 CResourceManager<OBJECT_TYPE, CHandle<OBJECT_TYPE> >::GetInstance()->GetResource(*hSecond)->GetWorldTransform().GetMatrix()(2,3) );
	
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Class for GUI system.
    template< class BASE_COMPONENT_TYPE >
    class CGuiSystem : private Phoenix::Core::CGraph<GUI_ELEMENT_TYPE>
    {
    protected:
      /// pointer to root window.
      //Phoenix::Gui::CWindow *m_pBaseWindow;
      BASE_COMPONENT_TYPE *m_pBaseWindow;
      /// Updater adapter.
      Phoenix::Gui::CGuiUpdateAdapter<BASE_COMPONENT_TYPE>      m_updaterAdapter;
      /// Message router 
      Phoenix::AI::CMessageRouter< BASE_COMPONENT_TYPE, GUI_MESSAGE_TYPES > m_msgRouter;
    public:
      ////////////////////
      /// Constructor.
      CGuiSystem() : m_msgRouter(GUI_NUM_OF_MESSAGE_TYPES)
      {
	m_pBaseWindow = NULL;
	//m_pBaseWindow = Create<Phoenix::Gui::CWindow>("main");
	//m_pBaseWindow = Create<Phoenix::Gui::CGuiElement>("main");
	//assert( m_pBaseWindow != NULL && "GUI: Basewindow creation failed! Out of memory?" );
      }
      ////////////////////
      /// Returns reference to root window:
      //Phoenix::Gui::CWindow & GetRoot()
      BASE_COMPONENT_TYPE & GetRoot()
      {
	return *m_pBaseWindow;
      }
      ////////////////////
      /// Returns reference to root window:
      //Phoenix::Gui::CWindow & GetRoot()
      void SetRoot( BASE_COMPONENT_TYPE *pRoot )
      {
	m_pBaseWindow = pRoot;
      }
      ////////////////////
      /// Prepares GUI for action.
      void Prepare()
      {
	m_msgRouter.Prepare();
      }
      ////////////////////
      /// Updates element positions / scaling.
      void EvaluateLayout( CGraphNode<GUI_ELEMENT_TYPE> * pNode = NULL )
      {
	if ( !pNode )
	{
	  pNode = static_cast<CGraphNode<GUI_ELEMENT_TYPE> *>(m_pBaseWindow->GetTransformNode());
	}
	TravelDF<CGuiUpdateAdapter<BASE_COMPONENT_TYPE>, GUI_ELEMENT_TYPE, std::string, int>( pNode, &m_updaterAdapter );
        SorterZ< BASE_COMPONENT_TYPE > sort_func;
	m_msgRouter.SortReceivers( sort_func );
      }
      ////////////////////
      /// Passes events to listeners.
      template<class MSG_ADAPTER_TYPE> void Update( MSG_ADAPTER_TYPE & rAdapter )
      {
	m_msgRouter.Update( rAdapter);
      }
      ////////////////////
      /// Returns component by its name.
      /// \param szResourceName Resource name.
      /// \returns Component pointer, or NULL if not found.
      BASE_COMPONENT_TYPE * GetComponent( const char *szResourceName) 
      {
	return Phoenix::Core::CResourceManager< BASE_COMPONENT_TYPE, Phoenix::Core::CHandle<BASE_COMPONENT_TYPE> >::GetInstance()->GetResource( szResourceName );
      }
      ////////////////////
      /// Factory method for creating GUI elements.
      template< typename ELEMENT_TYPE > ELEMENT_TYPE * Create( const char *szResourceName)
      {
	// create node for transformation graph.
	GUI_ELEMENT_TNODE_TYPE *pNode = Phoenix::Core::CGraph<GUI_ELEMENT_TYPE>::CreateNode<GUI_ELEMENT_TNODE_TYPE>();
	// Create actual button, and pass proper pointer to transform node
	ELEMENT_TYPE *pElement = new ELEMENT_TYPE();
	pElement->SetTransformNode(pNode);
	// Make element managed by gui element resourcemanager
	assert( (Phoenix::Core::CResourceManager< BASE_COMPONENT_TYPE, 
		Phoenix::Core::CHandle<BASE_COMPONENT_TYPE> >::GetInstance()->
		 Create( pElement, szResourceName, pNode->GetHandle() ) == 0) && "GUI System: Conflicting element name or Out Of Memory!");
	return pElement;
      }
      ////////////////////
      /// Deletes given element from GUI.
      /// \param pPtr Pointer to an element to be deleted.
      void Delete( BASE_COMPONENT_TYPE *pPtr )
      {
	assert( pPtr != NULL && "GuiSystem: delete failed, ptr NULL");
	
	// A hack; THIS SHOULD NEVER BE DONE IN THIS WAY.
	// But we use only the index of handle to destroy object; 
	// handle gets destroyed when transform node is destroyed, so we can't really use it directly.
	// Also the handle is never registered to resource manager and never used outside this scope,
	// so we're on the safe side.
	Phoenix::Core::CHandle<BASE_COMPONENT_TYPE> hTmp = pPtr->GetTransformNode()->GetHandle();
	// Delete transform node from graph.
        Phoenix::Core::CGraph<GUI_ELEMENT_TYPE>::DeleteNode( static_cast< CGraphNode<GUI_ELEMENT_TYPE> *>(pPtr->GetTransformNode()));
	// Delete actual object from manager using temp handle.
        Phoenix::Core::CResourceManager< BASE_COMPONENT_TYPE, 
   	         Phoenix::Core::CHandle<BASE_COMPONENT_TYPE> >::GetInstance()->Destroy( hTmp );
	
      }
      ////////////////////
      /// Registers a receiver.
      void RegisterReceiver( const GUI_MESSAGE_TYPES & tType, BASE_COMPONENT_TYPE & rPtr )
      {
	m_msgRouter.RegisterReceiver( tType, rPtr.GetTransformNode()->GetHandle() );
      }
      ////////////////////
      /// Sends mouse click event.
      void EnqueueMouseClick( const Phoenix::Math::CVector2<int> & vPosition, int iButton )
      {
 	m_msgRouter.EnqueueMessage( new CMouseClickEvent<BASE_COMPONENT_TYPE>( vPosition, iButton ) );
      }
      ////////////////////
      /// Sends mouse down event.
      void EnqueueMouseDown( const Phoenix::Math::CVector2<int> & vPosition, int iButton )
      {
 	m_msgRouter.EnqueueMessage( new CMouseDownEvent<BASE_COMPONENT_TYPE>( vPosition, iButton ) );
      }
      ////////////////////
      /// Sends mouse up event.
      void EnqueueMouseUp( const Phoenix::Math::CVector2<int> & vPosition, int iButton )
      {
 	m_msgRouter.EnqueueMessage( new CMouseUpEvent<BASE_COMPONENT_TYPE>( vPosition, iButton ) );
      }
      ////////////////////
      /// Sends mouse motion event.
      void EnqueueMouseMotion( const Phoenix::Math::CVector2<int> & vPosition, int iButton  )
      {
	m_msgRouter.EnqueueMessage( new CMouseMotionEvent<BASE_COMPONENT_TYPE>( vPosition, iButton ) );
      }
      ////////////////////
      /// Sends all visible components into renderer adapter.
      template <class RENDERER_ADAPTER_TYPE> void Render( CGuiRenderAdapter<BASE_COMPONENT_TYPE, RENDERER_ADAPTER_TYPE> & rAdapter)
      {
	TravelDF<CGuiRenderAdapter<BASE_COMPONENT_TYPE, RENDERER_ADAPTER_TYPE>, GUI_ELEMENT_TYPE, 
	std::string, int>(  static_cast<CGraphNode<GUI_ELEMENT_TYPE> *>(m_pBaseWindow->GetTransformNode()), &rAdapter );
      }
      ////////////////////
      /// Returns reference to message router.
      /// \returns Message router.
      Phoenix::AI::CMessageRouter< BASE_COMPONENT_TYPE, GUI_MESSAGE_TYPES > & GetMessageRouter()
      {
	return m_msgRouter;
      }
    };
  } // namespace Gui
} // namespace Phoenix
#undef GUI_ELEMENT_TNODE_TYPE
/////////////////////////////////////////////////////////////////
#endif
