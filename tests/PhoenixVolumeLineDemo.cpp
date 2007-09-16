#include "Phoenix.h"
#include <iostream>
#include <SDL.h>
#include <sstream>
using std::string;
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
class CEnergyBeam : public Phoenix::Graphics::CModel
{
private:
  static unsigned int m_nNumBeams;
  std::string m_strName;
  CVertexDescriptor *m_pTimeParam;
public:
  ////////////////////
  /// Constructor.
  CEnergyBeam()
  {
    unsigned int nNum = m_nNumBeams++;
    std::string shadername("EnergyBeamShader");
    // Allocate proper shader.
    assert( g_DefaultShaderManager->AttachHandle( shadername, GetShaderHandle() ) == 0);
    
    // Create name for resources.
    std::ostringstream stream;
    stream << "EnergyBeam_" << nNum;
    m_strName = stream.str();

    // Create vertices for beam
    CVertexDescriptor *pVertices = new CVertexDescriptor( ELEMENT_TYPE_VERTEX_3F, 4);
    assert(g_DefaultVertexManager->Create( pVertices, m_strName, GetVertexHandle() ) == 0);
    
    // Create index array or use existing one.
    std::string strBeamShaderIndices("EnergyBeamShaderIndices");
    CIndexArray *pIndices = g_DefaultIndexManager->GetResource( strBeamShaderIndices);
    AddIndexHandle(INDEX_HANDLE());    
    if ( pIndices == NULL)
    {
      CIndexArray *pIndices = new CIndexArray( PRIMITIVE_QUAD_LIST, 4 );
      pIndices->GetPointer<unsigned short int>()[0] = 0;
      pIndices->GetPointer<unsigned short int>()[1] = 1;
      pIndices->GetPointer<unsigned short int>()[2] = 2;
      pIndices->GetPointer<unsigned short int>()[3] = 3;

      g_DefaultIndexManager->Create( pIndices, strBeamShaderIndices, GetIndexHandles().back());
    }
    else
    {
      g_DefaultIndexManager->AttachHandle( strBeamShaderIndices, GetIndexHandles().back() );
    }
    
    std::ostringstream stream2;
    stream2 << "EnergyBeam_" << nNum << "_params";
    
    m_pTimeParam = new CVertexDescriptor( ELEMENT_TYPE_UNIFORM_1F, 1 );
    m_pTimeParam->GetPointer<float>()[0] = 0.0f;
    
    /// Create shader parameters.
    CVertexDescriptor *pEndposThickness = new CVertexDescriptor( ELEMENT_TYPE_ATTRIB_4F, 4);
    SetShaderParameter( "endposThickness", VERTEX_HANDLE());
    assert(g_DefaultVertexManager->Create( pEndposThickness, stream2.str(), GetShaderParameters().back().second ) == 0);
    SetShaderParameter( "beamTexture", 0);
    
    std::ostringstream stream3;
    stream3 << "EnergyBeam_" << nNum << "_time";
    SetShaderParameter( "time", VERTEX_HANDLE());
    assert(g_DefaultVertexManager->Create( m_pTimeParam, stream3.str(), GetShaderParameters().back().second ) == 0);
    
    ////////////////////
    /// Create texture coordinates or attach to them if they already exist.
    string strEnergyBeamTexCoords("EnergyBeamTexCoords");    
    CVertexDescriptor *pTexCoords = g_DefaultVertexManager->GetResource( strEnergyBeamTexCoords );

    if ( pTexCoords == 0 )
    {
      pTexCoords = new CVertexDescriptor( ELEMENT_TYPE_TEX_2F, 4 );
      pTexCoords->GetPointer<float>()[0] = 0.0f;
      pTexCoords->GetPointer<float>()[1] = 0.0f;
      pTexCoords->GetPointer<float>()[2] = 0.0f;
      pTexCoords->GetPointer<float>()[3] = 1.0f;
      pTexCoords->GetPointer<float>()[4] = 1.0f;
      pTexCoords->GetPointer<float>()[5] = 1.0f;
      pTexCoords->GetPointer<float>()[6] = 1.0f;
      pTexCoords->GetPointer<float>()[7] = 0.0f;
      assert( g_DefaultVertexManager->Create( pTexCoords, strEnergyBeamTexCoords, GetTextureCoordinateHandle()) == 0 );
    }
    else
    {
      g_DefaultVertexManager->AttachHandle( strEnergyBeamTexCoords, GetTextureCoordinateHandle());
    }
    
  }
  ////////////////////
  /// Destructor.
  ~CEnergyBeam()
  {
    std::string killname;
    
    g_DefaultShaderManager->Release( GetShaderHandle() );
    g_DefaultVertexManager->Release( GetTextureCoordinateHandle());
    g_DefaultIndexManager->Release( GetIndexHandles()[0] );

    killname = g_DefaultVertexManager->GetResourceName(GetVertexHandle());
    g_DefaultVertexManager->Destroy( killname );
    
    killname = g_DefaultVertexManager->GetResourceName(GetShaderParameters()[0].second);
    g_DefaultVertexManager->Destroy( killname );

    killname = g_DefaultVertexManager->GetResourceName(GetShaderParameters()[1].second);
    g_DefaultVertexManager->Destroy( killname );

    for(unsigned int n=0;n<TEXTURE_HANDLE_COUNT;n++)
    {
      g_DefaultTextureManager->Release( GetTextureHandle(n) );
    }

  }
  ////////////////////
  /// Initializes energy beam.
  /// \param vStartPos Start position.
  /// \param vEndPos End position.
  /// \param fThickness Beam thickness value (total width = thickness * 2 ).
  void Initialize( const Phoenix::Math::CVector3<float> & vStartPos, const Phoenix::Math::CVector3<float> &vEndPos, float fThickness )
  {
    
    CVertexDescriptor *pEndposThickness = g_DefaultVertexManager->GetResource( Phoenix::Graphics::CModel::GetShaderParameters()[0].second  );
    
    pEndposThickness->GetPointer<float>()[0] = vEndPos[0];
    pEndposThickness->GetPointer<float>()[1] = vEndPos[1];
    pEndposThickness->GetPointer<float>()[2] = vEndPos[2];
    pEndposThickness->GetPointer<float>()[3] = fThickness;

    pEndposThickness->GetPointer<float>()[4] = vEndPos[0];
    pEndposThickness->GetPointer<float>()[5] = vEndPos[1];
    pEndposThickness->GetPointer<float>()[6] = vEndPos[2];
    pEndposThickness->GetPointer<float>()[7] = -fThickness;

    pEndposThickness->GetPointer<float>()[8] =  vStartPos[0];
    pEndposThickness->GetPointer<float>()[9] =  vStartPos[1];
    pEndposThickness->GetPointer<float>()[10] = vStartPos[2];
    pEndposThickness->GetPointer<float>()[11] = fThickness;

    pEndposThickness->GetPointer<float>()[12] = vStartPos[0];
    pEndposThickness->GetPointer<float>()[13] = vStartPos[1];
    pEndposThickness->GetPointer<float>()[14] = vStartPos[2];
    pEndposThickness->GetPointer<float>()[15] = -fThickness;

    CVertexDescriptor *pVertices = g_DefaultVertexManager->GetResource( m_strName );

    pVertices->GetPointer<float>()[0] = vStartPos[0];
    pVertices->GetPointer<float>()[1] = vStartPos[1];
    pVertices->GetPointer<float>()[2] = vStartPos[2];

    pVertices->GetPointer<float>()[3] = vStartPos[0];
    pVertices->GetPointer<float>()[4] = vStartPos[1];
    pVertices->GetPointer<float>()[5] = vStartPos[2];

    pVertices->GetPointer<float>()[6] = vEndPos[0];
    pVertices->GetPointer<float>()[7] = vEndPos[1];
    pVertices->GetPointer<float>()[8] = vEndPos[2];

    pVertices->GetPointer<float>()[9] = vEndPos[0];
    pVertices->GetPointer<float>()[10] = vEndPos[1];
    pVertices->GetPointer<float>()[11] = vEndPos[2];
  }
  ////////////////////
  /// Adds value to passed time.
  /// \param fValue Time passed.
  void IncreaseTime( float fValue )
  {
    m_pTimeParam->GetPointer<float>()[0] += fValue;
  }
};
// Initialize beam count.
unsigned int CEnergyBeam::m_nNumBeams = 0;
/////////////////////////////////////////////////////////////////
int g_bLoop = 1;
int main()
{
  
  if ( !CSDLScreen::GetInstance() )
  {
    std::cerr << "Couldn't open screen" << std::endl;
    return 1;
  }
  
  string strVertexTest("VertexTest");
  string tmpName;
  CVector3<float> vStartPos(0,0,0);
  CVector3<float> vEndPos(13,0,0);
  //CVector3<float> vDir = vEndPos - vStartPos;
  float fThickness = 0.25f;
  ////////////////////

  COglRenderer *pOglRenderer = new COglRenderer();
  

  CCamera camera;
  camera.SetPosition( 0,0,2.0f);
  camera.SetViewport( 0,0, 640, 480 );
  camera.SetNearClipping( 0.1f);
  camera.SetFarClipping( 100.0f );
  camera.SetFieldOfView( 45.0f);
  SDL_Event event;
  CVector3<float> vMove(0,0,0.2f);
  
  
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  CShader *pShader = pOglRenderer->CreateShader( std::string("Resources/Shaders/line_vertex.glsl"), 
						 std::string("Resources/Shaders/line_frag.glsl") );
  
  assert( pShader != NULL ) ;
  SHADER_HANDLE hShaderHandle;
  assert(g_DefaultShaderManager->Create( pShader, "EnergyBeamShader", hShaderHandle) == 0 );
  

  


  float fAngle = 0.0f;
  float fMagnitude = 12.0f;
  float afAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, afAmbient );
  
  COglTexture *pBeamTexture = pOglRenderer->CreateTexture( std::string("Resources/Textures/beam.tga"));
  COglTexture *pBeamTexture2 = pOglRenderer->CreateTexture( std::string("Resources/Textures/beam2.tga"));
  
  CEnergyBeam *pBeam, *pBeam2;
  pBeam = new CEnergyBeam();
  pBeam2 = new CEnergyBeam();
  pBeam->Initialize( vStartPos, vEndPos, fThickness );
  assert( g_DefaultTextureManager->Create( pBeamTexture, "EnergyBeamTexture_Red", pBeam->GetTextureHandle()) == 0);
  pBeam->AddTextureFilter( MIN_NEAREST );
  pBeam->AddTextureFilter( MAG_LINEAR );
  pBeam->AddTextureFilter( S_WRAP_REPEAT );
  
  pBeam2->Initialize( vStartPos, vEndPos, fThickness );
  assert( g_DefaultTextureManager->Create( pBeamTexture2, "EnergyBeamTexture_Another", pBeam2->GetTextureHandle()) == 0);
  pBeam2->AddTextureFilter( MIN_NEAREST );
  pBeam2->AddTextureFilter( MAG_LINEAR );
  pBeam2->AddTextureFilter( S_WRAP_REPEAT );
  
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
	  camera.Move( 0.3f );
	}      
	else if ( event.key.keysym.sym == SDLK_DOWN )
	{
	  camera.Move( -0.3f );
	} 
	else if ( event.key.keysym.sym == SDLK_LEFT )
	{
	  camera.RotateAroundUp( 2.93f );
	}      
	else if ( event.key.keysym.sym == SDLK_RIGHT )
	{
	  camera.RotateAroundUp( -2.93f );
	} 
	else if ( event.key.keysym.sym == SDLK_PAGEUP )
	{
	  camera.Elevate( 0.3f );
	}      
	else if ( event.key.keysym.sym == SDLK_PAGEDOWN )
	{
	  camera.Elevate( -0.3f );
	} 
	else if ( event.key.keysym.sym == SDLK_RETURN )
	{
	  std::cerr << camera.GetPosition() << std::endl;
	  std::cerr << camera.GetViewMatrix() << std::endl;
	}
	break;
      default:
	break;
      }
    }
    
    pOglRenderer->ClearBuffer( COLOR_BUFFER );
    pOglRenderer->ClearBuffer( DEPTH_BUFFER );
    pOglRenderer->CommitCamera( camera );
    
    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE);

    pOglRenderer->CommitModel( *pBeam );
    pOglRenderer->CommitModel( *pBeam2 );
    
    pOglRenderer->Finalize();
    vStartPos[2] = cosf(fAngle) * 2.0f;
    vStartPos[1] = sinf(fAngle) * 2.0f;
    
    pBeam->Initialize(vStartPos, vEndPos, fThickness );

    vStartPos[2] = cosf(-fAngle) * 1.0f;
    vStartPos[1] = sinf(fAngle) * 1.0f;
    
    pBeam2->Initialize(vStartPos, vEndPos, fThickness );
    
    //sleep(1);
    //g_bLoop = 0;
    CSDLScreen::GetInstance()->SwapBuffers();
    fAngle += 0.001f;
    pBeam->IncreaseTime(0.025f);
    pBeam2->IncreaseTime(0.025f);
    
  }
  delete pBeam;
  delete pBeam2;
  



  CSDLScreen::DestroyInstance();

  return 0;
}
