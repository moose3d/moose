#include <UnitTest++/UnitTest++.h>
#include "PhoenixCore.h"
#include <string>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
/////////////////////////////////////////////////////////////////
TEST( PhoenixTimeStamp )
{

  CTimeStamp tTime(10,100);
  CTimeStamp tTime2(10,200);
  CTimeStamp tTimeRes;  

  ////////////////////
  CHECK_EQUAL( 10, tTime.GetSeconds() );
  CHECK_EQUAL( 10, tTime2.GetSeconds() );
  
  CHECK_EQUAL( 100, tTime.GetMilliSeconds());
  CHECK_EQUAL( 200, tTime2.GetMilliSeconds());
  ////////////////////
  /// Addition
  tTimeRes = tTime + tTime2;
  CHECK_EQUAL( 20, tTimeRes.GetSeconds());
  CHECK_EQUAL( 300, tTimeRes.GetMilliSeconds());
  
  tTimeRes = tTime2 + tTime;
  CHECK_EQUAL( 20, tTimeRes.GetSeconds());
  CHECK_EQUAL( 300, tTimeRes.GetMilliSeconds());
  ////////////////////
  /// Subtraction
  tTimeRes = tTime2 - tTime;
  CHECK_EQUAL( 0, tTimeRes.GetSeconds());
  CHECK_EQUAL( 100, tTimeRes.GetMilliSeconds());

  tTimeRes = tTime - tTime2;
  CHECK_EQUAL( -1, tTimeRes.GetSeconds());
  CHECK_EQUAL( 900, tTimeRes.GetMilliSeconds());
  ////////////////////
  /// Comparisions.
  CHECK( tTime < tTime2 != 0 );
  CHECK( tTime > tTime2 == 0 );
  CHECK( !(tTime == tTime2)  );
  ////////////////////
  /// Assignment 
  tTimeRes = tTime;
  CHECK_EQUAL( 10,  tTimeRes.GetSeconds());
  CHECK_EQUAL( 100, tTimeRes.GetMilliSeconds());
  
  CHECK( tTime == tTimeRes );
}
/////////////////////////////////////////////////////////////////
