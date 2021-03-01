// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: rl
// Date: 2011/6/10
//
// Semaphore implementation on posix system

#include "base/base_config.h"

#if defined(OS_POSIX)

#include "base/synchronization/semaphore.h"
#include <unistd.h>
#include <fcntl.h>

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
		sem_destroy(semaphore_);
	}
}

bool Semaphore::CreateSemaphore(const char *sem_name, uint32_t init_value, uint32_t max_value)
{
	if (init_value > max_value)
		return false;

	init_value_ = init_value;

	/* note : here we will use named semaphore because iOS not supported unamed semaphore*/
	if (sem_name == NULL)
	{
		sem_unlink("netease_noname_semaphore");
		semaphore_ = sem_open("netease_noname_semaphore", O_CREAT | O_EXCL,  0, max_value);
		sem_unlink("netease_noname_semaphore");
		if (semaphore_ == SEM_FAILED)
		{
			return false;
		}
		for (uint32_t i = 0; i < max_value - init_value; ++i)
			sem_wait(semaphore_);
	}
	else
	{
		sem_unlink(sem_name);
		semaphore_ = sem_open(sem_name, O_EXCL);
		sem_unlink(sem_name);
		if (semaphore_ == SEM_FAILED)
		{
			semaphore_ = sem_open(sem_name, O_CREAT, 0, max_value);

			if (semaphore_  == SEM_FAILED)
			{
				return false;
			}
			for (uint32_t i = 0; i < max_value - init_value; ++i)
				sem_wait(semaphore_);
		}           
	}
	
	return true;
}

bool Semaphore::WaitSemaphore()
{
	if (semaphore_)
	{
		if (sem_wait(semaphore_) == 0)
			return true;
	}
	return false;
}

bool Semaphore::WaitSemaphore(uint32_t timeout_ms)
{
	if (semaphore_)
	{
		int time_count = 0;
		int ret        = -1;
		while ((ret != 0) && (time_count < timeout_ms))
		{
			ret = sem_trywait(semaphore_);
			usleep(1000);
			time_count++;
		}
		if (ret == 0)
			return true;
	}
	return false;
}

bool Semaphore::PostSemaphore(uint32_t post_num)
{
	if (semaphore_)
	{
		for (uint32_t i = 0; i < post_num; i++)
		{
			int ret = sem_post(semaphore_);
			if (ret != 0)
				return false;
		}
		return true;
	}
	return false;
}

}  // namespace nbase

#endif  // OS_WIN