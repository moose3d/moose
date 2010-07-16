#ifndef __MooseMessageSystem_h__
#define __MooseMessageSystem_h__
/////////////////////////////////////////////////////////////////
#include <MooseCore.h>
#include <MooseResourceManager.h>
#include <MooseExceptions.h>
#include "MooseAPI.h"
#include <iomanip>
#include <algorithm>
#include <list>
#include <map>
#include <queue>
#include <vector>
#include <typeinfo>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace AI
  {
    class MOOSE_API TypeInfo
    {
    private:
      const std::type_info & m_TypeInfo;
    public:
      explicit TypeInfo(const std::type_info& info) : m_TypeInfo(info) {};

      bool operator < (const TypeInfo& rhs) const
      {
	return m_TypeInfo.before(rhs.m_TypeInfo) != 0;
      }
    };

    ////////////////////
    /// Base type for all messages.
    class MOOSE_API CMessage
    {
    protected:
      /// Timestamp of this message.
      Moose::Core::CTimeStamp	   m_tTimeStamp;
    public:
      ////////////////////
      /// Destructor.
      virtual ~CMessage() {}
      ////////////////////
      /// Assign timestamp.
      /// \param tTimeStamp Timestamp to be set in milliseconds.
      void SetTimeStamp( const Moose::Core::CTimeStamp & tTimeStamp );
      ////////////////////
      /// Returns timestamp.
      /// \returns Timestamp.
      const Moose::Core::CTimeStamp & GetTimeStamp() const;
    };
    /////////////////////////////////////////////////////////////////
    class MOOSE_API CHandlerFunctionBase
    {
    public:
      virtual ~CHandlerFunctionBase() {}
      ////////////////////
      ///
      inline void     Exec( const Moose::AI::CMessage * pMessage )
      {
	Call(pMessage);
      }

      virtual void Call( const Moose::AI::CMessage *pMessage ) = 0;

      virtual void * GetObjectPtr() = 0;
    };
    /////////////////////////////////////////////////////////////////
    /// Handler by ...err.. Handle.
    template <class CLASS_TYPE, class MESSAGE_TYPE>
    class MOOSE_API CMemberFunctionHandler : public CHandlerFunctionBase
    {
    private:
      Moose::Core::CHandle<CLASS_TYPE> m_hHandler;
      typedef void (CLASS_TYPE::* MemberFunc)(MESSAGE_TYPE *);
      MemberFunc m_Function;

    public:
      ////////////////////
      ///
      CMemberFunctionHandler(const Moose::Core::CHandle<CLASS_TYPE> & hHandle, MemberFunc memFunc) : m_Function(memFunc)
      {
	m_hHandler = hHandle;
      };
      ////////////////////
      /// Calls registered event handler.
      /// \param pEvent Pointer to event to be handled.
      void Call(const Moose::AI::CMessage* pMessage)
      {
	if ( ! m_hHandler.IsNull())
	  ( (*m_hHandler)->*m_Function)(static_cast<MESSAGE_TYPE*>(pMessage));
      }
      ////////////////////
      /// Is object of this handler deleted.
      /// \returns true, if object is null.
      /// \returns false otherwise.
      bool IsNull() const
      {
	return m_hHandler.IsNull();
      }
      void * GetObjectPtr()
      {
	return *m_hHandler;
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Handler by object pointer.
    template <class CLASS_TYPE, class MESSAGE_TYPE>
    class MOOSE_API CMemberPtrFunctionHandler : public CHandlerFunctionBase
    {
    private:
      CLASS_TYPE *m_pObj;
      typedef void (CLASS_TYPE::* MemberFunc)(MESSAGE_TYPE *);
      MemberFunc m_Function;

    public:
      ////////////////////
      ///
      CMemberPtrFunctionHandler( CLASS_TYPE *pObj, MemberFunc memFunc) : m_pObj(pObj), m_Function(memFunc)
      {
	if (m_pObj == NULL )  throw Moose::Exceptions::CNullPointerException("CMemberPtrFunctionHandler: Object is NULL!");
      };
      ////////////////////
      /// Calls registered event handler.
      /// \param pEvent Pointer to event to be handled.
      void Call(const Moose::AI::CMessage* pMessage)
      {
	(m_pObj->*m_Function)(static_cast<const MESSAGE_TYPE*>(pMessage));
      }
      ////////////////////
      /// Returns pointer to object.
      void * GetObjectPtr()
      {
	return m_pObj;
      }
    };
    typedef std::list< CHandlerFunctionBase *> Handlers;
    /////////////////////////////////////////////////////////////////
    /// Contains a list of handler functions to be called.
    class MOOSE_API CHandlerList
    {
    protected:

      Handlers m_lstHandlers;
    public:
      ////////////////////
      /// Returns reference to object handle vector.
      /// \returns List with pointers to handles.
      Handlers & GetHandlers() { return m_lstHandlers; }
      ////////////////////
      /// Destructor.
      ~CHandlerList();
      ////////////////////
      /// Adds object as receiver. Another handle is formed from given handle.
      /// \param hObject Handle to object.
      void PushReceiver( const CHandlerFunctionBase * pHandler );
      ////////////////////
      /// Removes receiver from queue.
      /// \param hObject Handle to object.
      void RemoveReceiver( const CHandlerFunctionBase * pHandler );
      ////////////////////
      /// Sends message to all (or just to receiver) in this queue.
      /// \param pMessage Message type.
      void SendMessage( const Moose::AI::CMessage *pMessage);
    };
    ////////////////////
    /// Message router for a single object.
    typedef std::pair< Moose::AI::CHandlerFunctionBase *, Moose::AI::CMessage *> SoleMessageHandlerPair;

    class MOOSE_API CMessageQueue : protected Moose::Core::CTimer
    {

    private:

      struct Priority
      {
	bool operator()( const CMessage *pFirst, const CMessage *pSecond)
	{
	  return (pFirst->GetTimeStamp() > pSecond->GetTimeStamp());
	}
      };


      /*struct SoleMessagePriority
      {
	bool operator()( const std::pair<CHandlerFunctionBase *, CMessage *> * first, const std::pair<CHandlerFunctionBase *, CMessage *>  * second)
	{
	  return (first->second->GetTimeStamp() > second->second->GetTimeStamp());
	}
        };*/

      typedef std::map<TypeInfo, CHandlerList *> HandlerMap;
      HandlerMap  m_mapHandlers;
      typedef std::priority_queue< CMessage *,
				   std::vector< CMessage *>,
				   Priority > MessagePriorityQueue;
      /*typedef std::priority_queue< SoleMessageHandlerPair *,
				   std::vector< SoleMessageHandlerPair *>,
				   SoleMessagePriority > SoleMessagePriorityQueue;*/

      MessagePriorityQueue	   m_priqMessages;
      /// Message queue for sole messages ( sent only to a single objects )
      //SoleMessagePriorityQueue	   m_priqSoleMessages;
    public:
        CMessageQueue() {}
      ////////////////////
      /// Destructor.
      ~CMessageQueue();
    

      ////////////////////
      /// Enqueues message. Time complexity is O( log number of message types registered )
      void EnqueueMessage( Moose::AI::CMessage *pMessage,
			   const Moose::Core::CTimeStamp & tTimeStamp);
      ////////////////////
      /// Enqueues sole message. Time complexity is O( number of objects handling same type of event).
      /*template <class CLASS_TYPE >
      void EnqueueMessage( Moose::AI::CMessage *pMessage,
			   const Moose::Core::CTimeStamp & tTimeStamp,
			   const Moose::Core::CHandle<CLASS_TYPE> & hReceiver );*/

      ////////////////////
      /// Enqueues sole message. Time complexity is O( number of objects handling same type of event).
      /*template <class CLASS_TYPE >
      void EnqueueMessage( Moose::AI::CMessage *pMessage,
			   const Moose::Core::CTimeStamp & tTimeStamp,
			   CLASS_TYPE * pObj );*/

      ////////////////////
      /// Registers handler for specific event using handle.
      template <class CLASS_TYPE, class MESSAGE_TYPE>
      void RegisterHandler( const Moose::Core::CHandle<CLASS_TYPE> & hReceiver,
			    void (CLASS_TYPE::*memFunc)(MESSAGE_TYPE*));

      ////////////////////
      /// Registers handler for specific event using pointer to object.
      template <class CLASS_TYPE, class MESSAGE_TYPE>
      void RegisterHandler( CLASS_TYPE *pObj,
			    void (CLASS_TYPE::*memFunc)(MESSAGE_TYPE*));
      void Update();
      void Prepare();
    private:
      ////////////////////
      /// Returns pointer to next available message.
      CMessage * GetNextMessage();
      ////////////////////
      /// Returns next available sole message/handler pair.
      //SoleMessageHandlerPair * GetNextSoleMessage();
    };

  } // Ai
} // Moose
/////////////////////////////////////////////////////////////////
template <class CLASS_TYPE, class MESSAGE_TYPE>
void
Moose::AI::CMessageQueue::RegisterHandler( const Moose::Core::CHandle<CLASS_TYPE> & hReceiver, void (CLASS_TYPE::*memFunc)(MESSAGE_TYPE*))
{
  TypeInfo typeInfo = TypeInfo(typeid(MESSAGE_TYPE));
  HandlerMap::iterator it = m_mapHandlers.find(typeInfo);
  if ( it == m_mapHandlers.end())
  {
    // create new handlerlist
    m_mapHandlers[typeInfo] = new CHandlerList();
    // push handler into list.
    m_mapHandlers[typeInfo]->PushReceiver( new Moose::AI::CMemberFunctionHandler<CLASS_TYPE, MESSAGE_TYPE>(hReceiver, memFunc) );
  }
  else
  {
    it->second->PushReceiver( new Moose::AI::CMemberFunctionHandler<CLASS_TYPE, MESSAGE_TYPE>(hReceiver, memFunc) );
  }
}
/////////////////////////////////////////////////////////////////
template <class CLASS_TYPE, class MESSAGE_TYPE>
void
Moose::AI::CMessageQueue::RegisterHandler( CLASS_TYPE * pObj, void (CLASS_TYPE::*memFunc)(MESSAGE_TYPE*))
{
  TypeInfo typeInfo = TypeInfo(typeid(MESSAGE_TYPE));
  HandlerMap::iterator it = m_mapHandlers.find(typeInfo);
  if ( it == m_mapHandlers.end())
  {
    // create new handlerlist
    m_mapHandlers[typeInfo] = new CHandlerList();
    // push handler into list.
    m_mapHandlers[typeInfo]->PushReceiver( new Moose::AI::CMemberPtrFunctionHandler<CLASS_TYPE, MESSAGE_TYPE>(pObj, memFunc) );
  }
  else
  {
    it->second->PushReceiver( new Moose::AI::CMemberPtrFunctionHandler<CLASS_TYPE, MESSAGE_TYPE>(pObj, memFunc) );
  }
}
/////////////////////////////////////////////////////////////////
/*template <class CLASS_TYPE >
void
Moose::AI::CMessageQueue::EnqueueMessage( Moose::AI::CMessage *pMessage,
					    const Moose::Core::CTimeStamp & tTimeStamp,
					    const Moose::Core::CHandle<CLASS_TYPE> & hReceiver )
{
  pMessage->SetTimeStamp(GetTime() + tTimeStamp);
  // Find handler for message
  HandlerMap::iterator it = m_mapHandlers.find(TypeInfo(typeid(*pMessage)));
  if ( it != m_mapHandlers.end())
  {
    // seek proper handler object; this causes linear complexity, better way would be to
    // create a map or AVLTree for handlers.
    Handlers::iterator itHandlerFunc = (*it).second->GetHandlers().begin();
    Handlers::iterator itEnd = (*it).second->GetHandlers().end();
    for( ; itHandlerFunc != itEnd; itHandlerFunc++)
    {
      if ( (*itHandlerFunc)->GetObjectPtr() == (void *)*hReceiver)
      {
	m_priqSoleMessages.push( new SoleMessageHandlerPair( (*itHandlerFunc), pMessage ) );
	return;
      }
    }
    // if we get here, no handler was found
    delete pMessage;
  }
  else
  {
    delete pMessage;
  }

  }*/
/////////////////////////////////////////////////////////////////
 /*template <class CLASS_TYPE >
void
Moose::AI::CMessageQueue::EnqueueMessage( Moose::AI::CMessage *pMessage,
					    const Moose::Core::CTimeStamp & tTimeStamp,
					    CLASS_TYPE * pObj )
{
  pMessage->SetTimeStamp(GetTime() + tTimeStamp);
  // Find handler for message
  HandlerMap::iterator it = m_mapHandlers.find(TypeInfo(typeid(*pMessage)));
  if ( it != m_mapHandlers.end())
  {
    // seek proper handler object; this causes linear complexity, better way would be to
    // create a map or AVLTree for handlers.
    Handlers::iterator itHandlerFunc = (*it).second->GetHandlers().begin();
    Handlers::iterator itEnd = (*it).second->GetHandlers().end();
    for( ; itHandlerFunc != itEnd; itHandlerFunc++)
    {
      if ( (*itHandlerFunc)->GetObjectPtr() == (void *)pObj)
      {
	m_priqSoleMessages.push( new SoleMessageHandlerPair( (*itHandlerFunc), pMessage ) );
	return;
      }
    }
    // if we get here, no handler was found
    delete pMessage;
  }
  else
  {
    delete pMessage;
  }

  }*/
/////////////////////////////////////////////////////////////////
inline void
Moose::AI::CMessage::SetTimeStamp( const Moose::Core::CTimeStamp & tTimeStamp )
{
  m_tTimeStamp = tTimeStamp;
}
/////////////////////////////////////////////////////////////////
inline const Moose::Core::CTimeStamp &
Moose::AI::CMessage::GetTimeStamp() const
{
  return m_tTimeStamp;
}
/////////////////////////////////////////////////////////////////
#endif
