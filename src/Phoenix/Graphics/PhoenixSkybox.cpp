#include "PhoenixSkybox.h"
#include "PhoenixVertexDescriptor.h"
#include "PhoenixOGLRenderer.h"
#include <iostream>
#include <assert.h>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
inline void CreateTexCoords( Phoenix::Graphics::CVertexDescriptor *pTexCoords )
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
inline void CreateVertices( Phoenix::Graphics::CVertexDescriptor *pVertices )
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
inline void CreateIndices( Phoenix::Graphics::CIndexArray *pIndices )
{
  // left
    pIndices->GetPointer<unsigned short int>()[0] = 0;
    pIndices->GetPointer<unsigned short int>()[1] = 1;
    pIndices->GetPointer<unsigned short int>()[2] = 3;
    pIndices->GetPointer<unsigned short int>()[3] = 2;
    // back
    pIndices->GetPointer<unsigned short int>()[4] = 2;
    pIndices->GetPointer<unsigned short int>()[5] = 3;
    pIndices->GetPointer<unsigned short int>()[6] = 5;
    pIndices->GetPointer<unsigned short int>()[7] = 4;
    // right
    pIndices->GetPointer<unsigned short int>()[8] = 4;
    pIndices->GetPointer<unsigned short int>()[9] = 5;
    pIndices->GetPointer<unsigned short int>()[10] = 7;
    pIndices->GetPointer<unsigned short int>()[11] = 6;
    // roof
    pIndices->GetPointer<unsigned short int>()[12] = 2;
    pIndices->GetPointer<unsigned short int>()[13] = 4;
    pIndices->GetPointer<unsigned short int>()[14] = 6;
    pIndices->GetPointer<unsigned short int>()[15] = 0;
    // front
    pIndices->GetPointer<unsigned short int>()[16] = 0;
    pIndices->GetPointer<unsigned short int>()[17] = 6;
    pIndices->GetPointer<unsigned short int>()[18] = 7;
    pIndices->GetPointer<unsigned short int>()[19] = 1;
    
    pIndices->GetPointer<unsigned short int>()[20] = 1;
    pIndices->GetPointer<unsigned short int>()[21] = 7;
    pIndices->GetPointer<unsigned short int>()[22] = 5;
    pIndices->GetPointer<unsigned short int>()[23] = 3;
    
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CSkybox::CSkybox( ) 
{
  m_hModel = PHOENIX_SKYBOX_MODEL;

  if ( m_hModel.IsNull() )
  {
  	// create model
  	CModel *pModel = new CModel();
  	// create texcoords
  	CVertexDescriptor *pTexCoords = new Phoenix::Graphics::CVertexDescriptor( Phoenix::Graphics::ELEMENT_TYPE_TEX_3F, 8);
  	CreateTexCoords(pTexCoords);
  	assert(g_VertexMgr->Create( pTexCoords, PHOENIX_SKYBOX_TEXCOORDS, pModel->GetTextureCoordinateHandle(0)) == 0);
  	// vertices
  	CVertexDescriptor *pVertices = new Phoenix::Graphics::CVertexDescriptor( Phoenix::Graphics::ELEMENT_TYPE_VERTEX_3F, 8);
  	CreateVertices( pVertices );
  	assert(g_VertexMgr->Create( pVertices, PHOENIX_SKYBOX_VERTICES, pModel->GetVertexHandle()) == 0);
  	// indices
  	CIndexArray *pIndices = new Phoenix::Graphics::CIndexArray( Phoenix::Graphics::PRIMITIVE_QUAD_LIST, 24);
    CreateIndices( pIndices );
    assert(g_IndexMgr->Create( pIndices, PHOENIX_SKYBOX_INDICES, pModel->GetIndices()) == 0);
    // manage actual model
		g_ModelMgr->Create( pModel, PHOENIX_SKYBOX_MODEL, m_hModel);
  }
  // Set usually required renderstate.
  CRenderState & state = GetRenderState();
  state.AddTextureFilter(T_WRAP_CLAMP_TO_EDGE);
  state.AddTextureFilter(S_WRAP_CLAMP_TO_EDGE);
  state.AddTextureFilter(R_WRAP_CLAMP_TO_EDGE);
  state.AddTextureFilter( MIN_LINEAR );
  state.AddTextureFilter( MAG_LINEAR );
  state.GetLighting() = false;
  state.GetDepthTest() = false;
  state.GetFaceCulling() = false;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CSkybox::~CSkybox()
{
  
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CSkybox::Render( Phoenix::Graphics::COglRenderer & renderer)
{
	Phoenix::Graphics::CCamera & camera = *renderer.GetCurrentCamera();
	// Get view matrix, reset translation part
	Phoenix::Math::CMatrix4x4<float> mView = camera.GetViewMatrix();
	mView(0,3) = 0.0f;
	mView(1,3) = 0.0f;
	mView(2,3) = 0.0f;

	glPushMatrix();
	glLoadTransposeMatrixf( mView.GetArray());
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
}


