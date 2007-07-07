
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
    
  }; // namespace core
}// namespace Phoenix
#endif 
