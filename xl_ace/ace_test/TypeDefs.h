#ifndef HISO_DEFINE_H
#define HISO_DEFINE_H

#include <sys/types.h>

//#include <ace/ACE_export.h>
#include <boost/cstdint.hpp>

#include "CompilerDefs.h"

#define HISO_LITTLEENDIAN 0
#define HISO_BIGENDIAN    1

#if !defined(HISO_ENDIAN)
#  if defined (ACE_BIG_ENDIAN)
#    define HISO_ENDIAN HISO_BIGENDIAN
#  else //ACE_BYTE_ORDER != ACE_BIG_ENDIAN
#    define HISO_ENDIAN HISO_LITTLEENDIAN
#  endif //ACE_BYTE_ORDER
#endif //HISO_ENDIAN

#if PLATFORM == PLATFORM_WINDOWS
#  define HISO_Export __declspec(dllexport)
#  define HISO_LIBRARY_HANDLE HMODULE
#  define HISO_LOAD_LIBRARY(a) ::LoadLibrary(a)
#  define HISO_CLOSE_LIBRARY ::FreeLibrary
#  define HISO_GET_PROC_ADDR ::GetProcAddress
#  define HISO_IMPORT __cdecl
#  define HISO_SCRIPT_EXT ".dll"
#  define HISO_SCRIPT_NAME "HisoScript"
#  define HISO_PATH_MAX MAX_PATH
#else //PLATFORM != PLATFORM_WINDOWS
#  define HISO_LIBRARY_HANDLE void*
#  define HISO_Export
#  define HISO_LOAD_LIBRARY(a) dlopen(a,RTLD_NOW)
#  define HISO_CLOSE_LIBRARY dlclose
#  define HISO_GET_PROC_ADDR dlsym
#  if defined(__APPLE_CC__) && defined(BIG_ENDIAN)
#    define HISO_IMPORT __attribute__ ((longcall))
#  elif defined(__x86_64__)
#    define HISO_IMPORT
#  else
#    define HISO_IMPORT __attribute__ ((cdecl))
#  endif //__APPLE_CC__ && BIG_ENDIAN
#  if defined(__APPLE_CC__)
#    define HISO_SCRIPT_EXT ".dylib"
#    if defined(DO_SCRIPTS)
#      define HISO_SCRIPT_NAME "../lib/libtrinityscript"
#    else
#      define HISO_SCRIPT_NAME "../lib/libtrinityinterface"
#    endif // DO_SCRIPTS
#  else
#    define HISO_SCRIPT_EXT ".so"
#    if defined(DO_SCRIPTS)
#      define HISO_SCRIPT_NAME "libtrinityscript"
#    else
#      define HISO_SCRIPT_NAME "libtrinityinterface"
#    endif // DO_SCRIPTS
#  endif //__APPLE_CC__
#  define HISO_PATH_MAX PATH_MAX
#endif //PLATFORM

#if PLATFORM == PLATFORM_WINDOWS
#  ifdef HISO_WIN32_DLL_IMPORT
#    define HISO_DLL_DECL __declspec(dllimport)
#  else //!HISO_WIN32_DLL_IMPORT
#    ifdef HISO_WIND_DLL_EXPORT
#      define HISO_DLL_DECL __declspec(dllexport)
#    else //!HISO_WIND_DLL_EXPORT
#      define HISO_DLL_DECL
#    endif //HISO_WIND_DLL_EXPORT
#  endif //HISO_WIN32_DLL_IMPORT
#else //PLATFORM != PLATFORM_WINDOWS
#  define HISO_DLL_DECL
#endif //PLATFORM

#if PLATFORM == PLATFORM_WINDOWS
#  define HISO_DLL_SPEC __declspec(dllexport)
#  ifndef DECLSPEC_NORETURN
#    define DECLSPEC_NORETURN __declspec(noreturn)
#  endif //DECLSPEC_NORETURN
#else //PLATFORM != PLATFORM_WINDOWS
#  define HISO_DLL_SPEC
#  define DECLSPEC_NORETURN
#endif //PLATFORM

#if !defined(DEBUG)
#  define HISO_INLINE inline
#else //DEBUG
#  if !defined(HISO_DEBUG)
#    define HISO_DEBUG
#  endif //HISO_DEBUG
#  define HISO_INLINE
#endif //!DEBUG

#if COMPILER == COMPILER_GNU
#  define ATTR_NORETURN __attribute__((noreturn))
#  define ATTR_PRINTF(F,V) __attribute__ ((format (printf, F, V)))
#else //COMPILER != COMPILER_GNU
#  define ATTR_NORETURN
#  define ATTR_PRINTF(F,V)
#endif //COMPILER == COMPILER_GNU


typedef boost::int64_t  int64;
typedef boost::int32_t  int32;
typedef boost::int16_t  int16;
typedef boost::int8_t   int8;
typedef boost::uint64_t uint64;
typedef boost::uint32_t uint32;
typedef boost::uint16_t uint16;
typedef boost::uint8_t  uint8;


typedef uint64 OBJECT_HANDLE;

//commented out
//so all future merges with invalid style will drop build errors
//this way devs will at least read thru what they merge to find the error

#if defined(HISO_DEBUG)
#  ifndef HISO_DEBUG
#    define HISO_DEBUG
#  endif
#endif

#ifndef CLIENT_VER
#define CLIENT_VER 313
#endif

#endif //HISO_DEFINE_H
