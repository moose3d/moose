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
    
    struct ObjNormal
    {
      float x,y,z;
    };
    
    /// negative index stands for NOT USED.
    struct ObjFace
    {
      ObjFace() : v1(-1), v2(-1), v3(-1),
		  t1(-1), t2(-1), t3(-1),
		  n1(-1), n2(-1), n3(-1)  {}
      
      int v1,v2,v3;
      int t1,t2,t3;
      int n1,n2,n3;
      
    };

    // for easier updating
    typedef std::vector<Phoenix::Data::ObjVertex>   ObjVertexVector;
    typedef std::vector<Phoenix::Data::ObjNormal>   ObjNormalVector;
    typedef std::vector<Phoenix::Data::ObjTexCoord> ObjTexCoordVector;
    typedef std::vector<Phoenix::Data::ObjFace>     ObjFaceVector;
    
    /////////////////////////////////////////////////////////////////
    /// Class for loading .obj models.
    class CObjLoader : public CModelLoader
    {
    private:
      /// vertices.
      Phoenix::Data::ObjVertexVector   m_Vertices;
      /// Normals.
      Phoenix::Data::ObjNormalVector   m_Normals;
      // texcoords
      Phoenix::Data::ObjTexCoordVector m_TexCoords;
      /// Faces.
      Phoenix::Data::ObjFaceVector     m_Faces;
      /// Currently processed line.
      size_t			       m_currLine;
    public:
      ////////////////////
      /// Loads model.
      /// \param szFilename name of file.
      int Load( const char *szFilename );
      
      Phoenix::Graphics::CVertexDescriptor * GetTexCoordArray( size_t nTexUnit = 0);
      Phoenix::Graphics::CVertexDescriptor * GetColorArray();
      Phoenix::Graphics::CVertexDescriptor * GetNormalArray();
      Phoenix::Graphics::CVertexDescriptor * GetVertexArray();

      Phoenix::Graphics::CVertexDescriptor * GetInterleavedArray( Phoenix::Graphics::ELEMENT_TYPE tType = 
								  Phoenix::Graphics::ELEMENT_TYPE_V3F_N3F_T2F);
      
      Phoenix::Graphics::CIndexArray *	     GetIndices();      
    private:
      ////////////////////
      /// Parses position data from zero-terminated line.
      /// \param szLine line to be parsed.
      void ParsePosition( const char *szLine );
      ////////////////////
      /// Parses normal data from zero-terminated line.
      /// \param szLine line to be parsed.
      void ParseNormal( const char *szLine );
      ////////////////////
      /// Parses texcoord data from zero-terminated line.
      /// \param szLine line to be parsed.
      void ParseTexCoord( const char *szLine );
      ////////////////////
      /// Parses face data from zero-terminated line.
      /// \param szLine line to be parsed.
      void ParseFace( const char *szLine );
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
