/////////////////////////////////////////////////////////////////
#ifndef __PhoenixSDLScreen_h__
#define __PhoenixSDLScreen_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;

namespace Phoenix
{
  namespace Window
  {
    /////////////////////////////////////////////////////////////////
    /// The Screen Singleton. Each application can have only one screen where
    /// drawing occurs. Hence, the Singleton nature.
    class PHOENIX_API CSDLScreenParams 
    {
    public:
      /// The number of red bits  
      int m_iRedSize;
      /// The number of green bits
      int m_iGreenSize;
      /// The number of blue bits
      int m_iBlueSize;
      /// The double buffer flag
      int m_bDoubleBuffer;
      /// The bits for the depth buffer
      int m_iDepthBufferSize;
      /// The width of the screen
      int m_iWidth;
      //7 The height of the screen
      int m_iHeight;
      /// The depth of the screen
      int m_iScreenDepth;
      /// The video mode flags for SDL Screen
      int m_iVideoModeFlags;
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
    class PHOENIX_API CSDLScreen : public CSingleton<CSDLScreen>
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
}; // namespace Phoenix
#endif
/////////////////////////////////////////////////////////////////
