/////////////////////////////////////////////////////////////////
#ifndef __MooseSDLScreen_h__
#define __MooseSDLScreen_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseAPI.h"
#include <string>
/////////////////////////////////////////////////////////////////
#if !defined(MOOSE_APPLE_IPHONE)
namespace Moose
{
  namespace Window
  {
    /////////////////////////////////////////////////////////////////
    /// The Screen Singleton. Each application can have only one screen where
    /// drawing occurs. Hence, the Singleton nature.
    class MOOSE_API CSDLScreenParams
    {
    public:
    	// These need to remain as INTs otherwise scripting fails.
      int m_iRedSize;///< The number of red bits
      int m_iGreenSize;///< The number of green bits
      int m_iBlueSize;///< The number of blue bits
      int m_bDoubleBuffer;///< The double buffer flag
      int m_iDepthBufferSize;///< The bits for the depth buffer
      int m_iWidth;///< The width of the screen
      int m_iHeight;///< The height of the screen
      int m_iScreenDepth;///< The depth of the screen
      int m_iVideoModeFlags; ///< The video mode flags for SDL Screen
      int m_iMultiSampleBuffers; ///< Number of multisample buffers
      int m_iMultiSampleSamples;  ///< Number of samples.

      int m_iGLMajorVersion;      ///< Requested OGL major version, defaults to 3. Applies only to SDL-1.3+
      int m_iGLMinorVersion;      ///< Requested OGL minor version, defaults to 2. Applies only to SDL-1.3+
      int m_bVerticalSync;        ///< Vertical sync on / off.
      std::string m_ScreenName;        ///< Screen name for window
      ////////////////////
      /// The default constructor, sets default values for members.
      CSDLScreenParams();
      ////////////////////
      /// Prints out screen parameters to output stream.
      /// \param stream output stream
      /// \param oglSP SDL screne object.
      /// \retruns reference to output stream.
#ifndef SWIG
      friend std::ostream& operator<<( std::ostream &stream,
				       const CSDLScreenParams & oglSP);
#endif
      std::string ToString() const;
    };
#ifndef SWIG
    std::ostream& operator<<( std::ostream &stream, const CSDLScreenParams & oglSP);
#endif
    /////////////////////////////////////////////////////////////////
    class MOOSE_API CSDLScreen : public Moose::Core::CSingleton<Moose::Window::CSDLScreen>
    {
      friend class Moose::Core::CSingleton<Moose::Window::CSDLScreen>;
    private:
#ifdef MOOSE_USE_OPENGL3
      SDL_Window *  m_pMainWindow; ///< Our window.
      SDL_GLContext m_pGLContext;  ///< GL context for 3.0+
#endif
      ////////////////////
      /// The constructor
      CSDLScreen();
      ////////////////////
      /// The deconstructor
      ~CSDLScreen();
    public:
      static CSDLScreenParams m_SDLScreenParams;
      ////////////////////
      /// Swaps buffers if double buffering is used.
      void SwapBuffers();
    };
    /////////////////////////////////////////////////////////////////
  }; // namespace Window
}; // namespace Moose
#endif
#endif
/////////////////////////////////////////////////////////////////
