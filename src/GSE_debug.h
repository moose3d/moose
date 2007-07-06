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

#ifndef __GSE_debug_h__
#define __GSE_debug_h__



#define DEBUG_HEADER  "in " << __FILE__ << ", line " << __LINE__ << " at " << __FUNCTION__ << "() : "
#define DEBUG_HEADER_FULL << "in " << __FILE__ << ", line " << __LINE__ << " at " << __PRETTY_FUNCTION__ << " : "


#ifdef DEBUG
#define DEBUG_FUNCTION_EXIT() { Core::GSE_Logger::Error() << "Exiting " << __PRETTY_FUNCTION__ << std::endl;}
#define DEBUG_FUNCTION_ENTER(){ Core::GSE_Logger::Error() << "Entering " << __PRETTY_FUNCTION__ << std::endl;}
#else
#define DEBUG_FUNCTION_EXIT() 
#define DEBUG_FUNCTION_ENTER()
#endif
#endif
