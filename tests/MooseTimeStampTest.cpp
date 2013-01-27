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
#include <MooseCore.h>
#include <string>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Moose::Core;
/////////////////////////////////////////////////////////////////
TEST( MooseTimeStamp )
{
  CTimeStamp tTime(10,100);
  CTimeStamp tTime2(10,200);

  CTimeStamp tTime3(10.200f);
  CTimeStamp tTime4(10.100f);

  CTimeStamp tTimeRes;  
  
  ////////////////////
  CHECK_EQUAL( 10, tTime.GetSeconds() );
  CHECK_EQUAL( 100, tTime.GetMilliSeconds());

  CHECK_EQUAL( 10, tTime2.GetSeconds() );
  CHECK_EQUAL( 200, tTime2.GetMilliSeconds());
  
  CHECK_EQUAL( 10, tTime3.GetSeconds() );
  CHECK_EQUAL( 200, tTime3.GetMilliSeconds());
  
  CHECK_EQUAL( 10, tTime4.GetSeconds() );
  CHECK_EQUAL( 100, tTime4.GetMilliSeconds());
}
/////////////////////////////////////////////////////////////////
TEST( MooseTimeStamp_Addition )
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
TEST( MooseTimeStamp_Subtraction )
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
TEST( MooseTimeStamp_LessThan )
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
TEST( MooseTimeStamp_GreaterThan )
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
TEST( MooseTimeStamp_Equal )
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
TEST( MooseTimeStamp_GreaterThanOrEqual )
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
TEST( MooseTimeStamp_LessThanOrEqual )
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
TEST( MooseTimeStamp_Assignment )
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
