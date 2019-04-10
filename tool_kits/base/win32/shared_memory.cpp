// Copyright (c) 2011, NetEase Inc. All rights reserved.
// All rights reserved.
//
// Author: wrt(guangguang)
// Date: 2011/7/4
//
// A wrapper class of Windows shared memory

#include "base/win32/shared_memory.h"
#if defined(OS_WIN)
#include "base/win32/security_util.h"

namespace nbase
{
namespace win32
{

bool SharedMemory::Create(const wchar_t *name, int size)
{
	assert(mapping_ == NULL);
	mapping_ = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
		0, size, name);
	if(mapping_)
	{
		win32::SetObjectToLowIntegrity(mapping_);
	}
	return mapping_ != NULL;
}


bool SharedMemory::Open(const wchar_t *name)
{
	assert(mapping_ == NULL);
	mapping_ = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, name);
	return mapping_ != NULL;
}


void SharedMemory::Close()
{
	if (mapping_ != NULL)
	{
		CloseHandle(mapping_);
		mapping_ = NULL;
	}
}


bool SharedMemory::MappedView::OpenView(SharedMemory *shared_memory, bool read_write)
{
	assert(!view_);
	DWORD access = FILE_MAP_READ;
	if (read_write) access |= FILE_MAP_WRITE;
	view_ = reinterpret_cast<unsigned char*>(::MapViewOfFile(shared_memory->mapping_, access, 0, 0, 0));
	return view_ != NULL;
}


void SharedMemory::MappedView::CloseView()
{
	if (view_)
	{
		::UnmapViewOfFile(view_);
		view_ = NULL;
	}
}

} // namespace win32
} // namespace nbase

#endif // OS_WIN