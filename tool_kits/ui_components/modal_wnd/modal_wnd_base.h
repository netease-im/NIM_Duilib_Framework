#ifndef NIM_MSG_BOX_MODAL_WND_BASE_H_
#define NIM_MSG_BOX_MODAL_WND_BASE_H_

class ModalWndBase
{
public:
	virtual void SyncShowModal() = 0;
	virtual ~ModalWndBase() {}
};

#endif // NIM_MSG_BOX_MODAL_WND_BASE_H_
