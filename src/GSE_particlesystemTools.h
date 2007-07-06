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
#ifndef __GSE_particlesystemTools_h__
#define __GSE_particlesystemTools_h__
/////////////////////////////////////////////////////////////////
#include "GSE_structures.h"
#include "GSE_particlesystems.h"
#include "GSE_GraphNodes.h"
#include "GSE_Material.h"
/////////////////////////////////////////////////////////////////
/// \brief A class for creating complete renderable particle systems.
class GSE_ParticleSystemConverter
{

 private:
  GSE_ParticleSystemConverter(){}
 public:
  static void Convert( GSE_ParticleSystemBase *pParticleSystem,
		       GSE_Texture *pTexture,
		       GSE_TextureNode **ppTextureNode,
		       GSE_ParticleSystemNode **ppParticleSystemNode,
		       GSE_AnimParticleSystemNode **ppAnimNode,
		       GSE_AnimationGraph *pAnimGraph,
		       GSE_RenderGraph *pRenderGraph);
};
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
// $Log: GSE_particlesystemTools.h,v $
// Revision 1.3  2007/04/04 20:57:53  entity
// retrieval of the psnode
//
// Revision 1.2  2007/04/03 12:51:16  entity
// renamed Helper->Converter and added texture quality
//
// Revision 1.1  2007/03/23 07:38:18  entity
// separated from GSE_sgutils.h
//
/////////////////////////////////////////////////////////////////
