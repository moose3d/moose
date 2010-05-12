#ifndef __PhoenixOGLConsts_h__
#define __PhoenixOGLConsts_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixAPI.h"
#if defined(PHOENIX_APPLE_IPHONE)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GL/GLee.h>
#include <OpenGL/gl.h>
#endif
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /// Which model data is left unrendered.
    enum MODEL_EXCLUDE_DATA
    {
      M_TEXTURE_DATA = 1,
      M_SHADER_DATA = 1 << 1,
      M_NORMAL_DATA = 1 << 2,
      M_INDEX_DATA = 1 << 3
    };
    enum CLIENT_STATE_TYPE
    {
      CLIENT_STATE_VERTEX_ARRAY = 0,
      CLIENT_STATE_COLOR_ARRAY = 1,
      CLIENT_STATE_NORMAL_ARRAY,
      CLIENT_STATE_TEX0_ARRAY,
      CLIENT_STATE_TEX1_ARRAY,
      CLIENT_STATE_TEX2_ARRAY,
      CLIENT_STATE_TEX3_ARRAY,
      CLIENT_STATE_TEX4_ARRAY,
      CLIENT_STATE_TEX5_ARRAY,
      CLIENT_STATE_TEX6_ARRAY,
      CLIENT_STATE_TEX7_ARRAY
    };
    enum STATE_TYPE
    {
#if defined(PHOENIX_APPLE_IPHONE)
      STATE_LIGHTING = 0,
      STATE_DEPTH_TEST = GL_DEPTH_TEST,
      STATE_ALPHA_TEST = 0,
      STATE_BLENDING   = GL_BLEND,
      STATE_DEPTH_WRITE = GL_TRUE,
      STATE_FACECULLING = GL_CULL_FACE,
      STATE_MULTISAMPLE = 0
#else
        STATE_LIGHTING = GL_LIGHTING,
        STATE_DEPTH_TEST = GL_DEPTH_TEST,
        STATE_ALPHA_TEST = GL_ALPHA_TEST,
        STATE_BLENDING   = GL_BLEND,
        STATE_DEPTH_WRITE = GL_TRUE,
        STATE_FACECULLING = GL_CULL_FACE,
        STATE_MULTISAMPLE = GL_MULTISAMPLE
#endif
    };
    enum ALPHA_TEST_TYPE
    {
      ALPHA_ALWAYS           = GL_ALWAYS,
      ALPHA_NEVER	     = GL_NEVER,
      ALPHA_LESS	     = GL_LESS,
      ALPHA_EQUAL	     = GL_EQUAL,
      ALPHA_LESS_OR_EQUAL    = GL_LEQUAL,
      ALPHA_GREATER	     = GL_GREATER,
      ALPHA_NOT_EQUAL	     = GL_NOTEQUAL,
      ALPHA_GREATER_OR_EQUAL = GL_GEQUAL
    };
    enum BLEND_SRC_TYPE
    {
      BLEND_SRC_ZERO                = GL_ZERO,
      BLEND_SRC_ONE                 = GL_ONE,
      BLEND_SRC_DST_COLOR           = GL_DST_COLOR,
      BLEND_SRC_ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
      BLEND_SRC_SRC_ALPHA           = GL_SRC_ALPHA,
      BLEND_SRC_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
      BLEND_SRC_DST_ALPHA           = GL_DST_ALPHA,
      BLEND_SRC_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
      BLEND_SRC_SRC_ALPHA_SATURATE  = GL_SRC_ALPHA_SATURATE
    };
    enum BLEND_DST_TYPE
    {
      BLEND_DST_ZERO		    = GL_ZERO,
      BLEND_DST_ONE		    = GL_ONE,
      BLEND_DST_SRC_COLOR	    = GL_SRC_COLOR,
      BLEND_DST_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
      BLEND_DST_SRC_ALPHA	    = GL_SRC_ALPHA,
      BLEND_DST_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
      BLEND_DST_DST_ALPHA	    = GL_DST_ALPHA,
      BLEND_DST_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA
    };
    ////////////////////
    /// Cache access performance _hint_, does not guarantee anything.
    /// Static  ( define once, use many times.)
    /// Dynamic ( define multiple times, use multiple times)
    /// Stream  ( define and use once per frame).
    enum CACHE_ACCESS_TYPE
    {
#if defined(PHOENIX_APPLE_IPHONE)
        
        CACHE_STATIC_DRAW,
      CACHE_STATIC_READ,
      CACHE_STATIC_COPY,
      CACHE_DYNAMIC_DRAW,
      CACHE_DYNAMIC_READ,
      CACHE_DYNAMIC_COPY,
      CACHE_STREAM_DRAW,
      CACHE_STREAM_READ,
      CACHE_STREAM_COPY
#else
        CACHE_STATIC_DRAW  = GL_STATIC_DRAW_ARB,
        CACHE_STATIC_READ  = GL_STATIC_READ_ARB,
        CACHE_STATIC_COPY  = GL_STATIC_COPY_ARB,
        CACHE_DYNAMIC_DRAW = GL_DYNAMIC_DRAW_ARB,
        CACHE_DYNAMIC_READ = GL_DYNAMIC_READ_ARB,
        CACHE_DYNAMIC_COPY = GL_DYNAMIC_COPY_ARB,
        CACHE_STREAM_DRAW  = GL_STREAM_DRAW_ARB,
        CACHE_STREAM_READ  = GL_STREAM_READ_ARB,
        CACHE_STREAM_COPY  = GL_STREAM_COPY_ARB
#endif        
    };
    /////////////////////////////////////////////////////////////////
    /// BufferType
    enum BUFFER_TYPE
    {
      COLOR_BUFFER = 0,
      DEPTH_BUFFER = 1
    };
    /////////////////////////////////////////////////////////////////
    /// Frame buffer flags
    enum FBO_BUFFER_FLAG
    {
      FBO_COLOR_BUFFER = 1 << 0,
      FBO_DEPTH_BUFFER = 1 << 1,
      FBO_STENCIL_BUFFER = 1 << 2
    };
    ////////////////////
    /// Texture formats
    enum TEXTURE_FORMAT
    {
      TEX_FORMAT_RGBA = GL_RGBA,
      TEX_FORMAT_RGB  = GL_RGB,
      TEX_FORMAT_DEPTH_COMPONENT = GL_DEPTH_COMPONENT
    };
    enum ELEMENT_TYPE
        {
          ELEMENT_TYPE_VERTEX_3F,
          ELEMENT_TYPE_NORMAL_3F,
          ELEMENT_TYPE_COLOR_4UB,
          ELEMENT_TYPE_COLOR_3F,
          ELEMENT_TYPE_COLOR_4F,
          ELEMENT_TYPE_TEX_2F,
          ELEMENT_TYPE_TEX_3F,
          ELEMENT_TYPE_TEX_4F,
          ELEMENT_TYPE_UNIFORM_1F,/* this is critical value, must be first of uniforms */
          ELEMENT_TYPE_UNIFORM_2F,
          ELEMENT_TYPE_UNIFORM_3F,
          ELEMENT_TYPE_UNIFORM_4F,
          ELEMENT_TYPE_UNIFORM_1I,
          ELEMENT_TYPE_UNIFORM_2I,
          ELEMENT_TYPE_UNIFORM_3I,
          ELEMENT_TYPE_UNIFORM_4I,
          ELEMENT_TYPE_UNIFORM_2X2F,
          ELEMENT_TYPE_UNIFORM_3X3F,
          ELEMENT_TYPE_UNIFORM_4X4F,/* this is critical value, must be last of uniforms */
          ELEMENT_TYPE_ATTRIB_1F, /* this is critical value, must be first of attribs */
          ELEMENT_TYPE_ATTRIB_2F,
          ELEMENT_TYPE_ATTRIB_3F,
          ELEMENT_TYPE_ATTRIB_4F,
          ELEMENT_TYPE_ATTRIB_1I,
          ELEMENT_TYPE_ATTRIB_2I,
          ELEMENT_TYPE_ATTRIB_3I,
          ELEMENT_TYPE_ATTRIB_4I,
          ELEMENT_TYPE_ATTRIB_1UB,
          ELEMENT_TYPE_ATTRIB_2UB,
          ELEMENT_TYPE_ATTRIB_3UB,
          ELEMENT_TYPE_ATTRIB_4UB, /* this is critical value, must be last of attribs */
          // following types are for combined data
          ELEMENT_TYPE_V3F_N3F_T2F
        };
  } // namespace Graphics
} // namespace Phoenix
#endif
