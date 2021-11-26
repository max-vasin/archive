/* src/libjasper/include/jasper/jas_config.h.  Generated by configure.  */
/* src/libjasper/include/jasper/jas_config.h.in.  Generated from configure.ac by autoheader.  */


/* Avoid problems due to multiple inclusion. */
#ifndef JAS_CONFIG_H
#define JAS_CONFIG_H

/* This preprocessor symbol identifies the version of JasPer. */
#define	JAS_VERSION "1.701.0"
/* If configure is being used, this symbol will be defined automatically
  at this point in the configuration header file. */

/* The preprocessor symbol JAS_WIN_MSVC_BUILD should not be defined
  unless the JasPer software is being built under Microsoft Windows
  using Microsoft Visual C. */
#ifndef	JAS_WIN_MSVC_BUILD
/* A configure-based build is being used. */



/* Extra debugging support */
/* #undef DEBUG */

/* Debugging memory allocator */
/* #undef DEBUG_MEMALLOC */

/* Debugging overflow detection */
/* #undef DEBUG_OVERFLOW */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef HAVE_DOPRNT */

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <io.h> header file. */
/* #undef HAVE_IO_H */

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdbool.h> header file. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
//#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
//#define HAVE_UNISTD_H 1

/* Have variable length arrays */
#define HAVE_VLA 1

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* Define to 1 if you have the <windows.h> header file. */
/* #undef HAVE_WINDOWS_H */

/* JasPer configure */
#define JAS_CONFIGURE 1

/* JasPer version */
#define JAS_VERSION "1.701.0"

/* Name of package */
#define PACKAGE "jasper"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "jasper"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "jasper 1.701.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "jasper"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.701.0"

/* The size of a `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of a `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of a `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of a `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* The size of a `unsigned int', as computed by sizeof. */
#define SIZEOF_UNSIGNED_INT 4

/* The size of a `unsigned long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG 4

/* The size of a `unsigned long long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG_LONG 8

/* The size of a `unsigned short', as computed by sizeof. */
#define SIZEOF_UNSIGNED_SHORT 2

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "1.701.0"

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define as `__inline' if that's what the C compiler calls it, or to nothing
   if it is not supported. */
/* #undef inline */

/* Define to `long long' if <sys/types.h> does not define. */
#define longlong long long

/* Define to `unsigned' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef ssize_t */

/* Define to `unsigned char' if <sys/types.h> does not define. */
#define uchar unsigned char

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef uint */

/* Define to `unsigned long' if <sys/types.h> does not define. */
/* #undef ulong */

/* Define to `unsigned long long' if <sys/types.h> does not define. */
#define ulonglong unsigned long long

/* Define to `unsigned short' if <sys/types.h> does not define. */
/* #undef ushort */


#else
/* A configure-based build is not being used. */
#include <jasper/jas_config2.h>
#endif

#endif

