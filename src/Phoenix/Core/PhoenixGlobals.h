#ifndef __PhoenixGlobals_h__
#define __PhoenixGlobals_h__
#include <cstdlib>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Globals
  {
    /// Maximum number of characters in a font.
    const unsigned int MAX_FONT_CHARACTERS = 256;
    const unsigned int NODE_NAME_MAX_SIZE =  128;
    /// magic number for hash table size and other stuff, too.
    const size_t PHOENIX_MAGIC_NUMBER = 33;
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
      enum Tags
      {
          kCollider = 1 << 0,
          kUserTag  = 1 << 1
      };
  }
}
#define PHOENIX_DEPRECATED __attribute__((deprecated))
/////////////////////////////////////////////////////////////////
#define IMG_OK              0x1
#define IMG_ERR_NO_FILE     0x2
#define IMG_ERR_MEM_FAIL    0x4
#define IMG_ERR_BAD_FORMAT  0x8
#define IMG_ERR_UNSUPPORTED 0x40
/////////////////////////////////////////////////////////////////
#define TEXTURE_HANDLE_COUNT 8
#endif
