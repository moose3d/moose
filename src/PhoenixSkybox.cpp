#include "PhoenixSkybox.h"
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CSkybox::CSkybox() : CModel()
{
  Phoenix::Graphics::CVertexDescriptor *pTexCoords = g_DefaultVertexManager->GetResource(PHOENIX_SKYBOX_TEXCOORDS);
  Phoenix::Graphics::CVertexDescriptor *pVertices  = g_DefaultVertexManager->GetResource(PHOENIX_SKYBOX_VERTICES);
  Phoenix::Graphics::CIndexArray *pIndices         = g_DefaultIndexManager->GetResource(PHOENIX_SKYBOX_INDICES);
  
  if ( pTexCoords == NULL )
  {
    cerr << "no previous texcoords!" << endl;
    pTexCoords = new Phoenix::Graphics::CVertexDescriptor( Phoenix::Graphics::ELEMENT_TYPE_TEX_2F, 24);
    ////////////////////
    // floor
    pTexCoords->GetPointer<float>()[0] = 0.0f;
    pTexCoords->GetPointer<float>()[1] = 0.0f;

    pTexCoords->GetPointer<float>()[2] = 1.0f;
    pTexCoords->GetPointer<float>()[3] = 0.0f;

    pTexCoords->GetPointer<float>()[4] = 1.0f;
    pTexCoords->GetPointer<float>()[5] = 1.0f;
    
    pTexCoords->GetPointer<float>()[6] = 0.0f;
    pTexCoords->GetPointer<float>()[7] = 1.0f;
    ////////////////////
    // ceiling
    pTexCoords->GetPointer<float>()[8] = 0.0f;
    pTexCoords->GetPointer<float>()[9] = 0.0f;

    pTexCoords->GetPointer<float>()[10] = 1.0f;
    pTexCoords->GetPointer<float>()[11] = 0.0f;

    pTexCoords->GetPointer<float>()[12] = 1.0f;
    pTexCoords->GetPointer<float>()[13] = 1.0f;

    pTexCoords->GetPointer<float>()[14] = 0.0f;
    pTexCoords->GetPointer<float>()[15] = 1.0f;
    ////////////////////
    // front
    pTexCoords->GetPointer<float>()[16] = 0.0f;
    pTexCoords->GetPointer<float>()[17] = 0.0f;

    pTexCoords->GetPointer<float>()[18] = 1.0f;
    pTexCoords->GetPointer<float>()[19] = 0.0f;

    pTexCoords->GetPointer<float>()[20] = 1.0f;
    pTexCoords->GetPointer<float>()[21] = 1.0f;
    
    pTexCoords->GetPointer<float>()[22] = 0.0f;
    pTexCoords->GetPointer<float>()[23] = 1.0f;
    ////////////////////
    // back
    pTexCoords->GetPointer<float>()[24] = 0.0f;
    pTexCoords->GetPointer<float>()[25] = 0.0f;

    pTexCoords->GetPointer<float>()[26] = 1.0f;
    pTexCoords->GetPointer<float>()[27] = 0.0f;

    pTexCoords->GetPointer<float>()[28] = 1.0f;
    pTexCoords->GetPointer<float>()[29] = 1.0f;
    
    pTexCoords->GetPointer<float>()[30] = 0.0f;
    pTexCoords->GetPointer<float>()[31] = 1.0f;
    ////////////////////
    // left
    pTexCoords->GetPointer<float>()[32] = 0.0f;
    pTexCoords->GetPointer<float>()[33] = 0.0f;

    pTexCoords->GetPointer<float>()[34] = 1.0f;
    pTexCoords->GetPointer<float>()[35] = 0.0f;

    pTexCoords->GetPointer<float>()[36] = 1.0f;
    pTexCoords->GetPointer<float>()[37] = 1.0f;
    
    pTexCoords->GetPointer<float>()[38] = 0.0f;
    pTexCoords->GetPointer<float>()[39] = 1.0f;
    ////////////////////
    // right
    pTexCoords->GetPointer<float>()[40] = 0.0f;
    pTexCoords->GetPointer<float>()[41] = 0.0f;

    pTexCoords->GetPointer<float>()[42] = 1.0f;
    pTexCoords->GetPointer<float>()[43] = 0.0f;

    pTexCoords->GetPointer<float>()[44] = 1.0f;
    pTexCoords->GetPointer<float>()[45] = 1.0f;
    
    pTexCoords->GetPointer<float>()[46] = 0.0f;
    pTexCoords->GetPointer<float>()[47] = 1.0f;

    assert(g_DefaultVertexManager->Create( pTexCoords, PHOENIX_SKYBOX_TEXCOORDS, GetTextureCoordinateHandle(0)) == 0);
    
  }
  /////////////////////////////////////////////////////////////////
  if ( pVertices == NULL )
  {
    cerr << "no previous vertices!" << endl;
    pVertices = new Phoenix::Graphics::CVertexDescriptor( Phoenix::Graphics::ELEMENT_TYPE_VERTEX_3F, 24);    

    // Floor 
    pVertices->GetPointer<float>()[0] = -1.0f;
    pVertices->GetPointer<float>()[1] = -1.0f;
    pVertices->GetPointer<float>()[2] = -1.0f;

    pVertices->GetPointer<float>()[3] = -1.0f;
    pVertices->GetPointer<float>()[4] = -1.0f;
    pVertices->GetPointer<float>()[5] =  1.0f;

    pVertices->GetPointer<float>()[6] =  1.0f;
    pVertices->GetPointer<float>()[7] = -1.0f;
    pVertices->GetPointer<float>()[8] =  1.0f;

    pVertices->GetPointer<float>()[9] =  1.0f;
    pVertices->GetPointer<float>()[10] = -1.0f;
    pVertices->GetPointer<float>()[11] = -1.0f;
    
    // ceiling
    pVertices->GetPointer<float>()[12] = -1.0f;
    pVertices->GetPointer<float>()[13] =  1.0f;
    pVertices->GetPointer<float>()[14] = -1.0f;

    pVertices->GetPointer<float>()[15] =  1.0f;
    pVertices->GetPointer<float>()[16] =  1.0f;
    pVertices->GetPointer<float>()[17] = -1.0f;

    pVertices->GetPointer<float>()[18] =  1.0f;
    pVertices->GetPointer<float>()[19] =  1.0f;
    pVertices->GetPointer<float>()[20] =  1.0f;

    pVertices->GetPointer<float>()[21] = -1.0f;
    pVertices->GetPointer<float>()[22] =  1.0f;
    pVertices->GetPointer<float>()[23] =  1.0f;

    // front wall
    pVertices->GetPointer<float>()[24] = -1.0f;
    pVertices->GetPointer<float>()[25] = -1.0f;
    pVertices->GetPointer<float>()[26] = -1.0f;

    pVertices->GetPointer<float>()[27] =  1.0f;
    pVertices->GetPointer<float>()[28] = -1.0f;
    pVertices->GetPointer<float>()[29] = -1.0f;

    pVertices->GetPointer<float>()[30] =  1.0f;
    pVertices->GetPointer<float>()[31] =  1.0f;
    pVertices->GetPointer<float>()[32] = -1.0f;

    pVertices->GetPointer<float>()[33] =  -1.0f;
    pVertices->GetPointer<float>()[34] =   1.0f;
    pVertices->GetPointer<float>()[35] =  -1.0f;

    // back wall
    pVertices->GetPointer<float>()[36] =   1.0f;
    pVertices->GetPointer<float>()[37] =  -1.0f;
    pVertices->GetPointer<float>()[38] =   1.0f;
    
    pVertices->GetPointer<float>()[39] =  -1.0f;
    pVertices->GetPointer<float>()[40] =  -1.0f;
    pVertices->GetPointer<float>()[41] =   1.0f;

    pVertices->GetPointer<float>()[42] =  -1.0f;
    pVertices->GetPointer<float>()[43] =   1.0f;
    pVertices->GetPointer<float>()[44] =   1.0f;

    pVertices->GetPointer<float>()[45] =   1.0f;
    pVertices->GetPointer<float>()[46] =   1.0f;
    pVertices->GetPointer<float>()[47] =   1.0f;
    
    //left wall
    pVertices->GetPointer<float>()[48] =  -1.0f;
    pVertices->GetPointer<float>()[49] =  -1.0f;
    pVertices->GetPointer<float>()[50] =   1.0f;
    
    pVertices->GetPointer<float>()[51] =  -1.0f;
    pVertices->GetPointer<float>()[52] =  -1.0f;
    pVertices->GetPointer<float>()[53] =  -1.0f;

    pVertices->GetPointer<float>()[54] =  -1.0f;
    pVertices->GetPointer<float>()[55] =   1.0f;
    pVertices->GetPointer<float>()[56] =  -1.0f;

    pVertices->GetPointer<float>()[57] =  -1.0f;
    pVertices->GetPointer<float>()[58] =   1.0f;
    pVertices->GetPointer<float>()[59] =   1.0f;
    
    // right wall
    pVertices->GetPointer<float>()[60] =   1.0f;
    pVertices->GetPointer<float>()[61] =  -1.0f;
    pVertices->GetPointer<float>()[62] =  -1.0f;
    
    pVertices->GetPointer<float>()[63] =   1.0f;
    pVertices->GetPointer<float>()[64] =  -1.0f;
    pVertices->GetPointer<float>()[65] =   1.0f;

    pVertices->GetPointer<float>()[66] =   1.0f;
    pVertices->GetPointer<float>()[67] =   1.0f;
    pVertices->GetPointer<float>()[68] =   1.0f;

    pVertices->GetPointer<float>()[69] =   1.0f;
    pVertices->GetPointer<float>()[70] =   1.0f;
    pVertices->GetPointer<float>()[71] =  -1.0f;
    
    assert(g_DefaultVertexManager->Create( pVertices, PHOENIX_SKYBOX_VERTICES, GetVertexHandle()) == 0);
  }
  /////////////////////////////////////////////////////////////////
  pIndices = g_DefaultIndexManager->GetResource("PHOENIX_SKYBOX_INDICES1");
  if ( pIndices == NULL )
  {
    cerr << "no previous indices" << endl;
    pIndices = new Phoenix::Graphics::CIndexArray( Phoenix::Graphics::PRIMITIVE_QUAD_LIST, 4);
    pIndices->GetPointer<unsigned short int>()[0] = 0;
    pIndices->GetPointer<unsigned short int>()[1] = 1;
    pIndices->GetPointer<unsigned short int>()[2] = 2;
    pIndices->GetPointer<unsigned short int>()[3] = 3;
    
    AddIndexHandle( INDEX_HANDLE());
    assert(g_DefaultIndexManager->Create( pIndices, "PHOENIX_SKYBOX_INDICES1", GetIndexHandles().back()) == 0);
  }
  pIndices = g_DefaultIndexManager->GetResource("PHOENIX_SKYBOX_INDICES2");
  if ( pIndices == NULL )
  {
    pIndices = new Phoenix::Graphics::CIndexArray( Phoenix::Graphics::PRIMITIVE_QUAD_LIST, 4);
    pIndices->GetPointer<unsigned short int>()[0] = 4;
    pIndices->GetPointer<unsigned short int>()[1] = 5;
    pIndices->GetPointer<unsigned short int>()[2] = 6;
    pIndices->GetPointer<unsigned short int>()[3] = 7;
    AddIndexHandle( INDEX_HANDLE());
    assert(g_DefaultIndexManager->Create( pIndices, "PHOENIX_SKYBOX_INDICES2", GetIndexHandles().back()) == 0);
  }
  pIndices = g_DefaultIndexManager->GetResource("PHOENIX_SKYBOX_INDICES3");
  if ( pIndices == NULL )
  {
    pIndices = new Phoenix::Graphics::CIndexArray( Phoenix::Graphics::PRIMITIVE_QUAD_LIST, 4);
    pIndices->GetPointer<unsigned short int>()[0] = 8;
    pIndices->GetPointer<unsigned short int>()[1] = 9;
    pIndices->GetPointer<unsigned short int>()[2] = 10;
    pIndices->GetPointer<unsigned short int>()[3] = 11;
    AddIndexHandle( INDEX_HANDLE());
    assert(g_DefaultIndexManager->Create( pIndices, "PHOENIX_SKYBOX_INDICES3", GetIndexHandles().back()) == 0);
  }
  pIndices = g_DefaultIndexManager->GetResource("PHOENIX_SKYBOX_INDICES4");
  if ( pIndices == NULL )
  {
    pIndices = new Phoenix::Graphics::CIndexArray( Phoenix::Graphics::PRIMITIVE_QUAD_LIST, 4);
    pIndices->GetPointer<unsigned short int>()[0] = 12;
    pIndices->GetPointer<unsigned short int>()[1] = 13;
    pIndices->GetPointer<unsigned short int>()[2] = 14;
    pIndices->GetPointer<unsigned short int>()[3] = 15;
    AddIndexHandle( INDEX_HANDLE());
    assert(g_DefaultIndexManager->Create( pIndices, "PHOENIX_SKYBOX_INDICES4", GetIndexHandles().back()) == 0);
  }
  pIndices = g_DefaultIndexManager->GetResource("PHOENIX_SKYBOX_INDICES5");
  if ( pIndices == NULL )
  {
    pIndices = new Phoenix::Graphics::CIndexArray( Phoenix::Graphics::PRIMITIVE_QUAD_LIST, 4);
    pIndices->GetPointer<unsigned short int>()[0] = 16;
    pIndices->GetPointer<unsigned short int>()[1] = 17;
    pIndices->GetPointer<unsigned short int>()[2] = 18;
    pIndices->GetPointer<unsigned short int>()[3] = 19;
    AddIndexHandle( INDEX_HANDLE());
    assert(g_DefaultIndexManager->Create( pIndices, "PHOENIX_SKYBOX_INDICES5", GetIndexHandles().back()) == 0);
  }
  pIndices = g_DefaultIndexManager->GetResource("PHOENIX_SKYBOX_INDICES6");
  if ( pIndices == NULL )
  {
    pIndices = new Phoenix::Graphics::CIndexArray( Phoenix::Graphics::PRIMITIVE_QUAD_LIST, 4);
    pIndices->GetPointer<unsigned short int>()[0] = 20;
    pIndices->GetPointer<unsigned short int>()[1] = 21;
    pIndices->GetPointer<unsigned short int>()[2] = 22;
    pIndices->GetPointer<unsigned short int>()[3] = 23;
    AddIndexHandle( INDEX_HANDLE());
    assert(g_DefaultIndexManager->Create( pIndices, "PHOENIX_SKYBOX_INDICES6", GetIndexHandles().back()) == 0);
  }
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CSkybox::~CSkybox()
{
  
}
/////////////////////////////////////////////////////////////////
