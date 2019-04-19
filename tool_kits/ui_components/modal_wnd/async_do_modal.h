#ifndef NIM_WIN_GUI_MSG_BOX_ASYNC_DO_MODAL_H_
#define NIM_WIN_GUI_MSG_BOX_ASYNC_DO_MODAL_H_

#include "modal_wnd_base.h"

// The function will create a helper thread and run a modal dialog on it.
// Once the modal dialog ended, the thread will be destroyed automatically.
// NOTE: Once this function is called, the ownership of |dlg| will be taken.
bool AsyncDoModal(ModalWndBase *dlg);

// This function will send requests to all modal dialogs created by AsyncDoModal
// and wait until all of them ended.
// Typically, the function should be called when the application wants to quit.
void CancelAllAsyncModalDialogs();

#endif // NIM_WIN_GUI_MSG_BOX_ASYNC_DO_MODAL_H_
