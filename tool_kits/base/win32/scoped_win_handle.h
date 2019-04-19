#ifndef BASE_WIN32_SCOPED_WIN_HANDLE_H_
#define BASE_WIN32_SCOPED_WIN_HANDLE_H_

#include "base/base_config.h"
#if defined(OS_WIN)

#include <windows.h>
#include "base/base_export.h"
#include "base/base_types.h"

namespace nbase
{
namespace win32
{

class BASE_EXPORT ScopedWinHandle
{
public:

	ScopedWinHandle() : handle_(INVALID_HANDLE_VALUE) {}
	ScopedWinHandle(HANDLE handle) : handle_(handle) {}
	~ScopedWinHandle() { Reset(INVALID_HANDLE_VALUE); }

	bool Valid() const { return handle_ != INVALID_HANDLE_VALUE; }
	HANDLE Get() const { return handle_; }
	HANDLE Release() { HANDLE old_handle = handle_; handle_ = INVALID_HANDLE_VALUE; return old_handle; }
	void Reset(HANDLE handle) { if (Valid()) ::CloseHandle(handle_); handle_ = handle; }
	HANDLE* operator&() { return &handle_; }
	operator HANDLE() const { return handle_; }

private:
	HANDLE handle_;

	DISALLOW_COPY_AND_ASSIGN(ScopedWinHandle);
};

} // namespace win32
} // namespace nbase

#endif // OS_WIN
#endif // BASE_WIN32_SCOPED_WIN_HANDLE_H_
