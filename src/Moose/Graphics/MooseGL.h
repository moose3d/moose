#include "MooseAPI.h"
#if defined(MOOSE_APPLE_IPHONE)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined(__APPLE__)
#include <GL/glew.h>
//#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#define GL_GLEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#endif
