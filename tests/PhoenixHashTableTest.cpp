#include <UnitTest++/UnitTest++.h>
#include <PhoenixCore.h>
#include <string>
#include <iostream>
using namespace Phoenix::Core;
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


