#include <Phoenix.h>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
using namespace Phoenix::Scene;
using namespace Phoenix::Volume;
using namespace Phoenix::AI;
using namespace std;
/////////////////////////////////////////////////////////////////
enum MESSAGE_TYPE
{
  MSG_FIRST = 0,
  MSG_SECOND = 1,
  MSG_NUM_MESSAGE_TYPES
};
/////////////////////////////////////////////////////////////////
#define g_IntManager ( CResourceManager<int, CHandle<int> >::GetInstance())
/////////////////////////////////////////////////////////////////
class CFirstMessage : public CMessage<int, MESSAGE_TYPE>
{
protected:
  std::string m_strData;
public:
  CFirstMessage() 
  {
    SetType( MSG_FIRST );
    m_strData = "first message";
  }
  friend std::ostream & operator<<( std::ostream & stream, const CFirstMessage & msg )
  {
    stream << msg.GetTimeStamp() << ": FIRST, from " 
	   << g_IntManager->GetResourceName(msg.GetSender())
	   << " to " 
	   << g_IntManager->GetResourceName(msg.GetReceiver())
	   << " saying " << msg.m_strData;
    return stream;
  }
};
/////////////////////////////////////////////////////////////////
class CSecondMessage : public CMessage<int, MESSAGE_TYPE>
{
protected:
  float  m_fData;
public:
  CSecondMessage() 
  {
    SetType( MSG_SECOND );
    m_fData =  3.5f;
  }
  friend std::ostream & operator<<( std::ostream & stream, const CSecondMessage & msg )
  {
    stream << msg.GetTimeStamp() << ": FIRST, from " 
	   << g_IntManager->GetResourceName(msg.GetSender())
	   << " to " 
	   << g_IntManager->GetResourceName(msg.GetReceiver())
	   << " saying " << msg.m_fData;
    return stream;
  }

};
/////////////////////////////////////////////////////////////////
class CMessageAdapter
{

public:
  const std::string ConvertToString( MESSAGE_TYPE type )
  {
    switch( type )
    {
    case MSG_FIRST:
      return std::string("FIRST");
      break;
    case MSG_SECOND:
      return std::string("SECOND");
    default:
      return std::string("UNKNOWN");
      break;
    }

  }
  void Process( const CMessage<int, MESSAGE_TYPE> &rMessage, int &rInt )
  {
    
    switch( rMessage.GetType() )
    {
    case MSG_FIRST:
      cout << static_cast<const CFirstMessage & >(rMessage) << endl;
      break;
    case MSG_SECOND:
      cout << static_cast<const CSecondMessage & >(rMessage) << endl;
      break;
    default:
      cout << "UNKNOWN MESSAGE" << endl;
      break;
    }


  }
  


};
/////////////////////////////////////////////////////////////////
int main()
{
  CHandle<int> hFirst, hSecond;
  int *pFirst, *pSecond;
  pFirst = new int();
  pSecond = new int();
  *pFirst = 1;
  *pSecond = 2;
  
  g_IntManager->Create( pFirst,  "nro 1", hFirst  );
  g_IntManager->Create( pSecond, "nro 2", hSecond );
  
  CMessageRouter< int, MESSAGE_TYPE > messageRouter(MSG_NUM_MESSAGE_TYPES);
  messageRouter.RegisterReceiver( MSG_FIRST, hFirst );
  messageRouter.RegisterReceiver( MSG_SECOND, hSecond );
  messageRouter.RegisterReceiver( MSG_SECOND, hFirst );
  
  // Send message to #2 from #1
  CFirstMessage *pMsg = new CFirstMessage();
  g_IntManager->DuplicateHandle( hFirst,  pMsg->GetReceiver());
  g_IntManager->DuplicateHandle( hSecond, pMsg->GetSender());

  // Send message to self from #2
  CSecondMessage *pMsg2 = new CSecondMessage();
  g_IntManager->DuplicateHandle( hSecond,  pMsg2->GetReceiver());
  g_IntManager->DuplicateHandle( hSecond,  pMsg2->GetSender());

  // Send message to all from #1
  CSecondMessage *pMsg3 = new CSecondMessage();
  g_IntManager->DuplicateHandle( hFirst,  pMsg3->GetSender());

  messageRouter.Prepare();  
  messageRouter.EnqueueMessage( pMsg2, CTimeStamp(2,250));  
  messageRouter.EnqueueMessage( pMsg );  
  messageRouter.EnqueueMessage( pMsg3, CTimeStamp(3,500) );  

  CMessageAdapter msgAdapter;
  cerr << "starting ..." << endl;
  while( 1 ) 
  {
    messageRouter.Update( msgAdapter );
    usleep(1000);
  }

  return 0;
}
/////////////////////////////////////////////////////////////////
