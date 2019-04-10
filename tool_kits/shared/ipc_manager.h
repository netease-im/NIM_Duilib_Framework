#ifndef SHARED_IPC_MANAGER_H_
#define SHARED_IPC_MANAGER_H_

#include "base/synchronization/lock.h"
#include "base/memory/singleton.h"
#include <string>
#include <map>

namespace shared
{
// IPC
class IPCManager
{
public:
	SINGLETON_DEFINE(IPCManager);
	IPCManager();
	~IPCManager();
	bool CreateSharedMemory(const std::wstring &tag, const std::string &data);
	void DestroySharedMemory(const std::wstring &tag);

private:
	std::map<std::wstring, HANDLE>		shared_memory_handle_map_;
	nbase::NLock						lock_;
};
}//namespace shared

#endif //SHARED_IPC_MANAGER_H_
