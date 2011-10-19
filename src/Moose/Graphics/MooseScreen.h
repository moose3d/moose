#ifndef __MooseScreen_h__
#define __MooseScreen_h__
#include "MooseCore.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Window
  {

    class MOOSE_API CScreenParams
    {
    public:
      // These need to remain as INTs otherwise scripting fails.
      static int m_iRedSize;///< The number of red bits
      static int m_iGreenSize;///< The number of green bits
      static int m_iBlueSize;///< The number of blue bits
      static int m_bDoubleBuffer;///< The double buffer flag
      static int m_iDepthBufferSize;///< The bits for the depth buffer
      static int m_iWidth;///< The width of the screen
      static int m_iHeight;///< The height of the screen
      static int m_iScreenDepth;///< The depth of the screen
      static int m_iVideoModeFlags; ///< The video mode flags for SDL Screen
      static int m_iMultiSampleBuffers; ///< Number of multisample buffers
      static int m_iMultiSampleSamples;  ///< Number of samples.

      static int m_iGLMajorVersion;      ///< Requested OGL major version, defaults to 3. Applies only to SDL-1.3+
      static int m_iGLMinorVersion;      ///< Requested OGL minor version, defaults to 2. Applies only to SDL-1.3+
      static int m_bVerticalSync;        ///< Vertical sync on / off.
      static std::string m_ScreenName;        ///< Screen name for window
      ////////////////////
      /// The default constructor, sets default values for members.
      CScreenParams();
      ////////////////////
      /// Prints out screen parameters to output stream.
      /// \param stream output stream
      /// \param oglSP SDL screne object.
      /// \retruns reference to output stream.
#ifndef SWIG
      friend std::ostream& operator<<( std::ostream &stream,
				       const CScreenParams & oglSP);
#endif
      std::string ToString() const;
    };
#ifndef SWIG
    std::ostream& operator<<( std::ostream &stream, const CScreenParams & oglSP);
#endif
    /////////////////////////////////////////////////////////////////
    /// The Screen Singleton. Each application can have only one screen where
    /// drawing occurs. Hence, the Singleton nature.
    /////////////////////////////////////////////////////////////////
    class MOOSE_API CScreen : public CScreenParams,
                              public Moose::Core::CSingleton<Moose::Window::CScreen>
    {
      friend class Moose::Core::CSingleton<Moose::Window::CScreen>;
    protected:
      CScreen();
      virtual ~CScreen();
    public:
      ////////////////////
      /// Swaps buffers if double buffering is used.
      void SwapBuffers();
    };
  }
}
#endif
    
