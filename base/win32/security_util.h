// utilities for security associated operations

#ifndef BASE_WIN32_SECURITY_UTIL_H_
#define BASE_WIN32_SECURITY_UTIL_H_

#include "base/base_config.h"

#if defined(OS_WIN)

#include "base/base_export.h"
#include <accctrl.h>

namespace nbase
{
namespace win32
{

BASE_EXPORT bool SetObjectToLowIntegrity(HANDLE hObject, SE_OBJECT_TYPE type = SE_KERNEL_OBJECT);

// Determine if the user is part of the adminstators group. This will return
// true in case of XP and 2K if the user belongs to admin group. In case of
// Vista, it only returns true if the admin is running elevated.
BASE_EXPORT bool IsUserAdmin();

// Returns true if the user is running as a non-elevated admin in case of
// Vista. In case of XP always returns false.
BASE_EXPORT bool IsUserNonElevatedAdmin();

// Determine the mandatory level of a SID
HRESULT GetSidIntegrityLevel(PSID sid, MANDATORY_LEVEL* level);

// Determine the mandatory level of a process
//   processID, the process to query, or (0) to use the current process
//   On Vista, level should alwys be filled in with either
//     MandatoryLevelLow (IE)
//     MandatoryLevelMedium(user), or
//     MandatoryLevelHigh( Elevated Admin)
//   On error, level remains unchanged
BASE_EXPORT HRESULT GetProcessIntegrityLevel(DWORD processID, MANDATORY_LEVEL* level);
}
}

#endif // OS_WIN
#endif // BASE_WIN32_SECURITY_UTIL_H_