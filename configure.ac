AC_PREREQ(2.61)
AC_INIT(src/Phoenix/Phoenix.h)

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AC_CONFIG_HEADER(src/Phoenix/config.h)
AM_INIT_AUTOMAKE(Phoenix, 0.99, entity@iki.fi)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_LANG(C++)


# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([memory.h stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT32_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_PROG_INSTALL
AC_DISABLE_SHARED
AC_PROG_LIBTOOL


# Checks for library functions.
AC_CHECK_FUNCS([floor gettimeofday memset pow sqrt strdup])

AC_ARG_ENABLE (debug, "Should debugging flags be used", [AM_CXXFLAGS="$AM_CXXFLAGS -g -Wall"], [AM_CXXFLAGS="$AM_CXXFLAGS -Wall"])
#AM_CXXFLAGS="$AM_CXXFLAGS -g -Wall"

# adding the static library 'libtristripper.a'

# check for CEGUI
PKG_CHECK_MODULES(CEGUI, CEGUI >= 0.6.0 CEGUI-OPENGL >= 0.6.0)
AM_CXXFLAGS="$AM_CXXFLAGS $CEGUI_CFLAGS"
LIBS="$LIBS $CEGUI_LIBS"

PKG_CHECK_MODULES(LIBCONFIG, libconfig++ >= 1.2.1)
AM_CXXFLAGS="$AM_CXXFLAGS $LIBCONFIG_CFLAGS"
LIBS="$LIBS $LIBCONFIG_LIBS"


PKG_CHECK_MODULES(VORBIS, vorbis >= 1.2.0)
AM_CXXFLAGS="$AM_CXXFLAGS $VORBIS_CFLAGS"
LIBS="$LIBS $VORBIS_LIBS"

PKG_CHECK_MODULES(OGG, ogg >= 1.1.2)
AM_CXXFLAGS="$AM_CXXFLAGS $OGG_CFLAGS"
LIBS="$LIBS $OGG_LIBS"

PKG_CHECK_MODULES(VORBISFILE, vorbisfile >= 1.2.0)
AM_CXXFLAGS="$AM_CXXFLAGS $VORBISFILE_CFLAGS"
LIBS="$LIBS $VORBISFILE_LIBS"

PKG_CHECK_MODULES(OPENAL, openal >= 0.0.8)
AM_CXXFLAGS="$AM_CXXFLAGS $OPENAL_CFLAGS"
LIBS="$LIBS $OPENAL_LIBS"

PKG_CHECK_MODULES(SDL, sdl >= 1.2.0)
AM_CXXFLAGS="$AM_CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

PKG_CHECK_MODULES(FREETYPE2, freetype2 > 2.3.5 )
AM_CXXFLAGS="$AM_CXXFLAGS $FREETYPE2_CFLAGS"
LIBS="$LIBS $FREETYPE2_LIBS"

# introduce the optional configure parameter for the path of libXXX.a
AC_ARG_WITH(tristripper,
	AC_HELP_STRING(
	    [--with-tristripper=prefix],
	    [try this for the tristripper-library prefix install directory]
	  ),
	  TRISTRIPPER_PATHSET=1,
	  TRISTRIPPER_PATHSET=0
)

# if optional parameter used, extend path flags for compliler and linker
if test  $TRISTRIPPER_PATHSET = 1 ; then
		AM_CXXFLAGS="$AM_CXXFLAGS -I$with_tristripper/include"
		CFLAGS="$CFLAGS -I$with_tristripper/include"
		LDFLAGS="$LDFLAGS -L$with_tristripper/lib"
		LIBS="-ltristripper $LIBS"
fi

# output of action
AC_MSG_CHECKING([for the tristripper header])

# try to compile a file that includes a header of the library XXX
AC_COMPILE_IFELSE(
     [ #include <tri_stripper.h> ],
     [ 
       AC_MSG_RESULT(yes)
       TRISTRIPPER_NOTFOUND=0;
     ],
     [
       AC_MSG_RESULT(no)
       TRISTRIPPER_NOTFOUND=1;
     ]
)

# check if library XXX was found and abort otherwise

# help variable if more than one library to check
DEPENDENCYNOTFOUND=0;

# error output if tristripper-headers not found
if test $TRISTRIPPER_NOTFOUND = 1; then
#	AC_MSG_NOTICE()
#	AC_MSG_NOTICE(the tristripper library is required.)
#	AC_MSG_NOTICE(it can be obtained from http://users.pandora.be/tfautre/softdev/tristripper/ )
#	AC_MSG_NOTICE()
# 	if test $TRISTRIPPER_PATHSET = 1; then
#		AC_MSG_NOTICE(cant find tristripper in given path '$with_tristripper'.)
# 	else
#		AC_MSG_NOTICE(if tristripper is installed elsewhere use --with-tristripper=prefix.)
# 	fi
 	#DEPENDENCYNOTFOUND=1;
	AC_DEFINE(TRISTRIPPER, 0, is tristripper library available. )
else
	AC_DEFINE(TRISTRIPPER, 1, is tristripper library available. )

fi

dnl **************************
dnl check OpenGL
dnl **************************
AC_CHECK_HEADER( [GL/gl.h], [], 
		 [echo "You need OpenGL!";
 		 exit -1;] ) 
AC_CHECK_LIB( [GL], [glVertex3f], [ LIBS="$LIBS -lGL" ], [
 		 echo "You need OpenGL!";
 		 exit -1;])
AC_CHECK_HEADER( [GL/glu.h], [], 
		 [echo "You need GLU!";
 		 exit -1;] ) 
AC_CHECK_LIB( [GLU], [gluProject], [ LIBS="$LIBS -lGLU" ], [
 		 AC_MSG_ERROR(You need GLU!);
 		 exit -1;])


dnl **************************
dnl check GLee
dnl **************************

AC_CHECK_HEADER(GL/GLee.h, [ LIBS="$LIBS -lGLee" ], 
			   [AC_MSG_ERROR(GLee is missing!);])

# error ABORT if one of the libraries was not found
if test $DEPENDENCYNOTFOUND = 1; then
	AC_MSG_NOTICE()
	AC_MSG_ERROR(Library not found! Try './configure --help'.)
	AC_MSG_NOTICE()
fi

AC_MSG_NOTICE(  );
AC_MSG_NOTICE( $CXXFLAGS );

# overwrite global variable (used for Makefile generation)
AC_SUBST(CXXFLAGS, "$CXXFLAGS $AM_CXXFLAGS" )
AC_SUBST(CFLAGS, "$CXXFLAGS $AM_CXXFLAGS" )
AC_SUBST(LDFLAGS, $LDFLAGS )

AC_OUTPUT( Makefile \
	   src/Phoenix/Makefile \
	   src/Phoenix/Audio/Makefile \
	   src/Phoenix/AI/Makefile \
	   src/Phoenix/Core/Makefile \
	   src/Phoenix/Math/Makefile \
	   src/Phoenix/Spatial/Makefile \
	   src/Phoenix/Scene/Makefile \
	   src/Phoenix/Volume/Makefile \
	   src/Phoenix/Graphics/Makefile \
	   phoenix.pc
)
AC_MSG_NOTICE(  );
AC_MSG_NOTICE( $AM_CXXFLAGS );

AC_MSG_NOTICE(  );
AC_MSG_NOTICE( $LIBS );