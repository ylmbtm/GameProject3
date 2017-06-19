/*
   Copyright (c) 2000, 2010, Oracle and/or its affiliates. All rights reserved.










   The lines above are intentionally left blank
*/

/*
  Helper macros used for setting different __attributes__
  on functions in a portable fashion
*/

#ifndef _my_attribute_h
#define _my_attribute_h

#if defined(__GNUC__)
# ifndef GCC_VERSION
#  define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
# endif
#endif

/*
  Disable __attribute__() on gcc < 2.7, g++ < 3.4, and non-gcc compilers.
  Some forms of __attribute__ are actually supported in earlier versions of
  g++, but we just disable them all because we only use them to generate
  compilation warnings.
*/
#ifndef __attribute__
# if !defined(__GNUC__)
#  define __attribute__(A)
# elif GCC_VERSION < 2008
#  define __attribute__(A)
# elif defined(__cplusplus) && GCC_VERSION < 3004
#  define __attribute__(A)
# endif
#endif

/*
  __attribute__((format(...))) is only supported in gcc >= 2.8 and g++ >= 3.4
  But that's already covered by the __attribute__ tests above, so this is
  just a convenience macro.
*/
#ifndef ATTRIBUTE_FORMAT
# define ATTRIBUTE_FORMAT(style, m, n) __attribute__((format(style, m, n)))
#endif

/*

   __attribute__((format(...))) on a function pointer is not supported
   until  gcc 3.1
*/
#ifndef ATTRIBUTE_FORMAT_FPTR
# if (GCC_VERSION >= 3001)
#  define ATTRIBUTE_FORMAT_FPTR(style, m, n) ATTRIBUTE_FORMAT(style, m, n)
# else
#  define ATTRIBUTE_FORMAT_FPTR(style, m, n)
# endif /* GNUC >= 3.1 */
#endif


#endif
