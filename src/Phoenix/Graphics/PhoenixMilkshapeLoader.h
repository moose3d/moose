#ifndef __PhoenixMilkshapeLoader_h__
#define __PhoenixMilkshapeLoader_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixVertexDescriptor.h>
#include <PhoenixIndexArray.h>
#include <PhoenixSpatial.h>
#include <PhoenixModelLoader.h>
#include "PhoenixAPI.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Data
  {
#ifdef __GNUC__
#define PACKED __attribute__ ((packed))
#else
#define PACKED
#endif
#pragma pack(1)
    /////////////////////////////////////////////////////////////////
    // So we can use the structs directly while reading the MS3D file

    /////////////////////////////////////////////////////////////////
    // Loader class for Milkshape 3D format (.ms3d) v1.3 & v1.4
    // using http://astronomy.swin.edu.au/~pbourke/dataformats/ms3d/ms3dspec.h 
    // as basis.
    /////////////////////////////////////////////////////////////////
    // The milkshape3d format is structured as follows:
    // 1.  MS3D_Header_t
    // 2.  WORD nNumVertices
    // 3.  nNumVertices * MS3D_Vertex_t
    // 4.  WORD nNumTriangles
    // 5.  nNumTriangles * MS3D_Triangle_t
    // 6.  WORD nNumGroups
    // 7.  nNumGroups * MS3D_Group_t
    // 8.  WORD nNumMaterials
    // 9.  nNumMaterials * MS3D_Material_t
    // 10. MS3D_Keyframer_data_t
    // 11. WORD nNumJoints
    // 12. nNumJoints * MS3D_Joint_t
    // 13. int SubVersion, must be 1 ( additional stuff attached to file )
    // 14. int nNumGroupComments
    // 15. nNumGroupComments * MS3D_Comment_t
    // 16. int nNumMaterialComments
    // 17. nNumMaterialComments * MS3D_Comment_t
    // 18. int nNumJointComments 
    // 19. nNumJointComments * MS3D_Comment_t
    // 20. int nHasModelComment 
    // 21. nHasModelComment * MS3D_Comment_t
    // 22. nNumVertices * MS3D_Vertex_ex_t
    //
    //
    /////////////////////////////////////////////////////////////////
    // Mesh Transformation:
    // 
    // 0. Build the transformation matrices from the rotation and position
    // 1. Multiply the vertices by the inverse of local reference matrix (lmatrix0)
    // 2. then translate the result by (lmatrix0 * keyFramesTrans)
    // 3. then multiply the result by (lmatrix0 * keyFramesRot)
    //
    // For normals skip step 2.
    //
    /////////////////////////////////////////////////////////////////
#ifndef BYTE
    /// typedef for BYTE type.
    typedef unsigned char BYTE;
#endif
#ifndef WORD
    /// typedef for WORD type.
    typedef unsigned short WORD;
#endif
    /////////////////////////////////////////////////////////////////
    // Max values as presented at 
    // http://www.swissquake.ch/chumbalum-soft/ms3d/help/htmlfiles/Appendices/MS3DFAQ.html
    //
    /////////////////////////////////////////////////////////////////
#define MAX_VERTICES    8192
#define MAX_TRIANGLES   16384
#define MAX_GROUPS      128
#define MAX_MATERIALS   128
#define MAX_JOINTS      128
#define MAX_KEYFRAMES	256
    /////////////////////////////////////////////////////////////////
    // flags
    /////////////////////////////////////////////////////////////////
#define SELECTED        1
#define HIDDEN          2
#define SELECTED2       4
#define DIRTY           8
    /////////////////////////////////////////////////////////////////

    ////////////////////
    /// MS3D_Header_t struct
    struct MS3D_Header_t 
    {
      /// should be MS3D000000.
      char  id[10];
      /// we accept either 3 or 4.
      int	version;	
    } PACKED;
    ////////////////////
    /// MS3D_Vertex_t struct
    struct  MS3D_Vertex_t
    {
      /// Accepted values SELECTED | SELECTED2 | HIDDEN.
      BYTE flags;		
      /// Vertex coordinates.
      float vertex[3];	
      ///  -1 indicates no bone assignment.
      char boneId;
      /// Used by milkshape.
      BYTE referenceCount;
    } PACKED;
    ////////////////////
    /// MS3D_Triangle_t struct.
    struct MS3D_Triangle_t
    {
      /// Used by milkshape.
      WORD flags;
      /// Indices of vertices.
      WORD vertexIndices[3];
      /// Vertex normals for each vertex.
      float vertexNormals[3][3];
      /// Texture s coordinate for triangle's  vertices.
      float s[3];
      /// Texture t coordinate for triangle's vertices.
      float t[3];			
      /// Used by milkshape, accepted values 1-32.
      BYTE  smoothingGroup; 
      /// To which group this triangle belongs to.
      BYTE  groupIndex;
    } PACKED;
    ////////////////////
    /// MS3D_Group_t struct.
    struct MS3D_Group_t
    {
      /// SELECTED | HIDDEN.
      BYTE flags;			
      /// Group name.
      char name[32];
      /// Triangles are organized by groups.
      WORD nNumTriangles;		
      /// iTriangleIndices[nNumTriangles].
      WORD *pTriangleIndices;	
      /// -1 = no material.
      char materialIndex;		
    } PACKED;
    ////////////////////
    /// MS3D_Material_t struct.
    struct MS3D_Material_t
    {
      /// Material name.
      char  name[32];
      /// Ambient color.
      float ambient[4];
      /// Diffuse color.
      float diffuse[4];
      /// Specular color.
      float specular[4];
      /// Emission color.
      float emission[4];
      /// Shininess value, 0.0f - 128.0f.
      float shininess;		
      /// Transparency value, 0.0f - 1.0f 
      float transparency;		
      /// Unused, but 0, 1 or 2.
      char mode;			
      /// The texture file basename.
      char texture[128];		
      /// The alphamap file basename.
      char alphamap[128];		
    } PACKED;
    ////////////////////
    /// MS3D_Keyframer_data_t
    struct MS3D_Keyframer_data_t 
    {
      /// Animation speed.
      float	fAnimationFPS;
      /// propably used only in Milkshape3D modeler.
      float	fCurrentTime; 
      /// Total number of frames in animation.
      int	iTotalFrames;
    } PACKED;
    ////////////////////
    /// MS3D_Keyframe_rot_t
    struct MS3D_Keyframe_rot_t
    {
      /// Event time in seconds.
      float		time;		
      /// x,y,z rotation angles.
      float		rotation[3];	
    } PACKED;
    ////////////////////
    /// MS3D_Keyframe_pos_t
    struct MS3D_Keyframe_pos_t
    {
      /// Event time in seconds.
      float		time;		
      /// x,y,z position.
      float		position[3];	
    } PACKED;
    ////////////////////
    /// MS3D_Joint_t
    struct  MS3D_Joint_t 
    {
      /// Eiher SELECTED | DIRTY.
      BYTE		flags;
      /// Name of this joint.
      char		name[32];
      /// Name of the parent joint.
      char		parentName[32];
      /// Local reference matrix for rotations; the euler angles.
      float		rotation[3];
      /// Local reference matrix for translations; the euler angles.
      float		position[3];
      /// Number of rotation keyframes.  
      WORD		nNumKeyFramesRot;	
      /// Number of translation keyframes.
      WORD		nNumKeyFramesTrans;     
      /// Local animation matrices, MS3D_Keyframe_rot_t[nNumKeyFramesRot].
      MS3D_Keyframe_rot_t	*keyFramesRot;		
      /// Local animation matrices, MS3D_Keyframe_pos_t[nNumKeyFramesTrans].
      MS3D_Keyframe_pos_t	*keyFramesTrans;	
    } PACKED;
    ////////////////////
    /// MS3D_Comment_t
    struct MS3D_Comment_t
    { 
      /// index of the group, material or joint.
      int index;
      /// length of the comment, terminator char is not saved.
      int iCommentLength;				
      /// the comment itself, sComment[iCommentLength].
      char *sComment;				
    } PACKED;
    ////////////////////
    /// MS3D_Vertex_ex_t
    struct  MS3D_Vertex_ex_t
    {
      /// index of the joint or -1, With -1 weight is ignored.
      char boneIds[3];
      /// vertex weight, range 0 - 255, last weight is computed by 1.0 - sum(all weights).
      BYTE weights[3];  
      // weights[0] is the weight for boneId in MS3D_Vertex_t
      // weights[1] is the weight for boneIds[0]
      // weights[2] is the weight for boneIds[1]
      // 1.0f - weights[0] - weights[1] - weights[2] is the weight for boneIds[2]
    } PACKED;
#pragma pack()
    struct MSVertexCompare
    {
      bool operator()( const Phoenix::Spatial::CVertex & v1, const Phoenix::Spatial::CVertex & v2) const
      {
	if ( v1.GetPosition()[0] < v2.GetPosition()[0] ) return true;
	return false;

      }
    };
    typedef std::map<Phoenix::Spatial::CVertex, size_t, Phoenix::Data::MSVertexCompare> VertexIndexMap;
    /////////////////////////////////////////////////////////////////
    /// Actual model class, which encapsulates structs above.
    class PHOENIX_API CMilkshapeLoader : public Phoenix::Data::CModelLoader
    {
    public:
      /// Vertex array size.
      WORD m_nNumVertices;
      /// Triangle array size.
      WORD m_nNumTriangles;
      /// Group array size.
      WORD m_nNumGroups;
      /// Material array size.
      WORD m_nNumMaterials;
      /// Joint array size.
      WORD m_nNumJoints;
      
      /////////////////////////////////////////////////////////////////
      /// Vertex data itself.
      MS3D_Vertex_t         *m_pVertices;
      /// Triangle data itself.
      MS3D_Triangle_t       *m_pTriangles;
      /// Group data itself.
      MS3D_Group_t          *m_pGroups;
      /// Material data itself.
      MS3D_Material_t       *m_pMaterials;
      /// Keyframe data itself.
      MS3D_Keyframer_data_t m_Animationdata;
      /// Joint data itself.
      MS3D_Joint_t          *m_pJoints;
      /// Vertex weight data itself (not currently used).
      MS3D_Vertex_ex_t      *m_pVertexWeights;
      /////////////////////////////////////////////////////////////////
      //
      // Optional comments 
      //
      /////////////////////////////////////////////////////////////////
      //MS3D_Comment_t  *m_pGroupComments;
      //MS3D_Comment_t  *m_pMaterialComments;
      //MS3D_Comment_t  *m_pJointComments;
      //MS3D_Comment_t  *m_pModelComments;
      //unsigned int m_nNumGroupComments;
      //unsigned int m_nNumMaterialComments;
      //unsigned int m_nNumJointComments;
      //unsigned int m_nHasModelComment;
    public:
      ////////////////////
      /// Constructor.
      CMilkshapeLoader();
      ////////////////////
      /// Desctructor.
      virtual ~CMilkshapeLoader();
      ////////////////////
      /// Loads the model data from file.
      /// \param  sFilename Path to file as string.
      /// \returns Non-zero on error, zero otherwise.
      int Load(const std::string &sFilename);
      ////////////////////
      /// Loads the model data from file.
      /// \param szFilename Path to file as zero-terminated char array.
      /// \returns Non-zero on error, zero otherwise.
      int Load(const char * szFilename);
      Phoenix::Graphics::CVertexDescriptor * GetVertexArray( float fScale = 1.0f) const;
      Phoenix::Graphics::CVertexDescriptor * GetColorArray() const;
      Phoenix::Graphics::CVertexDescriptor * GetNormalArray() const;
      Phoenix::Graphics::CVertexDescriptor * GetTexCoordArray( size_t nTexUnit = 0) const;
      Phoenix::Graphics::CVertexDescriptor * GetInterleavedArray( Phoenix::Graphics::ELEMENT_TYPE tType = Phoenix::Graphics::ELEMENT_TYPE_V3F_N3F_T2F) const;
      Phoenix::Graphics::CIndexArray *       GetIndexArray( const char *szGroupName = NULL ) const;
      ///////////////////
      /// Checks whether file has proper header.
      /// \param szFilename file to be checked.
      /// \returns true if ok, false otherwise.
      static bool IsMilkshapeFile( const char *szFilename );

    private:
      ////////////////////
      /// Creates vertexdescriptors for vertex positions, normals, colors, texcoord and indices.
      /// \param iVertexCompareFlags Which components of vertices are used in comparision when creating lists. By default, all.
      void GenerateModelData();
      ////////////////////
      /// Copy constructor (since model has pointer members).
      CMilkshapeLoader( const CMilkshapeLoader &ref) {}
      ////////////////////
      /// Assignment operator ( since model has pointer members).
      void operator=( const CMilkshapeLoader & obj ) {}
      ////////////////////
      /// Initializes the model structures.
      void		Init();
      ////////////////////
      /// Frees the memory reserved by the model.
      void		Destroy();
      ////////////////////
      /// Validates the file - returns 0 if ms3d v.1.3-1.4, non-zero otherwise.
      int		 Check_File( std::fstream &File);
      ////////////////////
      /// Reads entire file into a buffer, including the header.
      /// \param File fstream object representing MS3D file.
      /// \param pFilesize pointer to unsigned int variable where file size in bytes is stored.
      unsigned char *Read_Into_Buffer( std::fstream &File, unsigned int *pFilesize);
      ////////////////////
      /// Handles vertex data from buffer, assumes the first item to be the number of vertices.
      /// \param pWorkBuffer Byte buffer where data is read from.
      /// \param pEnd The end of the buffer.
      unsigned char *Handle_Vertices( unsigned char *pWorkBuffer, unsigned char *pEnd );
      ////////////////////
      /// Processes triangles.
      /// \param pWorkBuffer Byte buffer where data is read from.
      unsigned char *Handle_Triangles( unsigned char *pWorkBuffer);
      ////////////////////
      /// Processes Groups.
      /// \param pWorkBuffer Byte buffer where data is read from.
      unsigned char *Handle_Groups( unsigned char *pWorkBuffer);
      ////////////////////
      /// Processes Groups.
      /// \param pWorkBuffer Byte buffer where data is read from.
      unsigned char *Handle_Materials( unsigned char *pWorkBuffer);
      ////////////////////
      /// Processes Keyframes.
      /// \param pWorkBuffer Byte buffer where data is read from.
      unsigned char *Handle_Keyframer( unsigned char *pWorkBuffer);
      ////////////////////
      /// Processes Joints.
      /// \param pWorkBuffer Byte buffer where data is read from.
      unsigned char *Handle_Joints( unsigned char *pWorkBuffer);
      ////////////////////
      /// Creates IndexArrays for each group, stores them into a map.
      /// Should be called after CreateTriangleList( ... ).
      void CreateGroupIndexMap( std::vector<Phoenix::Spatial::CVertex> &vecVertices );
      ////////////////////
      /// Creates new vertex for every occasion where position is same
      /// but normal and/or texture coordinate is different.
      /// \param vecVertices Vector of new vertices.
      /// \param vecIndices Vector of new indices representing triangle list.
      void CreateTriangleList( std::vector<Phoenix::Spatial::CVertex> &vecVertices, std::vector<unsigned int> &vecIndices );
    };
    //////////////////////////////////////////////////////////////// 
  }; // namespace Data
}; // namespace Phoenix

#endif
