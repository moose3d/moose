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

#ifndef __GSE_OglName_h__
#define __GSE_OglName_h__

#include <GL/GLee.h>
#include <GL/gl.h>

class GSE_OglNameMgr : public GSE_Singleton<GSE_OglNameMgr>
{

  friend class GSE_Singleton<GSE_OglNameMgr>;

 protected:
  
  GLuint       m_nName;
  
  GSE_OglNameMgr()
  {
    m_nName = 1;
  }
  
  ~GSE_OglNameMgr() 
  {
    
  }
  
 public:  

  GLuint GetNextName()
  {
    return m_nName++;
  }
 
};



#endif

