#include "stdafx.h"

namespace ui
{

EventType StringToEnum(const std::wstring& messageType)
{
	if (messageType == EVENTSTR_ALL) {
		return kEventAll;
	}
	else if (messageType == EVENTSTR_KEYDOWN) {
		return kEventKeyDown;
	}
	else if (messageType == EVENTSTR_KEYUP) {
		return kEventKeyUp;
	}
	else if (messageType == EVENTSTR_CHAR) {
		return kEventChar;
	}
	else if (messageType == EVENTSTR_SYSKEY) {
		return kEventSystemKey;
	}
	else if (messageType == EVENTSTR_SETFOCUS) {
		return kEventSetFocus;
	}
	else if (messageType == EVENTSTR_KILLFOCUS) {
		return kEventKillFocus;
	}
	else if (messageType == EVENTSTR_SETCURSOR) {
		return kEventSetCursor;
	}
	else if (messageType == EVENTSTR_MOUSEMOVE) {
		return kEventMouseMove;
	}
	else if (messageType == EVENTSTR_MOUSEENTER) {
		return kEventMouseEnter;
	}
	else if (messageType == EVENTSTR_MOUSELEAVE) {
		return kEventMouseLeave;
	}
	else if (messageType == EVENTSTR_MOUSEHOVER) {
		return kEventMouseHover;
	}
	else if (messageType == EVENTSTR_BUTTONDOWN) {
		return kEventMouseButtonDown;
	}
	else if (messageType == EVENTSTR_BUTTONUP) {
		return kEventMouseButtonUp;
	}
	else if (messageType == EVENTSTR_RBUTTONDOWN) {
		return kEventMouseRightButtonDown;
	}
	else if (messageType == EVENTSTR_DOUBLECLICK) {
		return kEventMouseDoubleClick;
	}
	else if (messageType == EVENTSTR_SELECT) {
		return kEventSelect;
	}
	else if (messageType == EVENTSTR_UNSELECT) {
		return kEventUnSelect;
	}
	else if (messageType == EVENTSTR_MENU) {
		return kEventMouseMenu;
	}
	else if (messageType == EVENTSTR_EXPAND) {
		return kEventExpand;
	}
	else if (messageType == EVENTSTR_UNEXPAND) {
		return kEventUnExpand;
	}
	else if (messageType == EVENTSTR_SCROLLWHEEL) {
		return kEventMouseScrollWheel;
	}
	else if (messageType == EVENTSTR_SCROLLCHANGE) {
		return kEventScrollChange;
	}
	else if (messageType == EVENTSTR_VALUECHANGE) {
		return kEventValueChange;
	}
	else if (messageType == EVENTSTR_RETURN) {
		return kEventReturn;
	}
	else if (messageType == EVENTSTR_TAB) {
		return kEventTab;
	}
	else if (messageType == EVENTSTR_WINDOWCLOSE) {
		return kEventWindowClose;
	}
	else {
		ASSERT(FALSE);
		return kEventNone;
	}
}

}