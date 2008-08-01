#ifndef __PhoenixModelLoader_h__
#define __PhoenixModelLoader_h__
////////////////////////////////////////////////////////////////
#include "PhoenixVertexDescriptor.h"
#include "PhoenixDefaultEntities.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Data
  {
    class CModelLoader
    {
    public:
      virtual ~CModelLoader() {}
      virtual int Load( const char *szFilename ) = 0;
      virtual Phoenix::Graphics::CVertexDescriptor * GetTexCoordArray( size_t nTexUnit = 0) const = 0;
      virtual Phoenix::Graphics::CVertexDescriptor * GetColorArray() const = 0;
      virtual Phoenix::Graphics::CVertexDescriptor * GetNormalArray() const = 0;
      virtual Phoenix::Graphics::CVertexDescriptor * GetVertexArray() const = 0;

      virtual Phoenix::Graphics::CVertexDescriptor * GetInterleavedArray( Phoenix::Graphics::ELEMENT_TYPE tType = 
									  Phoenix::Graphics::ELEMENT_TYPE_V3F_N3F_T2F) const = 0;
      
      virtual Phoenix::Graphics::CIndexArray *	     GetIndexArray( const char *szGroupName = NULL ) const = 0;
    };
  }
}
////////////////////////////////////////////////////////////////
#endif
