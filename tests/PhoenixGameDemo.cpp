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
  CSpatialGraph() : COctree<Phoenix::Scene::CGameObject *>(4,400){}
  ////////////////////
  /// 
  void InsertObject( CGameObject *pGameObject )
  {
    assert(pGameObject!=NULL);
    unsigned int nSpatialIndex = COctree<Phoenix::Scene::CGameObject *>::InsertObject( 
				  pGameObject, 
				  pGameObject->GetBoundingSphere().GetPosition() + pGameObject->GetTransform().GetTranslation(),
				  pGameObject->GetBoundingSphere().GetRadius());
    
    pGameObject->SetSpatialIndex( nSpatialIndex );
    //cerr << "object inserted into " << nSpatialIndex << endl;
  }
  ////////////////////
  /// 
  void DeleteObject( CGameObject *pGameObject )
  {
    COctreeNode<Phoenix::Scene::CGameObject *> *pNode = COctree<Phoenix::Scene::CGameObject *>::GetNodeAt( pGameObject->GetSpatialIndex());
    if ( pNode != NULL )
    {
      pNode->DeleteObject( pGameObject );
    }
  }
  ////////////////////
  /// 
  void Update( Phoenix::Scene::CGameObject *pGameObject )
  {
    CVector3<float> vTmp = pGameObject->GetTransform().GetTranslation();
    vTmp += pGameObject->GetBoundingSphere().GetPosition();
    float fWorldHalfSizeNeg = -GetWorldHalfSize();
    float fWorldHalfSizePos =  GetWorldHalfSize()-0.001f;
 
    unsigned int nLevel = COctree<Phoenix::Scene::CGameObject *>::GetObjectDepth( pGameObject->GetBoundingSphere().GetRadius() );
    unsigned int nIndex = COctree<Phoenix::Scene::CGameObject *>::GetIndex1D(nLevel, vTmp[0], vTmp[1], vTmp[2]);
    // If object has changed spatial location enough
    if ( pGameObject->GetSpatialIndex() != nIndex)
    {
      // remove pointer from previous node
      GetNodeAt( pGameObject->GetSpatialIndex())->DeleteObject( pGameObject );
      GetNodeAt( nIndex )->GetObjects().push_back( pGameObject );
      //cerr << "inserted object into " << nIndex << endl;
      pGameObject->SetSpatialIndex( nIndex );
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
protected:
  CCamera *m_pCamera;
public:
  ////////////////////
  /// 
  CGameObjectOGLAdapter() : m_pCamera(NULL)
  { 
    
  }
  ////////////////////
  ///
  void Commit( COglRenderer &renderer, const CGameObject *pObject)
  {
    //renderer.CommitColor( CVector4<unsigned char>(255,255,255,255));
    CVector3<float> vDiff;
    if ( GetCamera() != NULL ) 
    {
      vDiff = pObject->GetTransform().GetTranslation() - GetCamera()->GetPosition();
    } 
    unsigned int nLength = (unsigned int)vDiff.Length();
    renderer.CommitTransform( pObject->GetTransform() );
    if ( nLength < 40 )
      renderer.CommitModel( *g_PhoenixModelManager->GetResource(pObject->GetModelHandle()));
    else
    {
      CModel &model = *g_PhoenixModelManager->GetResource(pObject->GetModelHandle());
      // Retrieve resources
      COglTexture *pTexture = NULL;
      CVertexDescriptor *pTemp = NULL;
      CVertexDescriptor *pVertices = g_DefaultVertexManager->GetResource(model.GetVertexHandle());
      CIndexArray *pIndices = NULL;
  
      // Commit textures
      for( unsigned int i=0; i<TEXTURE_HANDLE_COUNT; i++)
      {
	pTemp    = g_DefaultVertexManager->GetResource(  model.GetTextureCoordinateHandle(i));
	pTexture = g_DefaultTextureManager->GetResource( model.GetTextureHandle(i) );
    
	// check that texcoord resources actually exist
	if ( pTemp     != NULL ) 
	{ 
	  renderer.CommitVertexDescriptor( pTemp, i ); 
	} 
	// check that texture resource exists
	if ( pTexture  != NULL ) 
	{ 
	  renderer.CommitTexture( i, pTexture ); 
	  // Apply texture filters.
	  std::vector<TEXTURE_FILTER> &vecFilters = model.GetTextureFilters(i);
      
	  for(unsigned int nFilter=0; nFilter<vecFilters.size(); nFilter++)
	  {
	    renderer.CommitFilter( vecFilters[nFilter], pTexture->GetType() );
	  }
	}
      }
      // if shader exists
      if ( model.GetShaderHandle().IsNull() == 0 )
      {
	CShader *pShader = g_DefaultShaderManager->GetResource(model.GetShaderHandle());
	renderer.CommitShader( pShader );
	CVertexDescriptor *pParam = NULL;
	// Go through all parameters and commit them
	for(unsigned int nSP=0; nSP< model.GetShaderParameters().size(); nSP++)
	{
	  pParam = g_DefaultVertexManager->GetResource( model.GetShaderParameters()[nSP].second );
	  if ( pParam != NULL )
	  {
	    renderer.CommitShaderParam( *pShader, model.GetShaderParameters()[nSP].first, *pParam );
	  }
	}
	// Go through all int parameters and commit them
	for(unsigned int nSP=0; nSP< model.GetShaderIntParameters().size(); nSP++)
	{
	  renderer.CommitUniformShaderParam( *pShader, model.GetShaderIntParameters()[nSP].first, model.GetShaderIntParameters()[nSP].second );
	}
	// Go through all float parameters and commit them
	for(unsigned int nSP=0; nSP< model.GetShaderFloatParameters().size(); nSP++)
	{
	  renderer.CommitUniformShaderParam( *pShader, model.GetShaderFloatParameters()[nSP].first, model.GetShaderFloatParameters()[nSP].second );
	}
      }
      // check and commit resources
      if ( pVertices != NULL ) 
      { 
	renderer.CommitVertexDescriptor ( pVertices ); 
      }
      CVertexDescriptor *pNormals = g_DefaultVertexManager->GetResource(model.GetNormalHandle());
      if ( pNormals != NULL ) 
      { 
	renderer.CommitVertexDescriptor( pNormals ); 
      }
      unsigned int nMax;
      if ( nLength > 400)
      {
	nMax = 10;
      } 
      else  if ( nLength > 300 )
      {
	nMax = 200;
      }
      else  if ( nLength > 200 )
      {
	nMax = 500;
      }
      else if ( nLength > 100 )
      {
	nMax = 700;
      }
      else 
      {
	nMax = 1690;
      }
      unsigned int nCurrentCount = 0;
      for(unsigned int n=0;n<model.GetIndexHandles().size();n++)
      {
	pIndices = g_DefaultIndexManager->GetResource( model.GetIndexHandles()[n] );
	
	if ( nCurrentCount >= nMax ) break;

	if ( pIndices  != NULL ) 
	{ 
	  renderer.CommitPrimitive ( pIndices );         
	  nCurrentCount += pIndices->GetNumIndices()-2;
	}
	
      }

    }
    renderer.RollbackTransform();
    //m_bFirstTime = 0;
    //glFlush();
  }
  ////////////////////
  //
  inline void SetCamera( CCamera & rCamera )
  {
    m_pCamera = &rCamera;
  }
  ////////////////////
  ///
  inline CCamera * GetCamera() 
  {
    return m_pCamera;
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
  camera.SetFarClipping( 1500.0f );
  camera.SetFieldOfView( 43.0f);

  CCamera camera2;
  camera2.SetPosition( 0, 250.0f, 0.0f);
  camera2.SetViewport( 480,360, 160, 120 );
  camera2.SetNearClipping( 0.1f);
  camera2.SetFarClipping( 800.0f );
  camera2.SetViewOrtho( -800,800, -800, 400);
  camera2.RotateAroundRight( -90.0 );
  SDL_Event event;
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  COglRenderer *pOglRenderer = new COglRenderer();
  float afAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, afAmbient );
  
  CGameObject *pGameObject = new CGameObject();
  CGameObjectOGLAdapter oglAdapter;
  
  Phoenix::Graphics::CRenderQueue<CGameObject *> *pRenderQueue = 
      new Phoenix::Graphics::CRenderQueue<CGameObject *>();


  /////////////////////////////////////////////////////////////////
  /// Model loading code
  Phoenix::Data::CMilkshapeLoader loader;
  std::string name("Resources/Models/omega.ms3d");
  assert ( loader.Load( name ) == 0 && "Could not open model file!");
  loader.GenerateModelData();
  //loader.Stripify();
  
  CModel *pModel = new CModel();//g_PhoenixModelManager->GetResource(gameobject.GetModelHandle());
  
  assert(g_DefaultVertexManager->Create( loader.GetVertices(),  "fighter_vertices",  pModel->GetVertexHandle() )            == 0);  
  assert(g_DefaultVertexManager->Create( loader.GetTexCoords(), "fighter_texcoords", pModel->GetTextureCoordinateHandle() ) == 0);
  assert(g_DefaultVertexManager->Create( loader.GetNormals(),   "fighter_normals",   pModel->GetNormalHandle() )            == 0);

  // create resources for indices.
  for(unsigned int i=0;i<loader.GetIndices().size();i++)
  {
    std::ostringstream stream;
    stream << "fighter_indices" << i;
    pModel->AddIndexHandle( INDEX_HANDLE() );    

    assert( g_DefaultIndexManager->Create( loader.GetIndices()[i], 
					   stream.str(), 
					   pModel->GetIndexHandles().back() ) == 0);

    pOglRenderer->CommitCache(*loader.GetIndices()[i]);

  }  

  loader.ResetVertices();
  loader.ResetTexCoords();
  loader.ResetNormals();
  loader.ResetIndices();
  
  CVertexDescriptor *pVD = (g_DefaultVertexManager->GetResource(pModel->GetVertexHandle()));
  pOglRenderer->CommitCache( *pVD );

  CSphere sphere = CalculateBoundingSphereTight( *pVD);
  pGameObject->GetBoundingSphere() = sphere;
  
  std::cerr << "bounding sphere;" << sphere << std::endl;
  pGameObject->GetTransform().SetTranslation( 0,-10,0);  
  assert( g_PhoenixModelManager->Create( pModel, "OmegaModel", 
					 pGameObject->GetModelHandle()) == 0);
  
  CSpatialGraph *pSpatialGraph = new CSpatialGraph();
  pSpatialGraph->InsertObject( pGameObject );
  
  oglAdapter.SetCamera( camera);

  // Shove in about 512 objects
#define INSERT_OBJ
#ifdef INSERT_OBJ
  for(unsigned int w=0;w<8;w++)
    for(unsigned int h=0;h<8;h++)
      for(unsigned int d=0;d<8;d++)
      {
	CGameObject *pTemp = new CGameObject();
	pTemp->GetTransform().SetTranslation( -200.0f+w*45,-200.0f+h*45,-200.0f+d*45);  
	pTemp->GetBoundingSphere() = sphere;
	g_PhoenixModelManager->AttachHandle( "OmegaModel", pTemp->GetModelHandle());
	
	pSpatialGraph->InsertObject(pTemp);
	//std::cerr << "inserting " << std::endl;
      }
#endif
  CFpsCounter fpsCounter;
  fpsCounter.Reset();
  unsigned int nCollected = 0;
  int bChange = 0;
  glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );    
  glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
  
  while( g_bLoop )
  {
    fpsCounter.Update();
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
    //pGameObject->GetTransform().Move( 0.01f,0,0); 
    //pSpatialGraph->Update( pGameObject );
    
    
    pRenderQueue->Clear();

    //cerr << "Collected: " << nCollected << endl;
    
    
    

    
    if ( camera.IsProjectionChanged() ) 
    {
      camera.UpdateProjection();
    }
    
    if ( camera.IsViewChanged() ) 
    {
      camera.UpdateView();
    }
    

    camera.CalculateFrustum();    
    camera.CalculateBoundingSphere();
    camera.CalculateBoundingCone();
    bChange = 0;

    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    nCollected = pRenderQueue->CollectObjects( camera, *pSpatialGraph ) ;
    pOglRenderer->CommitCamera( camera );
    //pOglRenderer->DisableState( STATE_LIGHTING );
    //pOglRenderer->CommitColor( CVector4<unsigned char>(255,255,255,255));
    //pOglRenderer->CommitTransform( pGameobject->GetTransform() );
    //pOglRenderer->CommitModel( *g_PhoenixModelManager->GetResource(gameobject.GetModelHandle()));
    
    pRenderQueue->Render<CGameObjectOGLAdapter>( *pOglRenderer, oglAdapter );
    
    //pOglRenderer->RollbackTransform();
    //pOglRenderer->CommitCamera( camera2);
    //pRenderQueue->Render<CGameObjectOGLAdapter>( *pOglRenderer, oglAdapter );
    //DrawFrustum( camera );
    
    pOglRenderer->Finalize();
    CSDLScreen::GetInstance()->SwapBuffers();
    fpsCounter++;
    if ( fpsCounter.HasPassedMS(1000) )
    {
      cerr << "FPS: " << fpsCounter << ", with visible objects #"<< nCollected <<endl;
      fpsCounter.Reset();
    }

  }
  pOglRenderer->RollbackCache( *pVD );
  CSDLScreen::DestroyInstance();
  return 0;
}
