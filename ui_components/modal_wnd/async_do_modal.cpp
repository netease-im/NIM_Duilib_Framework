#include "stdafx.h"
#include "async_do_modal.h"
#include "async_modal_runner.h"

bool AsyncDoModal(ModalWndBase *dlg)
{
	return AsyncModalRunnerManager::GetInstance()->DoModal(dlg);
}

void CancelAllAsyncModalDialogs()
{
	AsyncModalRunnerManager::GetInstance()->CancelAllThreads();
}