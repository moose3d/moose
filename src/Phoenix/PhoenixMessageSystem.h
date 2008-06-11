#ifndef __PhoenixMessageSystem_h__
#define __PhoenixMessageSystem_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixCore.h>
#include <PhoenixResourceManager.h>
#include <iomanip>
#include <algorithm>
#include <list>
#include <map>
#include <queue>
#include <vector>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace AI
  {
    class TypeInfo
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
    /////////////////////////////////////////////////////////////////
    /// Base class for message receivers and senders.
    template <typename OBJECT_TYPE>
    class CMessenger
    {
    private:
      /// Handle to object itself.
      Phoenix::Core::CHandle<OBJECT_TYPE> m_hThis;
    protected:
      ////////////////////
      /// Constructor.
      CMessenger() { } 
    public:
      ////////////////////
      /// Returns handle to object itself - Handle is null handle, unless
      /// object has been registered into CMessageQueue.
      /// \returns Handle to object itself. 
      Phoenix::Core::CHandle<OBJECT_TYPE> & GetObjectHandle() { return m_hThis; }
    };

    ////////////////////
    /// Base for all messages.
    class CMessageBase 
    {
    protected:
      /// Timestamp of this message.
      Phoenix::Core::CTimeStamp	   m_tTimeStamp;
    public:
      ////////////////////
      /// Destructor.
      virtual ~CMessageBase() {} 
      ////////////////////
      /// Assign timestamp.
      /// \param tTimeStamp Timestamp to be set in milliseconds.
      void SetTimeStamp( const Phoenix::Core::CTimeStamp & tTimeStamp );
      ////////////////////
      /// Returns timestamp.
      /// \returns Timestamp.
      const Phoenix::Core::CTimeStamp & GetTimeStamp() const;
    };
    ////////////////////
    /// Base for all messages passed. Child classes may contain 
    /// additional message data, interpreted by message adapters.
    template <typename RECEIVER, typename SENDER>
    class CMessage : public CMessageBase
    {
    protected:
      /// Handle to receiver.
      Phoenix::Core::CHandle<RECEIVER> m_hReceiver;
      /// Handle to sender.
      Phoenix::Core::CHandle<SENDER> m_hSender;
    public:
      ////////////////////
      /// Destructor.
      virtual ~CMessage();
      ////////////////////
      /// Returns reference to handle of sender.
      /// \returns Handle to sender.
      Phoenix::Core::CHandle<SENDER> & GetSender();
      ////////////////////
      /// Returns reference to handle of sender.
      /// \returns Handle to sender.
      const Phoenix::Core::CHandle<SENDER> & GetSender() const;
      ////////////////////
      /// Returns reference to handle of receiver.
      /// \returns Handle to receiver.
      Phoenix::Core::CHandle<RECEIVER> & GetReceiver();
      ////////////////////
      /// Returns reference to handle of receiver.
      /// \returns Handle to receiver.
      const Phoenix::Core::CHandle<RECEIVER> & GetReceiver() const;
    };
    /////////////////////////////////////////////////////////////////
    class CHandlerFunctionBase
    {
    public:
      virtual ~CHandlerFunctionBase() {}
      ////////////////////
      /// 
      inline void     Exec( const Phoenix::AI::CMessageBase * pMessage ) 
      { 
	Call(pMessage);
      }
      virtual void Call( const Phoenix::AI::CMessageBase *pMessage ) = 0;
    };
    /////////////////////////////////////////////////////////////////
    template <class CLASS_TYPE, class MESSAGE_TYPE>
    class CMemberFunctionHandler : public CHandlerFunctionBase
    {
    private:
      Phoenix::Core::CHandle<CLASS_TYPE> m_hHandler;
      typedef void (CLASS_TYPE::* MemberFunc)(MESSAGE_TYPE *);
      MemberFunc m_Function;
      
    public:
      ////////////////////
      /// 
      CMemberFunctionHandler(const Phoenix::Core::CHandle<CLASS_TYPE> & hHandle, MemberFunc memFunc) : m_Function(memFunc) 
      {
	m_hHandler = hHandle;
      };
      ////////////////////
      /// Calls registered event handler.
      /// \param pEvent Pointer to event to be handled.
      void Call(const Phoenix::AI::CMessageBase* pMessage)
      {
	if ( ! m_hHandler.IsNull())
	  ( m_hHandler->*m_Function)(static_cast<MESSAGE_TYPE*>(pMessage));
      }
      ////////////////////
      /// Is object of this handler deleted.
      /// \returns true, if object is null. 
      /// \returns false otherwise.
      bool IsNull() const 
      {
	return m_hHandler.IsNull(); 
      }
      ////////////////////
      /// Returns a handle to handler object.
      /// \returns Handler object handle.
      Phoenix::Core::CHandle<CLASS_TYPE> & GetHandle()
      {
	return m_hHandler;
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Contains a list of handler functions to be called.
    class CHandlerList 
    {
    protected:
      typedef std::list< CHandlerFunctionBase *> Handlers;
      
      Handlers m_lstHandlers;
    public:
      ////////////////////
      /// Returns reference to object handle vector.
      /// \returns List with pointers to handles.
      Handlers & GetHandlers() { return m_lstHandlers; }
      ////////////////////
      /// Constructor.
      CHandlerList();
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
      void SendMessage( const Phoenix::AI::CMessageBase *pMessage);      
    };
    ////////////////////
    /// Message router. 
    class CMessageQueue : protected Phoenix::Core::CTimer
    {
    private:
      // 
      struct Priority 
      {
	bool operator()( const CMessageBase *pFirst, const CMessageBase *pSecond) 
	{
	  return (pFirst->GetTimeStamp() < pSecond->GetTimeStamp());
	}
      };

      typedef std::map<TypeInfo, CHandlerList *> HandlerMap;
      HandlerMap  m_mapHandlers;
      typedef std::priority_queue< CMessageBase *,
				   std::vector< CMessageBase *>,
				   Priority > MessagePriorityQueue;
      MessagePriorityQueue  m_priqMessages;

    public:
      ////////////////////
      /// Destructor.
      ~CMessageQueue();
      ////////////////////
      /// Enqueues message.
      void EnqueueMessage( Phoenix::AI::CMessageBase *pMessage, const Phoenix::Core::CTimeStamp & tTimeStamp);
      ////////////////////
      /// Registers handler for specific event.
      template <class CLASS_TYPE, class MESSAGE_TYPE>
      void RegisterHandler( const Phoenix::Core::CHandle<CLASS_TYPE> & hReceiver, void (CLASS_TYPE::*memFunc)(MESSAGE_TYPE*));

      ////////////////////
      /// Returns pointer to next available message.
      CMessageBase * GetNextMessage();

      void Update();
      void Prepare();
    };
    
  } // Ai
} // Phoenix
/////////////////////////////////////////////////////////////////
template <class CLASS_TYPE, class MESSAGE_TYPE>
void 
Phoenix::AI::CMessageQueue::RegisterHandler( const Phoenix::Core::CHandle<CLASS_TYPE> & hReceiver, void (CLASS_TYPE::*memFunc)(MESSAGE_TYPE*))
{
  TypeInfo typeInfo = TypeInfo(typeid(MESSAGE_TYPE));
  HandlerMap::iterator it = m_mapHandlers.find(typeInfo);
  if ( it == m_mapHandlers.end())
  {
    // create new handlerlist
    m_mapHandlers[typeInfo] = new CHandlerList();
    // push handler into list.
    m_mapHandlers[typeInfo]->PushReceiver( new Phoenix::AI::CMemberFunctionHandler<CLASS_TYPE, MESSAGE_TYPE>(hReceiver, memFunc) );
  }
  else
  {
    it->second->PushReceiver( new Phoenix::AI::CMemberFunctionHandler<CLASS_TYPE, MESSAGE_TYPE>(hReceiver, memFunc) );
  }
}
/////////////////////////////////////////////////////////////////
template <typename RECEIVER, typename SENDER>
Phoenix::AI::CMessage<RECEIVER,SENDER>::~CMessage()
{
  Phoenix::Core::CResourceManager<RECEIVER, Phoenix::Core::CHandle<RECEIVER> >::GetInstance()->Release( GetReceiver());
  Phoenix::Core::CResourceManager<SENDER,   Phoenix::Core::CHandle<SENDER>   >::GetInstance()->Release( GetSender());
}
/////////////////////////////////////////////////////////////////
template <typename RECEIVER, typename SENDER>
inline Phoenix::Core::CHandle<SENDER> &
Phoenix::AI::CMessage<RECEIVER,SENDER>::GetSender()
{
  return m_hSender;
}
/////////////////////////////////////////////////////////////////
template <typename RECEIVER, typename SENDER>
inline const Phoenix::Core::CHandle<SENDER> &
Phoenix::AI::CMessage<RECEIVER,SENDER>::GetSender() const
{
  return m_hSender;
}
/////////////////////////////////////////////////////////////////
template <typename RECEIVER, typename SENDER>
inline Phoenix::Core::CHandle<RECEIVER> &
Phoenix::AI::CMessage<RECEIVER,SENDER>::GetReceiver()
{
  return m_hReceiver;
}
template <typename RECEIVER, typename SENDER>
inline const Phoenix::Core::CHandle<RECEIVER> &
Phoenix::AI::CMessage<RECEIVER,SENDER>::GetReceiver() const
{
  return m_hReceiver;
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::AI::CMessageBase::SetTimeStamp( const Phoenix::Core::CTimeStamp & tTimeStamp )
{
  m_tTimeStamp = tTimeStamp;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Core::CTimeStamp &
Phoenix::AI::CMessageBase::GetTimeStamp() const
{
  return m_tTimeStamp;
}
/////////////////////////////////////////////////////////////////
Phoenix::AI::CHandlerList::~CHandlerList()
{
  Handlers::iterator it = m_lstHandlers.begin();

  // Release handles 
  while( it != m_lstHandlers.end() )
  {
    // CHECK THAT THIS WORKS!!! 
    delete *it;
    ++it;
  }
  // cleanup
  m_lstHandlers.clear();
}
/////////////////////////////////////////////////////////////////
void
Phoenix::AI::CHandlerList::PushReceiver( const Phoenix::AI::CHandlerFunctionBase *pHandler  )
{
  m_lstHandlers.push_back( const_cast<Phoenix::AI::CHandlerFunctionBase *>(pHandler) );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::AI::CHandlerList::RemoveReceiver( const Phoenix::AI::CHandlerFunctionBase *pHandler )
{
  Handlers::iterator it = m_lstHandlers.begin();
  
  // Release handle if found.
  for( ; it != m_lstHandlers.end(); it++)
  {
    if ( pHandler == *it )
    {
      delete *it;
      m_lstHandlers.erase( it );
      break;
    }
  }
}
/////////////////////////////////////////////////////////////////
void
Phoenix::AI::CHandlerList::SendMessage( const Phoenix::AI::CMessageBase *pMessage )
{
  //   if ( pMessage->GetReceiver().IsNull())
  //   {
  //     Handlers::iterator it = m_lstHandlers.begin();

  //     // send message to each object listening
  //     for( ; it != m_lstHandlers.end(); it++)
  //     {
  //       (*it)->Call( pMessage );
  //     }
  //   }
  //   else
  //   {
  //     Handlers::iterator it = m_lstHandlers.begin();
  //     for( ; it != m_lstHandlers.end(); it++)
  //     {

  // // Send message only to specific receiver
  //       if ( (*it)->GetHandle() == pMessage->GetReceiver())
  //       {
  // 	(*it)->Call( pMessage );
  //        	break;
  //       }
  //     }
  //   }

  Handlers::iterator it = m_lstHandlers.begin();
  
  // send message to each object listening
  for( ; it != m_lstHandlers.end(); it++)
  {
    (*it)->Call( pMessage );
  }

}
/////////////////////////////////////////////////////////////////
#endif
