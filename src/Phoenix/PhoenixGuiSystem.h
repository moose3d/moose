#ifndef __CGuiSystem_h__
#define __CGuiSystem_h__
/////////////////////////////////////////////////////////////////
/// 
namespace Phoenix
{
  namespace GUI
  {
    /// Forward declaration.
    struct SDL_Event;
    ////////////////////
    /// Adapter for CEGUI.
    class CGuiSystem
    {
    public:
      ////////////////////
      /// Init necessary systems.
      void Initialize( size_t nWidth, size_t nHeight );
      ////////////////////
      /// Setup data files from config file.
      void LoadResources( const char *szPath );
      ////////////////////
      ///  Injects mouse up event into GUI system.
      void InjectMouseUp( SDL_Event & e );
      ////////////////////
      ///  Injects mouse down event into GUI system.
      void InjectMouseDown( SDL_Event & e );
      void InjectMouseMotion( SDL_Event & e);
      ////////////////////
      ///  Injects key down event into GUI system.
      void InjectKeyDown( SDL_Event & e );
      ////////////////////
      ///  Injects key down event into GUI system.
      void InjectKeyUp( SDL_Event & e );
      ////////////////////
      ///       
      void Update( size_t nPassedTime );
    };  
  };
};
/////////////////////////////////////////////////////////////////
#endif

