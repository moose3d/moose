#include <UnitTest++/UnitTest++.h>
#include <MooseMessageSystem.h>
#include <string>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Moose::AI;
using namespace Moose::Core;
/////////////////////////////////////////////////////////////////
class MyQueue : public CMessageQueue,
                public CSingleton<MyQueue>
{
  friend class CSingleton<MyQueue>;
};
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
    MyQueue::GetInstance()->RegisterHandler(this, &EventTestObject::HandleTestEvent);
    MyQueue::GetInstance()->RegisterHandler(this, &EventTestObject::HandleTestEventSomething);
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
  
TEST( MooseEventHandler )
{
  
  EventTestObject obj;;
  TestEvent *event = new TestEvent();
  TestEvent2 *event2 = new TestEvent2();

  MyQueue::GetInstance()->Prepare();
  MyQueue::GetInstance()->EnqueueMessage( event, CTimeStamp(0,0) );
  MyQueue::GetInstance()->EnqueueMessage( event2, CTimeStamp(0,0) );
  MyQueue::GetInstance()->Update();
  
  ////////////////////
  CHECK_EQUAL( 1, obj.testValue );
  CHECK_EQUAL( 2, obj.testValue2 );

}
/////////////////////////////////////////////////////////////////
