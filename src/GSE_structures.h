/////////////////////////////////////////////////////////////////
#ifndef __Cstructures_h__
#define __Cstructures_h__
/////////////////////////////////////////////////////////////////
#include <sstream>
#include <iostream>
/////////////////////////////////////////////////////////////////
/// The base class for Objects with position.
class CPositional 
{
 protected:
  /// Vector where position is stored.
  CVector3 m_vPosition;
  /// has the position changed.
  int	      m_bPositionChanged;
 public:
  /// Default constructor.
  CPositional() : m_vPosition(0.0f,0.0f,0.0f),m_bPositionChanged(0)
  {

  }
  /// Parametrized constructor.
  /// \param vPosition vector for location-
  CPositional( const CVector3 & vPosition ) : m_vPosition(vPosition),m_bPositionChanged(0)
  {
  }
  /// Assigns position.
  /// \param vPosition vector of which values are used.
  inline void SetPosition( const CVector3 &vPosition )
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
  inline const CVector3 & GetPosition() const
  {
    return m_vPosition;
  }
  /// Adds vector to current position.
  void Move( CVector3 vPosition )
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
class CDirectional 
{
 protected:
  /// Direction vector.
  CVector3 m_vDirection;
 public:
  /// Assigns the direction.
  inline void SetDirection( CVector3 vDirection )
  {
    m_vDirection = vDirection;
  }
  /// Returns the direction.
  inline CVector3 GetDirection()
  {
    return m_vDirection;
  }
};
/////////////////////////////////////////////////////////////////
/// A value list container.
template <typename T>
class CValueList 
{
 protected:
  /// List of values.
  std::list<T> m_lstValues;
 public:
  /// Constructor.
  CValueList()
  {
  }
  /// Destructor.
  ~CValueList()
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
class CContainer
{
 protected:
  /// vector of objects.
  std::vector<T *> m_Objects;
  
 public:
  /// Default constructor.
  CContainer()
  {
     
  }
  /// Destructor.
  ~CContainer()
  {
    CDEBUG( "Deleting CContainer (" << m_Objects.size() <<")...");
    Clear();
    CDEBUG( "Done deleting CContainer" );
  }
  /// deletes all objects.
  void Clear()
  {
    for( unsigned int i=0;i<m_Objects.size();i++)
    {
      CDEBUG( "deleting " << i << "ptr:" << m_Objects[i] );
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
class CSingleton
{

 private:
  static T *m_pSingleton;
  CSingleton( const CSingleton &ref){}
 protected:
  CSingleton() {}
  virtual ~CSingleton() {}
   
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
template<class T> T *CSingleton<T>::m_pSingleton = NULL;

#ifndef _WIN32EXPORT 
// WHOA!! This was probably the most mind-boggling thing to fix. This is 
// related to the differences between UNIX shared object files (.so) and 
// Windows's Dynamically Linked Libraries (.DLL) 
// Using Singleton objects in a DLL and application using the DLL without 
// the following extern declaration makes the compiler to create two instances
// of the singleton; one inside the DLL and one inside the application binary.
// And since the point of Singleton is to have a single instance of an object, so 
// this nasty feature wreaks havoc on implementation. Following statements 
// say that the CSingleton<CMaterialMgr> template has already been 
// created in DLL and should not be created again.
#ifdef WIN32
class CMaterialMgr;
extern template class __declspec(dllimport) CSingleton<CMaterialMgr>;
#endif
#endif
/////////////////////////////////////////////////////////////////
/// A template which creates KEY-VALUE maps between given types.
/// If you use pointers as keys, you better know the effects.
/// ie. const char * will make your dustbin go wah-wah in no time. Get that?
/// Yup, that's what I thought.
template <typename KEY, typename VALUE>
class CMapper
{
 protected:
  /// map template
  std::map< KEY , VALUE * > m_Map;
 public:
  /// Constructor.
  CMapper()
  {
    
  }
  /// Destructor.
  ~CMapper()
  {
    CDEBUG( "Mapper:: delete\n");
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
class CVertex
{
 public:
  /// Vertex position.
  CVector3 m_vPosition;
  /// Vertex normal.
  CVector3 m_vNormal;
  /// Vertex texture coordinates.
  CVector2 m_vTexCoord;
  /// The equality comparison operator.
  /// \param vert CVertex object which this is compared against.
  bool operator==( CVertex vert)
  {
    return (m_vPosition == vert.m_vPosition &&
            m_vNormal   == vert.m_vNormal   &&
	    m_vTexCoord == vert.m_vTexCoord );
  }
  /// The less than comparison operator.
  bool operator< (CVertex vert)
  {
    return m_vPosition[CVector3::Y] < vert.m_vPosition[CVector3::Y];
  }
};
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
