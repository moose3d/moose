#ifndef __PhoenixMessageSystem_h__
#define __PhoenixMessageSystem_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixCore.h>
#include <PhoenixResourceManager.h>
#include <iomanip>
#include <algorithm>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace AI
  {
    ////////////////////
    /// Base for all messages passed. Child classes may contain 
    /// additional message data, interpreted by message adapters.
    template <typename OBJECT_TYPE, typename MSG_TYPE>
    class CMessage : public Phoenix::Core::CTypeBase<MSG_TYPE>
    {
    protected:
      /// Handle to receiver.
      Phoenix::Core::CHandle<OBJECT_TYPE> m_hReceiver;
      /// Handle to sender.
      Phoenix::Core::CHandle<OBJECT_TYPE> m_hSender;
      /// Timestamp of this message.
      Phoenix::Core::CTimeStamp	   m_tTimeStamp;
    public:
      ////////////////////
      /// Constructor.
      CMessage();
      ////////////////////
      /// Destructor.
      virtual ~CMessage();
      ////////////////////
      /// Returns reference to handle of sender.
      /// \returns Handle to sender.
      Phoenix::Core::CHandle<OBJECT_TYPE> & GetSender();
      ////////////////////
      /// Returns reference to handle of sender.
      /// \returns Handle to sender.
      const Phoenix::Core::CHandle<OBJECT_TYPE> & GetSender() const;
      ////////////////////
      /// Returns reference to handle of receiver.
      /// \returns Handle to receiver.
      Phoenix::Core::CHandle<OBJECT_TYPE> & GetReceiver();
      ////////////////////
      /// Returns reference to handle of receiver.
      /// \returns Handle to receiver.
      const Phoenix::Core::CHandle<OBJECT_TYPE> & GetReceiver() const;
      ////////////////////
      /// Assign timestamp.
      /// \param tTimeStamp Timestamp to be set in milliseconds.
      void SetTimeStamp( const Phoenix::Core::CTimeStamp & tTimeStamp );
      ////////////////////
      /// Returns timestamp.
      /// \returns Timestamp.
      const Phoenix::Core::CTimeStamp & GetTimeStamp() const;
      ////////////////////
      /// \param rMessage Message to compare this against - latter time becomes first.
      /// \returns is this message before the other.
      //bool operator<( const Phoenix::AI::CMessage<OBJECT_TYPE, MSG_TYPE> & rMessage );
    };
    template <typename OBJECT_TYPE, typename MSG_TYPE> class CMessageRouter;
    ////////////////////
    /// ReceiverQueue
    template <typename OBJECT_TYPE, typename MSG_TYPE>
    class CReceiverQueue 
    {
      friend class CMessageRouter<OBJECT_TYPE,MSG_TYPE>;
    protected:
      std::vector< Phoenix::Core::CHandle<OBJECT_TYPE> * > m_vecObjects;
      ////////////////////
      /// Returns reference to object handle vector.
      /// \returns vector with pointers to handles.

    public:
      std::vector< Phoenix::Core::CHandle<OBJECT_TYPE> * > & GetObjectHandles() { return m_vecObjects; }
      ////////////////////
      /// Constructor.
      CReceiverQueue();
      ////////////////////
      /// Destructor.
      ~CReceiverQueue();
      ////////////////////
      /// Adds object as receiver. Another handle is formed from given handle.
      /// \param hObject Handle to object.
      void PushReceiver( const Phoenix::Core::CHandle<OBJECT_TYPE> &hObject );
      ////////////////////
      /// Removes receiver from queue.
      /// \param hObject Handle to object.
      void RemoveReceiver( const Phoenix::Core::CHandle<OBJECT_TYPE> &hObject );
      ////////////////////
      /// Sends message to all (or just to receiver) in this queue.
      /// \param rMessage 
      template <class MSG_ADAPTER_TYPE> 
      void PropagateMessage( const CMessage< OBJECT_TYPE, MSG_TYPE > &rMessage, MSG_ADAPTER_TYPE &rMsgAdapter );
      
      
    };
    ////////////////////
    /// Message router class.
    template <typename OBJECT_TYPE, typename MSG_TYPE>
    class CMessageRouter : protected Phoenix::Core::CTimer
    {
    private:
      /// Struct for priority_queue that sorts messages by timestamp.
      /// Earlier timestamp yieds higher priority.
      struct Priority 
      {
	bool operator()( const CMessage<OBJECT_TYPE, MSG_TYPE> *pFirst,
			 const CMessage<OBJECT_TYPE, MSG_TYPE> *pSecond) 
	{
	  return (pFirst->GetTimeStamp() < pSecond->GetTimeStamp());
	}
      };
    protected:
      /// How many slots there is
      size_t m_nNumSlots;
      /// Receivers for different types of messages.
      std::vector<Phoenix::AI::CReceiverQueue<OBJECT_TYPE, MSG_TYPE> > m_vecMsgReceivers;
      /// Queued messages.
      std::priority_queue< Phoenix::AI::CMessage< OBJECT_TYPE, MSG_TYPE > *, 
      std::vector<Phoenix::AI::CMessage< OBJECT_TYPE, MSG_TYPE > *>, 
      Priority > m_priqMessages;
      ////////////////////
      /// Returns number of reserverd message slots.
      size_t GetSlotCount() const;
    public:
      //////////////////// 
      /// Creates router with specific number of message slots.
      CMessageRouter( size_t nNumSlots );
      ////////////////////
      /// Sets given object to receive all messages typed MSG_TYPE.
      /// \param rType Message type to be received
      /// \param hObject Receiver object handle.
      void RegisterReceiver( const MSG_TYPE & rType, const Phoenix::Core::CHandle<OBJECT_TYPE> &hObject );
      ////////////////////
      /// Unregisters object from receiving messages typed MSG_TYPE.
      /// \param rType Message type to be unregistered.
      /// \param hObject Receiver object handle.
      void UnregisterReceiver( const MSG_TYPE & rType, const Phoenix::Core::CHandle<OBJECT_TYPE> &hObject );
      ////////////////////
      /// Enqueues message with timestamp. 
      /// \param pMessage Pointer to message to be sent.
      /// \param tTimeStamp Timestamp for message delivery. Value indicates time to wait (in millisecond precision) 
      ///        before sending the message, default value is 0, meaning message will be sent immediately.
      void EnqueueMessage( CMessage<OBJECT_TYPE, MSG_TYPE> * pMessage, const Phoenix::Core::CTimeStamp & tTimeStamp = Phoenix::Core::CTimeStamp(0,0) ); 
      ////////////////////
      /// Delivers messages that need it.
      /// \param nMilliSeconds Passed time in milliseconds.
      /// \param rMsgAdapter Message handler. Receives message and object as parameters.
      template <class MSG_ADAPTER_TYPE> void Update( MSG_ADAPTER_TYPE &rMsgAdapter );
      ////////////////////
      /// Returns pointer to next message and removes it from queue.
      /// \returns Message pointer.
      CMessage<OBJECT_TYPE, MSG_TYPE> * GetNextMessage();
      ////////////////////
      /// Starts message routing (resets timer).
      void Prepare();
      ////////////////////
      /// Sorts receiver queues using SORT_FUNC.
      template<typename SORT_FUNC> void SortReceivers( SORT_FUNC & sort_func );
      std::vector<Phoenix::AI::CReceiverQueue<OBJECT_TYPE, MSG_TYPE> > & GetReceivers() { return m_vecMsgReceivers; }
    };
    /////////////////////////////////////////////////////////////////
    /// Base class for message receivers and senders.
    template <typename OBJECT_TYPE, typename MSG_TYPE>
    class CMessageObject
    {
    private:
      /// Handle to object itself.
      Phoenix::Core::CHandle<OBJECT_TYPE> m_hThis;
    protected:
      ////////////////////
      /// Constructor.
      CMessageObject() { } 
    public:
      ////////////////////
      /// Returns handle to object itself - Handle is null handle, unless
      /// object has been registered into CMessageRouter.
      /// \returns Handle to object itself. 
      Phoenix::Core::CHandle<OBJECT_TYPE> & GetObjectHandle() { return m_hThis; }
    };
  } // Core
} // Phoenix

