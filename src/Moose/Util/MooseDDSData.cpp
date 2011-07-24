#include <MooseDDSData.h>
#include <MooseGL.h>
#include <MooseExceptions.h>
#include <MooseDDSImage.h>
#include <MooseGlobals.h>
#include <cstring>
////////////////////////////////////////////////////////////////////////////////
namespace prefix=Moose::Util;
using namespace Moose::Exceptions;
using namespace Moose::Graphics;

////////////////////////////////////////////////////////////////////////////////
void
prefix::CDDSData::Load( const char *szFile )
{
    CDDSImage *pImage = new CDDSImage();
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
    }

    ////////////////////
    m_iBlockSize = 16;

    ////////////////////
    // Check correct format:
    switch (pImage->GetFormat())
    {
    case DDS_FORMAT_DXT1:
        m_iBlockSize = 8;
        m_iGLFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case DDS_FORMAT_DXT3:
        m_iGLFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        m_iBlockSize = 16;
        break;
    case DDS_FORMAT_DXT5:
        m_iGLFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        m_iBlockSize = 16;
        break;
    default:

        err << "Not in DXT1, DXT3, DXT5 format (was "
                  << (pImage->GetFormat() == DDS_FORMAT_DXT2  ? "DXT2" : "DXT4")
            << "):  '" << szFile << "'" ;
        delete pImage;

        throw err;
    }
    m_nDataByteSize = pImage->GetByteSize();
    m_pData = new unsigned char[m_nDataByteSize];
    memcpy(m_pData, pImage->GetPixelData(), m_nDataByteSize);
    delete pImage;
    
}
////////////////////////////////////////////////////////////////////////////////