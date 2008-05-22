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
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_PROG_INSTALL
AC_PROG_LIBTOOL

# Checks for library functions.
AC_CHECK_FUNCS([floor gettimeofday memset pow sqrt strdup])

AC_ARG_WITH(tristripper,
	AC_HELP_STRING(
	    [--with-tristripper=prefix],
	    [try this for the tristripper-library prefix install directory]
	  ),
	  TRISTRIPPER_PATHSET=1,
	  TRISTRIPPER_PATHSET=0
)

# adding the static library 'libtristripper.a'

# adding the lib to the files to link
LIBS="-ltristripper $LIBS"

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
		CXXFLAGS="$CXXFLAGS -I$with_tristripper/include"
		CFLAGS="$CFLAGS -I$with_tristripper/include"
		LDFLAGS="$LDFLAGS -L$with_tristripper/lib"
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
	AC_MSG_NOTICE()
	AC_MSG_NOTICE(the tristripper library is required.)
	AC_MSG_NOTICE(it can be obtained from http://users.pandora.be/tfautre/softdev/tristripper/ )
	AC_MSG_NOTICE()
 	if test $TRISTRIPPER_PATHSET = 1; then
		AC_MSG_NOTICE(cant find tristripper in given path '$with_tristripper'.)
 	else
		AC_MSG_NOTICE(if tristripper is installed elsewhere use --with-tristripper=prefix.)
 	fi
 	DEPENDENCYNOTFOUND=1;
	AC_DEFINE(TRISTRIPPER, 1, is tristripper library available. )
else
	AC_DEFINE(TRISTRIPPER, 0, is tristripper library available. )
fi

dnl **** Check for freetype2 library *****

AC_CHECK_PROGS(ft_devel,[freetype-config freetype2-config],false)

if test "$ft_devel" != "false"; then
     ac_freetype_libs=`$ft_devel --libs`
     ac_freetype_incl=`$ft_devel --cflags`
     CXXFLAGS="$CXXFLAGS $ac_freetype_incl"
     CFLAGS="$CFLAGS $ac_freetype_incl"
     LDFLAGS="$LDFLAGS $ac_freetype_libs"
     AC_MSG_CHECKING([for the Freetype2 ])
     AC_COMPILE_IFELSE( 
     [ #include <ft2build.h>
       #include FT_FREETYPE_H ],
     [
       AC_MSG_RESULT(yes)
       FREETYPE_NOTFOUND=0;
     ],
     [
       AC_MSG_RESULT(no)
       FREETYPE_NOTFOUND=1;
     ]
     )
     if test $FREETYPE_NOTFOUND = 1; then
	$DEPENDENCYNOTFOUND=1;
	AC_MSG_NOTICE()
	AC_MSG_ERROR(Freetype library not found!)
     fi

else
	$DEPENDENCYNOTFOUND=1;
fi

AM_PATH_SDL(1.2.11, 
	    [
		SDL_NOTFOUND=0;
	    ], 
	    [
		SDL_NOTFOUND=1;
	    ]
)

dnl **** check for SDL ****

AC_CHECK_PROGS(sdl_devel,[sdl-config],false)

if test "$sdl_devel" != "false"; then
     CXXFLAGS="$CXXFLAGS `${sdl_devel} --cflags`"
     CFLAGS="$CFLAGS `${sdl_devel} --cflags`"
     LDFLAGS="$LDFLAGS `${sdl_devel} --libs`"

     AC_MSG_CHECKING([for the SDL ])
     AC_COMPILE_IFELSE( 
     [ #include <SDL.h> ],
     [
       AC_MSG_RESULT(yes)
       SDL_NOTFOUND=0;
     ],
     [
       AC_MSG_RESULT(no)
       SDL_NOTFOUND=1;
     ]
     )

     if test $SDL_NOTFOUND = 1; then
	$DEPENDENCYNOTFOUND=1;
	AC_MSG_NOTICE()
	AC_MSG_ERROR(SDL library not found!)
     fi

else
	$DEPENDENCYNOTFOUND=1;
fi

# error ABORT if one of the libraries was not found
if test $DEPENDENCYNOTFOUND = 1; then
	AC_MSG_NOTICE()
	AC_MSG_ERROR(library not found! Try './configure --help'.)
fi	





# overwrite global variable (used for Makefile generation)
AC_SUBST(GLOBALCXXFLAGS, $CXXFLAGS )
AC_SUBST(GLOBALCFLAGS, $CFLAGS )
AC_SUBST(GLOBALLDFLAGS, $LDFLAGS )

AC_OUTPUT(Makefile src/Phoenix/Makefile)