/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::CMessage() 
{
  
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::~CMessage()
{
  Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle<OBJECT_TYPE> >::GetInstance()->Release( GetSender());
  Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle<OBJECT_TYPE> >::GetInstance()->Release( GetReceiver());
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
inline Phoenix::Core::CHandle<OBJECT_TYPE> &
Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::GetSender()
{
  return m_hSender;
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
inline const Phoenix::Core::CHandle<OBJECT_TYPE> &
Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::GetSender() const
{
  return m_hSender;
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
inline Phoenix::Core::CHandle<OBJECT_TYPE> &
Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::GetReceiver()
{
  return m_hReceiver;
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
inline const Phoenix::Core::CHandle<OBJECT_TYPE> &
Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::GetReceiver() const
{
  return m_hReceiver;
}
/////////////////////////////////////////////////////////////////
/* template <typename OBJECT_TYPE, typename MSG_TYPE> */
/* inline bool  */
/* Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::operator<( const Phoenix::AI::CMessage<OBJECT_TYPE, MSG_TYPE> & rMessage ) */
/* { */
/*   return (GetTimeStamp() < rMessage.GetTimeStamp()); */
/* } */
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
inline void 
Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::SetTimeStamp( const Phoenix::Core::CTimeStamp & tTimeStamp )
{
  m_tTimeStamp = tTimeStamp;
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
inline const Phoenix::Core::CTimeStamp &
Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::GetTimeStamp() const
{
  return m_tTimeStamp;
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
Phoenix::AI::CReceiverQueue<OBJECT_TYPE,MSG_TYPE>::CReceiverQueue()
{
  
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
Phoenix::AI::CReceiverQueue<OBJECT_TYPE,MSG_TYPE>::~CReceiverQueue()
{
  typename std::vector< Phoenix::Core::CHandle<OBJECT_TYPE> * >::iterator it;
  it = m_vecObjects.begin();

  // Release handles 
  for( ; it != m_vecObjects.end(); it++)
  {
    Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle<OBJECT_TYPE> >::GetInstance()->Release( *(*it) );
    delete *it;
  }
  // cleanup
  m_vecObjects.clear();
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
void
Phoenix::AI::CReceiverQueue<OBJECT_TYPE,MSG_TYPE>::PushReceiver( const Phoenix::Core::CHandle<OBJECT_TYPE> &hObject )
{

  Phoenix::Core::CHandle<OBJECT_TYPE> *pHandle = new Phoenix::Core::CHandle<OBJECT_TYPE>();
  Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle< OBJECT_TYPE> >::GetInstance()->DuplicateHandle( hObject, *pHandle);
  // Not thread-safe, requires mutex
  m_vecObjects.push_back( pHandle );

}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
void
Phoenix::AI::CReceiverQueue<OBJECT_TYPE,MSG_TYPE>::RemoveReceiver( const Phoenix::Core::CHandle<OBJECT_TYPE> &hObject )
{

  typename std::vector<Phoenix::Core::CHandle<OBJECT_TYPE> *>::iterator it;
  it = m_vecObjects.begin();
  
  // Release handles 
  for( ; it != m_vecObjects.end(); it++)
  {
    // Release and Erase
    if ( hObject == *(*it) )
    {
      Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle<OBJECT_TYPE> >::GetInstance()->Release( *(*it) );      
      delete *it;
      m_vecObjects.erase( it );
      break;
    }
  }
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
template <class MESSAGE_ADAPTER>
void
Phoenix::AI::CReceiverQueue<OBJECT_TYPE,MSG_TYPE>::PropagateMessage( const Phoenix::AI::CMessage<OBJECT_TYPE, MSG_TYPE> &rMessage, MESSAGE_ADAPTER &rMsgAdapter )
{
  typename std::vector<Phoenix::Core::CHandle<OBJECT_TYPE> * >::iterator it;
  it = m_vecObjects.begin();
  OBJECT_TYPE *pObject = NULL;
  if ( rMessage.GetReceiver().IsNull())
  {
    // send message to each object listening
    for( ; it != m_vecObjects.end(); it++)
    {
      if ( (*it)->IsNull() ) continue;
      pObject = Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle< OBJECT_TYPE> >::GetInstance()->GetResource(*(*it));
      if ( pObject != NULL )
      {
	// if adapter explicitly tells to stop passing message any further
	if ( rMsgAdapter.Process( rMessage, *pObject ) ) break;
      }
    }
  }
  else
  {
    // Send message only to specific receiver
    pObject = Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle< OBJECT_TYPE> >::GetInstance()->GetResource(rMessage.GetReceiver());
    if ( pObject != NULL )
    {
      rMsgAdapter.Process( rMessage, *pObject );
    }
  }
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
Phoenix::AI::CMessageRouter<OBJECT_TYPE, MSG_TYPE>::CMessageRouter( size_t nNumSlots )
{
  m_nNumSlots = nNumSlots;
  m_vecMsgReceivers.reserve(nNumSlots);
  for( size_t i=0; i<nNumSlots; i++)
  {
    m_vecMsgReceivers.push_back( CReceiverQueue<OBJECT_TYPE, MSG_TYPE>() );
  }
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
void
Phoenix::AI::CMessageRouter<OBJECT_TYPE, MSG_TYPE>::RegisterReceiver( const MSG_TYPE & rType, const Phoenix::Core::CHandle<OBJECT_TYPE> &hObject )
{
  assert( (unsigned int)rType < GetSlotCount() && "Message slots exceeded - this is not proper type!!!");
  m_vecMsgReceivers[(unsigned int)rType].PushReceiver( hObject );

  // check that object actually exists - it must be derived from type CMessageObject
  assert( hObject.IsNull() == 0);
  OBJECT_TYPE *pPtr = NULL;   
  pPtr = Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle<OBJECT_TYPE> >::GetInstance()->GetResource(hObject);
  Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle<OBJECT_TYPE> >::GetInstance()->DuplicateHandle( hObject, pPtr->GetObjectHandle());
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
inline void
Phoenix::AI::CMessageRouter<OBJECT_TYPE, MSG_TYPE>::UnregisterReceiver( const MSG_TYPE & rType, const Phoenix::Core::CHandle<OBJECT_TYPE> &hObject )
{
  assert( (unsigned int)rType < GetSlotCount() && "Message slots exceeded!!!");
  m_vecMsgReceivers[(unsigned int)rType].RemoveReceiver( hObject );
  
  OBJECT_TYPE *pPtr = NULL;   
  pPtr = Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle<OBJECT_TYPE> >::GetInstance()->GetResource(hObject);
  if ( pPtr != NULL )
  {
    Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle<OBJECT_TYPE> >::GetInstance()->Release( pPtr->GetObjectHandle());
  }
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
inline void 
Phoenix::AI::CMessageRouter<OBJECT_TYPE, MSG_TYPE>::EnqueueMessage( Phoenix::AI::CMessage<OBJECT_TYPE, MSG_TYPE> *pMessage, const Phoenix::Core::CTimeStamp & tTimeStamp )
{
  pMessage->SetTimeStamp( GetCurrentTime()+tTimeStamp );
  m_priqMessages.push( pMessage );
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
Phoenix::AI::CMessage<OBJECT_TYPE, MSG_TYPE> * 
Phoenix::AI::CMessageRouter<OBJECT_TYPE, MSG_TYPE>::GetNextMessage()
{
  //size_t nTime = GetCurrentTime();
  // Check are there any messages left
  if ( m_priqMessages.empty()) return NULL;

  Phoenix::AI::CMessage<OBJECT_TYPE, MSG_TYPE> * pMessage = m_priqMessages.top();
  //std::cerr << "Current time is : " << GetCurrentTime() << " vs. " << pMessage->GetTimeStamp() << std::endl;
  if ( pMessage->GetTimeStamp() <= GetCurrentTime() )
  {
    m_priqMessages.pop();
    return pMessage;
  }
  return NULL;
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
template <class MSG_ADAPTER_TYPE> 
void 
Phoenix::AI::CMessageRouter<OBJECT_TYPE, MSG_TYPE>::Update( MSG_ADAPTER_TYPE &rMsgAdapter )
{
  Phoenix::Core::CTimer::Reset();
  Phoenix::AI::CMessage<OBJECT_TYPE, MSG_TYPE> * pMessage = NULL;
  while ( (pMessage = GetNextMessage()) != NULL )
  {
    m_vecMsgReceivers[ (unsigned int)pMessage->GetType() ].PropagateMessage( *pMessage, rMsgAdapter);
    delete pMessage;
  }
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
inline size_t 
Phoenix::AI::CMessageRouter<OBJECT_TYPE, MSG_TYPE>::GetSlotCount() const
{
  return m_nNumSlots;
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
void
Phoenix::AI::CMessageRouter<OBJECT_TYPE, MSG_TYPE>::Prepare()
{
  Phoenix::Core::CTimer::Reset();
  
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
template< typename SORT_FUNC > 
void 
Phoenix::AI::CMessageRouter<OBJECT_TYPE, MSG_TYPE>::SortReceivers( SORT_FUNC & sort_func)
{
  //SORT_FUNC sort_func;
  typename std::vector<Phoenix::AI::CReceiverQueue<OBJECT_TYPE, MSG_TYPE> >::iterator recv_it;
  recv_it = m_vecMsgReceivers.begin();
  ////////////////////
  // sort all receiver vectors using sort_func:
  for( ; recv_it != m_vecMsgReceivers.end(); recv_it++)
  {
    std::sort( (*recv_it).GetObjectHandles().begin(), (*recv_it).GetObjectHandles().end(), sort_func );
  }
}
/////////////////////////////////////////////////////////////////
#endif
