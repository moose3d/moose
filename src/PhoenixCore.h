/////////////////////////////////////////////////////////////////
#ifndef __Ccore_h__
#define __Ccore_h__
/////////////////////////////////////////////////////////////////
#include <sstream>
#include <iostream>
#include <ostream>
#include <vector>
#include <map>
#include <list>
#include <assert.h>
/////////////////////////////////////////////////////////////////
using std::string;
using std::vector;
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core 
  {
    using namespace Phoenix::Core;
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
      ////////////////////
      /// The constructor.
      CTimer()
      {
	m_nPassedTimeMS = 0;
	m_nStartTimeMS = 0;
      }
      ////////////////////
      /// Sets the starting time for calculations.
      /// \param nTimeMS the starting time in milliseconds.
      void SetStartTimeMS( unsigned int nTimeMS )
      {
	m_nStartTimeMS = nTimeMS;
      }
      ////////////////////
      /// Sets the current time in milliseconds. If passed time is less than 
      /// starting time, the passed time will be set as starting time.
      /// \param nTimeMS The current time in milliseconds, 
      /// preferably after the starting time.
      void SetCurrentTimeMS( unsigned int nTimeMS )
      {
	if ( nTimeMS < m_nStartTimeMS ) {
	  SetStartTimeMS( nTimeMS );
	}
	else			  m_nPassedTimeMS = nTimeMS - m_nStartTimeMS;
      }
      ////////////////////
      /// Returns the passed time in milliseconds.
      /// \returns unsigned int the passed time relative to starting time.
      inline unsigned int GetPassedTimeMS()
      {
	return m_nPassedTimeMS;
      }
      ////////////////////
      /// Checks whether given time has passed since the last call to 
      /// SetStartTime().
      /// \param nTimeMS time in milliseconds which is desired to be passed.
      /// \returns true, if nTimeMS milliseconds have passed, false if not.
      inline int HasPassedMS( unsigned int nTimeMS )
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
    /// The base object for hash table storing.
    template<typename A>
    class CHashItem
    {
    protected:
      /// The identification number for the object.
      unsigned int	m_nId;
      /// Object itself.
      A		m_Object;
    public:
      ////////////////////
      /// Constructor.
      CHashItem( unsigned int nId, A &object ) : m_nId(nId), m_Object(object)
      {
      }
      ////////////////////
      /// Returns the ID number for this object.
      inline unsigned int GetID() const
      {
	return m_nId;
      }
      ////////////////////
      /// Returns reference to object.
      inline A & GetObject()
      {
	return m_Object;
      }
      ////////////////////
      /// Comparison operator.
      inline bool operator==( const CHashItem & item )
      {
	return (item.GetID() == GetID());
      }
      ////////////////////
      /// Assignment operator.
      inline void operator=(const CHashItem &item)
      {
	m_nId = item.GetID();
	m_Object = item.GetObject();
      }
    };
    
    /////////////////////////////////////////////////////////////////
    /// Hash table class for several types.
    template <typename T>
    class CHashTable
    {
    protected:
      /// vector of vector of objects.
      std::vector< CHashItem<T> > *m_pTable;
      unsigned int		   m_nSize;
    public:
      ////////////////////
      /// Default constructor.
      CHashTable( unsigned int nSize) : m_nSize(nSize) 
      {
	assert ( m_nSize > 0 );
	m_pTable = new std::vector< CHashItem<T> >[GetSize()];
      }
      ////////////////////
      /// Destructor.
      ~CHashTable() 
      {    
	delete [] m_pTable;
      }
      ////////////////////
      /// Inserts new item to hashtable.
      /// \param obj object to be inserted.
      inline void Insert( CHashItem<T> &obj )
      {
	unsigned int nHash = CreateHash( obj.GetKey() );
	m_pTable[nHash].push_back(obj);
      }
      ////////////////////
      /// Returns size of this CHashTable
      /// \return number of hash slots.
      inline int GetSize() const
      {
	return m_nSize();
      }
      ////////////////////
      /// Deletes object with key.
      /// \param nKey Key to object.
      void Delete( unsigned int nKey )
      {
	CHashItem<T> item;
	item.SetKey(nKey);
	
	int nHash = CreateHash(nKey);
	std::vector< CHashItem<T> > *pHashChain = &m_pTable[nHash];
	typename std::vector< CHashItem<T> >::iterator it;

	for( it = pHashChain->begin(); it!= pHashChain->end(); it++)
	{
	  if ( *it == item )
	  {
	    pHashChain->erase(it);
	    break;
	  }
	}
      }
      ////////////////////
      /// Finds HashItem by key. 
      /// \param nKey Key to object.
      /// \returns NULL if not found, pointer to hashitem otherwise.
      CHashItem<T> * Find( int nKey )
      {
	
	CHashItem<T> item;
	item.SetKey(nKey);
	
	int nHash = CreateHash(nKey);
	std::vector<CHashItem<T> > *pHashChain = &m_pTable[nHash];
	for( unsigned int i =0; i<pHashChain.size();  i++)
	{
	  if ((*pHashChain)[i] == item ) return (&(*pHashChain)[i]);
	}
	return NULL;
      }
      unsigned int CreateHash( unsigned int nKey )
      {
	return nKey % GetSize();
      }
      ////////////////////
      /// Creates hash of string. Uses djb2 algorithm.
      unsigned int CreateHash( const std::string &string )
      {
	unsigned int nHash = 5381;
        int c;
	char *pStr = string.c_str();
	while (c = *pStr++)
	  nHash = ((nHash << 5) + nHash) + c; /* hash * 33 + c */
	
        return nHash % GetSize();
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
    /////////////////////////////////////////////////////////////////
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
    /////////////////////////////////////////////////////////////////
    /// A value list container.
    template <typename T>
    class CValueList 
    {
    protected:
      /// List of values.
      std::list<T> m_lstValues;
    public:
      ////////////////////
      /// Constructor.
      CValueList()
      {
      }
      ////////////////////
      /// Destructor.
      ~CValueList()
      {
	Clear();
      }
      ////////////////////
      /// Adds value to list.
      /// \param value Value to be added.
      inline void Add( T value )
      {
	m_lstValues.push_back(value);
      }
      ////////////////////
      /// Clears the list.
      inline void Clear()
      {
	m_lstValues.clear();
      }
      ////////////////////
      /// Returns a reference to the value list.
      const std::list<T> & GetValues()
      {
	return m_lstValues;
      }

    };
    /////////////////////////////////////////////////////////////////
  }; // namespace core
}// namespace Phoenix
/////////////////////////////////////////////////////////////////
#include "PhoenixObjectCounter.h"
/////////////////////////////////////////////////////////////////
#endif 
/////////////////////////////////////////////////////////////////
