#include "stdafx.h"
#include "misc_thread.h"

MiscThread::MiscThread(enum ThreadId thread_id, const char* name)
	: FrameworkThread(name)
	, thread_id_(thread_id)
{

}

MiscThread::~MiscThread(void)
{

}

void MiscThread::Init()
{
	nbase::ThreadManager::RegisterThread(thread_id_);
}

void MiscThread::Cleanup()
{
	nbase::ThreadManager::UnregisterThread();
}