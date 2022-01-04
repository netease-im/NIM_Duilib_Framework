// This file defines some useful macros

#ifndef BASE_MACROS_H_
#define BASE_MACROS_H_

#include <assert.h>
#include <string>
#include "base_types.h"

// check the pointer is null and return null
#define PTR_NULL(p)				{ assert(p); if (NULL == p) return NULL; }
// check the pointer is null and return void
#define PTR_VOID(p)				{ assert(p); if (NULL == p) return; }
// check the pointer avalible， if not then return false
#define PTR_FALSE(p)			{ assert(p); if (NULL == p) return false; }

// delete pointer on safe mode
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if (NULL != p) { delete (p); (p) = NULL; } }
#endif

// release object on safe mode
#define SAFE_RELEASE(p)			{ if (NULL != p) { (p)->Release(); (p) = NULL; } }

//namespace
#ifndef NAMESPACE_USING_
#define NAMESPACE_USING_(n)		using namespace n;
#endif
#ifndef NAMESPACE_BEGIN_
#define NAMESPACE_BEGIN_(n)		namespace n {
#endif
#ifndef NAMESPACE_END_
#define NAMESPACE_END_(n)		}
#endif

#ifndef COUNT_OF
#define COUNT_OF(array)			(sizeof(array)/sizeof(array[0]))
#endif

// define some format string
#define CHAR_FORMAT                             "%c"
#define POINTER_FORMAT                          "%p"
#define STRING_FORMAT                           "%s"

#define INT8_FORMAT                             "%d"
#define UINT8_FORMAT                            "%u"
#define INT16_FORMAT                            "%d"
#define UINT16_FORMAT                           "%u"
#define INT32_FORMAT                            "%d"
#define UINT32_FORMAT                           "%u"

#define HEX8_FORMAT                             "%x"
#define HEX16_FORMAT                            "%x"
#define HEX32_FORMAT                            "%x"

#define OCT8_FORMAT                             "%o"
#define OCT16_FORMAT                            "%o"
#define OCT32_FORMAT                            "%o"

#define FLOAT_FORMAT                            "%f"
#define DOUBLE_FORMAT                           "%lf"

#if defined(COMPILER_MSVC)
#define INT64_FORMAT                            "%I64d"
#define UINT64_FORMAT                           "%I64u"
#define HEX64_FORMAT                            "%I64x"
#define OCT64_FORMAT                            "%I64o"
#else  //OS_WIN
#define UINT64_FORMAT                           "%llu"
#define INT64_FORMAT                            "%lld"
#define HEX64_FORMAT                            "%llx"
#define OCT64_FORMAT                            "%llo"
#endif //OS_WIN

#endif  // BASE_MACROS_H_
