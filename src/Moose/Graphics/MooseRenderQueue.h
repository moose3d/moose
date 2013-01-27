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

#ifndef __MooseRenderQueue_h__
#define __MooseRenderQueue_h__
/////////////////////////////////////////////////////////////////
#include "MooseOctree.h"
#include "MooseCamera.h"
#include "MooseOGLRenderer.h"
#include "MooseAPI.h"
#include <vector>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class for collecting and sorting renderable objects.
    template<typename TYPE>
    class MOOSE_API CRenderQueue
    {
    protected:
      std::list<TYPE> m_lstObjects;
    public:
      ////////////////////
      /// Constructor.
      CRenderQueue();
      ////////////////////
      /// Destructor.
      ~CRenderQueue();
      ////////////////////
      /// Returns object list.
      /// \returns Reference to object list.
      std::list<TYPE> & GetObjectList();
      ////////////////////
      /// Renders current queue using given renderer.
      /// \param renderer OpenGL renderer.
      void Render( Moose::Graphics::COglRenderer &renderer );
      ////////////////////
      /// Clears current queue.
      void Clear();
      ////////////////////
      /// Performs sorting using adapter class.
      template<class SORTER> void Sort( SORTER & rAdapter );
      ////////////////////
      /// Returns number of elements in queue.
      /// \returns queue size
      size_t GetSize() const;
    };
  }
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Moose::Graphics::CRenderQueue<TYPE>::CRenderQueue()
{
  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Moose::Graphics::CRenderQueue<TYPE>::~CRenderQueue()
{
  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
std::list<TYPE> & 
Moose::Graphics::CRenderQueue<TYPE>::GetObjectList()
{
  return m_lstObjects;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Moose::Graphics::CRenderQueue<TYPE>::Clear()
{
  m_lstObjects.clear();
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void 
Moose::Graphics::CRenderQueue<TYPE>::Render( Moose::Graphics::COglRenderer &renderer)
{
  typename std::list<TYPE>::iterator it = m_lstObjects.begin();
  for( ; it != m_lstObjects.end(); it++)
  {
    (*it)->Render(renderer); 
  }      
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
template<class SORTER> 
inline void 
Moose::Graphics::CRenderQueue<TYPE>::Sort( SORTER &rS )
{
  rS( m_lstObjects );
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
size_t
Moose::Graphics::CRenderQueue<TYPE>::GetSize() const
{
  return m_lstObjects.size();
}
/////////////////////////////////////////////////////////////////
#endif
