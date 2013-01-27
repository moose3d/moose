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

#ifndef __MooseVertexDescriptor_h__
#define __MooseVertexDescriptor_h__
/////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include "MooseCacheable.h"
#include "MooseTypeBase.h"
#include "MooseAPI.h"
#include "MooseOGLConsts.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {

    /////////////////////////////////////////////////////////////////
    /// Data for vertices.
    class MOOSE_API CVertexDescriptor : public Moose::Core::CCacheable<GLuint>,
                                          public Moose::Core::CTypeBase<Moose::Graphics::ELEMENT_TYPE>
    {
    protected:
      /// number of elements.
      size_t 		        m_nSize;
      /// element data
      void *			m_pData;
      /// Size of one element in bytes.
      size_t			m_nElementByteSize;
    public:
      ////////////////////
      /// default constructor.
      /// \param nType Element type
      /// \param nNumElements how many elements this vertexbuffer have (ie. number of vertices).
      /// \par   Not the number of elements in vertex (3 floats), but #vertices.
      CVertexDescriptor( ELEMENT_TYPE nType, unsigned int nNumElements);
      ////////////////////
      /// Copy constructor.
      CVertexDescriptor( const Moose::Graphics::CVertexDescriptor & obj );
      ////////////////////
      /// Frees reserverd memory.
      ~CVertexDescriptor();
      ////////////////////
      /// Returns float pointer.
      /// \returns pointer to float array
      template<typename TYPE>
      inline TYPE * GetPointer( size_t nElement = 0) const
      {
	return reinterpret_cast<TYPE *>(reinterpret_cast<unsigned char *>(m_pData)+(GetElementByteSize()*nElement) );
      }
      ////////////////////
      /// Returns number of elements in descriptor.
      /// \returns Number of elements.
      inline size_t GetSize() const
      {
	return m_nSize;
      }
      ////////////////////
      /// Element byte size.
      inline size_t GetElementByteSize()  const
      {
	return m_nElementByteSize;
      }
      ////////////////////
      /// Returns byte size for entire buffer.
      inline size_t GetByteSize() const
      {
	return GetElementByteSize()*GetSize();
      }
      ///////////////////
      /// Copies data from existing array.
      /// \param nToWhichIndex To which index data is copied in vertexdescriptor.
      /// \param nNumElements Number of elements copied from pData.
      /// \param pData Data buffer pointer where data is copied from. 
      void Copy( size_t nToWhichIndex, size_t nNumElements, const void *pData );
      ////////////////////
      /// Creates a VBO cache for this object.
      void CreateCache();
      ////////////////////
      /// Recreates a VBO cache for this object.
      void UpdateCache();
    };
  };  // end namespace Graphics
};// end namespace Moose
/////////////////////////////////////////////////////////////////
#endif
