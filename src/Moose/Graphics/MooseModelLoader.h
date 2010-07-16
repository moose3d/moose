#ifndef __MooseModelLoader_h__
#define __MooseModelLoader_h__
////////////////////////////////////////////////////////////////
#include "MooseVertexDescriptor.h"
#include "MooseIndexArray.h"
#include "MooseDefaultEntities.h"
#include "MooseAPI.h"
#include <map>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Data
  {
    class MOOSE_API CModelLoader
    {
    protected:
      /// Vertex positions.
      Moose::Graphics::CVertexDescriptor *m_pPositions;
      /// Vertex normals.
      Moose::Graphics::CVertexDescriptor *m_pNormals;
      /// Vertex colors.
      Moose::Graphics::CVertexDescriptor *m_pColors;
      /// Vertex texture coordinates.
      Moose::Graphics::CVertexDescriptor *m_pTexCoords;
      /// Vector of triangle indices (either a strip with one list or just a list).
      Moose::Graphics::CIndexArray      *m_pIndices;
      ////////////////////
      /// Constructor. Initializes attributes to NULL.
      CModelLoader() : m_pPositions(NULL),
		       m_pNormals(NULL),
		       m_pColors(NULL),
		       m_pTexCoords(NULL),
		       m_pIndices(NULL) {}
      /// Map of group names to index arrays.
      std::map<std::string, Moose::Graphics::CIndexArray *> m_mapGroups;
    public:
      virtual ~CModelLoader();
      virtual int Load( const char *szFilename ) = 0;
      virtual Moose::Graphics::CVertexDescriptor * GetTexCoordArray( size_t nTexUnit = 0) const = 0;
      virtual Moose::Graphics::CVertexDescriptor * GetColorArray() const = 0;
      virtual Moose::Graphics::CVertexDescriptor * GetNormalArray() const = 0;
      virtual Moose::Graphics::CVertexDescriptor * GetVertexArray( float fScale = 1.0f) const = 0;

      virtual Moose::Graphics::CVertexDescriptor * GetInterleavedArray( Moose::Graphics::ELEMENT_TYPE tType =
									  Moose::Graphics::ELEMENT_TYPE_V3F_N3F_T2F) const = 0;

      virtual Moose::Graphics::CIndexArray *	     GetIndexArray( const char *szGroupName = NULL ) const = 0;

      virtual bool  HasTexCoordArray( size_t nTexUnit = 0) const	{ return  ( m_pTexCoords != NULL); }
      virtual bool  HasColorArray() const  											 	{ return  ( m_pPositions != NULL); }
      virtual bool  HasNormalArray() const 												{ return  ( m_pNormals   != NULL); }
      virtual bool  HasVertexArray() const 												{ return  ( m_pColors    != NULL); }
      virtual bool  HasIndexArray() const  												{ return  ( m_pIndices   != NULL); }
    };
  }
}
////////////////////////////////////////////////////////////////
#endif
