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
// file : $Id: GSE_Logger.h,v 1.8 2007/04/01 15:25:05 entity Exp $
// desc : Logger utility for GSE
// author : AG / eNtity
// history : 
//
//		$Log: GSE_Logger.h,v $
//		Revision 1.8  2007/04/01 15:25:05  entity
//		texture system overhaul, Elements->Indexbuffer rename
//
//		Revision 1.7  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.6  2006/10/17 11:06:09  entity
//		Removed __FUNCTION__ from GSE_DEBUG() macro
//
//		Revision 1.5  2006/10/13 08:03:49  entity
//		Added separate warning stream
//
//		Revision 1.4  2006/10/13 08:00:58  entity
//		Added macros for debug,log,error and warning messages
//
//		Revision 1.3  2006/10/13 07:39:09  entity
//		Changed file from header to Id
//
//		Revision 1.2  2006/10/13 07:37:55  entity
//		Added handy macros and header for the file
//
//		
//		<WAY BACK> AG/eNtity : initial version
// -----------------------------------------------------------------
#ifndef __GSE_Logger_h__
#define __GSE_Logger_h__

#include <iostream>
using std::endl;

// -----------------------------------------------------------------

// Macros which make life easier 
#define GSE_LOG( MSG )  Core::GSE_Logger::Log()    << "Log: "   << MSG << endl;
#define GSE_ERR( MSG )  Core::GSE_Logger::Error()  << "Error: " << MSG << endl;
#define GSE_WARN( MSG ) Core::GSE_Logger::Error()  << "Warning: " << MSG << endl;
// debug output 
#ifdef DEBUG
#define GSE_DEBUG( MSG ) Core::GSE_Logger::Error()  << "Debug: " << __FILE__ << "line "<< __LINE__ << " : " << MSG << endl;
#else
#define GSE_DEBUG( MSG ) 
#endif

// -----------------------------------------------------------------
namespace Core 
{

  class GSE_Logger 
  {
  private:
    // Stream reference which is used for text output.
    // Can be file, cerr, cout, etc.
    static std::ostream *m_pLog;
    static std::ostream *m_pError;
    static std::ostream *m_pWarning;
    // The constructor for logger, private due to the Singleton nature.
    GSE_Logger() 
    { 
	
    }

      
  public:
    // Method for retrieving GSE_Logger instance.
    //static GSE_Logger *GetLogger();
      
    // Returns the stream where output can be written using << 
    static std::ostream &Log()
    {
      return *m_pLog;
    }

    static std::ostream &Error()
    {
      return *m_pError;
    }
    static std::ostream &Warning()
    {
      return *m_pWarning;
    }
    // Sets the stream which is used for output.
    static void SetLogStream( std::ostream *pStream);
    static void SetErrorStream ( std::ostream *pStream);
    static void SetWarningStream ( std::ostream *pStream);
  };
}

#endif
