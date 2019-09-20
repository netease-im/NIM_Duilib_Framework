// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: rl
// Date: 2011/6/9
//
// Lock implemetation on posix like Mac OS X/Linux/FreeBSD

#include "lock.h"

#if defined(OS_POSIX)

#include <errno.h>

namespace nbase
{

NLock::NLock()
{
	// In release, go with the default lock attributes.
	pthread_mutex_init(&os_lock_, NULL);
}

NLock::~NLock()
{
	pthread_mutex_destroy(&os_lock_);
}

bool NLock::Try()
{
	int rv = pthread_mutex_trylock(&os_lock_);
	return rv == 0;
}

void NLock::Lock()
{
	pthread_mutex_lock(&os_lock_);
}

void NLock::Unlock()
{
	pthread_mutex_unlock(&os_lock_);
}

}  // namespace

#endif  // OS_WIN