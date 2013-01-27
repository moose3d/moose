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


#ifndef MOOSETIMER_H_
#define MOOSETIMER_H_
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
  /////////////////////////////////////////////////////////////////
      /// Generic timer for calculating passed time.
      class MOOSE_API CTimer
      {
      protected:
        /// The passed time relative to nStartTimeMS in milliseconds.
        Moose::Core::CTimeStamp m_StartTime;
        //size_t m_nPassedTimeMS;
        /// The starting time in milliseconds where passed time is calculated from.
        Moose::Core::CTimeStamp m_PassedTime;
        //size_t m_nStartTimeMS;
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
        void SetStartTime( const Moose::Core::CTimeStamp & tTimeStamp )
        {
  	m_StartTime = tTimeStamp;
        }
        ////////////////////
        /// Returns current time.
        /// \returns Current time.
        CTimeStamp GetTime() const
        {
  	return m_StartTime + m_PassedTime;
        }
        ////////////////////
        /// Sets the current time in milliseconds. If passed time is less than
        /// starting time, the passed time will be set as starting time.
        /// \param iSec Time in seconds
        /// \param iMS Time in milliseconds.
        void SetTime( int iSec, short iMS )
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
  	SetTime( m_TimeVal.tv_sec, (short)(m_TimeVal.tv_usec * 0.001f) );
        }
        ////////////////////
        /// Updates start time.
        void Reset()
        {
  	gettimeofday(&m_TimeVal, NULL);
  	SetStartTime( m_TimeVal.tv_sec, (short)(m_TimeVal.tv_usec * 0.001f) );
            SetTime( m_TimeVal.tv_sec, (short)(m_TimeVal.tv_usec * 0.001f) );
            
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
        /// \param iSec Time in seconds desired to be passed.
        /// \param iMS time in milliseconds which is desired to be passed (in addition to seconds).
        /// \returns true, if time has passed, false if not.
        inline int HasPassed( int iSec, short iMS )
        {
  	Update();
  	return (m_PassedTime >= CTimeStamp(iSec, iMS));
        }

      };
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSETIMER_H_ */
