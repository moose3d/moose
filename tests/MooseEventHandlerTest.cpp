/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

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
