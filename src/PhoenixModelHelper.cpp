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
int
Phoenix::Data::LoadMilkshapeModel( const char *szFilename, const char *szName, CModel & rModel, int iFlags  )
{

  if ( szFilename == NULL )	return 1;
  if ( szName == NULL )		return 1;

  // Create milkshape loader
  CMilkshapeLoader *pLoader = new CMilkshapeLoader() ;
  assert( pLoader != NULL && "Out of memory, can't create Milkshapeloader");
  string name = szName;
  
  // If loading fails
  if ( pLoader->Load( szFilename ) ) 
  {
    delete pLoader;
    return 1;
  }

  pLoader->GenerateModelData();

  ////////////////////
  /// Resource allocation is one-way, either everything succeeds or nothing goes.
  
  // Create vertex handle
  assert( g_DefaultVertexManager->Create(pLoader->GetVertices(), 
					 name + "_vertices", 
					 rModel.GetVertexHandle()) == 0);
  // Mark resourcemanager-tracked data so pointers will not be free'd.
  pLoader->ResetVertices();

  // load vertex normals
  if ( iFlags & OPT_VERTEX_NORMALS )
  {
    // Create normal handle
    assert ( g_DefaultVertexManager->Create(pLoader->GetNormals(),  
					    name + "_normals",  
					    rModel.GetNormalHandle()) == 0);
    pLoader->ResetNormals();  
  }
  // load texture coordinates
  if ( iFlags & OPT_VERTEX_TEXCOORDS )
  {
    // Create texcoord handle
    assert ( g_DefaultVertexManager->Create(pLoader->GetTexCoords(),  
					    name + "_texcoords0",  
					    rModel.GetTextureCoordinateHandle()) == 0);
    pLoader->ResetTexCoords();
  }
  // load colors
  if ( iFlags & OPT_VERTEX_COLORS )
  {
    // Create texcoord handle
    assert ( g_DefaultVertexManager->Create(pLoader->GetTexCoords(),  
					    name + "_texcoords0",  
					    rModel.GetTextureCoordinateHandle()) == 0);
    
  }
  // Load indices
  if ( iFlags & OPT_VERTEX_INDICES )
  {
    // Create index handles 
    for ( size_t i=0;i<pLoader->GetIndices().size(); i++)
    {
      ostringstream stream;  
      stream << name << "_indices" << i;
      rModel.AddIndexHandle(INDEX_HANDLE());
      assert( g_DefaultIndexManager->Create( pLoader->GetIndices()[i], 
					     stream.str().c_str(), 
					     rModel.GetIndexHandles().back()) == 0 );
    }
    pLoader->ResetIndices();
  }

  // Release loader memory
  delete pLoader;
  return 0;
}
/////////////////////////////////////////////////////////////////

