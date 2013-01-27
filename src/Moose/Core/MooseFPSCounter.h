/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/


#ifndef MOOSEFPSCOUNTER_H_
#define MOOSEFPSCOUNTER_H_
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
  /////////////////////////////////////////////////////////////////
  /// A FPS Counter class for reporting framerate.
  class MOOSE_API CFpsCounter : public CTimer
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
#endif /* MOOSEFPSCOUNTER_H_ */
