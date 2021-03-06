/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#ifndef __MooseEnergyBeam_h__
#define __MooseEnergyBeam_h__
/////////////////////////////////////////////////////////////////
#include <MooseModel.h>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class for energy beams.
    class CEnergyBeam : public Moose::Graphics::CModel,
			public Moose::Core::CEnableable
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
	  pIndices->GetPointer<unsigned short int>()[0] = 3;
	  pIndices->GetPointer<unsigned short int>()[1] = 2;
	  pIndices->GetPointer<unsigned short int>()[2] = 1;
	  pIndices->GetPointer<unsigned short int>()[3] = 0;

	  g_DefaultIndexManager->Create( pIndices, strBeamShaderIndices, GetIndexHandles().back());
	}
	else
	{
	  g_DefaultIndexManager->AttachHandle( strBeamShaderIndices, GetIndexHandles().back() );
	}
    
	stream.str("");
	stream << "EnergyBeam_" << nNum << "_params";
	//std::cerr << "Stream contents is : " << stream.str() << std::endl;
	m_pTimeParam = new CVertexDescriptor( ELEMENT_TYPE_UNIFORM_2F, 1 );
	m_pTimeParam->GetPointer<float>()[0] = 0.0f;
    	m_pTimeParam->GetPointer<float>()[1] = 1.0f;
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
      void Initialize( const Moose::Math::CVector3<float> & vStartPos, const Moose::Math::CVector3<float> &vEndPos, float fThickness )
      {
    
	CVertexDescriptor *pEndposThickness = g_DefaultVertexManager->GetResource( Moose::Graphics::CModel::GetShaderParameters()[0].second  );

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
	if ( IsEnabled()) 
	{
	  m_pTimeParam->GetPointer<float>()[1] -= fValue;
	  if ( m_pTimeParam->GetPointer<float>()[1] <= 0.0f )
	  {
	    SetEnabled(0);
	  }
	}
      }
      ////////////////////
      /// Overrides behaviour.
      void SetEnabled(int bFlag ) 
      { 
	CEnableable::SetEnabled( bFlag );
	if ( bFlag == 1 )
	{
	  m_pTimeParam->GetPointer<float>()[1] = 1.0f;
	}
      }

    };
  } // namespace EnergyBeam
} // namespace Moose
/////////////////////////////////////////////////////////////////
// Initialize beam count.
unsigned int Moose::Graphics::CEnergyBeam::m_nNumBeams = 0;
/////////////////////////////////////////////////////////////////
#endif
