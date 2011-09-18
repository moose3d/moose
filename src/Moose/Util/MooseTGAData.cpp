#include "MooseTGAData.h"
#include "MooseGL.h"
#include "MooseExceptions.h"
#include "MooseTGAImage.h"
#include <cstring>
////////////////////////////////////////////////////////////////////////////////
using namespace Moose::Exceptions;
using namespace Moose::Graphics;
namespace prefix = Moose::Util;
////////////////////////////////////////////////////////////////////////////////
void
prefix::CTGAData::Load( const char *szFile )
{
  ////////////////////
  CTGAImage *pImage = new CTGAImage();
  CMooseRuntimeError err("");
  switch ( pImage->Load( szFile ) )
  {
  case IMG_OK:
    break;
  case IMG_ERR_NO_FILE:
      err << "No such file '" << szFile << "'";
      delete pImage;
      throw err;
    break;
  case IMG_ERR_UNSUPPORTED:
      err << "Unsupported format in file '" << szFile << "'";
      delete pImage;
      throw err;
    break;
  case IMG_ERR_MEM_FAIL:
      err << "Out of memory while loading file '" << szFile << "'";
      delete pImage;
      throw err;
    break;
  case IMG_ERR_BAD_FORMAT:
      err << "Bad format while loading file '" << szFile << "'";
      delete pImage;
      throw err;
    break;
  default:
    err << "Something unknown happened when loading TGA image.";
    throw err;
    break;
  }

  ////////////////////
  // Check correct depth
  switch (pImage->GetBPP())
  {
  case 8:
    m_iGLInternalFormat = 1;
    m_iGLFormat = GL_LUMINANCE;
    break;
  case 16:
    m_iGLInternalFormat = 2;
    m_iGLFormat = GL_LUMINANCE_ALPHA;
    break;
  case 24:
    m_iGLInternalFormat = 3;
    m_iGLFormat = GL_RGB;
    break;
  case 32:
    m_iGLInternalFormat = 4;
    m_iGLFormat = GL_RGBA;
    break;
  default:
      delete pImage;
      err << "Not 8, 16, 24 or 32 BBP image (was " << pImage->GetBPP()
	      << "):  '" << szFile << "'";
      throw err;
      break;
  }

  SetWidth(pImage->GetWidth());
  SetHeight(pImage->GetHeight());
  // width * height * bytes per pixel
  size_t nBytesize = GetWidth()*GetHeight()*(pImage->GetBPP()/8);
  m_pData = new unsigned char[nBytesize];
  memcpy(m_pData, pImage->GetImg(), nBytesize);
  m_nDataByteSize = nBytesize;
  delete pImage;
}
////////////////////////////////////////////////////////////////////////////////
