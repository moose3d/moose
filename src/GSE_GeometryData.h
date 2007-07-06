/////////////////////////////////////////////////////////////////
// file : GSE_GeometryData.h
// desc : Header file for the GeometryData and GeometryDataMgr classes
// author : AG/eNtity
/////////////////////////////////////////////////////////////////
#ifndef __GeometryData_h__
#define __GeometryData_h__
/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
#include "GSE_structures.h"
// ---------------------------------------------------------------
//
// The GeometryData is the foundation for geometry data in
// every 3D Object, containing vertex colors, texture coordinates
// normals, and position data.
// --------------------------------------------------------------------
namespace Phoenix 
{
  class Vertex 
  {
    
    Math::Vector3f    m_vPosition;
    Math::Vector3f  * m_vTexCoords[MAX_TEXCOORD_ARRAYS];
    Math::Vector4us   m_vColor;
    Math::Vector3f    m_vNormal;
    
  };
  
  class VertexBuffer
  {  
  public:
    // The caching methods for each of the arrays
    typedef enum {
      
      NO_CACHE = 0,		// Use normal vertex arrays
      REQUEST_VBO_CACHE = 1,	// Set the first VBO caching to occur next time
				// when renderer makes its pass
      CACHED_IN_VBO,		// Renderer has cached this.
      REGENERATE_VBO_CACHE,	// Renderer will regenerate the cache next time.
      DELETE_VBO_CACHE	        // Renderer will delete cache during next pass 
				// and set method to NO_CACHE. If you use this, you 
				// better MAKE SURE object will be destroyed after this.
      /////////////////////////////////////////////////////////////////
      // REQUEST_DL_CACHE,        // Set the first DL caching to occur next time
      // when renderer makes its pass
      // CACHED_IN_DL,		// Renderer has cached this.
      // REGENERATE_DL_CACHE,	// Renderer will regenerate the cache next time.
      // DELETE_DL_CACHE	        // Renderer will delete cache and set method
      // to NO_CACHE
      /////////////////////////////////////////////////////////////////
    } CacheMethod_t;
    
    // The targets which can be cached, names should be clear
    typedef enum {
      VERTEX_ARRAY = 0,
      TEXCOORD0_ARRAY = 1,
      TEXCOORD1_ARRAY = 2,
      TEXCOORD2_ARRAY = 3,
      TEXCOORD3_ARRAY = 4,
      TEXCOORD4_ARRAY = 5,
      TEXCOORD5_ARRAY = 6,
      TEXCOORD6_ARRAY = 7,
      TEXCOORD7_ARRAY = 8,
      NORMAL_ARRAY = 9,
      COLOR_ARRAY = 10

    } CacheTarget_t;
    // --------------------------------------------------------------------
  protected:
    // the array where cache method is stored. The values stored in the array
    // do not directly affect the caching, it is only a hint for the renderer.
    CacheMethod_t  m_CacheMethod[GSE_MAX_CACHE_TARGETS];    

    // This is for the OpenGL-specific name resolution 
    // of Display Lists (DL) and Vertex Buffer Objects (VBO)
    GLuint	m_ObjectName[GSE_MAX_CACHE_TARGETS];

  public:
    
