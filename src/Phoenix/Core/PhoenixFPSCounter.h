
#ifndef PHOENIXFPSCOUNTER_H_
#define PHOENIXFPSCOUNTER_H_
/////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
  /////////////////////////////////////////////////////////////////
  /// A FPS Counter class for reporting framerate.
  class PHOENIX_API CFpsCounter : public CTimer
  {
  protected:
    /// The number of frames.
    size_t m_nFrameCount;
  public:
    ////////////////////
    /// The constructor.
    CFpsCounter() : CTimer()
    {
	ResetFrames();
    }
#ifndef SWIG
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
#endif
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

  }
}
/////////////////////////////////////////////////////////////
#endif /* PHOENIXFPSCOUNTER_H_ */
