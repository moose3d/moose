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

#include "GSE_shaders.h"
#include "GSE_Logger.h"

using Core::GSE_Logger;

GSE_ShaderProgram::~GSE_ShaderProgram()
{
  if ( m_pShaderName != NULL ) delete m_pShaderName;
  m_pShaderName = NULL;
}

int
GSE_VertexShaderMgr::InitShaders()
{
  int bRetval = 0;
  int bError = 0;
  vector<GSE_VertexShader *>::iterator itVS;
  for(itVS=m_Objects.begin();itVS!=m_Objects.end();itVS++)
  {
    bError = (*itVS)->InitShader();
    bRetval = bRetval || bError;

    if ( bError ){
      GSE_Logger::Error() << "Shader initialization failed : " 
			  << (*itVS)->GetShaderFilename() 
			  << std::endl;
    }
  }
  return bRetval; 
}

int
GSE_FragmentShaderMgr::InitShaders()
{
  vector<GSE_FragmentShader *>::iterator itFS;

  int bRetval	= 0;
  int bError	= 0;

  for(itFS=m_Objects.begin();itFS!=m_Objects.end();itFS++)
  {

    bError = (*itFS)->InitShader();
    bRetval = bRetval || bError;

    if ( bError ){
      GSE_Logger::Error() << "Shader initialization failed : " 
			  << (*itFS)->GetShaderFilename() 
			  << std::endl;
    }
  }
  return bRetval;
}

int 
GSE_ShaderProgramMgr::InitPrograms()
{

  vector<GSE_ShaderProgram *>::iterator itSP;

  int bRetval	= 0;
  int bError	= 0;

  for(itSP=m_Objects.begin();itSP!=m_Objects.end();itSP++)
  {

    bError = (*itSP)->InitProgram();
    bRetval = bRetval || bError;
    if ( bError ){
      GSE_Logger::Error() << "Shader program initialization failed : " 
			  << (*itSP)->GetShaderName() 
			  << std::endl;
    }
  }
  return bRetval;
}
