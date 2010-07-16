#ifndef __MooseObjLoader_h__
#define __MooseObjLoader_h__
/////////////////////////////////////////////////////////////////
#include "MooseModelLoader.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
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

      int v1,v2,v3;		// Vertices for face
      int t1,t2,t3;		// Texcoords for face vertices
      int n1,n2,n3;		// normals for face vertices
      
    };

    // for easier updating
    typedef std::vector<Moose::Data::ObjVertex>   ObjVertexVector;
    typedef std::vector<Moose::Data::ObjNormal>   ObjNormalVector;
    typedef std::vector<Moose::Data::ObjTexCoord> ObjTexCoordVector;
    typedef std::vector<Moose::Data::ObjFace>     ObjFaceVector;
    typedef std::list< std::string >                GroupList;
    typedef std::map< std::string, std::list<size_t> > GroupFaces;
    /////////////////////////////////////////////////////////////////
    /// Class for loading .obj models.
    class MOOSE_API CObjLoader : public CModelLoader
    {
    private:
      /// vertices.
      Moose::Data::ObjVertexVector   m_Vertices;
      /// Normals.
      Moose::Data::ObjNormalVector   m_Normals;
      // texcoords
      Moose::Data::ObjTexCoordVector m_TexCoords;
      /// Faces.
      Moose::Data::ObjFaceVector     m_Faces;
      /// Currently processed line.
      size_t			       m_currLine;
      /// Currently selected groups.
      GroupList	       m_currGroups;
      /// 
      GroupFaces	m_mapGroupFaces;
      std::string       m_objName;
    public:
      CObjLoader();
      virtual ~CObjLoader();
      ////////////////////
      /// Loads model.
      /// \param szFilename name of file.
      int Load( const char *szFilename );
      
      Moose::Graphics::CVertexDescriptor * GetTexCoordArray( size_t nTexUnit = 0) const;
      Moose::Graphics::CVertexDescriptor * GetColorArray() const;
      Moose::Graphics::CVertexDescriptor * GetNormalArray() const;
      Moose::Graphics::CVertexDescriptor * GetVertexArray( float fScale = 1.0f ) const;

      Moose::Graphics::CVertexDescriptor * GetInterleavedArray( Moose::Graphics::ELEMENT_TYPE tType = 
								  Moose::Graphics::ELEMENT_TYPE_V3F_N3F_T2F) const;
      
      Moose::Graphics::CIndexArray *	     GetIndexArray( const char *szGroupName = NULL ) const;      
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
      ////////////////////
      /// Parses group data from zero-terminated line.
      /// \param szLine line to be parsed.
      void ParseGroups( const char *szLine );
      ////////////////////
      /// Parses object name data from zero-terminated line.
      /// \param szLine line to be parsed.
      void ParseObject( const char *szLine );
      ////////////////////
      /// Generates separate arrays for position, normal, and texcoord data.
      void GenerateModelData();

    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
