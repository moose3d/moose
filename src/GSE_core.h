/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/

// -----------------------------------------------------------------
// file : GSE_core.h
// desc :  various core helper utilities to make things prettier
// author: AG/eNtity
// history:
//
//		$Log: GSE_core.h,v $
//		Revision 1.11  2007/05/17 11:03:10  entity
//		Updateable interface added
//
//		Revision 1.10  2007/03/16 08:50:20  entity
//		Separated timer class from FpsCounter
//
//		Revision 1.9  2007/03/05 11:28:24  entity
//		operator++ added to FpsCounter
//
//		Revision 1.8  2007/03/05 10:08:32  entity
//		Comments added.
//
//		Revision 1.7  2007/03/05 10:02:03  entity
//		Added FpsCounter class.
//
//		Revision 1.6  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.5  2006/12/26 22:13:04  entity
//		doxygen comments
//
//		Revision 1.4  2006/12/18 18:56:19  entity
//		separated impl from header
//
//		Revision 1.3  2006/10/11 08:47:47  entity
//		Removed invalid managers
//
//		Revision 1.2  2006/10/11 08:46:30  entity
//		Added comment
//
//		Revision 1.1  2006/10/11 08:45:35  entity
//		Initial import to cvs
//
// -----------------------------------------------------------------

#ifndef __GSE_core_h__
#define __GSE_core_h__
#include <ostream>
/////////////////////////////////////////////////////////////////
namespace Core 
{
  /// \brief Function which cleans up every manager object in GSE.
  ///		Kinda handy.
  ///
  /// Detailed info.
  void CleanManagers();
  /////////////////////////////////////////////////////////////////
  // Generic timer for calculating passed time.
  /////////////////////////////////////////////////////////////////
  class GSE_Timer 
  {
  protected:
    /// The passed time relative to nStartTimeMS in milliseconds.
    unsigned int m_nPassedTimeMS;
    /// The starting time in milliseconds where passed time is calculated from.
    unsigned int m_nStartTimeMS;
  public:
    /// The constructor.
    GSE_Timer()
    {
      m_nPassedTimeMS = 0;
      m_nStartTimeMS = 0;
    }
    /// Sets the starting time for calculations.
    /// \param nTimeMS the starting time in milliseconds.
    void SetStartTime( unsigned int nTimeMS )
    {
      m_nStartTimeMS = nTimeMS;
    }
    /// Sets the current time in milliseconds. If passed time is less than 
    /// starting time, the passed time will be set as starting time.
    /// \param nTimeMS The current time in milliseconds, 
    /// preferably after the starting time.
    void SetCurrentTime( unsigned int nTimeMS )
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
  /////////////////////////////////////////////////////////////////
  class GSE_FpsCounter : public GSE_Timer
  {
  protected:
    /// The number of frames.
    unsigned int m_nFrameCount;
  public:
    /// The constructor.
    GSE_FpsCounter() : GSE_Timer()
    {
      ResetFrames();
    }
    /// Makes it possible to write ++fpsCounter in order to increase framecount.
    GSE_FpsCounter & operator++()
    {
      AddFrame();
      return *this;
    }
    /// Makes it possible to write fpsCounter++ in order to increase framecount.
    GSE_FpsCounter & operator++(int nNumber)
    {
      AddFrame();
      return *this;
    }
    /// So Frames per second can be printed directly using the object itself.
    friend std::ostream & operator<< ( std::ostream & s, GSE_FpsCounter & ref )
    {
      s << ref.GetFPS();
      return s;
    }
    /// Increases the frame count by 1.
    void AddFrame()
    {
      m_nFrameCount++;
    }
    /// Resets the frame count to 0.
    void ResetFrames()
    {
      m_nFrameCount = 0;
    }
    /// Returns how many frames per second have been processed.
    /// Calculates the figure from framecount and passed time.
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
  class GSE_Updateable 
  {
  protected:
    GSE_Updateable() {}
    virtual ~GSE_Updateable() {}
  public:
    ////////////////////
    /// 
    virtual void Update( unsigned int nPassedTime ) = 0;
  };
};
#endif
