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
#ifndef __GSE_h__
#define __GSE_h__
/////////////////////////////////////////////////////////////////
#include "GSE_animation.h"
#include "GSE_Camera.h"
#include "GSE_GraphNodes.h"
#include "GSE_Octree.h"
#include "GSE_debug.h"
#include "GSE_IndexBuffer.h"
#include "GSE_ffmpeg.h"
#include "GSE_Frustum.h"
#include "GSE_geometry.h"
#include "GSE_globals.h"
#include "GSE_graph.h"
#include "GSE_interfaces.h"
#include "GSE_Light.h"
#include "GSE_Logger.h"
#include "GSE_Material.h"
#include "GSE_math.h"
#include "GSE_GeometryData.h"
#include "GSE_OglRenderer.h"
#include "GSE_OglShaders.h"
#include "GSE_OglTexture.h"
#include "GSE_OglUtils.h"
#include "GSE_Orientable.h"
#include "GSE_particlesystems.h"
#include "GSE_Screen.h"
#include "GSE_shaders.h"
#include "GSE_structures.h"
#include "GSE_Milkshape.h"
#include "GSE_SceneRenderer.h"
#include "GSE_OglName.h"
#include "GSE_core.h"
#include "GSE_Configuration.h"
#include "GSE_SceneGraph.h"
#include "GSE_IlluminationSet.h"
#include "GSE_milkshapeTools.h"
#include "GSE_particlesystemTools.h"
#include "GSE_3dsTools.h"
#include "GSE_terrainTools.h"
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
// file : GSE.h
// desc : The Ultimate header file for GSE library
// author : AG/eNtity
// history:
//
//		$Log: GSE.h,v $
//		Revision 1.19  2007/06/24 17:01:11  entity
//		ms3d moved
//
//		Revision 1.18  2007/06/04 10:31:55  entity
//		removed obsolete headers
//
//		Revision 1.17  2007/04/01 15:25:05  entity
//		texture system overhaul, Elements->Indexbuffer rename
//
//		Revision 1.16  2007/03/31 16:20:43  entity
//		ObjStruct -> GeometryData rename
//
//		Revision 1.15  2007/03/31 15:56:35  entity
//		ElementList->IndexBuffer rename
//
//		Revision 1.14  2007/03/29 13:15:07  entity
//		added terraintools
//
//		Revision 1.13  2007/03/27 10:12:59  entity
//		3dstools added
//
//		Revision 1.12  2007/03/23 07:42:53  entity
//		sgutils.h gone, added particlesystemTools.h,gobTools.h,milkshapeTools.h
//
//		Revision 1.11  2007/03/22 19:54:33  entity
//		GSE_datastructures.h renamed
//
//		Revision 1.10  2007/03/20 20:39:28  entity
//		removed portal graph stuff
//
//		Revision 1.9  2007/03/16 08:23:28  entity
//		added illuminationset
//
//		Revision 1.8  2007/03/11 17:02:44  entity
//		SceneGraph in its own file
//
//		Revision 1.7  2007/03/08 20:08:57  entity
//		Added new header, GSE_GraphNodes.h
//
//		Revision 1.6  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.5  2006/12/29 13:59:06  entity
//		remove 3ds support
//
//		Revision 1.4  2006/12/02 15:10:42  entity
//		bezier added
//
//		Revision 1.3  2006/10/15 19:03:10  entity
//		Added GSE_Configuration.h
//
//		Revision 1.2  2006/10/11 08:44:56  entity
//		Added proper header
//
/////////////////////////////////////////////////////////////////
