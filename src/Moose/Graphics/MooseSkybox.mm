#include "MooseSkybox.h"
#include "MooseVertexDescriptor.h"
#include "MooseOGLRenderer.h"
#include "MooseDefaultEntities.h"
#include "MooseAssetBundle.h"
#include <iostream>
#include <cassert>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
using namespace Moose::Graphics;
using namespace Moose::Default;
using namespace Moose::Core;
/* *****************************************************************************
   matrix transpose code 
********************************************************************************
 "mat4 mv = mat4(m_viewMatrix[0][0], m_viewMatrix[0][1], m_viewMatrix[0][2],0.0," \
 "m_viewMatrix[1][0], m_viewMatrix[1][1], m_viewMatrix[1][2],0.0," \
 "m_viewMatrix[2][0], m_viewMatrix[2][1], m_viewMatrix[2][2],0.0," \
 "m_viewMatrix[3][0], m_viewMatrix[3][1], m_viewMatrix[3][2],1.0);" \
 "mat4 pr = mat4(m_projMatrix[0][0], m_projMatrix[0][1], m_projMatrix[0][2],m_projMatrix[0][3]," \
 "m_projMatrix[1][0], m_projMatrix[1][1], m_projMatrix[1][2],m_projMatrix[1][3]," \
 "m_projMatrix[2][0], m_projMatrix[2][1], m_projMatrix[2][2],m_projMatrix[2][3]," \
 "m_projMatrix[3][0], m_projMatrix[3][1], m_projMatrix[3][2],m_projMatrix[3][3]);" \
*/
////////////////////////////////////////////////////////////////////////////////
const char *SB_VERT_SH = "#version 150\n" \
 "in vec3 position;"         \
 "in vec3 a_texcoord;"                               \
 "out vec3 v_texcoord;"                                 \
 "uniform mat4 m_viewMatrix;"                               \
 "uniform mat4 m_projMatrix;"                               \
 "void main(){"                                             \
 "mat4 mv = m_viewMatrix;"                                  \
 "mv[3][0] = 0.0; mv[3][1] = 0.0; mv[3][2] = 0.0;"          \
 "gl_Position = m_projMatrix * mv * vec4(position,1.0);"    \
 "v_texcoord = a_texcoord;}";

const char *SB_FRAG_SH =                                \
 "#version 150\n#ifdef GL_ES\n"                                       \
 "varying mediump vec3 v_texcoord;\n"                   \
 "uniform lowp samplerCube diffuse;\n"                  \
 "#else\n"                                              \
 "in vec3 v_texcoord;\n"                           \
 "uniform samplerCube diffuse;\n"                       \
 "out vec4 gl_FragColor;\n#endif\n"                                             \
 "void main(){"                                         \
 "gl_FragColor = texture(diffuse,v_texcoord);}";
