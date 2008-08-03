#include <UnitTest++/UnitTest++.h>
#include <PhoenixObjLoader.h>
#include <string>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Data;
using namespace Phoenix::Graphics;
using namespace std;
/////////////////////////////////////////////////////////////////
TEST( PhoenixObjLoader )
{
  CModelLoader *pLoader = new CObjLoader();
  CHECK_EQUAL( 0, pLoader->Load( "box.obj" ) ); 
  CVertexDescriptor *pVertices = pLoader->GetVertexArray();
  cerr << "# vertices loaded: " << pVertices->GetSize() << endl;
  CIndexArray *pIndices = pLoader->GetIndexArray();
  cerr << "# indices : " << pIndices->GetNumIndices() << endl;
  delete pIndices;

  pIndices = pLoader->GetIndexArray( "testgroup1");
  cerr << "# indices in testgroup1: " << pIndices->GetNumIndices() << endl;
  delete pIndices;

  pIndices = pLoader->GetIndexArray( "testgroup2");
  cerr << "# indices in testgroup2: " << pIndices->GetNumIndices() << endl;
  delete pIndices;

  pIndices = pLoader->GetIndexArray( "testgroup3");
  cerr << "# indices in testgroup3: " << pIndices->GetNumIndices() << endl;
  delete pIndices;

  delete pVertices;
  delete pLoader;
}
/////////////////////////////////////////////////////////////////
