#include <UnitTest++/UnitTest++.h>
#include "PhoenixCore.h"
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

  pTable->Insert( tItem );
  CHashItem<std::string,unsigned int> *pItem =  pTable->Find( key );
  CHECK( pItem != NULL );
  if( pItem != NULL )
    CHECK( pItem->GetObject() == 12 );
  
  tItem.SetKey( (key = "oneshot"));
  tItem.SetObject((nObject = 1)) ; 
  pTable->Insert(tItem);

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
}


