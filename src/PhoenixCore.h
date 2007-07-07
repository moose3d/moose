
#ifndef __Ccore_h__
#define __Ccore_h__
#include <ostream>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core 
  {
    /////////////////////////////////////////////////////////////////
    /// The base object for everything in the system.
    class CPhoenixObject 
    {
    private:
      /// How many objects have been created.
      static unsigned int m_nObjectCounter;
    protected:
      /// The identification number for the object.
      unsigned int	m_nId;
      /// Object name.
      string	m_sObjectName;
    public:
      ////////////////////
      /// Default constructor.
      CPhoenixObject()
      {
	m_nId = m_nObjectCounter;
	m_nObjectCounter++;
	std::stringstream str;
	str << "Object" << GetId();
	SetName(str.str());
      }
      ////////////////////
      /// Assigns object name,
      /// \param sNewName name for object.
      inline void SetName( const std::string & sNewName )
      {
	m_sObjectName = sNewName;
      }
      ////////////////////
      /// Returns object name.
      /// \returns string containing object name,
      inline const std::string & GetName()
      {
	return m_sObjectName;
      }
      ////////////////////
      /// Returns the ID number for this object.
      inline unsigned int GetID()
      {
	return m_nId;
      }
    };
    // Initialize object count to zero.
    unsigned int CObject::m_nObjectCounter = 0;
    
    /////////////////////////////////////////////////////////////////
    /// Generic timer for calculating passed time.
    class CTimer 
    {
    protected:
      /// The passed time relative to nStartTimeMS in milliseconds.
      unsigned int m_nPassedTimeMS;
      /// The starting time in milliseconds where passed time is calculated from.
      unsigned int m_nStartTimeMS;
    public:
      /// The constructor.
      CTimer()
      {
	m_nPassedTimeMS = 0;
	m_nStartTimeMS = 0;
      }
      /// Sets the starting time for calculations.
      /// \param nTimeMS the starting time in milliseconds.
      void SetStartTimeMS( unsigned int nTimeMS )
      {
	m_nStartTimeMS = nTimeMS;
      }
      /// Sets the current time in milliseconds. If passed time is less than 
      /// starting time, the passed time will be set as starting time.
      /// \param nTimeMS The current time in milliseconds, 
      /// preferably after the starting time.
      void SetCurrentTimeMS( unsigned int nTimeMS )
      {
	if ( nTimeMS < m_nStartTimeMS ) {
	  SetStartTime( nTimeMS );
	}
	else			  m_nPassedTimeMS = nTimeMS - m_nStartTimeMS;
      }
      /// Returns the passed time in milliseconds.
      /// \returns unsigned int the passed time relative to starting time.
      unsigned int GetPassedTimeMS()
      {
	return m_nPassedTimeMS;
      }
      /// Checks whether given time has passed since the last call to 
      /// SetStartTime().
      /// \param nTimeMS time in milliseconds which is desired to be passed.
      /// \returns bool true, if nTimeMS milliseconds have passed, false if not.
      bool HasPassedMS( unsigned int nTimeMS )
      {
	return ( m_nPassedTimeMS >= nTimeMS );
      }
    
    };
    /////////////////////////////////////////////////////////////////
    /// The FPS Counter class for reporting framerate.
    class CFpsCounter : public CTimer
    {
    protected:
      /// The number of frames.
      unsigned int m_nFrameCount;
    public:
      ////////////////////
      /// The constructor.
      CFpsCounter() : CTimer()
      {
	ResetFrames();
      }
      ////////////////////
      /// Makes it possible to write ++fpsCounter in order to increase framecount.
      /// \returns reference to fpscounter object.
      CFpsCounter & operator++()
      {
	AddFrame();
	return *this;
      }
      ////////////////////
      /// Makes it possible to write fpsCounter++ in order to increase framecount.
      /// \param nNumber to make this different from ++fpsCounter.
      /// \returns reference to fpscounter object.
      inline CFpsCounter & operator++(int nNumber)
      {
	AddFrame();
	return *this;
      }
      ////////////////////
      /// So Frames per second can be printed directly using the object itself.
      /// \param s output stream.
      /// \param ref fpscounter object.
      /// \returns reference to output stream.
      friend std::ostream & operator<< ( std::ostream & s, CFpsCounter & ref )
      {
	s << ref.GetFPS();
	return s;
      }
      ////////////////////
      /// Increases the frame count by 1.
      inline void AddFrame()
      {
	m_nFrameCount++;
      }
      ////////////////////
      /// Resets the frame count to 0.
      inline void ResetFrames()
      {
	m_nFrameCount = 0;
      }
      ////////////////////
      /// Returns how many frames per second have been processed.
      /// Calculates the figure from framecount and passed time.
      /// \returns Frames per second using float value.
      float GetFPS() 
      {
	if ( m_nPassedTimeMS > 0 )  
	{
	  return (float)(m_nFrameCount) / (m_nPassedTimeMS *0.001f);
	} 
	return 0.0f;
      }
    
    };
    /////////////////////////////////////////////////////////////////
    /// Generic container class for several types.
    template <class T>
    class CContainer
    {
    protected:
      /// vector of objects.
      std::vector<T *> m_Objects;
      
    public:
      ////////////////////
      /// Default constructor.
      CContainer()      {}
      ////////////////////
      /// Destructor.
      ~CContainer() {    Clear();  }
      ////////////////////
      /// Deletes all objects.
      void Clear()
      {
	for( unsigned int i=0;i<m_Objects.size();i++)
	{
	  if ( m_Objects[i] != NULL ) 
	  {
	    delete m_Objects[i];
	  }
	}
	m_Objects.clear();
      }
      ////////////////////
      /// Adds an object to this container.
      /// \param pObject pointer to an object with type T.
      /// \returns -1 if unsuccessfull, >=0 otherwise.
      int Add( T *pObject )
      {
	int iIndex = -1;
	if ( pObject != NULL )
	{
	  m_Objects.push_back(pObject);
	  iIndex = m_Objects.size()-1;
	}
	return iIndex; 
      }
      ////////////////////
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
      /// Static pointer to only instance.
      static T *m_pSingleton;
      ////////////////////
      /// Copy constructor.
      CSingleton( const CSingleton &ref){}
    protected:
      ////////////////////
      /// Default constructor.
      CSingleton() {}
      ////////////////////
      /// Destructor.
      virtual ~CSingleton() {}
    public:
      ////////////////////
      /// Returns a pointer to this singleton.
      static T *GetInstance() 
      {
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
	  m_pSingleton = NULL;
	}
      }
    };
    /// Initialize the static member in the derived classes.
    template<class T> T *CSingleton<T>::m_pSingleton = NULL;
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
      ////////////////////
      /// Default constructor.
      CMapper()
      {
	
      }
      ////////////////////
      /// Destructor.
      ~CMapper()
      {
	CDEBUG( "Mapper:: delete\n");
	Clear();
      }
      ////////////////////
      /// Clears all mappings.
      void Clear()
      {
	m_Map.clear();
      }
      ////////////////////
      /// Adds pValue under the key kKey into the map.
      /// \param kKey key value.
      /// \param pValue a pointer.
      void Map( KEY kKey, VALUE *pValue )
      {
	m_Map[kKey] = pValue;
      }
      ////////////////////
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
  }; // namespace core
}// namespace Phoenix
#endif 
