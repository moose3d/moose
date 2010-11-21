#include <UnitTest++/UnitTest++.h>
#include <MooseObjLoader.h>
#include <string>
#include <iostream>
#include <cassert>
/////////////////////////////////////////////////////////////////
using namespace Moose::Data;
using namespace Moose::Graphics;
using namespace std;
/////////////////////////////////////////////////////////////////
TEST( MooseObjLoader )
{
  CModelLoader *pLoader = new CObjLoader();

  CHECK_EQUAL( 0, pLoader->Load( "box.obj" ) ); 

  CVertexDescriptor *pVertices = pLoader->GetVertexArray();
  cerr << "# vertices loaded: " << pVertices->GetSize() << endl;
  CIndexArray *pIndices = pLoader->GetIndexArray();
  assert( pIndices );
  cerr << "# indices : " << pIndices->GetNumIndices() << endl;
  delete pIndices;
    
  pIndices = pLoader->GetIndexArray( "testgroup1");
  if ( pIndices != NULL )
    cerr << "# indices in testgroup1: " << pIndices->GetNumIndices() << endl;
  delete pIndices;

  pIndices = pLoader->GetIndexArray( "testgroup2");
  if ( pIndices != NULL )
    cerr << "# indices in testgroup2: " << pIndices->GetNumIndices() << endl;
  delete pIndices;

  pIndices = pLoader->GetIndexArray( "testgroup3");
  if ( pIndices != NULL )
    cerr << "# indices in testgroup3: " << pIndices->GetNumIndices() << endl;
  delete pIndices;

  delete pVertices;   
    
  delete pLoader;

}
/////////////////////////////////////////////////////////////////
