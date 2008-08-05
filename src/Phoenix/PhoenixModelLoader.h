#ifndef __PhoenixModelLoader_h__
#define __PhoenixModelLoader_h__
////////////////////////////////////////////////////////////////
#include "PhoenixVertexDescriptor.h"
#include "PhoenixIndexArray.h"
#include "PhoenixDefaultEntities.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Data
  {
    class CModelLoader
    {
    protected:
      /// Vertex positions.
      Phoenix::Graphics::CVertexDescriptor *m_pPositions;
      /// Vertex normals.
      Phoenix::Graphics::CVertexDescriptor *m_pNormals;
      /// Vertex colors.
      Phoenix::Graphics::CVertexDescriptor *m_pColors;
      /// Vertex texture coordinates.
      Phoenix::Graphics::CVertexDescriptor *m_pTexCoords;
      /// Vector of triangle indices (either a strip with one list or just a list).
      Phoenix::Graphics::CIndexArray      *m_pIndices;
      ////////////////////
      /// Constructor. Initializes attributes to NULL.
      CModelLoader() : m_pPositions(NULL),
		       m_pNormals(NULL),
		       m_pColors(NULL),
		       m_pTexCoords(NULL),
		       m_pIndices(NULL) {} 
      /// Map of group names to index arrays.
      std::map<std::string, Phoenix::Graphics::CIndexArray *> m_mapGroups;
    public:
      virtual ~CModelLoader();
      virtual int Load( const char *szFilename ) = 0;
      virtual Phoenix::Graphics::CVertexDescriptor * GetTexCoordArray( size_t nTexUnit = 0) const = 0;
      virtual Phoenix::Graphics::CVertexDescriptor * GetColorArray() const = 0;
      virtual Phoenix::Graphics::CVertexDescriptor * GetNormalArray() const = 0;
      virtual Phoenix::Graphics::CVertexDescriptor * GetVertexArray( float fScale = 1.0f) const = 0;

      virtual Phoenix::Graphics::CVertexDescriptor * GetInterleavedArray( Phoenix::Graphics::ELEMENT_TYPE tType = 
									  Phoenix::Graphics::ELEMENT_TYPE_V3F_N3F_T2F) const = 0;
      
      virtual Phoenix::Graphics::CIndexArray *	     GetIndexArray( const char *szGroupName = NULL ) const = 0;
    };
  }
}
////////////////////////////////////////////////////////////////
#endif
