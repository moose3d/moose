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
#ifndef __GSE_Texture_h__
#define __GSE_Texture_h__
/////////////////////////////////////////////////////////////////
#include "GSE_globals.h"
/////////////////////////////////////////////////////////////////
/// \brief The interface template for texture objects.
class GSE_Texture 
{
 protected:
  /// \brief A hint for renderer which tells is this texture transparent.
  int m_bTransparent;
  /// Type for this texture.
  TextureType::Type m_nType;
  /// Texture activation flag for renderer.
  int m_bActive;
 public:
  /// \brief The constructor.
  GSE_Texture() : m_bActive(0)
  {
    SetTransparentHint( 0 );
  }
  /// \brief The destructor.
  virtual ~GSE_Texture()
  {
  }
  /// \brief Sets the transparent hint value.
  /// \param bFlag boolean value for transparency hint. 
  void SetTransparentHint( int bFlag )
  {
    m_bTransparent = bFlag;
  }
  /// \brief Returns the transparency hint flag value.
  /// \return char the transparency hint value as boolean.
  int IsTransparent()
  {
    return m_bTransparent;
  }
  /////////////////////////////////////////////////////////////////
  /// Assigns texture type.
  /// \param nType the texture type.
  void SetType( TextureType::Type nType )
  {
    m_nType = nType;
  }
  /////////////////////////////////////////////////////////////////
  /// Returns texture type.
  /// \returns the texture type.
  TextureType::Type GetType()
  {
    return m_nType;
  }
  ////////////////////
  /// Sets texture activation flag value.
  /// \param bFlag boolean.
  inline void SetActive( int bFlag )
  {
    m_bActive = bFlag;
  }
  ////////////////////
  /// Returns texture activation state.
  /// \return boolean 1 if active, 0 otherwise.
  inline int IsActive() const 
  {
    return m_bActive;
  }
};
/////////////////////////////////////////////////////////////////
/// \brief Interface for Texturemanagers.
class GSE_TextureMgrInterface
{
 public:
  /// The destructor.
  virtual ~GSE_TextureMgrInterface(){}
  
  /// \brief Creates an empty GSE_Texture.
  /// \param iWidth the width of the texture
  /// \param iHeight the height of the texture.
  /// \param bUseAlpha should the alpha channel be included in the texture.
  /// \param iType The type of the texture.
  /// \param iQuality The quality of the texture;
  /// PERFORMANCE uses less memory (looks uglier) and consumes less bandwidth.
  /// QUALITY uses more memory (looks better) and more bandwidth.
  /// \return A pointer to GSE_Texture object which was created or NULL on error.
  virtual GSE_Texture *CreateEmptyTexture( int iWidth, int iHeight, 
					   char bUseAlpha, 
					   TextureType::Type iType,
					   TextureQuality::Type iQuality) = 0;
  
  ///  \brief Creates a texture from image file.
  /// \param pFilename A zero-terminated char array representing the filename.
  /// \param bUseAlpha Should alpha channel be included if image file contains it.
  /// \param iType The type of the texture.
  /// \param iQuality The quality of the texture;
  /// PERFORMANCE uses less memory (looks uglier) and consumes less bandwidth.
  /// QUALITY uses more memory (looks better) and more bandwidth.
  /// \return A pointer to GSE_Texture object which was created or NULL on error.
  virtual GSE_Texture *CreateTextureFromImage( const char *pFilename, 
					       char bUseAlpha, 
					       TextureType::Type iType,
					       TextureQuality::Type iQuality ) = 0;
					       
  /// \brief Creates a texture from rendered image.
  /// Comes in handy when rendering reflections etc.
  /// \param iStartX The starting x-coordinate in window coordinates.
  /// \param iStartY The starting y-coordinate in window coordinates.
  /// \param nWidth the width of the texture
  /// \param nHeight the height of the texture.
  /// \param bUseAlpha should the alpha channel be included in the texture.
  /// \param iType The type of the texture.
  /// \param iQuality The quality of the texture;
  /// PERFORMANCE uses less memory (looks uglier) and consumes less bandwidth.
  /// QUALITY uses more memory (looks better) and more bandwidth.
  /// \return A pointer to GSE_Texture object which was created or NULL on error.
  virtual GSE_Texture *CreateTextureFromScreen( int iStartX, 
						int iStartY,
						unsigned int nWidth,
						unsigned int nHeight,
						char bUseAlpha,
						TextureType::Type iType,
						TextureQuality::Type iQuality ) = 0;
};
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
