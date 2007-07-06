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
#ifndef __GSE_Material_h__
#define __GSE_Material_h__
/////////////////////////////////////////////////////////////////
#include "GSE_structures.h"
#include "GSE_interfaces.h"
#include "GSE_Configuration.h"
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <GL/GLee.h>
#include <GL/gl.h>
/////////////////////////////////////////////////////////////////
/// A material class which holds the attributes which define
/// the appearance of an object.
class GSE_Material : public GSE_Object
{
 public:
  /// Material blendin modes.
  typedef enum {
    /// No blend.
    BLEND_NONE = 0,
    /// Use src alpha.
    BLEND_SRCALPHA = 1
    
  } GSE_MaterialBlendMode;
  
  /// Methods for material caching.
  typedef enum {
    /// Cache is not used.
    NO_CACHE,			
    /// Set the first DL caching to occur next time when renderer makes its pass.
    REQUEST_DL_CACHE,	        
    /// Renderer has cached this.
    CACHED_IN_DL,		
    /// Renderer will regenerate the cache next time.
    REGENERATE_DL_CACHE,	
    /// Renderer will delete cache and set method to NO_CACHE.
    DELETE_DL_CACHE	        
  } CacheMethod_t;

 protected:
  
  /// The diffuse color for this material.
  GSE_Color m_vDiffuse;
  /// The ambient color for this material.
  GSE_Color m_vAmbient;
  /// The specular color for this material.
  GSE_Color m_vSpecular;
  /// The emissive color for this material.
  GSE_Color m_vEmission;
  /// The shininess factor for this material ( 0.0f - 1.0f ).
  float	    m_fShininess;
  /// The message which defines what the renderer should do for the cache.
  CacheMethod_t m_nCacheMethod;
  /// The cache  for material properties (display list).
  GLuint m_nObjectName;
  /// The transparency hint flag for the renderer.
  char m_bTransparentHint;
 public:
  
  /// Default constructor.
  GSE_Material();
  /// default destructor.
  ~GSE_Material();
  /// Returns the Diffuse color
  const GSE_Color & GetDiffuse() const;
  /// Assigns the diffuse color to vDiffuse.
  void	    SetDiffuse( GSE_Color vDiffuse);
  /// Returns the Ambient color.
  const GSE_Color & GetAmbient() const;
  /// Assigns the ambient color to vAmbient.
  void	    SetAmbient( GSE_Color vAmbient);
  /// Returns the specular color.
  const GSE_Color & GetSpecular() const;
  /// Assigns the specular color to vSpecular.
  void	    SetSpecular( GSE_Color vSpecular);
  /// Returns the emissive color.
  const GSE_Color & GetEmission() const;
  /// Assigns the emissive color.
  void	    SetEmission( GSE_Color vEmission);
  /// Returns the shininess factor.
  float	    GetShininess();
  /// Assigns the shininess factor to fShininess.
  void	    SetShininess( float fShininess);
  /// Assigns the transparency hint to value of bFlag.
  void	    SetTransparent( int bFlag );
  /// Returns true if this material has transparent components.
  int	    IsTransparent();
  /// Returns the message for the renderer.
  CacheMethod_t	GetCacheMethod();
  /// Assigns the message for the renderer.
  void		SetCacheMethod( CacheMethod_t nMethod );    
  /// Returns a reference to the cache.
  GLuint &	Cache();
  /// For printing material properties easily.
  friend std::ostream & operator<<( std::ostream &stream, GSE_Material mat );
};
/////////////////////////////////////////////////////////////////
/// A class which holds every material.
class GSE_MaterialMgr : public GSE_Container<GSE_Material>,
			public GSE_Singleton<GSE_MaterialMgr>,
			public GSE_Mapper<std::string, GSE_Material>,
			protected GSE_Configuration
{
  friend class GSE_Singleton<GSE_MaterialMgr>;
 private:
  /// Default constructor.
  GSE_MaterialMgr();
  /// Destructor.
  ~GSE_MaterialMgr();
 public:
  /// Creates material with a name.
  /// \param szMaterialName material name.
  GSE_Material * CreateMaterial(const char *szMaterialName);
  /// Creates material from a config file.
  /// \param szFilename path to config file.
  /// \param szMaterialName material name, by default is comes from file. It can be overridden here if necessary.
  GSE_Material * GetMaterialFromFile(const char *szFilename, 
				     const char *szMaterialName = NULL);
 
}; // class GSE_MaterialMgr
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
