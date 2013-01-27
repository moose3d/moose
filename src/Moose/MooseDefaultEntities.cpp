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

#include "MooseModel.h"
#include "MooseScene.h"
#include "MooseDefaultEntities.h"
#include "MooseCamera.h"
#include "MooseCollider.h"
#include "MooseAudioSystem.h"
#include "MooseALSampleTypes.h"
#include "MooseCore.h"
#include "MooseModelHelper.h"
#include "MooseApplication.h"
#include "MooseSphere.h"
///////////////////////////////////////////////////////////////////////////////
Moose::Default::ObjectMgr *
Moose::Default::GetObjectMgr()
{
	return g_ObjectMgr;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Default::TextureManager *
Moose::Default::GetTextureMgr()
{
	return g_TextureMgr;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Default::VertexManager *
Moose::Default::GetVertexMgr()
{
	return g_VertexMgr;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Default::IndexManager  *
Moose::Default::GetIndexMgr()
{
	return g_IndexMgr;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Default::ShaderManager *
Moose::Default::GetShaderMgr()
{
	return g_ShaderMgr;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Default::ModelManager *
Moose::Default::GetModelMgr()
{
	return g_ModelMgr;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Default::CameraManager *
Moose::Default::GetCameraMgr()
{
	return g_CameraMgr;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Default::ColliderManager *
Moose::Default::GetColliderMgr()
{
	return g_ColliderMgr;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Sound::CAudioSystem *
Moose::Default::GetAudioSystem()
{
    return g_AudioSystem;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Default::SoundSampleManager *
Moose::Default::GetSoundSampleMgr()
{
    return g_SampleMgr;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Default::SoundStreamManager *
Moose::Default::GetSoundStreamMgr()
{
    return g_StreamMgr;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Default::SphereManager *      
Moose::Default::GetSphereMgr()
{
  return g_SphereMgr;
}
////////////////////////////////////////////////////////////////////////////////
Moose::Default::TextureDataManager * 
Moose::Default::GetTextureDataMgr()
{
    return g_TextureDataMgr;
}
///////////////////////////////////////////////////////////////////////////////
Moose::Core::CUniqueNameCreator * 
Moose::Default::GetNameCreator()
{
  return Moose::Core::CUniqueNameCreator::GetInstance();
}
////////////////////////////////////////////////////////////////////////////////
Moose::Core::CAssetBundle * 
Moose::Default::GetAssetBundle()
{
  return Moose::Core::CAssetBundle::GetInstance();
}
////////////////////////////////////////////////////////////////////////////////
Moose::Data::CModelHelper *       
Moose::Default::GetModelHelper()
{
  return g_ModelHelper;
}
////////////////////////////////////////////////////////////////////////////////
Moose::Window::CScreen *       
Moose::Default::GetScreen()
{
  return Moose::Window::CScreen::GetInstance();
}
////////////////////////////////////////////////////////////////////////////////
Moose::Scene::CApplication *
Moose::Default::GetApplication()
{
  return Moose::Scene::CApplication::GetInstance();
}
////////////////////////////////////////////////////////////////////////////////
