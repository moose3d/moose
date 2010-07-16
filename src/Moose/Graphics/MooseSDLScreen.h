/////////////////////////////////////////////////////////////////
#ifndef __MooseSDLScreen_h__
#define __MooseSDLScreen_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
using namespace Moose::Core;

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

      ////////////////////
      /// The default constructor, sets default values for members.
      CSDLScreenParams();
      ////////////////////
      /// Prints out screen parameters to output stream.
      /// \param stream output stream
      /// \param oglSP SDL screne object.
      /// \retruns reference to output stream.
      friend std::ostream& operator<<( std::ostream &stream,
				       const CSDLScreenParams & oglSP);
    };
    /////////////////////////////////////////////////////////////////
    class MOOSE_API CSDLScreen : public CSingleton<CSDLScreen>
    {
      friend class CSingleton<CSDLScreen>;
    private:
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
/////////////////////////////////////////////////////////////////
