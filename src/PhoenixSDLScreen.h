/////////////////////////////////////////////////////////////////
#ifndef __CScreen_h__
#define __CScreen_h__
#include <SDL.h> 

//
// The Screen Singleton. Each application can have only one screen where
// drawing occurs. Hence, the Singleton nature.
//

class COglScreenParams 
{
 public:
  // The number of red bits  
  int m_iRedSize;
  // The number of green bits
  int m_iGreenSize;
  // The number of blue bits
  int m_iBlueSize;
  // The double buffer flag
  int m_bDoubleBuffer;
  // The bits for the depth buffer
  int m_iDepthBufferSize;
  // The width of the screen
  int m_iWidth;
  // The height of the screen
  int m_iHeight;
  // The depth of the screen
  int m_iScreenDepth;
  // The video mode flags for SDL Screen
  int m_iVideoModeFlags;

  // The default constructor, sets default values for members.
  COglScreenParams()
  {
    m_iRedSize = 8;
    m_iGreenSize = 8;
    m_iBlueSize = 8;
    m_bDoubleBuffer = 0;
    m_iDepthBufferSize = 16;
    m_iScreenDepth = 24;
    m_iWidth = 640;
    m_iHeight = 480;
    m_iVideoModeFlags = SDL_OPENGL;
  }

  friend std::ostream& operator<<( std::ostream &stream, 
				   const COglScreenParams & oglSP);
};
/////////////////////////////////////////////////////////////////
class CScreen 
{

 private:
  // ------------------------------------------------
  static CScreen *m_pScreen;
  // ------------------------------------------------

  // The constructor 
  CScreen()
  {

  }
  
 public:  
  // ------------------------------------------------
  COglScreenParams m_OglScreenParams;
  // ------------------------------------------------
  static CScreen *GetScreen(COglScreenParams &rOglScreenParams);
  static CScreen *GetScreen();
  // The deconstructor
  ~CScreen();


  

  
};

#endif
