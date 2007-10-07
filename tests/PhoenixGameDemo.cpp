#include "Phoenix.h"
#include <iostream>
#include <SDL.h>
#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>
using std::string;
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
using namespace Phoenix::Scene;
using namespace Phoenix::Volume;
using std::cerr;
using std::endl; 
/////////////////////////////////////////////////////////////////
int g_bLoop = 1;
class CSpatialGraph : public COctree<Phoenix::Scene::CGameObject *>
{
public:
  ////////////////////
  /// 
  CSpatialGraph() : COctree<Phoenix::Scene::CGameObject *>(4,200){}
  ////////////////////
  /// 
  void InsertObject( CGameObject *pGameObject )
  {
    COctree<Phoenix::Scene::CGameObject *>::InsertObject( pGameObject, pGameObject->GetBoundingSphere());
  }
  ////////////////////
  /// 
  void DeleteObject( CGameObject *pGameObject )
  {
    COctree<Phoenix::Scene::CGameObject *>::GetNodeAt( pGameObject->GetSpatialIndex())->DeleteObject( pGameObject );
  }
  ////////////////////
  /// 
  void Update( Phoenix::Scene::CGameObject *pGameObject )
  {
    const CVector3<float> &vTmp = pGameObject->GetTransform().GetTranslation();
    
    unsigned int nLevel = COctree<Phoenix::Scene::CGameObject *>::GetObjectDepth( pGameObject->GetBoundingSphere().GetRadius() );
    unsigned int nIndex = COctree<Phoenix::Scene::CGameObject *>::GetIndex1D(nLevel, vTmp[0], vTmp[1], vTmp[2]);
    // If object has changed spatial location enough
    if ( pGameObject->GetSpatialIndex() != nIndex)
    {
      // remove pointer from previous node
      GetNodeAt( pGameObject->GetSpatialIndex())->DeleteObject( pGameObject );
      GetNodeAt( nIndex )->GetObjects().push_back( pGameObject );
    }
  }
};
/////////////////////////////////////////////////////////////////
int main()
{
  
  if ( !CSDLScreen::GetInstance() )
  {
    std::cerr << "Couldn't open screen" << std::endl;
    return 1;
  }
  
  CCamera camera;
  camera.SetPosition( 0, 0.0f,10.0f);
  camera.SetViewport( 0,0, 640, 480 );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 800.0f );
  camera.SetFieldOfView( 45.0f);
  SDL_Event event;
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  COglRenderer *pOglRenderer = new COglRenderer();
  COctree<float> *pOctree = new COctree<float>(5,100.0f);
  float afAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, afAmbient );
  
  CGameObject gameobject;
  

  /////////////////////////////////////////////////////////////////
  /// Model loading code
  Phoenix::Data::CMilkshapeLoader loader;
  std::string name("Resources/Models/omega.ms3d");
  assert ( loader.Load( name ) == 0 && "Could not open model file!");
  loader.GenerateModelData();
  loader.Stripify();
  
  assert(g_DefaultVertexManager->Create( loader.GetVertices(),  "fighter_vertices",  gameobject.GetModel().GetVertexHandle() ) == 0);  
  assert(g_DefaultVertexManager->Create( loader.GetTexCoords(), "fighter_texcoords", gameobject.GetModel().GetTextureCoordinateHandle() )== 0);
  assert(g_DefaultVertexManager->Create( loader.GetNormals(),   "fighter_normals",   gameobject.GetModel().GetNormalHandle() )== 0);

  // create resources for indices.
  for(unsigned int i=0;i<loader.GetIndices().size();i++)
  {
    std::ostringstream stream;
    stream << "fighter_indices" << i;
    gameobject.GetModel().AddIndexHandle( INDEX_HANDLE() );    
    assert( g_DefaultIndexManager->Create( loader.GetIndices()[i], 
					   stream.str(), 
					   gameobject.GetModel().GetIndexHandles().back() ) == 0);

  }  

  loader.ResetVertices();
  loader.ResetTexCoords();
  loader.ResetNormals();
  loader.ResetIndices();

  gameobject.GetBoundingSphere() = CalculateBoundingSphereTight( 
				      *(g_DefaultVertexManager->GetResource(gameobject.GetModel().GetVertexHandle())));
  
  gameobject.GetTransform().SetTranslation( 0,-10,0);  
  
  while( g_bLoop )
  {
    while ( SDL_PollEvent(&event ))
    {
      switch(event.type)
      {
      case SDL_KEYDOWN:
	if ( event.key.keysym.sym == SDLK_ESCAPE)	
	{
	  g_bLoop = 0;
	}
	else if ( event.key.keysym.sym == SDLK_UP )
	{
	  camera.Move( 1.3f );
	}      
	else if ( event.key.keysym.sym == SDLK_DOWN )
	{
	  camera.Move( -1.3f );
	} 
	else if ( event.key.keysym.sym == SDLK_LEFT )
	{
	  camera.RotateAroundUp( 2.93f );
	}      
	else if ( event.key.keysym.sym == SDLK_RIGHT )
	{
	  camera.RotateAroundUp( -2.93f );
	} 
	break;
      default:
	break;
      }
    }

    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );    
    glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->DisableState( STATE_LIGHTING );
    pOglRenderer->CommitCamera( camera );
    pOglRenderer->CommitColor( CVector4<unsigned char>(255,255,255,255));
    pOglRenderer->CommitTransform( gameobject.GetTransform());
    pOglRenderer->CommitModel( gameobject.GetModel());
    pOglRenderer->RollbackTransform();
    
    camera.UpdateProjection();
    camera.UpdateView();
    camera.CalculateFrustum();    
    camera.CalculateBoundingSphere();
    camera.CalculateBoundingCone();
    pOglRenderer->Finalize();
    CSDLScreen::GetInstance()->SwapBuffers();
  }
  CSDLScreen::DestroyInstance();
  return 0;
}