/////////////////////////////////////////////////////////////////
inline void CreateTexCoords( Moose::Graphics::CVertexDescriptor *pTexCoords )
{
  pTexCoords->GetPointer<float>()[0] = -1.0f;
  pTexCoords->GetPointer<float>()[1] =  1.0f;
  pTexCoords->GetPointer<float>()[2] =  1.0f;
  
  pTexCoords->GetPointer<float>()[3] = -1.0f;
  pTexCoords->GetPointer<float>()[4] = -1.0f;
  pTexCoords->GetPointer<float>()[5] =  1.0f;

  pTexCoords->GetPointer<float>()[6] = -1.0f;
  pTexCoords->GetPointer<float>()[7] =  1.0f;
  pTexCoords->GetPointer<float>()[8] = -1.0f;
    
  pTexCoords->GetPointer<float>()[9]  = -1.0f;
  pTexCoords->GetPointer<float>()[10] = -1.0f;
  pTexCoords->GetPointer<float>()[11] = -1.0f;
    
  pTexCoords->GetPointer<float>()[12] =  1.0f;
  pTexCoords->GetPointer<float>()[13] =  1.0f;
  pTexCoords->GetPointer<float>()[14] = -1.0f;
    
  pTexCoords->GetPointer<float>()[15] =  1.0f;
  pTexCoords->GetPointer<float>()[16] = -1.0f;
  pTexCoords->GetPointer<float>()[17] = -1.0f;
    
  pTexCoords->GetPointer<float>()[18] =  1.0f;
  pTexCoords->GetPointer<float>()[19] =  1.0f;
  pTexCoords->GetPointer<float>()[20] =  1.0f;

  pTexCoords->GetPointer<float>()[21] =  1.0f;
  pTexCoords->GetPointer<float>()[22] = -1.0f;
  pTexCoords->GetPointer<float>()[23] =  1.0f;
}
/////////////////////////////////////////////////////////////////
inline void CreateVertices( Moose::Graphics::CVertexDescriptor *pVertices )
{
  // 0. left-front-up
  pVertices->GetPointer<float>()[0] = -1.0f;
  pVertices->GetPointer<float>()[1] =  1.0f;
  pVertices->GetPointer<float>()[2] =  1.0f;
  // 1. left-front-down
  pVertices->GetPointer<float>()[3] = -1.0f;
  pVertices->GetPointer<float>()[4] = -1.0f;
  pVertices->GetPointer<float>()[5] =  1.0f;
  // 2. left-back-up
  pVertices->GetPointer<float>()[6] = -1.0f;
  pVertices->GetPointer<float>()[7] =  1.0f;
  pVertices->GetPointer<float>()[8] = -1.0f;
  // 3. left-back-down
  pVertices->GetPointer<float>()[9]  = -1.0f;
  pVertices->GetPointer<float>()[10] = -1.0f;
  pVertices->GetPointer<float>()[11] = -1.0f;
  // 4. right-back-up
  pVertices->GetPointer<float>()[12] =  1.0f;
  pVertices->GetPointer<float>()[13] =  1.0f;
  pVertices->GetPointer<float>()[14] = -1.0f;
  // 5. right-back-down
  pVertices->GetPointer<float>()[15] =  1.0f;
  pVertices->GetPointer<float>()[16] = -1.0f;
  pVertices->GetPointer<float>()[17] = -1.0f;
  // 6. right-front-up
  pVertices->GetPointer<float>()[18] =  1.0f;
  pVertices->GetPointer<float>()[19] =  1.0f;
  pVertices->GetPointer<float>()[20] =  1.0f;
  // 7. right-front-down
  pVertices->GetPointer<float>()[21] =  1.0f;
  pVertices->GetPointer<float>()[22] = -1.0f;
  pVertices->GetPointer<float>()[23] =  1.0f;
}
/////////////////////////////////////////////////////////////////
inline void CreateIndices( Moose::Graphics::CIndexArray *pIndices )
{
  // left
    pIndices->GetPointer<unsigned short int>()[0] = 0;
    pIndices->GetPointer<unsigned short int>()[1] = 1;
    pIndices->GetPointer<unsigned short int>()[2] = 3;
    pIndices->GetPointer<unsigned short int>()[3] = 0;
    pIndices->GetPointer<unsigned short int>()[4] = 3;
    pIndices->GetPointer<unsigned short int>()[5] = 2;
    // back
    pIndices->GetPointer<unsigned short int>()[6] = 2;
    pIndices->GetPointer<unsigned short int>()[7] = 3;
    pIndices->GetPointer<unsigned short int>()[8] = 5;
    pIndices->GetPointer<unsigned short int>()[9] = 2;
    pIndices->GetPointer<unsigned short int>()[10] = 5;
    pIndices->GetPointer<unsigned short int>()[11] = 4;
    // right
    pIndices->GetPointer<unsigned short int>()[12] = 4;
    pIndices->GetPointer<unsigned short int>()[13] = 5;
    pIndices->GetPointer<unsigned short int>()[14] = 7;
    pIndices->GetPointer<unsigned short int>()[15] = 4;
    pIndices->GetPointer<unsigned short int>()[16] = 7;
    pIndices->GetPointer<unsigned short int>()[17] = 6;
    
    // roof
    pIndices->GetPointer<unsigned short int>()[18] = 2;
    pIndices->GetPointer<unsigned short int>()[19] = 4;
    pIndices->GetPointer<unsigned short int>()[20] = 6;
    pIndices->GetPointer<unsigned short int>()[21] = 2;
    pIndices->GetPointer<unsigned short int>()[22] = 6;
    pIndices->GetPointer<unsigned short int>()[23] = 0;

    // front
    pIndices->GetPointer<unsigned short int>()[24] = 0;
    pIndices->GetPointer<unsigned short int>()[25] = 6;
    pIndices->GetPointer<unsigned short int>()[26] = 7;
    pIndices->GetPointer<unsigned short int>()[27] = 0;
    pIndices->GetPointer<unsigned short int>()[28] = 7;
    pIndices->GetPointer<unsigned short int>()[29] = 1;

    // floor 
    pIndices->GetPointer<unsigned short int>()[30] = 1;
    pIndices->GetPointer<unsigned short int>()[31] = 7;
    pIndices->GetPointer<unsigned short int>()[32] = 5;
    pIndices->GetPointer<unsigned short int>()[33] = 1;
    pIndices->GetPointer<unsigned short int>()[34] = 5;
    pIndices->GetPointer<unsigned short int>()[35] = 3;
    
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CSkybox::CSkybox() 
{
  m_hModel = MOOSE_SKYBOX_MODEL;

  if ( m_hModel.IsNull() )
  {
    // create model
    CModel *pModel = new CModel();
    // create texcoords
    CVertexDescriptor *pTexCoords = new Moose::Graphics::CVertexDescriptor( Moose::Graphics::ELEMENT_TYPE_TEX_3F, 8);
    CreateTexCoords(pTexCoords);
    int iRetval = g_VertexMgr->Create( pTexCoords, MOOSE_SKYBOX_TEXCOORDS, pModel->GetTextureCoordinateHandle(0));
    assert(iRetval == 0);
    // vertices
    CVertexDescriptor *pVertices = new Moose::Graphics::CVertexDescriptor( Moose::Graphics::ELEMENT_TYPE_VERTEX_3F, 8);
    CreateVertices( pVertices );
    iRetval = g_VertexMgr->Create( pVertices, MOOSE_SKYBOX_VERTICES, pModel->GetVertexHandle());
    assert( iRetval == 0);
    // indices
    CIndexArray *pIndices = new Moose::Graphics::CIndexArray( Moose::Graphics::PRIMITIVE_TRI_LIST, 36);
    CreateIndices( pIndices );
    iRetval = g_IndexMgr->Create( pIndices, MOOSE_SKYBOX_INDICES, pModel->GetIndices());
    assert(iRetval == 0);
    // manage actual model
    g_ModelMgr->Create( pModel, MOOSE_SKYBOX_MODEL, m_hModel);
  }

  // Set usually required renderstate.
  CRenderState & state = GetRenderState();
  
  // Set shader and create if missing 
  state.GetShaderHandle() = MOOSE_SKYBOX_SHADER;
  if ( state.GetShaderHandle().IsNull())
  {
    // OpenGL context needs to be created at this point.
    CShader *pShader = new CShader();
    pShader->LoadVertexShader(CAssetBundle::GetInstance()->GetAssetPath("Shaders/skybox.vert"));
    pShader->LoadFragmentShader(CAssetBundle::GetInstance()->GetAssetPath("Shaders/skybox.frag"));
    
    //pShader->CreateVertexShaderFromSource(SB_VERT_SH, "skybox_vertex_shader");
    //pShader->CreateFragmentShaderFromSource(SB_FRAG_SH, "skybox_frag_shader");
    int iRetval = g_ShaderMgr->Create(pShader, MOOSE_SKYBOX_SHADER, state.GetShaderHandle());
    assert( iRetval == 0);
  }
  state.AddTextureFilter(T_WRAP_CLAMP_TO_EDGE);
  state.AddTextureFilter(S_WRAP_CLAMP_TO_EDGE);
  state.AddTextureFilter(R_WRAP_CLAMP_TO_EDGE);
  state.AddTextureFilter( MIN_LINEAR );
  state.AddTextureFilter( MAG_LINEAR );
  state.GetLighting() = false;
  state.GetDepthTest() = false;
  state.GetFaceCulling() = false;
  state.GetDepthWrite() = false;
  // necessary params
  state.AddShaderAttrib("position", (*m_hModel)->GetVertexHandle() );
  state.AddShaderAttrib("a_texcoord", (*m_hModel)->GetTextureCoordinateHandle() );
  state.AddShaderUniform("diffuse", 0); 
  state.SetRenderLayer( kBackground );
  bool bVal = state.Prepare();
  assert(bVal);
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CSkybox::~CSkybox()
{
  
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CSkybox::Render( Moose::Graphics::COglRenderer & renderer)
{



        
  CModel *pModel = *GetModelHandle();
  COglTexture *pTexture = *GetRenderState().GetTextureHandle(0);
  CIndexArray *pIndices = *pModel->GetIndices();

	
  for( unsigned int i=0; i<TEXTURE_HANDLE_COUNT; i++)
  {
 
    pTexture = *GetRenderState().GetTextureHandle(i);
    //// check that texture resource exists
    if ( pTexture  != NULL )
    {
      
      renderer.CommitTexture( i, pTexture );
      renderer.CommitFilters( GetRenderState().GetTextureFilters(i), pTexture->GetType() );
    }
    else 
    { 
      renderer.DisableTexture(i, NULL);
    }
  } 

  CShader *pShader = *GetRenderState().GetShaderHandle();
  renderer.CommitShader( pShader );

  if ( !GetRenderState().GetShaderHandle().IsNull())
  {
    GetRenderState().GetShaderAttribs().Apply(renderer);
    GetRenderState().GetShaderUniforms().Apply(renderer);
    if ( renderer.GetCurrentCamera() )
    {
      // Update matrices 
      GetRenderState().GetShaderViewUniform().SetData(      &renderer.GetCurrentCamera()->GetViewMatrix());
      GetRenderState().GetShaderProjectionUniform().SetData(&renderer.GetCurrentCamera()->GetProjectionMatrix());
      // Send data to shader
      GetRenderState().GetShaderViewUniform().Apply(renderer);
      GetRenderState().GetShaderProjectionUniform().Apply(renderer);
    }
  }
  
  renderer.CommitRenderState( GetRenderState() );
  renderer.CommitPrimitive( pIndices );

  renderer.DisableTexture(0, pTexture);
  renderer.DisableCaches();


  /////////////////////////////////////////////////////////////////
  /*#if defined(MOOSE_APPLE_IPHONE)
#else
  glPushMatrix();
  glLoadTransposeMatrixf( renderer.GetCurrentCamera()->GetViewMatrix().GetArray());

  CModel *pModel = *GetModelHandle();
  COglTexture *pTexture = *GetRenderState().GetTextureHandle(0);
  CIndexArray *pIndices = *pModel->GetIndices();
  CVertexDescriptor *pTexCoords = *pModel->GetTextureCoordinateHandle(0);
  CVertexDescriptor *pVertices  = *pModel->GetVertexHandle();
  renderer.CommitRenderState( GetRenderState() );
  // draw actual skybox
  if ( pVertices )  renderer.CommitVertexDescriptor( pVertices );
  if ( pTexCoords ) renderer.CommitVertexDescriptor( pTexCoords );
  if ( pTexture )
  {
  renderer.CommitTexture( 0, pTexture );
  renderer.CommitFilters( GetRenderState().GetTextureFilters(0), pTexture->GetType());
}
  if ( pIndices )   renderer.CommitPrimitive( pIndices );

  renderer.DisableTexture(0, pTexture);
  renderer.CommitShader( NULL );
  renderer.DisableCaches();
  glUseProgram(0);

  for( int i=0;i<8;i++)
  {
  glClientActiveTexture( GL_TEXTURE0 + i);
  glDisableClientState( GL_TEXTURE_COORD_ARRAY);
}

  /////////////////////////////////////////////////////////////////
  glPopMatrix();
#endif
  */
}


