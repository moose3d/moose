#ifndef __PhoenixEnergyBeam_h__
#define __PhoenixEnergyBeam_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixModel.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class for energy beams.
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
	std::stringstream stream;
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
    
	stream.str("");
	stream << "EnergyBeam_" << nNum << "_params";
	//std::cerr << "Stream contents is : " << stream.str() << std::endl;
	m_pTimeParam = new CVertexDescriptor( ELEMENT_TYPE_UNIFORM_1F, 1 );
	m_pTimeParam->GetPointer<float>()[0] = 0.0f;
    
	/// Create shader parameters.
	CVertexDescriptor *pEndposThickness = new CVertexDescriptor( ELEMENT_TYPE_ATTRIB_4F, 4);
	SetShaderParameter( "endposThickness", VERTEX_HANDLE());
	assert(g_DefaultVertexManager->Create( pEndposThickness, stream.str(), GetShaderParameters().back().second ) == 0);
	SetShaderParameter( "beamTexture", 0);
    
	stream.str("");
	stream << "EnergyBeam_" << nNum << "_time";
	SetShaderParameter( "time", VERTEX_HANDLE());
	assert(g_DefaultVertexManager->Create( m_pTimeParam, stream.str(), GetShaderParameters().back().second ) == 0);
	
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
  } // namespace EnergyBeam
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
// Initialize beam count.
unsigned int Phoenix::Graphics::CEnergyBeam::m_nNumBeams = 0;
/////////////////////////////////////////////////////////////////
#endif
