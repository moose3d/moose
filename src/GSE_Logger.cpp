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
// $Id: GSE_Logger.cpp,v 1.7 2007/01/08 12:24:00 entity Exp $
// desc : Implementation for Logger utility.
// author : AG/eNtity 
// history : 
//		$Log: GSE_Logger.cpp,v $
//		Revision 1.7  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.6  2006/10/13 08:12:34  entity
//		Fixed Log pointer
//
//		Revision 1.5  2006/10/13 08:12:18  entity
//		Fixed Log pointer
//
//		Revision 1.4  2006/10/13 08:05:40  entity
//		made code clearer
//
//		Revision 1.3  2006/10/13 08:04:56  entity
//		Added header for the file
//
// -----------------------------------------------------------------
#include "GSE_Logger.h"
// -----------------------------------------------------------------
using namespace Core;
// -----------------------------------------------------------------
// initialize static pointers
std::ostream *	GSE_Logger::m_pLog      = &std::cout;
std::ostream *  GSE_Logger::m_pError    = &std::cerr;
std::ostream *  GSE_Logger::m_pWarning  = &std::cerr;
// -----------------------------------------------------------------
void
GSE_Logger::SetLogStream( std::ostream *pStream )
{
  m_pLog = pStream;
}
// -----------------------------------------------------------------
void
GSE_Logger::SetErrorStream( std::ostream *pStream )
{
  m_pError = pStream;
}
// -----------------------------------------------------------------
void
GSE_Logger::SetWarningStream( std::ostream *pStream )
{
  m_pWarning = pStream;
}
// -----------------------------------------------------------------
