#include "PhoenixGlobals.h"
#include "PhoenixDDSHeader.h"
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CDDSImage::CDDSImage() : 
  m_nWidth(0), m_nHeight(0), m_iComponents(0), m_Format(DDS_FORMAT_DXT1), 
  m_iNumMipMaps(0), m_pPixels(NULL)
{
  
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CDDSImage::~CDDSImage()
{
  
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Graphics::CDDSImage::Load( const char *szFilename )
{
  int iRetval		= 0;
  int iMipmapFactor	= 0;
  int iBufferSize	= 0;
  Destroy();
  using namespace std;
  // for identifying dds files.
  char fileHeader[4];

  ifstream file;
  file.open(szFilename,ios::binary);
  
  if ( file == NULL )
  {
    iRetval = IMG_ERR_NO_FILE;
    goto exit;
  }
  file.read( fileHeader, 4);
  if ( strncmp( fileHeader, "DDS ", 4) != 0)
  {
    iRetval = IMG_ERR_BAD_FORMAT;
    goto cleanup;
  }
  // create and read surface desciptor
  DDSURFACEDESC2 ddsd;
  file.read( (char *)&ddsd, sizeof(ddsd));
  ////////////////////
  // Determine file type.
  // compression ratios are 8:1 for DXT1, 4:1 for the rest.
  switch( ddsd.ddpfPixelFormat.dwFourCC )
  {
  case FOURCC_DXT1:
    m_Format = DDS_FORMAT_DXT1;
    iMipmapFactor = 2;
    break;
  case FOURCC_DXT3:
    m_Format = DDS_FORMAT_DXT3;
    iMipmapFactor = 4;
    break;
  case FOURCC_DXT5:
    m_Format = DDS_FORMAT_DXT5;
    iMipmapFactor = 4;
    break;
  case FOURCC_DXT4:
    m_Format = DDS_FORMAT_DXT4;
    iMipmapFactor = 4;
    break;
  case FOURCC_DXT2:
    m_Format = DDS_FORMAT_DXT2;
    iMipmapFactor = 4;
    break;
  }

  ////////////////////
  /// sanity check
  if ( ddsd.dwLinearSize == 0 )
  {
    iRetval = IMG_ERR_BAD_FORMAT;
    goto cleanup;
  }

  ////////////////////
  /// Mipmap factor
  if( ddsd.dwMipMapCount > 1 )
    iBufferSize = ddsd.dwLinearSize * iMipmapFactor;
  else
    iBufferSize = ddsd.dwLinearSize;
  ////////////////////
  /// allocate memory
  m_pPixels = new unsigned char[iBufferSize];
  if ( m_pPixels == NULL )
  {
    iRetval = IMG_ERR_MEM_FAIL;
    goto cleanup;
  }
  // read actual image data
  file.read( (char *)m_pPixels, iBufferSize);
  file.close();
  // set attributes 
  m_nWidth	= ddsd.dwWidth;
  m_nHeight	= ddsd.dwHeight;
  m_iNumMipMaps = ddsd.dwMipMapCount;
  
  if ( m_Format == DDS_FORMAT_DXT1 ) { m_iComponents = 3; }
  else				     { m_iComponents = 4; }  
  ////////////////////
  // we're good to go!
  goto exit;

  // release handles & stuff
 cleanup:
  file.close();
  if ( GetPixelData() != NULL )  
  { 
    delete m_pPixels; 
    m_pPixels = NULL;
  }
 exit:
  return iRetval;
}
/////////////////////////////////////////////////////////////////
unsigned char *
Phoenix::Graphics::CDDSImage::GetPixelData()
{
  return m_pPixels;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CDDSImage::Destroy()
{
  ////////////////////
  if ( GetPixelData() != NULL )
  {
    delete GetPixelData();
    m_pPixels = NULL;
  }
  ////////////////////
  m_nWidth	= 0;
  m_nHeight	= 0;
  m_iComponents = 0;
  m_Format	= DDS_FORMAT_DXT1;
  m_iNumMipMaps = 0;
  m_pPixels	= NULL;
  ////////////////////
  
}
/////////////////////////////////////////////////////////////////
