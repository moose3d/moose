/////////////////////////////////////////////////////////////////
#ifndef __GSE_structures_h__
#define __GSE_structures_h__
/////////////////////////////////////////////////////////////////
#include "GSE_globals.h"
#include "GSE_Logger.h"
#include <math.h>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
/////////////////////////////////////////////////////////////////
/// The base class for color representation with RGBA values.
class GSE_Color : public GSE_Vector< float, 4 >
{
 public:
  /// The enumerations for color indices.
  typedef enum ColorIndex_t 
  {
    /// Symbolic index for RED value.
    R = 0,
    /// Symbolic index for GREEN value.
    G = 1,
    /// Symbolic index for BLUE value.
    B = 2,
    /// Symbolic index for ALPHA value.
    A = 3
  } ColorIndex;

  /// The Default Constructor.
  GSE_Color()
  {
    // It's almost...BLACK!!!
    m_pValues[R] = 0.0;
    m_pValues[G] = 0.0;
    m_pValues[B] = 0.0;
    m_pValues[A] = 0.0;

  }
  /// The parametrized constructor.
  GSE_Color( float *pArray ) : GSE_Vector<float,4>(pArray)
  {
  }
  /// The parametrized constructor.
  GSE_Color( float r, float g, float b, float a)
  {
    
    m_pValues[R] = r;
    m_pValues[G] = g;
    m_pValues[B] = b;
    m_pValues[A] = a;

  }
  /// Destructor.
  ~GSE_Color()
  {

  }
  /// Assigns color values.
  /// \param fR Red value.
  /// \param fG Green value.
  /// \param fB Blue value.
  /// \param fA Transparency value.
  inline void Set( float fR, float fG, float fB, float fA)
  {
    m_pValues[R] = fR;
    m_pValues[G] = fG;
    m_pValues[B] = fB;
    m_pValues[A] = fA;
  }
  /// Returns the value array.
  inline const float *GetValues() const
  { 
    return m_pValues; 
  }
  /// For assisting in the accessing of color components.
  inline float & operator[](ColorIndex index) const
  {
    return m_pValues[index];
  }
  /// For debugging.
  friend std::ostream &operator<<( std::ostream &stream, 
				   GSE_Color color ) 
  {
    stream << color.m_pValues[R] << ","
	   << color.m_pValues[G] << ","
	   << color.m_pValues[B] << ","
	   << color.m_pValues[A] ;
    return stream; 
  }
};
/////////////////////////////////////////////////////////////////
/// The base object for everything in the system.
class GSE_Object 
{

 private:
  /// How many objects have been created.
  static unsigned int m_nObjectCounter;
 protected:
  /// The identification number for the object.
  unsigned int	m_nId;
  /// Object name.
  char  m_sName[GSE_OBJECT_MAX_NAME_SIZE];
 public:
  /// Constructor.
  GSE_Object();

