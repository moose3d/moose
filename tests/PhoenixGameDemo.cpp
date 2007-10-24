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
#define g_PhoenixModelManager (CResourceManager<CModel, CHandle<CModel> >::GetInstance())
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
    
    CVector3<float> vTmp = pGameObject->GetTransform().GetTranslation();

    CSphere sphere = pGameObject->GetBoundingSphere();
    sphere.SetPosition( sphere.GetPosition() + vTmp );
    COctree<Phoenix::Scene::CGameObject *>::InsertObject( pGameObject, sphere);
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
    CVector3<float> vTmp = pGameObject->GetTransform().GetTranslation();
    vTmp += pGameObject->GetBoundingSphere().GetPosition();

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
void DrawFrustum( CCamera &camera )
{

  CFrustum &frustum = camera.Frustum();
  Phoenix::Volume::CSphere &sphere = camera.FrustumSphere();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDisable(GL_CULL_FACE);

//   std::cerr << "NEAR :" << frustum.GetPlane(Phoenix::Volume::BACK) << std::endl;
//   std::cerr << "FAR :" << frustum.GetPlane(Phoenix::Volume::FRONT) << std::endl;
//   std::cerr << "TOP :" << frustum.GetPlane(Phoenix::Volume::TOP) << std::endl;
//   std::cerr << "BOTTOM :" << frustum.GetPlane(Phoenix::Volume::BOTTOM) << std::endl;
//   std::cerr << "LEFT :" << frustum.GetPlane(Phoenix::Volume::LEFT) << std::endl;
//   std::cerr << "RIGHT :" << frustum.GetPlane(Phoenix::Volume::RIGHT) << std::endl;

  glColor3f(1,0,0);
  glBegin(GL_QUADS);
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_NEAR_LEFT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_NEAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_FAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_FAR_LEFT).GetArray());

    glVertex3fv( frustum.GetCorner(CFrustum::TOP_NEAR_LEFT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_NEAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_FAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_FAR_LEFT).GetArray());
    
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_NEAR_LEFT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_NEAR_LEFT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_FAR_LEFT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_FAR_LEFT).GetArray());

    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_NEAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_NEAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::TOP_FAR_RIGHT).GetArray());
    glVertex3fv( frustum.GetCorner(CFrustum::BOTTOM_FAR_RIGHT).GetArray());

  glEnd();
  glColor3f(0,1,0);
  GLUquadric * q = gluNewQuadric();
  //cerr << "sphere radius: " << sphere.GetRadius() << endl;
  //cerr << "sphere pos: " << sphere.GetPosition() << endl;
  gluQuadricDrawStyle(q, GLU_SILHOUETTE);
  glPushMatrix();
    glTranslatef( sphere.GetPosition()[0], 
		  sphere.GetPosition()[1],
		  sphere.GetPosition()[2]);
    glRotatef( 90.0f, 1, 0, 0);
    gluDisk(q, 1.0, sphere.GetRadius(), 24, 24);
  glPopMatrix();
  gluDeleteQuadric(q);
}
/////////////////////////////////////////////////////////////////
class CGameObjectOGLAdapter 
{
public:
  void Commit( COglRenderer &renderer, const CGameObject *pObject)
  {
    renderer.CommitColor( CVector4<unsigned char>(255,255,255,255));
    renderer.CommitTransform( pObject->GetTransform() );
    renderer.CommitModel( *g_PhoenixModelManager->GetResource(pObject->GetModelHandle()));
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
  //camera.SetViewport( 480,340, 160, 120 );
  camera.SetViewport( 0,0, 640, 480 );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 100.0f );
  camera.SetFieldOfView( 43.0f);

  CCamera camera2;
  camera2.SetPosition( 0, 50.0f, 0.0f);
  camera2.SetViewport( 0,0, 640, 480 );
  camera2.SetNearClipping( 0.1f);
  camera2.SetFarClipping( 800.0f );
  camera2.SetViewOrtho( -200,200, -150, 150);
  camera2.RotateAroundRight( -90.0 );
  SDL_Event event;
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  COglRenderer *pOglRenderer = new COglRenderer();
  COctree<float> *pOctree = new COctree<float>(5,100.0f);
  float afAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, afAmbient );
  
  CGameObject *pGameObject = new CGameObject();
  CGameObjectOGLAdapter oglAdapter;
  
  Phoenix::Graphics::CRenderQueue<CGameObject *> *pRenderQueue = new Phoenix::Graphics::CRenderQueue<CGameObject *>();
  
  /////////////////////////////////////////////////////////////////
  /// Model loading code
  Phoenix::Data::CMilkshapeLoader loader;
  std::string name("Resources/Models/omega.ms3d");
  assert ( loader.Load( name ) == 0 && "Could not open model file!");
  loader.GenerateModelData();
  loader.Stripify();
  
  CModel *pModel = new CModel();//g_PhoenixModelManager->GetResource(gameobject.GetModelHandle());
  
  assert(g_DefaultVertexManager->Create( loader.GetVertices(),  "fighter_vertices",  pModel->GetVertexHandle() ) == 0);  
  assert(g_DefaultVertexManager->Create( loader.GetTexCoords(), "fighter_texcoords", pModel->GetTextureCoordinateHandle() )== 0);
  assert(g_DefaultVertexManager->Create( loader.GetNormals(),   "fighter_normals",   pModel->GetNormalHandle() )== 0);

  // create resources for indices.
  for(unsigned int i=0;i<loader.GetIndices().size();i++)
  {
    std::ostringstream stream;
    stream << "fighter_indices" << i;
    pModel->AddIndexHandle( INDEX_HANDLE() );    

    assert( g_DefaultIndexManager->Create( loader.GetIndices()[i], 
					   stream.str(), 
					   pModel->GetIndexHandles().back() ) == 0);

  }  

  loader.ResetVertices();
  loader.ResetTexCoords();
  loader.ResetNormals();
  loader.ResetIndices();
  

  CVertexDescriptor *pVD = (g_DefaultVertexManager->GetResource(pModel->GetVertexHandle()));
  CSphere sphere = CalculateBoundingSphereTight( *pVD);
  pGameObject->GetBoundingSphere() = sphere;

  std::cerr << "bounding sphere;" << sphere << std::endl;
  pGameObject->GetTransform().SetTranslation( 0,-10,0);  
  assert( g_PhoenixModelManager->Create( pModel, "OmegaModel", pGameObject->GetModelHandle()) == 0);
  sphere.SetPosition(sphere.GetPosition() + CVector3<float>(0,-10,0));

  CSpatialGraph *pSpatialGraph = new CSpatialGraph();
  pSpatialGraph->InsertObject( pGameObject);
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
	  camera.RotateAroundUp( 1.1f );
	}      
	else if ( event.key.keysym.sym == SDLK_RIGHT )
	{
	  camera.RotateAroundUp( -1.1f );
	} 
	break;
      default:
	break;
      }
    }
    pRenderQueue->Clear();
    cerr << "Collected: " << pRenderQueue->CollectObjects( camera, *pSpatialGraph ) << endl;
    
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );    
    glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->DisableState( STATE_LIGHTING );
    pOglRenderer->CommitCamera( camera );
    //pOglRenderer->CommitColor( CVector4<unsigned char>(255,255,255,255));
    //pOglRenderer->CommitTransform( pGameobject->GetTransform() );
    //pOglRenderer->CommitModel( *g_PhoenixModelManager->GetResource(gameobject.GetModelHandle()));
    pRenderQueue->Render<CGameObjectOGLAdapter>( *pOglRenderer, oglAdapter );
    
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
