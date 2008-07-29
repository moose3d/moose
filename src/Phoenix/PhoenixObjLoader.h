#ifndef __PhoenixObjLoader_h__
#define __PhoenixObjLoader_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixModelLoader.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Data
  {
    struct ObjVertex
    {
      float x,y,z;
    };

    struct ObjTexCoord
    {
      float u,v;
    };
    
    typedef ObjVertex ObjNormal;
    
    struct ObjFace
    {
      size_t v1,v2,v3;
      size_t t1,t2,t3;
      size_t n1,n2,n3;
    };
    
    typedef std::vector<Phoenix::Data::ObjVertex>   ObjVertexVector;
    typedef std::vector<Phoenix::Data::ObjNormal>   ObjNormalVector;
    typedef std::vector<Phoenix::Data::ObjTexCoord> ObjTexCoordVector;
    typedef std::vector<Phoenix::Data::ObjFace>     ObjFaceVector;

    class CObjLoader : public CModelLoader
    {
    private:
      Phoenix::Data::ObjVertexVector   m_Vertices;
      Phoenix::Data::ObjNormalVector   m_Normals;
      Phoenix::Data::ObjTexCoordVector m_TexCoords;
      Phoenix::Data::ObjFaceVector     m_Faces;
      size_t			       m_currLine;
    public:

      int Load( const char *szFilename );

      Phoenix::Graphics::CVertexDescriptor * GetTexCoordArray( size_t nTexUnit = 0);
      Phoenix::Graphics::CVertexDescriptor * GetColorArray();
      Phoenix::Graphics::CVertexDescriptor * GetNormalArray();
      Phoenix::Graphics::CVertexDescriptor * GetVertexArray();

      Phoenix::Graphics::CVertexDescriptor * GetInterleavedArray( Phoenix::Graphics::ELEMENT_TYPE tType = 
								  Phoenix::Graphics::ELEMENT_TYPE_V3F_N3F_T2F);
      
      Phoenix::Graphics::CIndexArray *	     GetIndices();      
    private:
      void ParsePosition( const char *szLine );
      void ParseNormal( const char *szLine );
      void ParseTexCoord( const char *szLine );
      void ParseFace( const char *szLine );
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
