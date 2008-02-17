#include <PhoenixModelHelper.h>
#include <PhoenixMilkshapeLoader.h>
#include <PhoenixDefaultEntities.h>
#include <sstream>
#include <string>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
using namespace Phoenix::Data;
using std::ostringstream;
using std::string;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CModel *
Phoenix::Data::LoadMilkshapeModel( const char *szFilename, const char *szName )
{

  if ( szFilename == NULL )	return NULL;
  if ( szName == NULL )		return NULL;

  // Create model
  CModel *pModel = new CModel();
  assert( pModel != NULL && "Out of memory, can't create model");
  // Create milkshape loader
  CMilkshapeLoader *pLoader = new CMilkshapeLoader() ;
  assert( pLoader != NULL && "Out of memory, can't create Milkshapeloader");
  string name = szName;
  
  // If loading fails
  if ( pLoader->Load( szFilename ) ) 
  {
    delete pModel;
    delete pLoader;
    return NULL;
  }

  pLoader->GenerateModelData();
  ////////////////////
  /// Resource allocation is one-way, either everything succeeds or nothing goes.

  // Create vertex handle
  assert( g_DefaultVertexManager->Create(pLoader->GetVertices(), 
					 name + "_vertices", 
					 pModel->GetVertexHandle()) == 0);
  // Create normal handle
  assert ( g_DefaultVertexManager->Create(pLoader->GetNormals(),  
					  name + "_normals",  
					  pModel->GetNormalHandle()) == 0);
  // Create texcoord handle
  assert ( g_DefaultVertexManager->Create(pLoader->GetTexCoords(),  
					  name + "_texcoords0",  
					  pModel->GetTextureCoordinateHandle()) == 0);
  // Create index handles 
  for ( size_t i=0;i<pLoader->GetIndices().size(); i++)
  {
    ostringstream stream;  
    stream << name << "_indices" << i;
    pModel->AddIndexHandle(INDEX_HANDLE());
    assert( g_DefaultIndexManager->Create( pLoader->GetIndices()[i], stream.str().c_str(), pModel->GetIndexHandles().back()) == 0 );
  }
  
  // Mark resourcemanager-tracked data so pointers will not be free'd.
  pLoader->ResetVertices();
  pLoader->ResetNormals();
  pLoader->ResetTexCoords();
  pLoader->ResetIndices();

  // Release loader memory
  delete pLoader;
  return pModel;
}
/////////////////////////////////////////////////////////////////

