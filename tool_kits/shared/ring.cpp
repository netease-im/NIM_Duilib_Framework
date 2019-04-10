#include "stdafx.h"
#include "ring.h"
#include "log.h"
#include "util.h"

static const std::wstring kNewMessage		= L"new_message.wav";
static const std::wstring kVoipConnecting	= L"voip_connecting.wav";
static const std::wstring kVoipNoResponse	= L"voip_no_response.wav";
static const std::wstring kVoipPeerBusy		= L"voip_peer_busy.wav";
static const std::wstring kVoipPeerReject 	= L"voip_peer_reject.wav";
static const std::wstring kVoipRing 		= L"voip_ring.wav";

namespace shared
{
Ring::Ring()
{
	hwnd_ = NULL;
}

Ring::~Ring()
{
	Destroy();
}

bool Ring::Init(HWND hwnd)
{
	hwnd_ = MCIWndCreateW(hwnd, NULL, WS_POPUP | MCIWNDF_NOERRORDLG | MCIWNDF_NOTIFYMODE, NULL);
	if (hwnd_ == NULL)
	{
		QLOG_ERR(L"MCIWndCreateW error {0}") << GetLastError();
		return false;
	}
	return true;
}

void Ring::Play(RingType type, bool replay)
{
	assert(hwnd_);
	if (hwnd_ == NULL)
		return;

	Stop();
	replay_ = replay;

	std::wstring dir = QPath::GetAppPath() + L"res\\audio\\";
	switch (type)
	{
	case RING_NEW_MESSAGE:
		dir.append(kNewMessage);
		break;
	case RING_VOIP_CONNTETING:
		dir.append(kVoipConnecting);
		break;
	case RING_VOIP_NORESPONSE:
		dir.append(kVoipNoResponse);
		break;
	case RING_VOIP_PEERBUSY:
		dir.append(kVoipPeerBusy);
		break;
	case RING_VOIP_PEERREJECT:
		dir.append(kVoipPeerReject);
		break;
	case RING_VOIP_RING:
		dir.append(kVoipRing);
		break;
	}

	DWORD ret = MCIWndOpen(hwnd_, dir.c_str(), 0);
	if (ret == 0)
		MCIWndPlay(hwnd_);
	else
		QLOG_ERR(L"MCIWndOpen error {0}") << ret;
}

void Ring::Replay()
{
	if (hwnd_ != NULL && MCIWndCanPlay(hwnd_))
	{
		MCIWndSeek(hwnd_, 0);
		MCIWndPlay(hwnd_);
	}
}


void Ring::Stop()
{
	replay_ = false;
	if (hwnd_)
	{
		MCIWndStop(hwnd_);
		MCIWndClose(hwnd_);
	}
}

void Ring::Destroy()
{
	if (hwnd_)
	{
		Stop();
	}
}

LRESULT Ring::Notify(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == MCIWNDM_NOTIFYMODE)
	{
		if (lParam == MCI_MODE_STOP && replay_)
		{
			Replay();
		}
	}
	return 0;
}
}