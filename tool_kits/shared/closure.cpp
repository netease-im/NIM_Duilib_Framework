#include "stdafx.h"
#include "closure.h"
#include "shared/threads.h"
#include "base/thread/thread_manager.h"

void Post2UI(const StdClosure &closure)
{
	nbase::ThreadManager::PostTask(kThreadUI, closure);
}

void Post2GlobalMisc(const StdClosure &closure)
{
	nbase::ThreadManager::PostTask(kThreadGlobalMisc, closure);
}

void Post2Database(const StdClosure &closure)
{
	nbase::ThreadManager::PostTask(kThreadDatabase, closure);
}
