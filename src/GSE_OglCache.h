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

#ifndef __GSE_OglCache_h__
#define __GSE_OglCache_h__

#include <GL/GLee.h>
#include <GL/gl.h>
#include "GSE_structures.h"
#include "GSE_Logger.h"
using Core::GSE_Logger;
// -----------------------------------------------------------------------
//
// The class which encapsulates the cache ( in our case, a display list )
// These babies can be used create a display list from the entire (sub)render graph.
// Creating a display list from a single node, say, GSE_IndexBuffer is possible 
// but the gain would be minimal at best. Large chunks is where you get the
// speedup.
// -----------------------------------------------------------------------
class GSE_OglCache 
{
 public:
  typedef enum {

    NO_CACHE = 0,		// Use normal vertex arrays
    REQUEST_DL_CACHE,           // Set the first DL caching to occur next time
				// when renderer makes its pass
    CACHED_IN_DL,		// Renderer has cached this.
    REGENERATE_DL_CACHE,	// Renderer will regenerate the cache next time.
    DELETE_DL_CACHE	        // Renderer will delete cache and set method
				// to NO_CACHE
    // -----------------------------------------------------------------------
  } CacheMethod_t;
  
 protected:

  GLuint	m_ObjectName;
  CacheMethod_t m_nCacheMethod;

 public:
  // The constructor
  GSE_OglCache()
  {
    
  }
  // Returns a reference to the object name
  GLuint &Cache()
  {
    return m_ObjectName;
  }
  // Assigns the cache message to nMethod
  void SetCacheMethod( CacheMethod_t nMethod )
  {
    m_nCacheMethod = nMethod;
  }
  // Returns the current cache message for renderer
  CacheMethod_t GetCacheMethod()
  {
    return m_nCacheMethod;
  }
  
};
// -----------------------------------------------------------------------
//
// The manager class for cache opengl cache objects.
//
// -----------------------------------------------------------------------
class GSE_OglCacheMgr : public GSE_Singleton<GSE_OglCacheMgr>,
			public GSE_Container<GSE_OglCache> 
{
  friend class GSE_Singleton<GSE_OglCacheMgr>;
 private:
  
   GSE_OglCacheMgr(){}
  ~GSE_OglCacheMgr()
  {
    GSE_Logger::Error() << "GSE_OglCacheMgr destructor.\n";
    GSE_Container<GSE_OglCache>::Clear();
  }
 public:
  
  
};

#endif
