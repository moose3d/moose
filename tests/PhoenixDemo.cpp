#include "Phoenix.h"
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
using namespace Phoenix::Window;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
int main()
{
  if ( !CSDLScreen::GetInstance() )
  {
    std::cerr << "Couldn't open screen" << std::endl;
    return 1;
  }
  ////////////////////
  CVertexDescriptor *pVertices = new CVertexDescriptor( ELEMENT_TYPE_VERTEX_3F, 3 );

  pVertices->GetPointer<float>()[0] = -1.0f;
  pVertices->GetPointer<float>()[1] = 0.0f;
  pVertices->GetPointer<float>()[2] = 0.0f;
  
  pVertices->GetPointer<float>()[3] = 1.0f;
  pVertices->GetPointer<float>()[4] = 0.0f;
  pVertices->GetPointer<float>()[5] = 0.0f;

  pVertices->GetPointer<float>()[6] = 0.0f;
  pVertices->GetPointer<float>()[7] = 1.0f;
  pVertices->GetPointer<float>()[8] = 0.0f;
  ////////////////////  
  CVertexDescriptor *pColors = new CVertexDescriptor( ELEMENT_TYPE_COLOR_4UB, 3 );
  
  pColors->GetPointer<unsigned char>()[0] = 255;
  pColors->GetPointer<unsigned char>()[1] = 0;
  pColors->GetPointer<unsigned char>()[2] = 0;
  pColors->GetPointer<unsigned char>()[3] = 255;

  pColors->GetPointer<unsigned char>()[4] = 0;
  pColors->GetPointer<unsigned char>()[5] = 255;
  pColors->GetPointer<unsigned char>()[6] = 0;
  pColors->GetPointer<unsigned char>()[7] = 255;

  pColors->GetPointer<unsigned char>()[8] = 0;
  pColors->GetPointer<unsigned char>()[9] = 0;
  pColors->GetPointer<unsigned char>()[10] = 255;
  pColors->GetPointer<unsigned char>()[11] = 255;

  CIndexArray *pIndices = new CIndexArray( PRIMITIVE_TRI_LIST, 3 );
  pIndices->GetPointer<unsigned short int>()[0] = 0;
  pIndices->GetPointer<unsigned short int>()[1] = 1;
  pIndices->GetPointer<unsigned short int>()[2] = 2;

  COglRenderer *pOglRenderer = new COglRenderer();
  pOglRenderer->ClearBuffer( COLOR_BUFFER );
  pOglRenderer->ClearBuffer( DEPTH_BUFFER );
  
  pOglRenderer->CommitVertexDescriptor( pVertices );
  pOglRenderer->CommitVertexDescriptor( pColors );
  pOglRenderer->CommitPrimitive( pIndices );
  
  pOglRenderer->DisableClientState( CLIENT_STATE_VERTEX_ARRAY );
  pOglRenderer->DisableClientState( CLIENT_STATE_COLOR_ARRAY );
  pOglRenderer->Finalize();
  sleep(2);
  CSDLScreen::Destroy();
  return 0;
}
