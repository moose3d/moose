#include <UnitTest++/UnitTest++.h>
#include <PhoenixEventHandler.h>
#include <string>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
/////////////////////////////////////////////////////////////////

class TestEvent : public CEvent
{
public:
  TestEvent(){}
};

class TestEvent2 : public CEvent
{
public:
  TestEvent2(){}
};


class EventTestObject : public CEventHandler
{
public:
  int testValue; 
  int testValue2;
  EventTestObject() : testValue(0), testValue2(0)
  {
    RegisterHandler(this, &EventTestObject::HandleTestEvent);
    RegisterHandler(this, &EventTestObject::HandleTestEventSomething);
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
  
  EventTestObject obj;
  TestEvent *event = new TestEvent();
  TestEvent2 *event2 = new TestEvent2();
  obj.HandleEvent( event );
  obj.HandleEvent( event2 );
  delete event;
  ////////////////////
  CHECK_EQUAL( 1, obj.testValue );
  CHECK_EQUAL( 2, obj.testValue2 );

}
/////////////////////////////////////////////////////////////////
