#ifndef PHOENIXVERTEX_H_
#define PHOENIXVERTEX_H_
/////////////////////////////////////////////////////////////
#include <iostream>
#include <PhoenixVector2.h>
#include <PhoenixVector3.h>
#include <PhoenixVector4.h>
#include <PhoenixGlobals.h>
#include <PhoenixPositional.h>
/////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Spatial
  {
  /// Vertex comparison flags
  enum VERTEX_COMP_FLAGS
  {
    VERTEX_COMP_POSITION  = 1 << 0,
    VERTEX_COMP_TEXCOORD  = 1 << 1,
    VERTEX_COMP_COLOR     = 1 << 2,
    VERTEX_COMP_NORMAL    = 1 << 3
  };
  ////////////////////
  /// Vertex class for some cases where it is easier to handle things as a blob.
  class PHOENIX_API CVertex : public Phoenix::Spatial::CPositional
  {
  public:
    /// Vertex normal.
    Phoenix::Math::CVector3<float> m_vNormal;
    /// Vertex texture coordinates.
    Phoenix::Math::CVector2<float> m_vTexCoord[TEXTURE_HANDLE_COUNT];
    /// Vertex color.
    Phoenix::Math::CVector4<unsigned char> m_vColor;

    Phoenix::Math::CVector4<float> m_vAttrib4;
    Phoenix::Math::CVector3<float> m_vAttrib3;
    Phoenix::Math::CVector2<float> m_vAttrib2;
    float			     m_fAttrib1;

    ////////////////////
    /// Constructor.
    CVertex()
    {
	// init normal to neg z-axis
	m_vNormal[0] =  0.0f;
	m_vNormal[1] =  0.0f;
	m_vNormal[2] = -1.0f;
	// init texture coordiantes to zero
	for(int iT=0;iT<TEXTURE_HANDLE_COUNT;iT++)
	{
	  m_vTexCoord[iT][0] = 0.0f;
	  m_vTexCoord[iT][1] = 0.0f;
	}
	// Reset color to grayish
	m_vColor[0] = m_vColor[1] = m_vColor[2] = 200;
	m_vColor[3] = 255;

	m_vAttrib4[0] = m_vAttrib4[1] = m_vAttrib4[2] = m_vAttrib4[3] = 0.0f;
	m_vAttrib3[0] = m_vAttrib3[1] = m_vAttrib3[2] = 0.0f;
	m_vAttrib2[0] = m_vAttrib2[1] = 0.0f;
	m_fAttrib1 = 0.0f;

    }

    //       bool operator==( const CVertex & vert)
//       {
// 	for(int iT=0;iT<TEXTURE_HANDLE_COUNT;iT++)
// 	{
// 	  if ( !(m_vTexCoord[iT] == vert.m_vTexCoord[iT]) )
// 	  {
// 	    return 0;
// 	  }
// 	}
// 	if ( !(GetPosition() == vert.GetPosition()))
// 	{
// 	  return 0;
// 	}
// 	if ( !(m_vNormal     == vert.m_vNormal))     { return 0; }
// 	if ( !(m_vColor      == vert.m_vColor ))     { return 0; }
// 	return 1;
//       }
    ////////////////////
    /// The equality comparison operator.
    /// \param vert CVertex object which this is compared against.
    /// \param iCompFlags Which parts of vertex data is compared. By default, all of them.
    bool Compare( const CVertex & vert, int iCompFlags = VERTEX_COMP_TEXCOORD | VERTEX_COMP_POSITION | VERTEX_COMP_NORMAL | VERTEX_COMP_COLOR )
    {
	// texture coordinate comparsion
	if ( iCompFlags & VERTEX_COMP_TEXCOORD )
	{
	  for(int iT=0;iT<TEXTURE_HANDLE_COUNT;iT++)
	  {
	    if ( !(m_vTexCoord[iT] == vert.m_vTexCoord[iT]) )
	    {
	      return 0;
	    }
	  }
	}

	// position comparision
	if ( iCompFlags & VERTEX_COMP_POSITION )
	{
	  if ( !(GetPosition() == vert.GetPosition()))
	  {
	    return 0;
	  }
	}

	// normal comparison
	if ( iCompFlags & VERTEX_COMP_NORMAL )
	{
	  if ( !(m_vNormal     == vert.m_vNormal))     { return 0; }
	}

	// color comparison
	if ( iCompFlags & VERTEX_COMP_COLOR )
	{
	  if ( !(m_vColor      == vert.m_vColor ))     { return 0; }
	}

	return 1;
    }
    ////////////////////
    /// The less than comparison operator.
    bool operator< (const CVertex & vert)
    {
	return m_vPosition[1] < vert.m_vPosition[1];
    }
    ////////////////////
    /// Assigns normal vector.
    /// \param vNormal Normal vector.
    inline void SetNormal( const Phoenix::Math::CVector3<float> &vNormal )
    {
	m_vNormal = vNormal;
    }
    ////////////////////
    /// Assigns normal vector.
    /// \param vNormal Normal vector.
    inline void SetNormal( float fX, float fY, float fZ )
    {
	m_vNormal[0] = fX;
	m_vNormal[1] = fY;
	m_vNormal[2] = fZ;
    }
    ////////////////////
    /// Returns normal vector.
    /// \returns Normal vector reference.
    inline const Phoenix::Math::CVector3<float> & GetNormal() const
    {
	return m_vNormal;
    }
    ////////////////////
    /// Sets texture coordinate.
    /// \param fS S-coordinate.
    /// \param fT T-coordinate.
    /// \param nTexUnit To which texunit coordinates apply. By default, it is zero.
    void SetTextureCoordinates( float fS, float fT, unsigned int nTexUnit = 0 )
    {
	m_vTexCoord[nTexUnit][0] = fS;
	m_vTexCoord[nTexUnit][1] = fT;
    }
    ////////////////////
    /// Sets texture coordinate.
    /// \param vCoords Coordinates.
    /// \param nTexUnit To which texunit coordinates apply. By default, it is zero.
    void SetTextureCoordinates( Phoenix::Math::CVector2<float> vCoords, unsigned int nTexUnit = 0 )
    {
	m_vTexCoord[nTexUnit] = vCoords;
    }
    ////////////////////
    /// Returns texture coordinates.
    /// \param nTexUnit To which texunit coordinates apply. By default, it is zero.
    /// \returns Texture coordinate vector.
    const Phoenix::Math::CVector2<float> & GetTextureCoordinates( unsigned int nTexUnit = 0 ) const
    {
	return m_vTexCoord[nTexUnit];
    }
    ////////////////////
    /// Returns color vector.
    /// \return Color vector.
    const Phoenix::Math::CVector4<unsigned char> & GetColor() const
    {
	return m_vColor;
    }
    ////////////////////
    /// Returns color vector.
    /// \return Color vector.
    Phoenix::Math::CVector4<unsigned char> & GetColor()
    {
	return m_vColor;
    }
    ////////////////////
    /// Assigns color vector.
    /// \param vColor Color to be assigned.
    void SetColor( const Phoenix::Math::CVector4<unsigned char> & vColor )
    {
	m_vColor = vColor;
    }
    ////////////////////
    /// Assigns vertex attribute values for shader.
    /// \param vValues Vector containing attrib values.
    void SetAttrib4( const Phoenix::Math::CVector4<float> & vValues )
    {
	m_vAttrib4 = vValues;
    }
    ////////////////////
    /// Assigns vertex attribute values for shader.
    /// \param vValues Vector containing attrib values.
    void SetAttrib3( const Phoenix::Math::CVector3<float> & vValues )
    {
	m_vAttrib3 = vValues;
    }
    ////////////////////
    /// Assigns vertex attribute values for shader.
    /// \param vValues Vector containing attrib values.
    void SetAttrib2( const Phoenix::Math::CVector2<float> & vValues )
    {
	m_vAttrib2 = vValues;
    }
    ////////////////////
    /// Assigns vertex attribute value for shader.
    /// \param fValue Attrib value.
    void SetAttrib1( const float & fValue )
    {
	m_fAttrib1 = fValue;
    }
    /////////////////////////////
    /// Returns vertex attribute.
    /// \returns vertex attribute
    const float & GetAttrib1() const
    {
	return m_fAttrib1;
    }
    /////////////////////////////
    /// Returns vertex attribute.
    /// \returns vertex attribute
    const Phoenix::Math::CVector2<float> & GetAttrib2() const
    {
	return m_vAttrib2;
    }
    /////////////////////////////
    /// Returns vertex attribute.
    /// \returns vertex attribute
    const Phoenix::Math::CVector3<float> & GetAttrib3() const
    {
	return m_vAttrib3;
    }
    /////////////////////////////
    /// Returns vertex attribute.
    /// \returns vertex attribute
    const Phoenix::Math::CVector4<float> & GetAttrib4() const
    {
	return m_vAttrib4;
    }
    ////////////////////
    /// For debugging.
    friend std::ostream & operator<<( std::ostream & stream, const CVertex & vertex)
    {
	stream << "pos: " << vertex.GetPosition();
	stream << "tc:  " ;
	for(int iT=0;iT<TEXTURE_HANDLE_COUNT;iT++)
	{
	  stream << vertex.m_vTexCoord[iT] << ",";
	}
	stream << "n: " << vertex.m_vNormal << ",";
	stream << "c: " << vertex.m_vColor;
	return stream;
    }
  };
  }
}
/////////////////////////////////////////////////////////////
#endif /* PHOENIXVERTEX_H_ */
