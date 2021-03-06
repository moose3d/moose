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


#ifndef MOOSETIMESTAMP_H_
#define MOOSETIMESTAMP_H_
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
  /////////////////////////////////////////////////////////////////
  /// Timestamp for various things.
  class MOOSE_API CTimeStamp
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
    /// Parametrized constructor.
    /// \param iSeconds Passed seconds.
    /// \param iMS Passed milliseconds.
    CTimeStamp( float fSeconds )
    {
	m_iSeconds = static_cast<int>(fSeconds);
      m_iMilliSeconds = static_cast<short int>(roundf((fSeconds-m_iSeconds)*1000.0f));
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
    /// Returns passed time as floating-point value in seconds.
    /// \returns Passed time in seconds.
    float ToSeconds() const
    {
	return static_cast<float>(m_iSeconds) + static_cast<float>(GetMilliSeconds())*0.001f;
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
	  --iSeconds;
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
	  ++iSeconds;
	  iMS -= 1000;
	}
	return CTimeStamp( iSeconds, iMS);
    }
    ////////////////////
    /// Prints timestamps to given stream.
    /// \param stream Stream where data is printed
    /// \param tTimeStamp Timestamp to be printed.
    /// \returns Reference to stream.
    friend std::ostream & operator<< ( std::ostream & stream, const Moose::Core::CTimeStamp & tTimeStamp )
    {
      stream << tTimeStamp.GetSeconds() << ".";

      if ( tTimeStamp.GetMilliSeconds() < 100 ) stream << "0";
      if ( tTimeStamp.GetMilliSeconds() < 10) stream << "0";
      stream << tTimeStamp.GetMilliSeconds();
      return stream;
    }

  };
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSETIMESTAMP_H_ */
