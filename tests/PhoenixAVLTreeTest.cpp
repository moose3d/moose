#include "../src/PhoenixAVLTree.h"
#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <math.h>
#include <assert.h>
using namespace Phoenix::Core::AVLTree;
#define IntStringKeyPair KeyValue<std::string>
/////////////////////////////////////////////////////////////////
class Handler 
{
public:
  int maxheight;
  int previous;
  int first;
  Handler()
  {
    first = 1;
    maxheight = 0;
    previous = 0;
  }
  void Handle( CAVLTreeNode< IntStringKeyPair > &node )
  {
    if ( first )
    {
      first = 0;
      previous = node.GetKeyValue().GetKey();
    } 
    else
    {
      assert( (previous + 1) == node.GetKeyValue().GetKey() && "Invalid key order!");
      previous = node.GetKeyValue().GetKey();
    }
    if ( node.GetHeight() > maxheight )  maxheight = node.GetHeight();
  }
};
TEST( AVLTree )
{
  const int MAX_TEST_NODES = 2000000;
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
  std::cerr << "tree height: " << handler.maxheight << std::endl;

}
/////////////////////////////////////////////////////////////////