  /// Assigns the string sNewName to the object name. 
  /// The length of the sNewName must be less or equal  
  /// to GSE_OBJECT_MAX_NAME_SIZE. 
  /// If NULL is passed, Object name will be object<N> 
  /// where N is running object number.
  void SetName( char *sNewName );
  /// Returns the pointer to the name of the object.
  const char * GetName();
  /// Returns the ID number for this object.
  unsigned int GetID();
};
/////////////////////////////////////////////////////////////////
/// The base class for Objects with position.
class GSE_Positional 
{
 protected:
  /// Vector where position is stored.
  GSE_Vector3 m_vPosition;
  /// has the position changed.
  int	      m_bPositionChanged;
 public:
  /// Default constructor.
  GSE_Positional() : m_vPosition(0.0f,0.0f,0.0f),m_bPositionChanged(0)
  {

  }
  /// Parametrized constructor.
  /// \param vPosition vector for location-
  GSE_Positional( const GSE_Vector3 & vPosition ) : m_vPosition(vPosition),m_bPositionChanged(0)
  {
  }
  /// Assigns position.
  /// \param vPosition vector of which values are used.
  inline void SetPosition( const GSE_Vector3 &vPosition )
  {
    m_vPosition = vPosition;
    SetPositionChanged(1);
  }
  /// Assigns the position.
  /// \param fX position on X-axis.
  /// \param fY position on Y-axis.
  /// \param fZ position on Z-axis.
  inline void SetPosition( float fX, float fY, float fZ )
  {
    m_vPosition[0] = fX;
    m_vPosition[1] = fY;
    m_vPosition[2] = fZ;
    SetPositionChanged(1);
  }
  /// Returns position.
  inline const GSE_Vector3 & GetPosition() const
  {
    return m_vPosition;
  }
  /// Adds vector to current position.
  void Move( GSE_Vector3 vPosition )
  {
    m_vPosition = m_vPosition + vPosition;
    SetPositionChanged(1);
  }
  /// Returns true if position has changed.
  int IsPositionChanged() const
  {
    return m_bPositionChanged;
  }
  /// Sets the position change flag.
  inline void SetPositionChanged(int bFlag )
  {
    m_bPositionChanged = bFlag;
  }

};
/////////////////////////////////////////////////////////////////
/// The class for single-directional object.
class GSE_Directional 
{
 protected:
  /// Direction vector.
  GSE_Vector3 m_vDirection;
 public:
  /// Assigns the direction.
  inline void SetDirection( GSE_Vector3 vDirection )
  {
    m_vDirection = vDirection;
  }
  /// Returns the direction.
  inline GSE_Vector3 GetDirection()
  {
    return m_vDirection;
  }
};
/////////////////////////////////////////////////////////////////
/// A value list container.
template <typename T>
class GSE_ValueList 
{
 protected:
  /// List of values.
  std::list<T> m_lstValues;
 public:
  /// Constructor.
  GSE_ValueList()
  {
  }
  /// Destructor.
  ~GSE_ValueList()
  {
    Clear();
  }
  /// Adds value to list.
  inline void Add( T value )
  {
    m_lstValues.push_back(value);
  }
  /// Clears the list.
  inline void Clear()
  {
    m_lstValues.clear();
  }
  /// Returns a reference to the value list.
  const std::list<T> & GetValues()
  {
    return m_lstValues;
  }
};
/////////////////////////////////////////////////////////////////
/// Generic container class for several types.
/////////////////////////////////////////////////////////////////
template <class T>
class GSE_Container
{
 protected:
  /// vector of objects.
  std::vector<T *> m_Objects;
  
 public:
  /// Default constructor.
  GSE_Container()
  {
     
  }
  /// Destructor.
  ~GSE_Container()
  {
    GSE_DEBUG( "Deleting GSE_Container (" << m_Objects.size() <<")...");
    Clear();
    GSE_DEBUG( "Done deleting GSE_Container" );
  }
  /// deletes all objects.
  void Clear()
  {
    for( unsigned int i=0;i<m_Objects.size();i++)
    {
      GSE_DEBUG( "deleting " << i << "ptr:" << m_Objects[i] );
      if ( m_Objects[i] != NULL ) 
      {
	delete m_Objects[i];
      }
       
    }
    m_Objects.clear();
  }
  /// Adds an object to this container.
  /// \param pObject pointer to an object with type T.
  int Add( T *pObject )
  {
    int iIndex = -1;
    if ( pObject != NULL ){
      m_Objects.push_back(pObject);
      iIndex = m_Objects.size()-1;
    }
    return iIndex; 
  }
  /// Delete, removing in O( number of elements) time.
  /// \param nIndex from which index an object is removed.
  void Delete( unsigned int nIndex )
  {
    if ( m_Objects[nIndex] != NULL ) 
    {
      if (nIndex < m_Objects.size())
      {
	T *pTmp = m_Objects[nIndex];
	typename std::vector<T *>::iterator it = m_Objects.begin();
	it += nIndex;
	m_Objects.erase(it);
	delete pTmp;
	 
      }
       
    }
    
  }
  /// Delete, removing in O( number of elements) time.
  /// \param pObject a pointer to an object to be deleted from this container.
  void Delete( T *pObject )
  {
    typename std::vector< T * >::iterator itResult;
    itResult = find( m_Objects.begin(), m_Objects.end(), pObject );    
    if ( itResult != m_Objects.end() )
    {
      T *pDEADBEEF = *itResult;
      m_Objects.erase(itResult);
      delete pDEADBEEF;
    }
  }   
  /// Gets the number of objects in this container.
  /// \return number of objects.
  inline const int GetSize() const
  {
    return m_Objects.size();
  }
  /// Gets object from an index in the object vector.
  /// \return pointer to an object.
  T *At(unsigned int iIndex)
  {
    if ( iIndex >= 0 && iIndex < m_Objects.size() )
    {
      return m_Objects[iIndex];
    } 
    else
    {
      return NULL;
    }
    }
};
/////////////////////////////////////////////////////////////////
/// Singleton template, which makes creating singleton objects easier.
template <class T>
class GSE_Singleton
{

