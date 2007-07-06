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

#include "GSE_particlesystems.h"
#include "GSE_Logger.h"
#include "GSE_debug.h"
#include <algorithm>
using Core::GSE_Logger;




// void 
// GSE_ParticleSystem::Update( unsigned int nPassedTimeInMs )
// {

//   float fPassedTimeInSec = (float)nPassedTimeInMs / 1000.0;

//   if ( fPassedTimeInSec < 0.0f)
//   {
    
//     fPassedTimeInSec = 0.0f;
//     GSE_Logger::Error() << DEBUG_HEADER
// 			<< "Passed time is negative!" 
// 			<< std::endl;
//     return;
//   }

//   for(unsigned int i = 0;i<m_nNumParticles;i++)
//   {

//     GSE_Particle &p = m_pParticles[i];
                                    
//     p.m_fEnergy   -= m_ParticleBase.m_fEnergyDecreasePerSecond * fPassedTimeInSec;
//     // Is particle dead
//     if ( p.m_fEnergy <= 0.0f ){
//       p.m_fEnergy = 0.0f;
//       m_nNumParticlesAlive--;
//       continue;
//     }
    
//     //p.m_vPosition += p.m_vVelocity * fPassedTimeInSec;
//     p.m_vPosition.m_pValues[GSE_Vector3::X] += p.m_vVelocity.m_pValues[GSE_Vector3::X] * fPassedTimeInSec;
//     p.m_vPosition.m_pValues[GSE_Vector3::Y] += p.m_vVelocity.m_pValues[GSE_Vector3::Y] * fPassedTimeInSec;
//     p.m_vPosition.m_pValues[GSE_Vector3::Z] += p.m_vVelocity.m_pValues[GSE_Vector3::Z] * fPassedTimeInSec;
    
//     p.m_fRotationAngle += p.m_fRotationSpeed * fPassedTimeInSec;
    
//   }
// }




// -----------------------------------------------------------------
// template <size_t, class ParticleType>
// GSE_ParticleSystemLoader::GSE_ParticleSystemLoader() : GSE_Configuration()
// {
//   SetSectionString("ParticleSystem");
//   SetSectionEndString("}");
//   SetWarningsEnabled(0);
// }

// GSE_ParticleSystemLoader::~GSE_ParticleSystemLoader()
// {
//   Reset();
// }

// GSE_ParticleSystem *
// GSE_ParticleSystemLoader::LoadSystem( const char *szFilename,
// 				      const char *szSystemName)
// {
//   Reset();
//   if ( szSystemName == NULL ) 
//   {
//     GSE_ERR("ParticleSystem name is NULL, cancelling load.");
//     return NULL;
//   }
//   string sSystemname(szSystemName);
//   GSE_ParticleSystem *pSystem = NULL;
//   if ( !ReadConfig(szFilename) )
//   {
//     GSE_ParticleBase  base;

//     base.m_vColor[GSE_Color::R] = GetFloat( sSystemname+"|ParticleColorR");
//     base.m_vColor[GSE_Color::G] = GetFloat( sSystemname+"|ParticleColorG");
//     base.m_vColor[GSE_Color::B] = GetFloat( sSystemname+"|ParticleColorB");
//     base.m_vColor[GSE_Color::A] = GetFloat( sSystemname+"|ParticleColorA");

    
//     unsigned int nMaxParticles  = GetInt(   sSystemname+"|MaxParticles");
//     float fThroughPut           = GetFloat( sSystemname+"|ThroughPut");
//     unsigned int nInitParticles  = GetInt( sSystemname+"|StartParticles");

//     base.m_fEnergyDecreasePerSecond = GetFloat( sSystemname+"|EnergyDrain");
//     base.m_fInitialEnergy = GetFloat( sSystemname+"|InitialEnergy");
//     base.m_fInitialRotationSpeed = GetFloat( sSystemname+"|RotationSpeed");
    
//     pSystem = new GSE_ParticleSystem( base, nMaxParticles );
//     pSystem->SetThroughPut( fThroughPut );
//     pSystem->Init(nInitParticles);

//     GSE_ParticleSystemMgr *pPSysMgr = GSE_ParticleSystemMgr::GetInstance();
//     pPSysMgr->Add( pSystem );
    
//   }
//   return pSystem;
// }
// // class GSE_ParticleSystem
// GSE_GravityWellSphere::GSE_GravityWellSphere() : GSE_GravityWell(), GSE_Sphere()
// {
//   m_iType = PUSH_FROM_CENTER;
// }

// GSE_GravityWellSphere::~GSE_GravityWellSphere()
// {
  
// }

// int 
// GSE_GravityWellSphere::InRange( GSE_Vector3 pPos )
// {
//   char bInRange = 0;
//   if ( (pPos - GetPosition()).Length() < GetRadius()) {
//     bInRange = 1;
//   } else {
//     bInRange = 0;
//   }
  
//   return bInRange;
// }

// float
// GSE_GravityWellSphere::GetForce()
// {
//   return 0.0;
// }

// void
// GSE_GravityWellSphere::SetForce(float fForce)
// {
//   m_fForce = fForce;
// }

// GSE_Vector3	
// GSE_GravityWellSphere::GetVelocity( GSE_Vector3 vPos, float fPassedTime )
// {
//   GSE_Vector3 vVelocity;
//   if ( InRange(vPos)){
//     vVelocity = (m_vDirection * m_fForce * fPassedTime);
//   }
//   return vVelocity;
// }

// void
// GSE_GravityWellSphere::SetType( GSE_GravityWellSphereType iType )
// {
//   m_iType = iType;
// }
