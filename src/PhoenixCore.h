/////////////////////////////////////////////////////////////////
#ifndef __PhoenixCore_h__
#define __PhoenixCore_h__
/////////////////////////////////////////////////////////////////
#include <sstream>
#include <iostream>
#include <ostream>
#include <vector>
#include <map>
#include <list>
#include <assert.h>
#include <sys/time.h>
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
    /// Template base for each typed object.
    template <typename TYPE>
    class CTypeBase
    {
    protected:
      /// Stores the type
      TYPE m_Type;
    public:
      ////////////////////
      /// Assigns type.
      /// \param rType TYPE to be set.
      void SetType( const TYPE &rType ) { m_Type = rType;  }
      ////////////////////
      /// Return current type.
      /// \returns Currently set type.
      const TYPE & GetType() const { return m_Type; }
    };
    /////////////////////////////////////////////////////////////////
    /// Timestamp for various things.
    class CTimeStamp
    {
    protected:
      /// Passed seconds 
      int m_iSeconds;
      /// Passed milliseconds after m_iSeconds.
      short m_iMilliSeconds;
    public:
      ////////////////////
      /// Constructor.
      CTimeStamp()
      {
      }
      ////////////////////
      /// Parametrized constructor.
      /// \param iSeconds Passed seconds.
      /// \param iMS Passed milliseconds.
      CTimeStamp( int iSeconds, int iMS ): m_iSeconds(iSeconds), m_iMilliSeconds(iMS) 
      {

      }
      ////////////////////
      /// Sets passed seconds.
      /// \param nSeconds Passed seconds.
      void SetSeconds( int iSeconds )
      {
	m_iSeconds = iSeconds;
      }
      ////////////////////
      /// Sets passed milliseconds.
      /// \param iMS Passed milliseconds.
      void SetMilliSeconds( short iMS )
      {
	m_iMilliSeconds = iMS;
      }
      ////////////////////
      /// Returns passed milliseconds.
      /// \returns Passed milliseconds.
      short GetMilliSeconds() const
      {
	return m_iMilliSeconds;
      }
      ////////////////////
      /// Returns passed seconds.
      /// \returns Passed seconds.
      int GetSeconds() const
      {
	return m_iSeconds;
      }
      ////////////////////
      /// Less than comparison operator.
      /// \param rAnother Another timestamp.
      int operator<( const CTimeStamp & rAnother) const
      {
	if      ( GetSeconds() < rAnother.GetSeconds() ) return 1;
	else if ( GetSeconds() > rAnother.GetSeconds()) return 0;
	else if ( GetMilliSeconds() < rAnother.GetMilliSeconds()) return 1;
	return 0;
      }
      ////////////////////
      /// Less than or equal comparison operator.
      /// \param rAnother Another timestamp.
      int operator<=( const CTimeStamp & rAnother) const
      {
	if ( GetSeconds() > rAnother.GetSeconds()) return 0;
	else if ( GetMilliSeconds() <= rAnother.GetMilliSeconds()) return 1;
	return 0;
      }
      ////////////////////
      /// Greater than or equal comparison operator.
      /// \param rAnother Another timestamp.
      int operator>=( const CTimeStamp & rAnother) const
      {
	if ( GetSeconds() < rAnother.GetSeconds()) return 0;
	else if ( GetMilliSeconds() >= rAnother.GetMilliSeconds()) return 1;
	return 0;
      }
      ////////////////////
      /// Greater than comparison operator.
      /// \param rAnother Another timestamp.
      int operator>( const CTimeStamp & rAnother) const
      {
	if      ( GetSeconds() > rAnother.GetSeconds() ) return 1;
	else if ( GetSeconds() < rAnother.GetSeconds()) return 0;
	else if ( GetMilliSeconds() > rAnother.GetMilliSeconds()) return 1;
	return 0;
      }
      ////////////////////
      /// Equality comparison operator.
      /// \param rAnother Another timestamp.
      int operator==( const CTimeStamp & rAnother) const
      {
	return ( GetSeconds() == rAnother.GetSeconds() && GetMilliSeconds() == rAnother.GetMilliSeconds() );
      }
      ////////////////////
      /// Reduces given timestamp from this.
      /// \param rAnother Other timestamp.
      /// \returns Resulting timestamp.
      CTimeStamp operator-( const CTimeStamp &rAnother ) const 
      {
	int iSeconds = GetSeconds() - rAnother.GetSeconds();
	int iMS = GetMilliSeconds() - rAnother.GetMilliSeconds();
	if ( iMS < 0 )
	{
	  iSeconds--;
	  iMS += 1000;
	}
	return CTimeStamp( iSeconds, iMS);
      }
      ////////////////////
      /// Adds given timestamp to this.
      /// \param rAnother Other timestamp.
      /// \returns Resulting timestamp.
      CTimeStamp operator+( const CTimeStamp &rAnother ) const 
      {
	int iSeconds = GetSeconds() + rAnother.GetSeconds();
	int iMS = GetMilliSeconds() + rAnother.GetMilliSeconds();
	if ( iMS > 1000 )
	{
	  iSeconds++;
	  iMS -= 1000;
	}
	return CTimeStamp( iSeconds, iMS);
      }
      ////////////////////
      /// Prints timestamps to given stream.
      /// \param stream Stream where data is printed
      /// \param tTimeStamp Timestamp to be printed.
      /// \returns Reference to stream.
      friend std::ostream & operator<< ( std::ostream & stream, const Phoenix::Core::CTimeStamp & tTimeStamp )
      {
	stream << tTimeStamp.GetSeconds() << "." << tTimeStamp.GetMilliSeconds();
	return stream;
      }

    };
    /////////////////////////////////////////////////////////////////
    /// Generic timer for calculating passed time.
    class CTimer 
    {
    protected:
      /// The passed time relative to nStartTimeMS in milliseconds.
      Phoenix::Core::CTimeStamp m_StartTime;
      //unsigned int m_nPassedTimeMS;
      /// The starting time in milliseconds where passed time is calculated from.
      Phoenix::Core::CTimeStamp m_PassedTime;
      //unsigned int m_nStartTimeMS;
      /// Time value for Update().
      struct timeval m_TimeVal;
    public:
      ////////////////////
      /// The constructor.
      CTimer()
      {
	m_StartTime.SetSeconds(0);
	m_StartTime.SetMilliSeconds(0);
	m_PassedTime.SetSeconds(0);
	m_PassedTime.SetMilliSeconds(0);
	//m_nPassedTimeMS = 0;
	//m_nStartTimeMS = 0;
      }
      ////////////////////
      /// Sets the starting time for calculations.
      /// \param iSec Seconds
      /// \param iMS  Milliseconds
      void SetStartTime( int iSec, short iMS )
      {
	m_StartTime.SetSeconds(iSec);
	m_StartTime.SetMilliSeconds( iMS );
      }
      ////////////////////
      /// Sets the starting time for calculations.
      /// \param tTimeStamp Timestamp to be set.
      void SetStartTime( const Phoenix::Core::CTimeStamp & tTimeStamp )
      {
	m_StartTime = tTimeStamp;
      }
      ////////////////////
      /// Returns current time.
      /// \returns Current time.
      CTimeStamp GetCurrentTime() const
      {
	return m_StartTime + m_PassedTime;
      }
      ////////////////////
      /// Sets the current time in milliseconds. If passed time is less than 
      /// starting time, the passed time will be set as starting time.
      /// \param iSec Time in seconds
      /// \param iMS Time in milliseconds.
      void SetCurrentTime( int iSec, short iMS )
      {
	CTimeStamp tTime(iSec, iMS );
	if ( tTime < m_StartTime ) 
	{
	  SetStartTime( tTime );
	  m_PassedTime.SetSeconds(0);
	  m_PassedTime.SetMilliSeconds(0);
	}
	else 
	{
	  m_PassedTime = tTime - m_StartTime;
	}
      }
      ////////////////////
      /// Updates current time.
      void Update()
      {
	gettimeofday(&m_TimeVal, NULL);
	SetCurrentTime( m_TimeVal.tv_sec, (short)(m_TimeVal.tv_usec * 0.001f) );
      }
      ////////////////////
      /// Updates start time.
      void Reset()
      {
	gettimeofday(&m_TimeVal, NULL);
	SetStartTime( m_TimeVal.tv_sec, (short)(m_TimeVal.tv_usec * 0.001f) );
      }
      ////////////////////
      /// Returns the passed time in milliseconds.
      /// \returns unsigned int the passed time relative to starting time.
      inline const CTimeStamp & GetPassedTime()
      {
	return m_PassedTime;
      }
      ////////////////////
      /// Checks whether given time has passed since the last call to 
      /// SetStartTime().
      /// \param nTimeMS time in milliseconds which is desired to be passed.
      /// \returns true, if nTimeMS milliseconds have passed, false if not.
      inline int HasPassedMS( int iSec, short iMS )
      {
	Update();
	return (m_PassedTime >= CTimeStamp(iSec, iMS));
      }
      
    };
    /////////////////////////////////////////////////////////////////
    /// A FPS Counter class for reporting framerate.
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
      /// Resets counter, frame count to zero, start time = current time.
      void Reset() 
      {
	CTimer::Reset();
	ResetFrames();
      }
      ////////////////////
      /// Returns how many frames per second have been processed.
      /// Calculates the figure from framecount and passed time.
      /// \returns Frames per second using float value.
      float GetFPS() 
      {
	if ( m_PassedTime.GetSeconds() > 0 || m_PassedTime.GetMilliSeconds() > 0 )  
	{
	  return (float)(m_nFrameCount) / (m_PassedTime.GetSeconds() + m_PassedTime.GetMilliSeconds()*0.001f);
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
    ////////////////////////////////////////////////////////////////
    /// Which states caches can have.
    enum CACHE_STATE_TYPE
    {
      CACHE_NOCACHE = 0,  /// Cache is not used.
      CACHE_REFRESH = 1,  /// Original data has changed, cache needs refreshing.
      CACHE_UP2DATE = 2   /// Cache matches original data.
    };
    /////////////////////////////////////////////////////////////////
    /// Template class for different cache objects.
    template<typename CACHE_TYPE>
    class CCacheable
    {
    protected:
      CACHE_STATE_TYPE m_nState;
      CACHE_TYPE       m_nCacheId;
      ////////////////////
      /// Constructor.
      CCacheable() : m_nState(CACHE_NOCACHE) { }
    public:      
      ////////////////////
      /// Constructor.
      ~CCacheable() { }
      ////////////////////
      /// Returns reference to cache.
      /// \returns Cache reference.
      CACHE_TYPE & GetCache() 
      {
	return m_nCacheId;
      }
      ////////////////////
      /// Returns reference to cache.
      /// \returns Cache reference.
      const CACHE_TYPE & GetCache() const
      {
	return m_nCacheId;
      }
      ////////////////////
      /// Checks is array cached.
      /// \returns non-zero if descriptor is cached.
      /// \returns zero otherwise.
      inline int IsCached() const
      {
	return (GetState()!=CACHE_NOCACHE);
      }
      ////////////////////
      /// Sets cache state.
      /// \param nState Cache state value.
      inline void SetState( CACHE_STATE_TYPE nState ) 
      {
	m_nState = nState;
      }
      ////////////////////
      /// Checks cache state.
      /// \returns Currently set CACHE_STATE_TYPE.
      inline CACHE_STATE_TYPE GetState() const
      {
	return m_nState;
      }
    };
    
  }; // namespace core
}// namespace Phoenix
/////////////////////////////////////////////////////////////////
#include "PhoenixObjectCounter.h"
/////////////////////////////////////////////////////////////////
#endif 
/////////////////////////////////////////////////////////////////
