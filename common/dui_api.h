#pragma once

#if defined(UILIB_DLL)
#if defined(UILIB_EXPORTS)
#if defined(_MSC_VER)
#define UILIB_API __declspec(dllexport)
#else
#define UILIB_API
#endif
#else
#if defined(_MSC_VER)
#define UILIB_API __declspec(dllimport)
#else
#define UILIB_API
#endif
#endif
#else
#define UILIB_API
#endif

#define UILIB_CALL __cdecl

#ifdef __cplusplus
#define UILIB_C extern "C"
#define UILIB_C_START extern "C" {
#define UILIB_C_END   }
#else
#define UILIB_C
#define UILIB_C_START
#define UILIB_C_END
#endif