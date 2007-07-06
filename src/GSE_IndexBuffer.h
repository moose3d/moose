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
/////////////////////////////////////////////////////////////////
#ifndef __GSE_IndexBuffer_h__
#define __GSE_IndexBuffer_h__
/////////////////////////////////////////////////////////////////
#include <list>
#include "GSE_structures.h"
#include <GL/GLee.h>
#include <GL/gl.h>
/////////////////////////////////////////////////////////////////
class GSE_IndexBufferMgr;
/////////////////////////////////////////////////////////////////
/// Contains an array of indices and their count.
class GSE_IndexBuffer
{
  friend class GSE_IndexBufferMgr;
  friend class GSE_Container<GSE_IndexBuffer>;
 public:
  /////////////////////////////////////////////////////////////////
  //
  // The caching methods for each of the arrays.
  // Allowed (and reasonable) transitions are:
  // 
  // Initial state: NO_CACHE
  // ------------------------
  // start state  ->   state which it can be changed
  // ==============================================
  // NO_CACHE -> REQUEST_VBO_CACHE 
  // REQUEST_VBO_CACHE -> CACHED_IN_VBO | NO_CACHE
  // CACHED_IN_VBO -> REGENERATE_VBO_CACHE | DELETE_VBO_CACHE 
  // DELETE_VBO_CACHE -> NO_CACHE
  // REGENERATE_VBO_CACHE -> CACHED_IN_VBO | NO_CACHE
  //
  /////////////////////////////////////////////////////////////////
  /// Available caching message values.
  typedef enum {
    /// Use normal vertex arrays.
    NO_CACHE = 0,		
    /// Set the first VBO caching to occur next time when renderer makes its pass.
    REQUEST_VBO_CACHE = 1,	
    /// Renderer has cached this.
    CACHED_IN_VBO,		
    /// Renderer will regenerate the cache next time.
    REGENERATE_VBO_CACHE,	
    /** Renderer will delete cache during next pass and set method to 
	NO_CACHE. If you use this, you  better MAKE SURE object will 
	be destroyed after this (otherwise horrible things happen in 
	OpenGL renderer). */
    DELETE_VBO_CACHE
  } CacheMethod_t;
  /////////////////////////////////////////////////////////////////
  /// a render batch struct 
  typedef struct RenderBatch_t
  {
    /// Start index of this batch.
    unsigned int m_nStart;
    /// End index of this batch.
    unsigned int m_nEnd;
    /// number of elements in this batch.
    unsigned int m_nCount;
    /// Batch data.
    unsigned short *m_pData;
    /// Default constructor.
    RenderBatch_t()
    {
      m_pData = NULL;
      m_nStart = m_nEnd = m_nCount = 0;
    }
    
  } RenderBatch_t;
  /////////////////////////////////////////////////////////////////
 protected:
  /// The message for renderer.
  CacheMethod_t m_nCacheMethod; 
  /// The name of the cache object.
  GLuint	m_ObjectName;   
  /// The list of batches which can be rendered.
  std::list< RenderBatch_t > m_lstRenderBatches;
  /////////////////////////////////////////////////////////////////
  /// The constructor.
  GSE_IndexBuffer();
  /////////////////////////////////////////////////////////////////
  /// The destructor.
  ~GSE_IndexBuffer();
 public:
  /// The number of element indices in total.
  unsigned int m_nNumIndices;
  /// The number of element indices to be drawn.
  unsigned short m_nDrawIndices;
  /// Should batching be used.
  bool m_bBatchingEnabled;
  /// the array of indices.
  unsigned short *m_pIndices;
  /// The number of vertices in each primitive, default 3.
  unsigned short m_nVerticesInPrimitive;
  /////////////////////////////////////////////////////////////////
  /// The copy constructor.
  GSE_IndexBuffer(const GSE_IndexBuffer &ref);
  ////////////////////////////////////////////////////////
  /// The assignment operator.
  GSE_IndexBuffer &operator=(GSE_IndexBuffer obj);
  /////////////////////////////////////////////////////////////////
  /// The initialization method. Allocates memory for the object by
  /// given element count.
  /// \param nSize the number of elements in the object.
  void Init( unsigned int nSize );
  ////////////////////
  /// Creates a continuous index set 0...m_nNumIndices-1. (Eg. 0,1,2,...)
  void CreateLinearIndexSet();
  /////////////////////////////////////////////////////////////////
  /// Sets the message for the renderer.
  void SetCacheMethod( CacheMethod_t nMethod );
  /////////////////////////////////////////////////////////////////
  /// Returns the message for the renderer.
  CacheMethod_t GetCacheMethod();
  /////////////////////////////////////////////////////////////////
  /// Returns a reference to the cache object name.
  GLuint &Cache();
  /////////////////////////////////////////////////////////////////
  /// Returns a reference to the RenderBatches list.
  std::list<RenderBatch_t> & RenderBatches();

 private:
  /////////////////////////////////////////////////////////////////
  /// A cleanup method.
  void Destroy();
  
}; 
/////////////////////////////////////////////////////////////////
/// A singleton manager class for GSE_IndexBuffer objects.
class GSE_IndexBufferMgr : public GSE_Container<GSE_IndexBuffer>,
			   public GSE_Singleton<GSE_IndexBufferMgr>
{

  friend class GSE_Singleton<GSE_IndexBufferMgr>;
  friend class GSE_IndexBuffer;

 private:
  /////////////////////////////////////////////////////////////////
  /// The constructor.
  GSE_IndexBufferMgr() : GSE_Container< GSE_IndexBuffer >() { }
  /////////////////////////////////////////////////////////////////
  /// The destructor.
  ~GSE_IndexBufferMgr()  {  }
 public:
  /////////////////////////////////////////////////////////////////
  /// The factory method for elementlist.
  /// \returns GSE_IndexBuffer which is managed by IndexBufferMgr.
  GSE_IndexBuffer * CreateIndexBuffer()
  {
    GSE_IndexBuffer *pTmp = new GSE_IndexBuffer();
    Add(pTmp);
    return pTmp;
  }
  
};
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
// $Log: GSE_IndexBuffer.h,v $
// Revision 1.5  2007/06/04 09:03:21  entity
// GLee fix
//
// Revision 1.4  2007/06/02 10:23:29  entity
// small syntax errors fixed.
//
// Revision 1.3  2007/06/01 08:55:43  entity
// doxygen comments added
//
// Revision 1.2  2007/05/21 12:50:00  entity
// glew to GLee
//
// Revision 1.1  2007/03/31 15:56:35  entity
// ElementList->IndexBuffer rename
//
// Revision 1.8  2007/03/30 12:58:34  entity
// CreateLinearIndexSet()
//
// Revision 1.7  2007/03/28 08:33:59  entity
// friend class declaration
//
// Revision 1.6  2007/03/27 10:13:36  entity
// some friend class fixes
//
// Revision 1.5  2007/03/27 08:01:04  entity
// more comments
//
// Revision 1.4  2007/03/27 08:00:48  entity
// Added log and comments.
//
/////////////////////////////////////////////////////////////////
