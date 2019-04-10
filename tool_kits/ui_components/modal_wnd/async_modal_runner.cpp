#include "stdafx.h"
#include "async_modal_runner.h"
#include <algorithm>
#include "base/thread/thread_manager.h"
#include "shared/threads.h"

static const char kModalThreadName[] = "AsyncModalRunner";

AsyncModalRunner::AsyncModalRunner(Delegate *delegate)
	: event_(true, false), is_running_(false), quit_posted_(false),
	  delegate_(delegate)
{

}

AsyncModalRunner::~AsyncModalRunner()
{
	CancelModalThenExit();
}

void AsyncModalRunner::CancelModalThenExit()
{
	if (!is_running_ || quit_posted_)
		return;
	quit_posted_ = true;
	PostThreadMessage(thread_id(), WM_QUIT, 0, 0);
}

bool AsyncModalRunner::DoModal(ModalWndBase *dlg)
{
	assert(!is_running_);
	modal_dlg_.reset(dlg);
	if (!Create())
		return false;
	is_running_ = event_.Wait();
	return is_running_;
}

void AsyncModalRunner::Run()
{
#ifndef NDEBUG
#if defined(OS_WIN) && defined(COMPILER_MSVC)
	nbase::SetThreadName(GetCurrentThreadId(), kModalThreadName);
#endif
#endif

	event_.Signal();
	if (modal_dlg_ != nullptr) {
		modal_dlg_->SyncShowModal();
	}
	if (delegate_ != nullptr) {
		delegate_->OnThreadWillExit(this);
	}
}

AsyncModalRunnerManager::AsyncModalRunnerManager()
{

}

AsyncModalRunnerManager::~AsyncModalRunnerManager()
{
	CancelAllThreads();
}

bool AsyncModalRunnerManager::DoModal(ModalWndBase *dlg)
{
	if (dlg == nullptr)
		return false;
	std::shared_ptr<AsyncModalRunner> runner(new AsyncModalRunner(this));
	{
		nbase::NAutoLock lock(&threads_lock_);
		runners_.push_back(runner);
	}
	if (runner->DoModal(std::move(dlg)))
		return true;
	return false;
}

void AsyncModalRunnerManager::CancelAllThreads()
{
	std::list<std::shared_ptr<AsyncModalRunner> > threads;
	{
		nbase::NAutoLock lock(&threads_lock_);
		threads.swap(runners_);
	}
	// First, we notify the modal dialogs to quit the modal loop
	std::for_each(threads.begin(), threads.end(),
		[](std::shared_ptr<AsyncModalRunner> runner) {
			runner->CancelModalThenExit();
		}
	);
	// Then, we wait util all modal runner exited
	std::for_each(threads.begin(), threads.end(),
		[](std::shared_ptr<AsyncModalRunner> runner) {
			runner->Close();
		}
	);
}

void AsyncModalRunnerManager::OnThreadWillExit(AsyncModalRunner *runner)
{
	nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&AsyncModalRunnerManager::Deregister, this, runner));
}

void AsyncModalRunnerManager::Deregister(AsyncModalRunner *runner)
{
	std::shared_ptr<AsyncModalRunner> found;
	{
		nbase::NAutoLock lock(&threads_lock_);
		for (std::list<std::shared_ptr<AsyncModalRunner> >::const_iterator iter = runners_.begin(); iter != runners_.end(); iter++) {
			if ((*iter).get() == runner) {
				found = *iter;
				runners_.erase(iter);
				break;
			}
		}
	}
	// If the runner is found,
	// it will be destroyed out of the scope of the lock
}