 private:
  static T *m_pSingleton;
  GSE_Singleton( const GSE_Singleton &ref){}
 protected:
  GSE_Singleton() {}
  virtual ~GSE_Singleton() {}
   
 public:
  /// Returns a pointer to this singleton.
  static T *GetInstance() 
  {
    //static T singleton;
    //m_pSingleton = &singleton;
 
    if ( m_pSingleton == NULL )
    {
      m_pSingleton = new T();
    }
    return m_pSingleton;
  }
  /// Destroys this singleton.
  static void Destroy()
  {
    if ( m_pSingleton != NULL )
    {
      delete m_pSingleton;
      //m_pSingleton = NULL;
    }
  }
};
/// Initialize the static member in the derived classes.
template<class T> T *GSE_Singleton<T>::m_pSingleton = NULL;

#ifndef _WIN32EXPORT 
// WHOA!! This was probably the most mind-boggling thing to fix. This is 
// related to the differences between UNIX shared object files (.so) and 
// Windows's Dynamically Linked Libraries (.DLL) 
// Using Singleton objects in a DLL and application using the DLL without 
// the following extern declaration makes the compiler to create two instances
// of the singleton; one inside the DLL and one inside the application binary.
// And since the point of Singleton is to have a single instance of an object, so 
// this nasty feature wreaks havoc on implementation. Following statements 
// say that the GSE_Singleton<GSE_MaterialMgr> template has already been 
// created in DLL and should not be created again.
#ifdef WIN32
class GSE_MaterialMgr;
extern template class __declspec(dllimport) GSE_Singleton<GSE_MaterialMgr>;
#endif
#endif
/////////////////////////////////////////////////////////////////
/// A template which creates KEY-VALUE maps between given types.
/// If you use pointers as keys, you better know the effects.
/// ie. const char * will make your dustbin go wah-wah in no time. Get that?
/// Yup, that's what I thought.
template <typename KEY, typename VALUE>
class GSE_Mapper
{
 protected:
  /// map template
  std::map< KEY , VALUE * > m_Map;
 public:
  /// Constructor.
  GSE_Mapper()
  {
    
  }
  /// Destructor.
  ~GSE_Mapper()
  {
    GSE_DEBUG( "Mapper:: delete\n");
    Clear();
  }
  /// Clears all mappings.
  void Clear()
  {
    m_Map.clear();
  }
  /// Adds pValue under the key kKey into the map.
  /// \param kKey key value.
  /// \param pValue a pointer.
  void Map( KEY kKey, VALUE *pValue )
  {
    m_Map[kKey] = pValue;
  }
  /// Finds the pointer to the object mapped with key kKey. 
  /// \param kKey key value for object.
  /// \return a pointer to found object, or if not found, NULL.
  VALUE *Find( KEY kKey )
  {
    // Try to find key-value pair
    typename std::map< KEY, VALUE * >::iterator itValue = m_Map.find( kKey ) ;
    
    if ( itValue == m_Map.end())
    {
      return NULL; // no luck, so we return NULL
    } 
    // Return the object pointer
    return itValue->second;
  }
};
// ---------------------------------------------------------------
/// Vertex class for some cases where it is easier to handle things as a blob.
class GSE_Vertex
{
 public:
  /// Vertex position.
  GSE_Vector3 m_vPosition;
  /// Vertex normal.
  GSE_Vector3 m_vNormal;
  /// Vertex texture coordinates.
  GSE_Vector2 m_vTexCoord;
  /// The equality comparison operator.
  /// \param vert GSE_Vertex object which this is compared against.
  bool operator==( GSE_Vertex vert)
  {
    return (m_vPosition == vert.m_vPosition &&
            m_vNormal   == vert.m_vNormal   &&
	    m_vTexCoord == vert.m_vTexCoord );
  }
  /// The less than comparison operator.
  bool operator< (GSE_Vertex vert)
  {
    return m_vPosition[GSE_Vector3::Y] < vert.m_vPosition[GSE_Vector3::Y];
  }
};
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
// history:
//
//	$Log: GSE_structures.h,v $
//	Revision 1.27  2007/06/01 08:55:43  entity
//	doxygen comments added
//
//	Revision 1.26  2007/06/01 06:47:01  entity
//	missing doxy comments again.
//
//	Revision 1.25  2007/06/01 06:26:19  entity
//	Added missing doxygen comments.
//
//	Revision 1.24  2007/05/31 14:00:12  entity
//	missing comments added.
//
//	Revision 1.23  2007/05/21 12:50:00  entity
//	glew to GLee
//
//	Revision 1.22  2007/05/18 08:08:09  entity
//	Vector3 constructor check & fixes
//
//	Revision 1.21  2007/05/14 09:44:27  entity
//	more reasonable operations for Vector2
//
//	Revision 1.20  2007/04/01 15:25:05  entity
//	texture system overhaul, Elements->Indexbuffer rename
//
//	Revision 1.19  2007/03/31 16:20:43  entity
//	ObjStruct -> GeometryData rename
//
//	Revision 1.18  2007/03/30 12:56:32  entity
//	SetElements->SetVertexCount rename
//
//	Revision 1.17  2007/03/29 06:58:10  entity
//	code cleanups
//
//	Revision 1.16  2007/03/27 07:41:32  entity
//	moved history to end of file
//
//	Revision 1.15  2007/03/26 18:24:55  entity
//	Rotate() modified and GetRotated() added
//
//	Revision 1.14  2007/03/14 10:54:15  entity
//	removed Position() and added dirty flag for GSE_Positional
//
//	Revision 1.13  2007/03/12 14:50:51  entity
//	Removed unneeded virtual functions
//
//	Revision 1.12  2007/03/11 12:48:18  entity
//	const keyword meddling and unneeded vars removed
//
//	Revision 1.11  2007/03/09 13:28:55  entity
//	const & issues
//
//	Revision 1.10  2007/03/09 12:27:01  entity
//	small stuff
//
//	Revision 1.9  2007/03/08 21:10:34  entity
//	inline keywords + const GetValues()
//
//	Revision 1.8  2007/03/07 13:33:38  entity
//	Vector3 with inlined functions
//
//	Revision 1.7  2007/02/28 11:53:50  entity
//	added GSE_Vertex class
//
//	Revision 1.6  2007/01/08 12:23:15  entity
//	added GPL licence and win32 fix for singleton objects
//
//	Revision 1.5  2006/12/29 14:00:23  entity
//	duh
//
//	Revision 1.4  2006/10/16 14:48:04  entity
//	Removed an unnecessary method
//
//	Revision 1.3  2006/10/16 07:10:48  entity
//	Created new GSE_Vector template and corresponding changes
//
//	Revision 1.2  2006/10/13 14:32:04  entity
//	Code cleanups, comments, replaced #ifdef DEBUG with GSE_DEBUG macro, Added file header
/////////////////////////////////////////////////////////////////
