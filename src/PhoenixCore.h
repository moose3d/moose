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
    /// magic number for hash table size and other stuff, too.
    const unsigned int PHOENIX_MAGIC_NUMBER = 33;
    /////////////////////////////////////////////////////////////////
    /// Nullable entity.
    class CNullable
    {
    private:
      /// State
      int m_bIsNull;
    protected:
      ////////////////////
      /// Constructor.
      CNullable() : m_bIsNull(1) { }
    public:
      ////////////////////
      /// Assigns null state.
      /// \param bNull Nullification state.
      inline void SetNull(int bNull) { m_bIsNull = bNull; }
      ////////////////////
      /// Returns nullification state.
      /// \returns state.
      inline int IsNull() const { return m_bIsNull; }
    };
    /////////////////////////////////////////////////////////////////
    class CEnableable
    {
    private:
      /// Is object enabled.
      int		m_bEnabled;
    protected:
      ////////////////////
      /// Default constructor. Disables object.
      CEnableable() : m_bEnabled(0) { }
    public:
      ////////////////////
      /// Returns is object enabled or disabled.
      /// \returns true for enabled, false for disabled.
      int IsEnabled() const {   return m_bEnabled; }
      ////////////////////
      /// Sets object enabled or disabled.
      /// \param bFlag Boolean flag - true for enabling, false for disabling.
      void SetEnabled(int bFlag ) { m_bEnabled = bFlag; }
    };
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
    template<typename KEYTYPE, typename OBJECTTYPE>
    class CHashItem
    {
    protected:
      /// The identification number for the object.
      KEYTYPE			m_Key;
      /// Object itself.
      OBJECTTYPE		m_Object;
    public:
      ////////////////////
      /// Constructor.
      CHashItem() { }
      ////////////////////
      /// 
      inline void SetObject( OBJECTTYPE & object )
      {
	m_Object = object;
      }
      ////////////////////
      /// Sets item key.
      /// \param nKey Item key.
      inline void SetKey( const KEYTYPE & key )
      {
	m_Key = key;
      }
      ////////////////////
      /// Returns the key for this object.
      /// \returns Hash key.
      inline KEYTYPE GetKey() const
      {
	return m_Key;
      }
      ////////////////////
      /// Returns reference to object.
      inline OBJECTTYPE & GetObject() 
      {
	return m_Object;
      }
      ////////////////////
      /// Comparison operator.
      inline bool operator==( const CHashItem & item ) const
      {
	return (item.GetKey() == GetKey());
      }
      ////////////////////
      /// Assignment operator.
      inline void operator=(CHashItem &item)
      {
	m_Key    = item.GetKey();
	m_Object = item.GetObject();
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Hash table class for several types.
    template <typename KEYTYPE, typename OBJECTTYPE>
    class CHashTable
    {
    protected:
      /// vector of vector of objects.
      std::vector< CHashItem<KEYTYPE,OBJECTTYPE> > *m_pTable;
      unsigned int		   m_nSize;
    public:
      ////////////////////
      /// Default constructor.
      CHashTable( unsigned int nSize) : m_nSize(nSize) 
      {
	assert ( m_nSize > 0 );
	m_pTable = new std::vector< CHashItem<KEYTYPE,OBJECTTYPE> >[GetSize()];
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
      inline void Insert( CHashItem<KEYTYPE,OBJECTTYPE> &obj )
      {
	unsigned int nHash = CreateHash( obj.GetKey() );
	m_pTable[nHash].push_back(obj);
      }
      ////////////////////
      /// Returns size of this CHashTable
      /// \return number of hash slots.
      inline int GetSize() const
      {
	return m_nSize;
      }
      ////////////////////
      /// Deletes object with key.
      /// \param nKey Key to object.
      void Delete( const KEYTYPE &nKey )
      {
	CHashItem<KEYTYPE,OBJECTTYPE> item;
	item.SetKey(nKey);
	
	int nHash = CreateHash(nKey);
	std::vector< CHashItem<KEYTYPE,OBJECTTYPE> > *pHashChain = &m_pTable[nHash];
	typename std::vector< CHashItem<KEYTYPE,OBJECTTYPE> >::iterator it;

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
      CHashItem<KEYTYPE,OBJECTTYPE> * Find( const KEYTYPE &nKey ) const
      {
	CHashItem<KEYTYPE,OBJECTTYPE> item;
	item.SetKey(nKey);
	
	int nHash = CreateHash(nKey);
	std::vector<CHashItem<KEYTYPE,OBJECTTYPE> > *pHashChain = &m_pTable[nHash];
	for( unsigned int i =0; i<pHashChain->size();  i++)
	{
	  if ((*pHashChain)[i] == item ) return (&(*pHashChain)[i]);
	}
	return NULL;
      }
        ////////////////////
      /// Creates hash of string. Uses djb2 algorithm.
      unsigned int CreateHash( const std::string &string ) const
      {
	unsigned int nHash = 5381;
        int c;
	const char *pStr = string.c_str();
	while ( (c = *pStr++) )
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
      inline static T *GetInstance() 
      {
	if ( m_pSingleton == NULL )
	{
	  m_pSingleton = new T();
	}
	return m_pSingleton;
      }
      /// Destroys this singleton.
      inline static void DestroyInstance()
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
  }; // namespace core
}// namespace Phoenix
/////////////////////////////////////////////////////////////////
#include "PhoenixObjectCounter.h"
/////////////////////////////////////////////////////////////////
#endif 
/////////////////////////////////////////////////////////////////
