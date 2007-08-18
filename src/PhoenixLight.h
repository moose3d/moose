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

#ifndef __GSE_light_h__
#define __GSE_light_h__

#include "GSE_structures.h"
#include "GSE_Configuration.h"

class GSE_LightMgr;

class GSE_Light : public GSE_Object, public GSE_Positional
{
  friend class GSE_LightMgr;
 public:
  // GSE_LightType defines does the light emit in 
  // every direction or is is a spotlight.
  typedef enum {
    SPOTLIGHT = 0,
    DIRECTIONAL = 1,
    POINTLIGHT = 2
  } GSE_LightType;

  // GSE_LightLocation defines how distant the light is; 
  // ie. sun might ne DIRECTIONAL, lightbulb POINTLIGHT.

  // @see GSE_Lighttype
  GSE_LightType		m_iLightType;

  // The diffuse color for this light
  GSE_Color	m_vDiffuseColor;
  // The ambient color for this light
  GSE_Color	m_vAmbientColor;
  // The specular component for this light
  GSE_Color	m_vSpecular;
  
  // The direction of this light, used when type is DIRECTIONAL or
  // SPOTLIGHT
  GSE_Vector3	m_vDirection;

  // accepted values [0.0 .. 90.0],180
  float		m_fSpotAngle;
  float		m_fSpotExponent;

  int		m_bEnabled;
  // The attenuation values
  float		m_fQuadraticAttenuation;
  float		m_fLinearAttenuation;
  float		m_fConstantAttenuation;
  /// Distance to be used when determining closest lights to an object.
  float		m_fBrightness;
 protected:
  // The default constructor.
  GSE_Light() : m_vDirection(-GSE_Vector3::GetWorldZ())
  {
    m_fSpotAngle    = 0.0;
    m_fSpotExponent = 0.0;
    m_iLightType    = DIRECTIONAL;
    m_bEnabled      = 1;
    m_fQuadraticAttenuation = 0.0;
    m_fLinearAttenuation    = 0.0;
    m_fConstantAttenuation  = 0.0;
    m_fBrightness = 0.0f;
  }
 public:
  inline int IsEnabled() const
  {
    return m_bEnabled;
  }
  inline void SetEnabled(int bFlag )
  {
    m_bEnabled = bFlag;
  }

};

//
// The Singleton manager class for every light
//
class GSE_LightMgr : public GSE_Container<GSE_Light>,
		     public GSE_Singleton< GSE_LightMgr >,
		     protected GSE_Configuration
{
  friend class GSE_Singleton<GSE_LightMgr>;
 private:
  
  // The default constructor, private due the Singleton nature.
  GSE_LightMgr() : GSE_Container<GSE_Light>(), GSE_Configuration() 
  {
    SetSectionString("Light");
    SetSectionEndString("}");
    SetWarningsEnabled(false);
  }
  ~GSE_LightMgr() {}
  
 public:
  GSE_Light *CreateLight()
  {
    GSE_Light *pLight = new GSE_Light();
    GSE_Container<GSE_Light>::Add( pLight );
    return pLight;
  }
  GSE_Light *GetLightFromFile( const char *szFilename, const char *szLightname = NULL)
  {
    string strLightName;
    GSE_Light *pLight = NULL;

    if ( ReadConfig(szFilename) )
    {
      return NULL;
    }
    else if ( GetNodeCount() > 1 ) // Check there was some options
    {
      if ( szLightname != NULL )
      {
	strLightName = szLightname;
      } 
      else
      {
	// Grab the first light
	strLightName = GetChildren("").front();
      }
      pLight = CreateLight();
      string strType = GetString(strLightName+"|Type");

      if      ( strType == "Point" )        pLight->m_iLightType = GSE_Light::POINTLIGHT;
      else if ( strType == "Spot" )	    pLight->m_iLightType = GSE_Light::SPOTLIGHT;
      else if ( strType == "Directional" )  pLight->m_iLightType = GSE_Light::DIRECTIONAL;
      else				    pLight->m_iLightType = GSE_Light::POINTLIGHT;

      
      pLight->SetPosition(       GSE_Vector3(GetVector<3>(strLightName+"|Position").m_pValues));
      pLight->m_vDiffuseColor  = GSE_Color(GetVector<4>(strLightName+"|DiffuseColor").m_pValues);
      pLight->m_vAmbientColor  = GSE_Color(GetVector<4>(strLightName+"|AmbientColor").m_pValues);
      pLight->m_vSpecular      = GSE_Color(GetVector<4>(strLightName+"|SpecularColor").m_pValues);
      pLight->m_vDirection     = GSE_Vector3(GetVector<3>(strLightName+"|Direction").m_pValues);

      pLight->m_fSpotAngle	       = GetFloat(strLightName+"|SpotAngle");
      pLight->m_fSpotExponent	       = GetFloat(strLightName+"|SpotExponent");
      pLight->m_bEnabled	       = GetBoolean(strLightName+"|Enabled");
      pLight->m_fQuadraticAttenuation  = GetFloat(strLightName+"|AttQuadratic");
      pLight->m_fLinearAttenuation     = GetFloat(strLightName+"|AttLinear");
      pLight->m_fConstantAttenuation   = GetFloat(strLightName+"|AttConstant");
    }
    Reset();
    return pLight;
  }
  
  //static GSE_LightMgr * GetManager();
  
};

#endif
