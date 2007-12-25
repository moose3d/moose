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
    cout << rMessage.GetTimeStamp() << "|" 
	 << ConvertToString(rMessage.GetType()) << ":"  
	 << g_IntManager->GetResourceName(rMessage.GetReceiver())
	 << " is processing message received from  "  
	 << g_IntManager->GetResourceName(rMessage.GetSender()) << endl;
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
  
  CMessage<int, MESSAGE_TYPE> *pMsg = new CMessage<int, MESSAGE_TYPE>();
  g_IntManager->DuplicateHandle( hFirst,  pMsg->GetReceiver());
  g_IntManager->DuplicateHandle( hSecond, pMsg->GetSender());
  pMsg->SetType( MSG_FIRST );

  CMessage<int, MESSAGE_TYPE> *pMsg2 = new CMessage<int, MESSAGE_TYPE>();
  g_IntManager->DuplicateHandle( hSecond,  pMsg2->GetReceiver());
  g_IntManager->DuplicateHandle( hSecond,  pMsg2->GetSender());
  pMsg2->SetType( MSG_SECOND );

  messageRouter.Prepare();  
  messageRouter.EnqueueMessage( pMsg2, CTimeStamp(2,250));  
  messageRouter.EnqueueMessage( pMsg );  
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
