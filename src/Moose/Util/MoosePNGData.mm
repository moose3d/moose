#include <MoosePNGData.h>
#include <MooseExceptions.h>
#include <MooseGL.h>
#if defined(MOOSE_APPLE_IPHONE)
#include <OpenGLES/EAGL.h>
#include <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>
#import <CoreGraphics/CoreGraphics.h>
#endif
////////////////////////////////////////////////////////////////////////////////
namespace prefix=Moose::Util;
using namespace Moose::Exceptions;
using namespace Moose::Graphics;
#define CLEANUP() { CGContextRelease(cgContext); free(data); CGColorSpaceRelease(colorSpace); }
////////////////////////////////////////////////////////////////////////////////
prefix::CPNGData::Load( const char *szFile )
{
    m_szFilename = szFile;
    CMooseRuntimeError err("");
    /* APPLE IPHONE CODE */

    // Right, boys and girls, this is Objective-C++!
    UIImage *       image = nil;
    CGImageRef      cgImage;
    //GLubyte *       data = nil;
    CGContextRef    cgContext;
    CGColorSpaceRef colorSpace;

    GLenum err;
  
    NSString *path =  [ [NSString alloc] initWithUTF8String:szFile ];
  
    image = [UIImage imageWithContentsOfFile:path];
  
  
    if (image == nil)
    {
        err << "Failed to load '" << szFile << "'";
        CLEANUP();
        throw err;
    }
  
    cgImage = [image CGImage];
    SetWidth(CGImageGetWidth(cgImage));
    SetHeight(CGImageGetHeight(cgImage));
    colorSpace = CGColorSpaceCreateDeviceRGB();
  
    // Malloc may be used instead of calloc if your cg image has dimensions equal to the dimensions of the cg bitmap context
    m_nDataByteSize = GetWidth() * GetHeight() * 4;
    m_pData = new unsigned char[m_nDataByteSize];

    cgContext = CGBitmapContextCreate(m_pData, GetWidth(), GetHeight(), 8, GetWidth() * 4, colorSpace, kCGImageAlphaPremultipliedLast);
    if (cgContext == NULL) 
    {
        err << "Could not create context"
        CLEANUP();
    }
    // Set the blend mode to copy. We don't care about the previous contents.
    CGContextSetBlendMode(cgContext, kCGBlendModeCopy);
    CGContextDrawImage(cgContext, CGRectMake(0.0f, 0.0f, GetWidth(), GetHeight()), cgImage);
    ////////////////////
    int    iGLInternalFormat = 3;
    GLenum iGLformat = GL_RGB;
    ////////////////////
    // Check correct depth
    switch ( CGImageGetBitsPerPixel(cgImage) )
    {
    case 8:
        m_iGLInternalFormat = 1;
        m_iGLformat = GL_LUMINANCE;
        break;
    case 16:
        m_iGLInternalFormat = 2;
        m_iGLformat = GL_LUMINANCE_ALPHA;
        break;
    case 24:
        m_iGLInternalFormat = 3;
        m_iGLformat = GL_RGB;
        break;
    case 32:
        m_iGLInternalFormat = 4;
        m_iGLformat = GL_RGBA;
        break;
    default:
    
        err << "Not 8, 16, 24 or 32 BBP image (was " 
            <<  CGImageGetBitsPerPixel(cgImage)
            << "):  '" << szFile << "'" << std::endl;
        CLEANUP();
        throw err;
    }
    
}
////////////////////////////////////////////////////////////////////////////////
