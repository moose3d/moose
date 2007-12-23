#ifndef __PhoenixStateMachine_h__
#define __PhoenixStateMachine_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixGraph.h>
#include <PhoenixResourceManager.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace AI
  {
    ////////////////////
    /// Base for all messages passed. Child classes may contain 
    /// additional message data, interpreted by message adapters.
    template <typename OBJECT_TYPE, typename MSG_TYPE>
    class CMessage : public CTypeBase<MSG_TYPE>
    {
    protected:
      /// Handle to receiver.
      CHandle<OBJECT_TYPE> m_hReceiver;
      /// Handle to sender.
      CHandle<OBJECT_TYPE> m_hSender;
      /// Timestamp of this message.
      float		   m_fTimeStamp;
    public:
      ////////////////////
      /// Constructor.
      CMessage();
      ////////////////////
      /// Destructor.
      ~CMessage();
      ////////////////////
      /// Returns reference to handle of sender.
      /// \returns Handle to sender.
      CHandle<OBJECT_TYPE> & GetSender();
      ////////////////////
      /// Returns reference to handle of sender.
      /// \returns Handle to sender.
      const CHandle<OBJECT_TYPE> & GetSender() const;
      ////////////////////
      /// Returns reference to handle of receiver.
      /// \returns Handle to receiver.
      CHandle<OBJECT_TYPE> & GetReceiver();
      ////////////////////
      /// Returns reference to handle of receiver.
      /// \returns Handle to receiver.
      const CHandle<OBJECT_TYPE> & GetReceiver() const;
    };
    ////////////////////
    /// ReceiverQueue
    template <typename OBJECT_TYPE, typename MSG_TYPE>
    class CReceiverQueue 
    {
    protected:
      std::vector<CHandle<OBJECT_TYPE> > m_vecObjects;
    public:
      ////////////////////
      /// Constructor.
      CReceiverQueue();
      ////////////////////
      /// Destructor.
      ~CReceiverQueue();
      ////////////////////
      /// Adds object as receiver. Another handle is formed from given handle.
      /// \param hObject Handle to object.
      void PushReceiver( const CHandle<OBJECT_TYPE> &hObject );
      ////////////////////
      /// Removes receiver from queue.
      /// \param hObject Handle to object.
      void RemoveReceiver( const CHandle<OBJECT_TYPE> &hObject );
      ////////////////////
      /// Sends message to all (or just to receiver) in this queue.
      template <class MSG_ADAPTER_TYPE> void PropagateMessage( const CMessage< OBJECT_TYPE, MSG_TYPE > &rMessage, MSG_ADAPTER_TYPE &rMsgAdapter );
    };
    ////////////////////
    /// Messages 
    template <typename OBJECT_TYPE, typename MSG_TYPE>
    class CMessageRouter
    {
    protected:
      std::vector<Phoenix::AI::CReceiverQueue> m_vecMsgReceivers;
    public:
      
      void RegisterReceiver( const MSG_TYPE & rType, const CHandle<OBJECT_TYPE> &hObject );
      void SendMessage( CMessage<OBJECT_TYPE, MSG_TYPE> &rMessage, float fTimeStamp = 0.0f ); 

      
    };
    ////////////////////
    template<typename STATE_TYPE>
    class CStateMachine 
    {
      
    };
  }
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::CMessage() : m_fTimeStamp(0.0f) 
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
CHandle<OBJECT_TYPE> &
Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::GetSender()
{
  return m_hSender;
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
const CHandle<OBJECT_TYPE> &
Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::GetSender() const
{
  return m_hSender;
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
CHandle<OBJECT_TYPE> &
Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::GetReceiver()
{
  return m_hReceiver;
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
const CHandle<OBJECT_TYPE> &
Phoenix::AI::CMessage<OBJECT_TYPE,MSG_TYPE>::GetReceiver() const
{
  return m_hReceiver;
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
  typename std::vector<CHandle<OBJECT_TYPE> >::iterator it;
  it = m_vecObjects.begin();

  // Release handles 
  for( ; it != m_vecObjects.end(); it++)
  {
    CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle<OBJECT_TYPE> >::GetInstance()->Release( *it );
  }
  // cleanup
  m_vecObjects.clear();
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
void
Phoenix::AI::CReceiverQueue<OBJECT_TYPE,MSG_TYPE>::PushReceiver( const CHandle<OBJECT_TYPE> &hObject )
{
  // Not thread-safe, requires mutex
  m_vecObjects.push_back(CHandle<OBJECT_TYPE>());
  std::string strName = Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle< OBJECT_TYPE> >::GetInstance()->GetResourceName( hObject);
  Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle< OBJECT_TYPE> >::GetInstance()->AttachHandle( strName, m_vecObjects.back());
}
/////////////////////////////////////////////////////////////////
template <typename OBJECT_TYPE, typename MSG_TYPE>
void
Phoenix::AI::CReceiverQueue<OBJECT_TYPE,MSG_TYPE>::RemoveReceiver( const CHandle<OBJECT_TYPE> &hObject )
{

  typename std::vector<CHandle<OBJECT_TYPE> >::iterator it;
  it = m_vecObjects.begin();

  // Release handles 
  for( ; it != m_vecObjects.end(); it++)
  {
    // Release and Erase
    if ( hObject == *it )
    {
      CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle<OBJECT_TYPE> >::GetInstance()->Release( *it );      
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
  typename std::vector<CHandle<OBJECT_TYPE> >::iterator it;
  it = m_vecObjects.begin();
  OBJECT_TYPE *pObject = NULL;
  // send message to each object
  for( ; it != m_vecObjects.end(); it++)
  {
    if ( (*it).IsNull() ) continue;
    pObject = Phoenix::Core::CResourceManager<OBJECT_TYPE, Phoenix::Core::CHandle< OBJECT_TYPE> >::GetInstance()->GetResource(*it);
    if ( pObject != NULL )
    {
      rMsgAdapter.Process( rMessage, *pObject );
    }
  }
}
/////////////////////////////////////////////////////////////////
#endif
