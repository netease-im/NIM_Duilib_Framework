// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: rl
// Date: 2011/6/10
//
// Semaphore implementation on windows

#include "base/base_config.h"

#if defined(OS_WIN)

#include "base/synchronization/semaphore.h"

namespace nbase
{

Semaphore::Semaphore()
{
	init_value_ = 0;
	semaphore_  = NULL;
}

Semaphore::~Semaphore()
{
	if (semaphore_ != NULL)
	{
		CloseHandle(semaphore_);
	}
}

bool Semaphore::CreateSemaphore(const char *sem_name, uint32_t init_value, uint32_t max_value)
{
	if (init_value > max_value)
		return false;

	init_value_ = init_value;

	semaphore_ = ::CreateSemaphoreA(NULL, init_value, max_value, sem_name);

	return true;
}

bool Semaphore::WaitSemaphore()
{
	if (semaphore_)
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(semaphore_, INFINITE))
			return true;
	}
	return false;
}

bool Semaphore::WaitSemaphore(uint32_t timeout_ms)
{
	if (semaphore_)
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(semaphore_, (DWORD)timeout_ms))
			return true;
	}
	return false;
}

bool Semaphore::PostSemaphore(uint32_t post_num)
{
	if (semaphore_)
	{
		::ReleaseSemaphore(semaphore_, post_num, NULL);
		return true;
	}
	return false;
}

}  // namespace nbase

#endif  // OS_WIN