#include <UnitTest++/UnitTest++.h>
#include <PhoenixCore.h>
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

}
/////////////////////////////////////////////////////////////////
TEST( PhoenixTimeStamp_Addition )
{
  CTimeStamp tTime(10,100);
  CTimeStamp tTime2(10,200);
  CTimeStamp tTimeRes;  
  ////////////////////
  /// Addition
  tTimeRes = tTime + tTime2;
  CHECK_EQUAL( 20, tTimeRes.GetSeconds());
  CHECK_EQUAL( 300, tTimeRes.GetMilliSeconds());
  
  tTimeRes = tTime2 + tTime;
  CHECK_EQUAL( 20, tTimeRes.GetSeconds());
  CHECK_EQUAL( 300, tTimeRes.GetMilliSeconds());
}
/////////////////////////////////////////////////////////////////
TEST( PhoenixTimeStamp_Subtraction )
{
  CTimeStamp tTime(10,100);
  CTimeStamp tTime2(10,200);
  CTimeStamp tTimeRes;  
  ////////////////////
  /// Subtraction
  tTimeRes = tTime2 - tTime;
  CHECK_EQUAL( 0, tTimeRes.GetSeconds());
  CHECK_EQUAL( 100, tTimeRes.GetMilliSeconds());

  tTimeRes = tTime - tTime2;
  CHECK_EQUAL( -1, tTimeRes.GetSeconds());
  CHECK_EQUAL( 900, tTimeRes.GetMilliSeconds());

}
/////////////////////////////////////////////////////////////////
TEST( PhoenixTimeStamp_LessThan )
{
  ////////////////////
  /// less than-comparisions.
  CHECK( CTimeStamp(10,10) < CTimeStamp(10,20) );
  CHECK( CTimeStamp(10,10) < CTimeStamp(10,11) );
  CHECK( !(CTimeStamp(10,10) < CTimeStamp(10,10)) );
  CHECK( !(CTimeStamp(10,10) < CTimeStamp(10,9)) );
  CHECK( !(CTimeStamp(10,10) < CTimeStamp(10,5)) );

  CHECK( CTimeStamp(0,10) < CTimeStamp(10,10) );
  CHECK( CTimeStamp(9,10) < CTimeStamp(10,10) );
  CHECK( !(CTimeStamp(10,10) < CTimeStamp(10,10)) );
  CHECK( !(CTimeStamp(11,10) < CTimeStamp(10,10)) );
  CHECK( !(CTimeStamp(20,10) < CTimeStamp(10,10)) );
}
/////////////////////////////////////////////////////////////////
TEST( PhoenixTimeStamp_GreaterThan )
{
  ////////////////////
  /// greater than-comparisions.
  CHECK( !(CTimeStamp(10,10) > CTimeStamp(10,20)) );
  CHECK( !(CTimeStamp(10,10) > CTimeStamp(10,11)) );
  CHECK( !(CTimeStamp(10,10) > CTimeStamp(10,10)) );
  CHECK( (CTimeStamp(10,10) > CTimeStamp(10,9)) );
  CHECK( (CTimeStamp(10,10) > CTimeStamp(10,5)) );

  CHECK( !(CTimeStamp(0,10) > CTimeStamp(10,10)) );
  CHECK( !(CTimeStamp(9,10) > CTimeStamp(10,10)) );
  CHECK( !(CTimeStamp(10,10) > CTimeStamp(10,10)) );
  CHECK( (CTimeStamp(11,10) > CTimeStamp(10,10)) );
  CHECK( (CTimeStamp(20,10) > CTimeStamp(10,10)) );
}
/////////////////////////////////////////////////////////////////
TEST( PhoenixTimeStamp_Equal )
{
  ////////////////////
  /// equality-comparisions.
  CHECK( !(CTimeStamp(10,10) == CTimeStamp(10,20)) );
  CHECK( !(CTimeStamp(10,10) == CTimeStamp(10,11)) );
  CHECK(  (CTimeStamp(10,10) == CTimeStamp(10,10)) );
  CHECK( !(CTimeStamp(10,10) == CTimeStamp(10,9)) );
  CHECK( !(CTimeStamp(10,10) == CTimeStamp(10,5)) );

  CHECK( !(CTimeStamp(0,10) == CTimeStamp(10,10)) );
  CHECK( !(CTimeStamp(9,10) == CTimeStamp(10,10)) );
  CHECK( (CTimeStamp(10,10) == CTimeStamp(10,10)) );
  CHECK( !(CTimeStamp(11,10) == CTimeStamp(10,10)) );
  CHECK( !(CTimeStamp(20,10) == CTimeStamp(10,10)) );
}
/////////////////////////////////////////////////////////////////
TEST( PhoenixTimeStamp_GreaterThanOrEqual )
{
  ////////////////////
  /// greater than or equal -comparisions.
  CHECK( !(CTimeStamp(10,10) >= CTimeStamp(10,20)) );
  CHECK( !(CTimeStamp(10,10) >= CTimeStamp(10,11)) );
  CHECK( (CTimeStamp(10,10) >= CTimeStamp(10,10)) );
  CHECK( (CTimeStamp(10,10) >= CTimeStamp(10,9)) );
  CHECK( (CTimeStamp(10,10) >= CTimeStamp(10,5)) );

  CHECK( !(CTimeStamp(0,10) >= CTimeStamp(10,10)) );
  CHECK( !(CTimeStamp(9,10) >= CTimeStamp(10,10)) );
  CHECK( (CTimeStamp(10,10) >= CTimeStamp(10,10)) );
  CHECK( (CTimeStamp(11,10) >= CTimeStamp(10,10)) );
  CHECK( (CTimeStamp(20,10) >= CTimeStamp(10,10)) );
}
/////////////////////////////////////////////////////////////////
TEST( PhoenixTimeStamp_LessThanOrEqual )
{
  ////////////////////
  /// less than or equal -comparisions.
  CHECK( (CTimeStamp(10,10) <= CTimeStamp(10,20)) );
  CHECK( (CTimeStamp(10,10) <= CTimeStamp(10,11)) );
  CHECK( (CTimeStamp(10,10) <= CTimeStamp(10,10)) );
  CHECK( !(CTimeStamp(10,10) <= CTimeStamp(10,9)) );
  CHECK( !(CTimeStamp(10,10) <= CTimeStamp(10,5)) );

  CHECK( (CTimeStamp(0,10) <= CTimeStamp(10,10)) );
  CHECK( (CTimeStamp(9,10) <= CTimeStamp(10,10)) );
  CHECK( (CTimeStamp(10,10) <= CTimeStamp(10,10)) );
  CHECK( !(CTimeStamp(11,10) <= CTimeStamp(10,10)) );
  CHECK( !(CTimeStamp(20,10) <= CTimeStamp(10,10)) );
}
/////////////////////////////////////////////////////////////////
TEST( PhoenixTimeStamp_Assignment )
{
  CTimeStamp tTime(123,55);
  CTimeStamp tTimeRes;
  ////////////////////
  /// Assignment 
  tTimeRes = tTime;
  CHECK_EQUAL( 123,  tTimeRes.GetSeconds());
  CHECK_EQUAL( 55, tTimeRes.GetMilliSeconds());
  CHECK( tTime == tTimeRes );
}
/////////////////////////////////////////////////////////////////
