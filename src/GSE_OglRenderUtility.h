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
#ifndef __GSE_OglRenderUtility_h__
#define __GSE_OglRenderUtility_h__
/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
#include <GSE_structures.h>
#include <GSE_Camera.h>
/////////////////////////////////////////////////////////////////
/// \brief An utility class for simple drawing commands.
/// \todo Implement fully and put it in use.
class GSE_OglRenderUtility 
{
 public:
  ////////////////////
  /// Renders a quad on screen.
  /// \param vCoord1 coordinate for first corner.
  /// \param vCoord2 coordinate for second corner.
  /// \param vCoord3 coordinate for third corner.
  /// \param vCoord4 coordinate for fourth corner.
  inline void RenderQuad( const GSE_Vector3 &vCoord1, 
			  const GSE_Vector3 &vCoord2,
			  const GSE_Vector3 &vCoord3,
			  const GSE_Vector3 &vCoord4 )
  {
    glBegin(GL_QUADS);
	glVertex3fv( vCoord1.m_pValues );
	glVertex3fv( vCoord2.m_pValues );
	glVertex3fv( vCoord3.m_pValues );
	glVertex3fv( vCoord4.m_pValues );
   glEnd();
  }
  ////////////////////
  /// Assigns color for drawing stuff.
  inline void SetColor( const GSE_Color & vColor )
  {
    glColor4fv( vColor.m_pValues );
  }

  ////////////////////
  /// Sets 2D rendering mode.
  /// \param pScreen Screen entity.
  void Set2DRenderingMode( GSE_Screen *pScreen );
  
};
/////////////////////////////////////////////////////////////////
// $Log: GSE_OglRenderUtility.h,v $
// Revision 1.5  2007/06/04 09:03:26  entity
// GLee fix
//
// Revision 1.4  2007/06/01 08:55:43  entity
// doxygen comments added
//
// Revision 1.3  2007/05/21 12:50:00  entity
// glew to GLee
//
// Revision 1.2  2007/05/10 12:59:13  entity
// 2drenderingmode added
//
// Revision 1.1  2007/05/10 10:43:59  entity
// Initial revision
//
/////////////////////////////////////////////////////////////////
#endif
