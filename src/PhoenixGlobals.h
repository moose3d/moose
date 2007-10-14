#ifndef __PhoenixGlobals_h__
#define __PhoenixGlobals_h__
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Globals
  {
    /// Maximum number of characters in a font.
    const unsigned int MAX_FONT_CHARACTERS = 256;
    const unsigned int NODE_NAME_MAX_SIZE =  128;
    /////////////////////////////////////////////////////////////////
    // handy macro for determining and storing new min/max.
#define STORE_MAX_MIN(NEWVAL, OLDMAX, OLDMIN) \
{\
	if ( NEWVAL > OLDMAX) \
        {\
		OLDMAX = NEWVAL;\
	}\
        else if ( NEWVAL < OLDMIN )\
        {\
		OLDMIN = NEWVAL;\
	}\
}
  }
}
#define PHOENIX_DEPRECATED __attribute__((deprecated))
/////////////////////////////////////////////////////////////////
#endif
