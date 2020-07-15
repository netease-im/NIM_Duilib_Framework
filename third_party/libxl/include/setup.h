#ifndef LIBXL_SETUP_CPP_H
#define LIBXL_SETUP_CPP_H

#ifdef LIBXL_STDCALL
  #define LIBXL_CALLING __stdcall
#else
  #define LIBXL_CALLING __cdecl
#endif

#if !defined(LIBXL_STATIC) && (defined(_MSC_VER) || defined(__WATCOMC__))

  #ifdef libxl_EXPORTS
      #define XLAPI __declspec(dllexport)
  #else
      #define XLAPI __declspec(dllimport)
  #endif

  #define XLAPIENTRY LIBXL_CALLING

#else

  #ifdef libxl_EXPORTS
      #define XLAPI __attribute__ ((visibility ("default")))
  #else
      #define XLAPI
  #endif

  #if defined(__MINGW32__)
    #define XLAPIENTRY LIBXL_CALLING
  #else
    #define XLAPIENTRY
  #endif

#endif

#endif
