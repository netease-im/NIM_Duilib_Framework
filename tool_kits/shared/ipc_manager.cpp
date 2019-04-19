#include "stdafx.h"
#include "ipc_manager.h"
#include "base/macros.h"

namespace shared
{

IPCManager::IPCManager()
{

}

IPCManager::~IPCManager()
{
	nbase::NAutoLock auto_lock(&lock_);

	for (std::map<std::wstring, HANDLE>::const_iterator it = shared_memory_handle_map_.begin();
		it != shared_memory_handle_map_.end(); ++it)
	{
		::CloseHandle(it->second);
	}
	shared_memory_handle_map_.clear();
}

bool IPCManager::CreateSharedMemory(const std::wstring &tag, const std::string &data)
{
	DestroySharedMemory(tag);

	nbase::NAutoLock auto_lock(&lock_);

	PSECURITY_DESCRIPTOR pSec = (PSECURITY_DESCRIPTOR)::LocalAlloc(LMEM_FIXED, SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (!pSec)
	{
		return false;
	}
	if (!::InitializeSecurityDescriptor(pSec, SECURITY_DESCRIPTOR_REVISION))
	{
		::LocalFree(pSec);
		return false;
	}
	if (!::SetSecurityDescriptorDacl(pSec, true, NULL, true))
	{
		::LocalFree(pSec);
		return false;
	}

	SECURITY_ATTRIBUTES attr;
	attr.bInheritHandle = false;
	attr.lpSecurityDescriptor = pSec;
	attr.nLength = sizeof(SECURITY_ATTRIBUTES);

	HANDLE h = ::CreateFileMapping(INVALID_HANDLE_VALUE, &attr, PAGE_READWRITE, 0, MAX_PATH, tag.c_str());
	if (h == NULL)
		return false;

	PVOID pView = ::MapViewOfFile(h, FILE_MAP_WRITE, 0, 0, 0);
	if (pView == NULL)
	{
		::CloseHandle(h);
		return false;
	}

	memset(pView, 0, MAX_PATH);
	memcpy(pView, data.c_str(), data.length());

	::UnmapViewOfFile(pView);
	shared_memory_handle_map_[tag] = h;
	return true;
}

void IPCManager::DestroySharedMemory(const std::wstring &tag)
{
	nbase::NAutoLock auto_lock(&lock_);

	if (shared_memory_handle_map_.find(tag) == shared_memory_handle_map_.end())
		return;

	::CloseHandle(shared_memory_handle_map_[tag]);
	shared_memory_handle_map_.erase(tag);
}
}//namespace shared