#include <UnitTest++/UnitTest++.h>
#include <PhoenixMessageSystem.h>
#include <string>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::AI;
using namespace Phoenix::Core;
/////////////////////////////////////////////////////////////////

class TestEvent : public CMessage
{
public:
  TestEvent(){}
};

class TestEvent2 : public CMessage
{
public:
  TestEvent2(){}
};


class EventTestObject
{
public:
  int testValue; 
  int testValue2;
  EventTestObject() : testValue(0), testValue2(0)
  {
    CMessageQueue::GetInstance()->RegisterHandler(this, &EventTestObject::HandleTestEvent);
    CMessageQueue::GetInstance()->RegisterHandler(this, &EventTestObject::HandleTestEventSomething);
  }

  void HandleTestEvent( const TestEvent *pTestEvent )
  {
    testValue = 1;
  }

  void HandleTestEventSomething( const TestEvent2 *pTestEvent )
  {
    testValue2 = 2;
  }
};
  
TEST( PhoenixEventHandler )
{
  
  EventTestObject obj;;
  TestEvent *event = new TestEvent();
  TestEvent2 *event2 = new TestEvent2();

  CMessageQueue::GetInstance()->Prepare();
  CMessageQueue::GetInstance()->EnqueueMessage( event, CTimeStamp(0,0) );
  CMessageQueue::GetInstance()->EnqueueMessage( event2, CTimeStamp(0,0) );
  CMessageQueue::GetInstance()->Update();
  
  ////////////////////
  CHECK_EQUAL( 1, obj.testValue );
  CHECK_EQUAL( 2, obj.testValue2 );

}
/////////////////////////////////////////////////////////////////
