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

#include "GSE_core.h"
#include "GSE_IndexBuffer.h"
#include "GSE_Camera.h"
#include "GSE_shaders.h"
#include "GSE_animation.h"
#include "GSE_GeometryData.h"
#include "GSE_OglName.h"
#include "GSE_OglTexture.h"
#include "GSE_particlesystems.h"
///
///
/// 
void 
Core::CleanManagers()
{
  GSE_CameraMgr::Destroy();
  GSE_IndexBufferMgr::Destroy();
  GSE_FragmentShaderMgr::Destroy();
  GSE_JointMgr::Destroy();
  GSE_LightMgr::Destroy();
  GSE_MaterialMgr::Destroy();
  GSE_GeometryDataMgr::Destroy();
  GSE_OglCacheMgr::Destroy();
  GSE_OglNameMgr::Destroy();
  GSE_OglTextureMgr::Destroy();
  GSE_ParticleSystemMgr::Destroy();
  GSE_ShaderProgramMgr::Destroy();
  GSE_VertexAttribStructMgr::Destroy();
  GSE_VertexShaderMgr::Destroy();

}
