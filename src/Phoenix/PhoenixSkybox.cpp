#include "PhoenixSkybox.h"
#include "PhoenixVertexDescriptor.h"
#include <iostream>
#include <assert.h>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
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
  pVertices->GetPointer<float>()[0] = -1.0f;
  pVertices->GetPointer<float>()[1] =  1.0f;
  pVertices->GetPointer<float>()[2] =  1.0f;
  
  pVertices->GetPointer<float>()[3] = -1.0f;
  pVertices->GetPointer<float>()[4] = -1.0f;
  pVertices->GetPointer<float>()[5] =  1.0f;

  pVertices->GetPointer<float>()[6] = -1.0f;
  pVertices->GetPointer<float>()[7] =  1.0f;
  pVertices->GetPointer<float>()[8] = -1.0f;
    
  pVertices->GetPointer<float>()[9]  = -1.0f;
  pVertices->GetPointer<float>()[10] = -1.0f;
  pVertices->GetPointer<float>()[11] = -1.0f;
    
  pVertices->GetPointer<float>()[12] =  1.0f;
  pVertices->GetPointer<float>()[13] =  1.0f;
  pVertices->GetPointer<float>()[14] = -1.0f;
    
  pVertices->GetPointer<float>()[15] =  1.0f;
  pVertices->GetPointer<float>()[16] = -1.0f;
  pVertices->GetPointer<float>()[17] = -1.0f;
    
  pVertices->GetPointer<float>()[18] =  1.0f;
  pVertices->GetPointer<float>()[19] =  1.0f;
  pVertices->GetPointer<float>()[20] =  1.0f;

  pVertices->GetPointer<float>()[21] =  1.0f;
  pVertices->GetPointer<float>()[22] = -1.0f;
  pVertices->GetPointer<float>()[23] =  1.0f;
}
/////////////////////////////////////////////////////////////////
inline void CreateIndices( Phoenix::Graphics::CIndexArray *pIndices )
{
    pIndices->GetPointer<unsigned short int>()[0] = 0;
    pIndices->GetPointer<unsigned short int>()[1] = 1;
    pIndices->GetPointer<unsigned short int>()[2] = 3;
    pIndices->GetPointer<unsigned short int>()[3] = 2;

    pIndices->GetPointer<unsigned short int>()[4] = 2;
    pIndices->GetPointer<unsigned short int>()[5] = 3;
    pIndices->GetPointer<unsigned short int>()[6] = 5;
    pIndices->GetPointer<unsigned short int>()[7] = 4;

    pIndices->GetPointer<unsigned short int>()[8] = 4;
    pIndices->GetPointer<unsigned short int>()[9] = 5;
    pIndices->GetPointer<unsigned short int>()[10] = 7;
    pIndices->GetPointer<unsigned short int>()[11] = 6;

    pIndices->GetPointer<unsigned short int>()[12] = 2;
    pIndices->GetPointer<unsigned short int>()[13] = 4;
    pIndices->GetPointer<unsigned short int>()[14] = 6;
    pIndices->GetPointer<unsigned short int>()[15] = 0;

    pIndices->GetPointer<unsigned short int>()[16] = 0;
    pIndices->GetPointer<unsigned short int>()[17] = 6;
    pIndices->GetPointer<unsigned short int>()[18] = 7;
    pIndices->GetPointer<unsigned short int>()[19] = 1;

    /*pIndices->GetPointer<unsigned short int>()[20] = 1;
    pIndices->GetPointer<unsigned short int>()[21] = 7;
    pIndices->GetPointer<unsigned short int>()[22] = 5;
    pIndices->GetPointer<unsigned short int>()[23] = 2;*/
    // This fixes problem on ATI hardware.
    pIndices->GetPointer<unsigned short int>()[20] = 2;
    pIndices->GetPointer<unsigned short int>()[21] = 5;
    pIndices->GetPointer<unsigned short int>()[22] = 7;
    pIndices->GetPointer<unsigned short int>()[23] = 1;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CSkybox::CSkybox() 
{
  
  Phoenix::Graphics::CVertexDescriptor *pTexCoords = g_DefaultVertexManager->GetResource(PHOENIX_SKYBOX_TEXCOORDS);
  Phoenix::Graphics::CVertexDescriptor *pVertices  = g_DefaultVertexManager->GetResource(PHOENIX_SKYBOX_VERTICES);
  Phoenix::Graphics::CIndexArray *pIndices         = g_DefaultIndexManager->GetResource(PHOENIX_SKYBOX_INDICES);
  
  if ( pTexCoords == NULL )
  {
    pTexCoords = new Phoenix::Graphics::CVertexDescriptor( Phoenix::Graphics::ELEMENT_TYPE_TEX_3F, 8);
    CreateTexCoords(pTexCoords);
    assert(g_DefaultVertexManager->Create( pTexCoords, PHOENIX_SKYBOX_TEXCOORDS, GetTextureCoordinateHandle(0)) == 0);
  }
  else
    assert(g_DefaultVertexManager->AttachHandle( PHOENIX_SKYBOX_TEXCOORDS, GetTextureCoordinateHandle(0)) == 0);
  
  /////////////////////////////////////////////////////////////////
  if ( pVertices == NULL )
  {
    pVertices = new Phoenix::Graphics::CVertexDescriptor( Phoenix::Graphics::ELEMENT_TYPE_VERTEX_3F, 8);    
    CreateVertices( pVertices );
    assert(g_DefaultVertexManager->Create( pVertices, PHOENIX_SKYBOX_VERTICES, GetVertexHandle()) == 0);
  }
  else
    assert(g_DefaultVertexManager->AttachHandle( PHOENIX_SKYBOX_VERTICES, GetVertexHandle()) == 0);

  /////////////////////////////////////////////////////////////////
  pIndices = g_DefaultIndexManager->GetResource( PHOENIX_SKYBOX_INDICES);
  /////////////////////////////////////////////////////////////////
  if ( pIndices == NULL )
  {

    pIndices = new Phoenix::Graphics::CIndexArray( Phoenix::Graphics::PRIMITIVE_QUAD_LIST, 24);
    CreateIndices( pIndices );
    assert(g_DefaultIndexManager->Create( pIndices, PHOENIX_SKYBOX_INDICES, GetIndices()) == 0);
  } else
    assert(g_DefaultIndexManager->AttachHandle( PHOENIX_SKYBOX_INDICES, GetIndices()) == 0);
  
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CSkybox::~CSkybox()
{
  
}
/////////////////////////////////////////////////////////////////

