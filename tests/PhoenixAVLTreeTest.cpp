#include "../src/PhoenixAVLTree.h"
#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <math.h>
#include <assert.h>
using namespace Phoenix::Core::AVLTree;
using namespace std;
const int MAX_TEST_NODES = 5;
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
    std::cerr << node.GetKeyValue().GetKey() << std::endl;
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
};

/////////////////////////////////////////////////////////////////
TEST( AVLTreeInsertLinearReverse )
{
  
  CAVLTree<IntStringKeyPair > *pTree = new  CAVLTree<IntStringKeyPair >(); 
  std::cerr << "inserting, please wait: " << std::endl;
  IntStringKeyPair tKeyValue( 5, std::string("Root of life") );
  pTree->Insert(tKeyValue );
  std::cerr << "insert 5 ok " << endl;
  tKeyValue.SetKey(4);
  pTree->Insert(tKeyValue );
  std::cerr << "insert 4 ok " << endl;
  tKeyValue.SetKey(3);
  pTree->Insert(tKeyValue );
  std::cerr << "insert 3 ok " << endl;
  tKeyValue.SetKey(2);
  pTree->Insert(tKeyValue );
  std::cerr << "insert 2 ok " << endl;
  tKeyValue.SetKey(1);
  pTree->Insert(tKeyValue );
  std::cerr << "insert 1 ok " << endl;
  Handler handler;
  pTree->ProcessInOrder<Handler>( handler, pTree->GetRoot() );
  

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
  std::cerr << "tree height: " << handler.maxheight << std::endl;
  
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
  

}
