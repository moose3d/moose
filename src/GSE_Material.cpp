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
#include "GSE_Material.h"
#include "GSE_debug.h"
#include <string>
/////////////////////////////////////////////////////////////////
using std::string;
/////////////////////////////////////////////////////////////////
GSE_Material::GSE_Material()
{
  m_fShininess = 0.0;
  // Initially, object is not cached.
  SetCacheMethod( NO_CACHE );
  m_bTransparentHint = 0;
  Cache() = 0;
}
/////////////////////////////////////////////////////////////////
GSE_Material::~GSE_Material()
{
  
}
/////////////////////////////////////////////////////////////////
const GSE_Color &
GSE_Material::GetDiffuse() const
{
  return m_vDiffuse;
}
/////////////////////////////////////////////////////////////////
void
GSE_Material::SetDiffuse( GSE_Color vDiffuse)
{
  m_vDiffuse = vDiffuse;
}
/////////////////////////////////////////////////////////////////
const GSE_Color &
GSE_Material::GetAmbient() const
{
  return m_vAmbient;
}
/////////////////////////////////////////////////////////////////
void
GSE_Material::SetAmbient( GSE_Color vAmbient )
{
  m_vAmbient = vAmbient;
}
/////////////////////////////////////////////////////////////////
const GSE_Color &
GSE_Material::GetSpecular() const
{
  return m_vSpecular;
}
/////////////////////////////////////////////////////////////////
void
GSE_Material::SetSpecular(GSE_Color vSpecular)
{
  m_vSpecular = vSpecular;
}
/////////////////////////////////////////////////////////////////
const GSE_Color &
GSE_Material::GetEmission() const
{
  return m_vEmission;
}
/////////////////////////////////////////////////////////////////
void
GSE_Material::SetEmission( GSE_Color vEmission)
{
  m_vEmission = vEmission;
}
/////////////////////////////////////////////////////////////////
float
GSE_Material::GetShininess()
{
  return m_fShininess;
}
/////////////////////////////////////////////////////////////////
void
GSE_Material::SetShininess( float fShininess)
{
  m_fShininess = fShininess;
}
/////////////////////////////////////////////////////////////////
int
GSE_Material::IsTransparent()
{
  return m_bTransparentHint;
}
/////////////////////////////////////////////////////////////////
void
GSE_Material::SetTransparent( int bFlag )
{
  m_bTransparentHint = bFlag;
}
/////////////////////////////////////////////////////////////////
GSE_Material::CacheMethod_t
GSE_Material::GetCacheMethod()
{
  return m_nCacheMethod;
}
/////////////////////////////////////////////////////////////////
void		
GSE_Material::SetCacheMethod( GSE_Material::CacheMethod_t nMethod )
{
  m_nCacheMethod = nMethod;
}
/////////////////////////////////////////////////////////////////
GLuint &
GSE_Material::Cache()
{
  return m_nObjectName;
}
/////////////////////////////////////////////////////////////////
// GSE_MaterialMgr implementation:
/////////////////////////////////////////////////////////////////
/// The default constructor.
GSE_MaterialMgr::GSE_MaterialMgr() :  GSE_Container<GSE_Material>()
{
  // Add the default material 
  GSE_Material *pDefaultMaterial = new GSE_Material();
  pDefaultMaterial->SetName("GSE_DEFAULTMATERIAL");
  pDefaultMaterial->SetDiffuse(GSE_Color(1.0,0.0,0.0,1.0));
  pDefaultMaterial->SetAmbient(GSE_Color(0.20,0.20,0.20,1.0));
  pDefaultMaterial->SetSpecular(GSE_Color(0.0,0.0,0.0,1.0));
  pDefaultMaterial->SetEmission(GSE_Color(0.0,0.0,0.0,1.0));
  pDefaultMaterial->SetShininess(0.0);
  Add(pDefaultMaterial);
  Map( string( pDefaultMaterial->GetName()), pDefaultMaterial );
  SetSectionString("Material");
  SetSectionEndString("}");
  SetWarningsEnabled(false);

}
/////////////////////////////////////////////////////////////////
GSE_MaterialMgr::~GSE_MaterialMgr()
{
  GSE_Container<GSE_Material>::Clear();
}
/////////////////////////////////////////////////////////////////
GSE_Material *
GSE_MaterialMgr::CreateMaterial( const char *szMaterialName )
{
  GSE_Material *pMaterial  = NULL;
  if ( szMaterialName != NULL )
  {
    // If material with same name is found, return it.
    if ( (pMaterial = Find( string(szMaterialName))) == NULL )
    {
      pMaterial = new GSE_Material();
      pMaterial->SetName( (char *)szMaterialName );
      
      // Insert material name into the map
      GSE_Mapper<string,GSE_Material>::Map( 
				    string(pMaterial->GetName()),
				    pMaterial);
      // Add material into the container
      GSE_Container<GSE_Material>::Add( pMaterial );
      
    } 
    else 
    {
      GSE_WARN("Material '" << szMaterialName 
	       << "' already exists, using it..");
    }
  } 
  else
  {
    GSE_WARN("Material name NULL, cannot create.");
  }
  return pMaterial;
}
/////////////////////////////////////////////////////////////////
std::ostream & 
operator<<( std::ostream &stream, GSE_Material mat )
{
  stream << "name: " << mat.m_sName << endl
	 << "diffuse: " << mat.m_vDiffuse  << endl
	 << "ambient: " << mat.m_vAmbient << endl
 	 << "specular: " << mat.m_vSpecular << endl
	 << "emission: " << mat.m_vEmission << endl
	 << "shininess: " << mat.m_fShininess << endl;
  return stream;
}
/////////////////////////////////////////////////////////////////
GSE_Material * 
GSE_MaterialMgr::GetMaterialFromFile(const char *szFilename, 
				     const char *szMaterialName )
{
  GSE_Material *pMaterial  = NULL;
  string strMaterialName;
  if ( ReadConfig( szFilename ))
  {
    return NULL;
  }
  
  if  ( GetNodeCount() > 1 ) // The should be at least one material.
    if ( szMaterialName != NULL )
    {
      strMaterialName = szMaterialName;
    } 
    else
    {
      // Grab the first material
      strMaterialName = GetChildren("").front();
    }
  
  if ( (pMaterial = Find( strMaterialName )) != NULL )
  {
    GSE_WARN("Material " << strMaterialName << "already exists, using it.");
    return pMaterial;
  }
  
  pMaterial = CreateMaterial( strMaterialName.c_str());
  
  pMaterial->SetDiffuse( GSE_Color(GetVector<4>(strMaterialName+"|DiffuseColor").m_pValues));
  pMaterial->SetAmbient( GSE_Color(GetVector<4>(strMaterialName+"|AmbientColor").m_pValues));
  pMaterial->SetSpecular( GSE_Color(GetVector<4>(strMaterialName+"|SpecularColor").m_pValues));
  pMaterial->SetEmission( GSE_Color(GetVector<4>(strMaterialName+"|Emission").m_pValues));
  pMaterial->SetShininess( GetFloat( strMaterialName+"|Shininess"));
  pMaterial->SetTransparent( GetBoolean( strMaterialName+"|Transparency"));
  Reset();
  return pMaterial;
}
/////////////////////////////////////////////////////////////////
// history:
//
//	$Log: GSE_Material.cpp,v $
//	Revision 1.8  2007/04/01 15:25:05  entity
//	texture system overhaul, Elements->Indexbuffer rename
//
//	Revision 1.7  2007/04/01 08:25:20  entity
//	separated textures from materials
//
//	Revision 1.6  2007/03/13 13:26:23  entity
//	removed commented code and added const & Get{Diffuse,Ambient,etc.}
//
//	Revision 1.5  2007/03/10 14:55:20  entity
//	CreateMaterial() implemented
//
//	Revision 1.4  2007/01/08 12:24:00  entity
//	added GPL licence
//
//	Revision 1.3  2006/12/29 14:01:21  entity
//	GetMaterialCount and Find implemented
//
//	Revision 1.2  2006/10/13 14:54:12  entity
//	Code cleanups
//
//
/////////////////////////////////////////////////////////////////
