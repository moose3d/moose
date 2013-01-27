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
using namespace Moose::Core;
/////////////////////////////////////////////////////////////////
TEST( CHashTable )
{
  CHashItem<std::string,unsigned int> tItem;
  CHashTable<std::string,unsigned int> *pTable = new CHashTable<std::string,unsigned int>(7);
  std::string key("twelve");
  unsigned int nObject = 12;
  tItem.SetObject( nObject );
  tItem.SetKey( key );
  // Insert items "oneshot" and "twelve"
  pTable->Insert( tItem );
  CHashItem<std::string,unsigned int> *pItem =  pTable->Find( key );
  CHECK( pItem != NULL );
  if( pItem != NULL )
    CHECK( pItem->GetObject() == 12 );
  
  tItem.SetKey( (key = "oneshot"));
  tItem.SetObject((nObject = 1)) ; 
  pTable->Insert(tItem);
  ////////////////////
  // Find those keys, plus one which should not be there.
  pItem =  pTable->Find(   (key = "twelve") );
  CHECK( pItem != NULL );
  if( pItem != NULL )
    CHECK( pItem->GetObject() == 12 );

  pItem =  pTable->Find(   (key="oneshot") );
  CHECK( pItem != NULL );
  if( pItem != NULL )
    CHECK( pItem->GetObject() == 1 );
  
  pItem =  pTable->Find(   (key="nothere") );
  CHECK( pItem == NULL );
  ////////////////////
  // Delete key "twelve", should leave "oneshot"
  pTable->Delete( (key="twelve"));

  pItem =  pTable->Find(   (key = "twelve") );
  CHECK( pItem == NULL );

  pItem =  pTable->Find(   (key="oneshot") );
  CHECK( pItem != NULL );
  if( pItem != NULL )
    CHECK( pItem->GetObject() == 1 );
  ////////////////////
  // Delete "oneshot", should leave nothing
  pTable->Delete( (key="oneshot"));
  pItem =  pTable->Find(   (key="oneshot") );
  CHECK( pItem == NULL );
  pItem =  pTable->Find(   (key = "twelve") );
  CHECK( pItem == NULL );
  pItem =  pTable->Find(   (key="nothere") );
  CHECK( pItem == NULL );

}


