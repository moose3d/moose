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

#include <MooseAVLTree.h>
#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <math.h>
#include <assert.h>
/////////////////////////////////////////////////////////////////
using namespace Moose::Core::AVLTree;
using namespace std;
const int MAX_TEST_NODES = 5;
/////////////////////////////////////////////////////////////////
#define IntStringKeyPair KeyValue<std::string>
/////////////////////////////////////////////////////////////////
class Handler 
{
public:
  int maxheight;
  int previous;
  int first;
  int failure;
  Handler()
  {
    first = 1;
    maxheight = 0;
    previous = 0;
    failure = 0;
  }
  void Handle( CAVLTreeNode< IntStringKeyPair > &node )
  {
    //std::cerr << node.GetKeyValue().GetKey() << std::endl;
    if ( first )
    {
      first = 0;
      previous = node.GetKeyValue().GetKey();
    } 
    else
    {
      previous = node.GetKeyValue().GetKey();
    }
    if ( node.GetHeight() > maxheight )  maxheight = node.GetHeight();
  }
  void Reset()
  {
    first = 1;
    failure = 0;
  }
  bool IsFailed() 
  {
    return failure;
  }
};

/////////////////////////////////////////////////////////////////
TEST( AVLTreeInsertLinearReverse )
{
  
  CAVLTree<IntStringKeyPair > *pTree = new  CAVLTree<IntStringKeyPair >(); 
  std::cerr << "inserting, please wait: " << std::endl;
  IntStringKeyPair tKeyValue( 5, std::string("Root of life") );
  CHECK( pTree->Insert(tKeyValue ) == 0 );
  tKeyValue.SetKey(4);
  CHECK( pTree->Insert(tKeyValue ) == 0);
  tKeyValue.SetKey(3);
  CHECK( pTree->Insert(tKeyValue ) ==  0);
  tKeyValue.SetKey(2);
  CHECK( pTree->Insert(tKeyValue ) == 0 );
  tKeyValue.SetKey(1);
  CHECK( pTree->Insert(tKeyValue ) == 0);
  Handler handler;
  pTree->ProcessInOrder<Handler>( handler, pTree->GetRoot() );
  CHECK( handler.IsFailed() == 0);
  CHECK( 1.44f*(logf(MAX_TEST_NODES)/logf(2)) >= pTree->GetRoot()->GetHeight()  );
}
/////////////////////////////////////////////////////////////////
TEST( AVLTreeInsertLinear )
{
  IntStringKeyPair tKeyValue( 0, std::string("Root of life") );
  CAVLTree<IntStringKeyPair > *pTree = new  CAVLTree<IntStringKeyPair >(); 
  std::cerr << "inserting, please wait: " << std::endl;
  for( int i=0;i<MAX_TEST_NODES;i++)
  {
    if ( i%10000 == 0 ) std::cerr << ".";
    IntStringKeyPair tKeyValue( i, std::string("Child of life") );  
    pTree->Insert(tKeyValue );
  }
  std::cerr << std::endl;
  Handler handler;
  pTree->ProcessInOrder<Handler>( handler, pTree->GetRoot() );
  CHECK( 1.44f*(logf(MAX_TEST_NODES)/logf(2)) >= pTree->GetRoot()->GetHeight()  );
  
  
}
/////////////////////////////////////////////////////////////////
TEST( AVLTree_DoubleRotateLeft )
{

  CAVLTree<IntStringKeyPair > *pTree = new  CAVLTree<IntStringKeyPair >(); 

  IntStringKeyPair tKeyValue( 5, std::string("Child of life") );  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(2);  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(8);  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(6);  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(9);  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(7);  
  pTree->Insert(tKeyValue );

  Handler handler;
  pTree->ProcessInOrder<Handler>( handler, pTree->GetRoot() );
  CHECK( handler.IsFailed() == 0);
  CHECK( 1.44f*(logf(6)/logf(2)) >= pTree->GetHeight()  );
  CHECK( pTree->GetRoot()->GetKeyValue().GetKey() == 6);
  CHECK( pTree->GetRoot()->GetLeftChild()->GetKeyValue().GetKey() == 5);
  CHECK( pTree->GetRoot()->GetRightChild()->GetKeyValue().GetKey() == 8);
  CHECK( pTree->GetRoot()->GetLeftChild()->GetLeftChild()->GetKeyValue().GetKey() == 2);
  CHECK( pTree->GetRoot()->GetRightChild()->GetLeftChild()->GetKeyValue().GetKey() == 7);
  CHECK( pTree->GetRoot()->GetRightChild()->GetRightChild()->GetKeyValue().GetKey() == 9);

  CHECK_EQUAL( 1, pTree->GetRoot()->GetRightChild()->GetRightChild()->GetHeight());
  CHECK_EQUAL( 1, pTree->GetRoot()->GetRightChild()->GetLeftChild()->GetHeight());
  CHECK_EQUAL( 1, pTree->GetRoot()->GetLeftChild()->GetLeftChild()->GetHeight());
  CHECK_EQUAL( 2, pTree->GetRoot()->GetRightChild()->GetHeight());
  CHECK_EQUAL( 2, pTree->GetRoot()->GetLeftChild()->GetHeight());
  CHECK_EQUAL( 3, pTree->GetRoot()->GetHeight());
}
/////////////////////////////////////////////////////////////////
TEST( AVLTree_DoubleRotateRight )
{
  
  CAVLTree<IntStringKeyPair > *pTree = new  CAVLTree<IntStringKeyPair >(); 
  IntStringKeyPair tKeyValue( 5, std::string("Child of life") );  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(8);  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(2);  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(4);  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(1);  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(3);  
  pTree->Insert(tKeyValue );
  
  Handler handler;
  pTree->ProcessInOrder<Handler>( handler, pTree->GetRoot() );
  CHECK( handler.IsFailed() == 0);
  CHECK( 1.44f*(logf(6)/logf(2)) >= pTree->GetHeight()  );

}
/////////////////////////////////////////////////////////////////
TEST( AVLTree_Delete )
{
  
  CAVLTree<IntStringKeyPair > *pTree = new  CAVLTree<IntStringKeyPair >(); 
  IntStringKeyPair tKeyValue( 5, std::string("Child of life") );  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(8);  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(2);  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(4);  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(1);  
  pTree->Insert(tKeyValue );
  tKeyValue.SetKey(3);  
  pTree->Insert(tKeyValue );
  CHECK( pTree->GetRoot()->GetKeyValue().GetKey() == 4 );

  CHECK( pTree->Delete( 15 ) != 0 );
  Handler handler;
  pTree->ProcessInOrder<Handler>( handler, pTree->GetRoot() );
  CHECK( handler.IsFailed() == 0);
  handler.Reset();
  CHECK( pTree->Delete( 5 ) == 0 );
  pTree->ProcessInOrder<Handler>( handler, pTree->GetRoot() );
  CHECK( handler.IsFailed() == 0);
  handler.Reset();
  CHECK( pTree->Delete( 8 ) == 0);
  pTree->ProcessInOrder<Handler>( handler, pTree->GetRoot() );
  CHECK( handler.IsFailed() == 0);
  handler.Reset();
  CHECK( pTree->Delete( 2 ) == 0);
  pTree->ProcessInOrder<Handler>( handler, pTree->GetRoot() );
  CHECK( handler.IsFailed() == 0);
  handler.Reset();
  CHECK( pTree->Delete( 4 ) == 0);
  pTree->ProcessInOrder<Handler>( handler, pTree->GetRoot() );
  CHECK( handler.IsFailed() == 0);
  handler.Reset();
  CHECK( pTree->Delete( 1 ) == 0);
  pTree->ProcessInOrder<Handler>( handler, pTree->GetRoot() );
  CHECK( handler.IsFailed() == 0);
  handler.Reset();
  CHECK( pTree->Delete( 3 ) == 0);
  pTree->ProcessInOrder<Handler>( handler, pTree->GetRoot() );
  CHECK( handler.IsFailed() == 0);
  CHECK( 1.44f*(logf(6)/logf(2)) >= pTree->GetHeight() );

}
