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
      virtual Phoenix::Graphics::CVertexDescriptor * GetTexCoordArray( size_t nTexUnit = 0) = 0;
      virtual Phoenix::Graphics::CVertexDescriptor * GetColorArray() = 0;
      virtual Phoenix::Graphics::CVertexDescriptor * GetNormalArray() = 0;
      virtual Phoenix::Graphics::CVertexDescriptor * GetVertexArray() = 0;

      virtual Phoenix::Graphics::CVertexDescriptor * GetInterleavedArray( Phoenix::Graphics::ELEMENT_TYPE tType = 
									  Phoenix::Graphics::ELEMENT_TYPE_V3F_N3F_T2F) = 0;
      
      virtual Phoenix::Graphics::CIndexArray *	     GetIndices()  = 0;
    };
  }
}
////////////////////////////////////////////////////////////////
#endif