    // The vertices for this object 
    Vertex *		m_pVertices;	
    PositionArray *	m_pPositions;
    //
    // The texture coordinates; The U and V coordinates are per vertex.
    //
    TextureCoordArray     m_pTexCoords[GSE_MAX_TEXCOORD_ARRAYS];
    //
    // The normal vectors for each vertex.
    //
    NormalArray		m_pNormals;    
    // The vertex colors
    ColorArray		m_pColors;
    // The number of elements in this struct
    unsigned int		m_iNumElements;
    // --------------------------------------------------------------------
    // Assigns the number of elements in this GeometryData
    void SetVertexCount( unsigned int iNumElements );
    // Initializes the memory for the vertex array
    void InitVertices();
    // Initializes the memory for the normal array
    void InitNormals();
    // Initializes the memory for the TexCoord array
    void InitTexCoords(unsigned int iMultiTexId);
    // Initializes the memory for the vertex color array
    void InitColors();
    // Does this GeometryData contain color buffer
    char HasColors();
    // Does this GeometryData contain vertex buffer
    char HasVertices();
    // Does this GeometryData contain normal vector buffer
    char HasNormals();
    // Does this GeometryData contain texture coordinate buffer
    char HasTexCoords( unsigned int nMultiTexCoordId );
    // --------------------------------------------------------------------
    // The default constructor, NULLifies pointers
    GSE_GeometryData();
    // The copy constructor
    GSE_GeometryData(const GSE_GeometryData &ref);
    // The assignment operator
    GSE_GeometryData &operator=(GSE_GeometryData obj);
    // The destructor
    ~GSE_GeometryData();
    // --------------------------------------------------------------------
    // Assigns coordinates to iVertIndexth vertex from GSE_Vector3 vCoords
    inline void SetVertex( unsigned int iVertIndex, GSE_Vector3 &vCoords)
    {
      unsigned int iIndex = (3 * iVertIndex);
      if ( m_pVertices != NULL ){
      
	m_pVertices[iIndex]   = vCoords[GSE_Vector3::X];
	m_pVertices[iIndex+1] = vCoords[GSE_Vector3::Y];
	m_pVertices[iIndex+2] = vCoords[GSE_Vector3::Z];

      } else {
      
	GSE_ERR( __FUNCTION__ << "m_pVertices is not initialized" );
      }
    }
    // Return the pointer to the coordinate array of iVertIndexth vertex
    VertexArray	GetVertexCoords( unsigned int iVertIndex );
    ////////////////////
    /// Sets the texture coordinates of the iVertexIndexth vertex
    void SetTexCoords( unsigned int iMultiTexId, 
		       unsigned int iVertexIndex, GSE_Vector2 vCoords );
    ////////////////////
    /// Sets the texture coordinates of the iVertexIndexth vertex
    void SetTexCoords( unsigned int iMultiTexId, 
		       unsigned int iVertexIndex, float fX, float fY );
    // returns the texture coordinates of the iVertexIndexth vertex
    TextureCoordArray GetTexCoords( unsigned int iMultiTexId, 
				    unsigned int iVertexIndex);
    // Assigns the normal vector values for iVertexIndexth vertex 
    inline void SetNormal( unsigned int iVertexIndex, GSE_Vector3 &vCoords )
    {
      unsigned int iIndex = (3 * iVertexIndex);
      if ( m_pNormals != NULL ){
      
	m_pNormals[iIndex]   = vCoords[GSE_Vector3::X];
	m_pNormals[iIndex+1] = vCoords[GSE_Vector3::Y];
	m_pNormals[iIndex+2] = vCoords[GSE_Vector3::Z];
      
      } else {
      
	GSE_ERR( __FUNCTION__ 
		 << "m_pNormals is not initialized!");

      
      }
    }
    // Returns the normal vector for iVertexIndexth vertex
    NormalArray GetNormal(unsigned int iVertexIndex);
    // Assigns the vertex color values for the iVertexIndexth vertex
    void SetColor( unsigned int iVertexIndex, GSE_Color vColor );
    // Returns the vertex color of the iVertexIndexth vertex
    ColorArray  GetColor(unsigned int iVertexIndex);
    // Sets the caching method for the specific target. 
    void SetCacheMethod( CacheTarget_t nTarget, CacheMethod_t nMethod );
    // Returns the caching method used by nTarget
    CacheMethod_t GetCacheMethod( CacheTarget_t nTarget);

    // Returns the reference to cache pointed by nTarget
    GLuint &Cache( CacheTarget_t nTarget );

  protected:
    // --------------------------------------------------------------------
    // Frees the memory
    void Destroy();
    // Sets pointers to NULL
    void Nullify();

  };
  /////////////////////////////////////////////////////////////////
  /// A singleton container class for GSE_GeometryDatas.
  /////////////////////////////////////////////////////////////////
  class GSE_GeometryDataMgr : public GSE_Container<GSE_GeometryData>, 
  public GSE_Singleton<GSE_GeometryDataMgr>
  {
    friend class GSE_Singleton<GSE_GeometryDataMgr>;
  private:
    /////////////////////////////////////////////////////////////////
    /// The constructor.
    GSE_GeometryDataMgr() : GSE_Container<GSE_GeometryData>() { }
    /////////////////////////////////////////////////////////////////
    /// The destructor.
    ~GSE_GeometryDataMgr()  {  }
  public:
    /////////////////////////////////////////////////////////////////
    /// Creates a new objstruct and manages it.
    GSE_GeometryData * CreateGeometryData()
    {
      GSE_GeometryData *pGeometryData = new GSE_GeometryData();
      Add(pGeometryData);
      return pGeometryData;
    }
  };
}; // end namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
