#ifndef UI_MACROS_H_
#define UI_MACROS_H_

#pragma once

namespace ui
{
///////////////////////////////////////////////////////
// windowsx.h
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

#define     SubclassWindow(hwnd, lpfn)       \
              ((WNDPROC)SetWindowLongPtr((hwnd), GWLP_WNDPROC, (LPARAM)(WNDPROC)(lpfn)))

#define     MapWindowRect(hwndFrom, hwndTo, lprc) \
                    MapWindowPoints((hwndFrom), (hwndTo), (POINT *)(lprc), 2)

#define     GetWindowStyle(hwnd)    ((DWORD)GetWindowLong(hwnd, GWL_STYLE))
#define     GetWindowExStyle(hwnd)  ((DWORD)GetWindowLong(hwnd, GWL_EXSTYLE))
#define     GetWindowOwner(hwnd)    GetWindow(hwnd, GW_OWNER)

///////////////////////////////////////////////////////
// WM_TOUCH触摸消息
#define WM_TOUCH                        0x0240
#define WM_GESTURE                      0x0119
#define WM_GESTURENOTIFY                0x011A

/*
* RegisterTouchWindow flag values
*/
#define TWF_FINETOUCH       (0x00000001)
#define TWF_WANTPALM        (0x00000002)

/*
* Conversion of touch input coordinates to pixels
*/
#define TOUCH_COORD_TO_PIXEL(l)         ((l) / 100)

/*
* Touch input flag values (TOUCHINPUT.dwFlags)
*/
#define TOUCHEVENTF_MOVE            0x0001
#define TOUCHEVENTF_DOWN            0x0002
#define TOUCHEVENTF_UP              0x0004
#define TOUCHEVENTF_INRANGE         0x0008
#define TOUCHEVENTF_PRIMARY         0x0010
#define TOUCHEVENTF_NOCOALESCE      0x0020
#define TOUCHEVENTF_PEN             0x0040
#define TOUCHEVENTF_PALM            0x0080

/*
* Touch input mask values (TOUCHINPUT.dwMask)
*/
#define TOUCHINPUTMASKF_TIMEFROMSYSTEM  0x0001  // the dwTime field contains a system generated value
#define TOUCHINPUTMASKF_EXTRAINFO       0x0002  // the dwExtraInfo field is valid
#define TOUCHINPUTMASKF_CONTACTAREA     0x0004  // the cxContact and cyContact fields are valid

///////////////////////////////////////////////////////
// DPI适配
typedef enum PROCESS_DPI_AWARENESS {
	PROCESS_DPI_UNAWARE = 0,
	PROCESS_SYSTEM_DPI_AWARE = 1,
	PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

typedef enum MONITOR_DPI_TYPE {
	MDT_EFFECTIVE_DPI = 0,
	MDT_ANGULAR_DPI = 1,
	MDT_RAW_DPI = 2,
	MDT_DEFAULT = MDT_EFFECTIVE_DPI
} MONITOR_DPI_TYPE;

DECLARE_HANDLE(HTOUCHINPUT);
typedef struct tagTOUCHINPUT {
	LONG x;
	LONG y;
	HANDLE hSource;
	DWORD dwID;
	DWORD dwFlags;
	DWORD dwMask;
	DWORD dwTime;
	ULONG_PTR dwExtraInfo;
	DWORD cxContact;
	DWORD cyContact;
} TOUCHINPUT, *PTOUCHINPUT;
typedef TOUCHINPUT const * PCTOUCHINPUT;

///////////////////////////////////////////////////////
// WM_POINTER消息
#define WM_NCPOINTERUPDATE              0x0241
#define WM_NCPOINTERDOWN                0x0242
#define WM_NCPOINTERUP                  0x0243
#define WM_POINTERUPDATE                0x0245
#define WM_POINTERDOWN                  0x0246
#define WM_POINTERUP                    0x0247
#define WM_POINTERENTER                 0x0249
#define WM_POINTERLEAVE                 0x024A
#define WM_POINTERACTIVATE              0x024B
#define WM_POINTERCAPTURECHANGED        0x024C
#define WM_TOUCHHITTESTING              0x024D
#define WM_POINTERWHEEL                 0x024E
#define WM_POINTERHWHEEL                0x024F
#define DM_POINTERHITTEST               0x0250

enum tagPOINTER_INPUT_TYPE {
	PT_POINTER = 0x00000001,   // Generic pointer
	PT_TOUCH = 0x00000002,   // Touch
	PT_PEN = 0x00000003,   // Pen
	PT_MOUSE = 0x00000004,   // Mouse
#if(WINVER >= 0x0603)
	PT_TOUCHPAD = 0x00000005,   // Touchpad
#endif /* WINVER >= 0x0603 */
};
typedef DWORD POINTER_INPUT_TYPE;

typedef UINT32 POINTER_FLAGS;

typedef enum tagPOINTER_BUTTON_CHANGE_TYPE {
	POINTER_CHANGE_NONE,
	POINTER_CHANGE_FIRSTBUTTON_DOWN,
	POINTER_CHANGE_FIRSTBUTTON_UP,
	POINTER_CHANGE_SECONDBUTTON_DOWN,
	POINTER_CHANGE_SECONDBUTTON_UP,
	POINTER_CHANGE_THIRDBUTTON_DOWN,
	POINTER_CHANGE_THIRDBUTTON_UP,
	POINTER_CHANGE_FOURTHBUTTON_DOWN,
	POINTER_CHANGE_FOURTHBUTTON_UP,
	POINTER_CHANGE_FIFTHBUTTON_DOWN,
	POINTER_CHANGE_FIFTHBUTTON_UP,
} POINTER_BUTTON_CHANGE_TYPE;

typedef struct tagPOINTER_INFO {
	POINTER_INPUT_TYPE    pointerType;
	UINT32          pointerId;
	UINT32          frameId;
	POINTER_FLAGS   pointerFlags;
	HANDLE          sourceDevice;
	HWND            hwndTarget;
	POINT           ptPixelLocation;
	POINT           ptHimetricLocation;
	POINT           ptPixelLocationRaw;
	POINT           ptHimetricLocationRaw;
	DWORD           dwTime;
	UINT32          historyCount;
	INT32           InputData;
	DWORD           dwKeyStates;
	UINT64          PerformanceCount;
	POINTER_BUTTON_CHANGE_TYPE ButtonChangeType;
} POINTER_INFO;

typedef UINT32 TOUCH_FLAGS;
#define TOUCH_FLAG_NONE                 0x00000000 // Default

typedef UINT32 TOUCH_MASK;
#define TOUCH_MASK_NONE                 0x00000000 // Default - none of the optional fields are valid
#define TOUCH_MASK_CONTACTAREA          0x00000001 // The rcContact field is valid
#define TOUCH_MASK_ORIENTATION          0x00000002 // The orientation field is valid
#define TOUCH_MASK_PRESSURE             0x00000004 // The pressure field is valid

typedef struct tagPOINTER_TOUCH_INFO {
	POINTER_INFO    pointerInfo;
	TOUCH_FLAGS     touchFlags;
	TOUCH_MASK      touchMask;
	RECT            rcContact;
	RECT            rcContactRaw;
	UINT32          orientation;
	UINT32          pressure;
} POINTER_TOUCH_INFO;

typedef UINT32 PEN_FLAGS;
#define PEN_FLAG_NONE                   0x00000000 // Default
#define PEN_FLAG_BARREL                 0x00000001 // The barrel button is pressed
#define PEN_FLAG_INVERTED               0x00000002 // The pen is inverted
#define PEN_FLAG_ERASER                 0x00000004 // The eraser button is pressed

typedef UINT32 PEN_MASK;
#define PEN_MASK_NONE                   0x00000000 // Default - none of the optional fields are valid
#define PEN_MASK_PRESSURE               0x00000001 // The pressure field is valid
#define PEN_MASK_ROTATION               0x00000002 // The rotation field is valid
#define PEN_MASK_TILT_X                 0x00000004 // The tiltX field is valid
#define PEN_MASK_TILT_Y                 0x00000008 // The tiltY field is valid

typedef struct tagPOINTER_PEN_INFO {
	POINTER_INFO    pointerInfo;
	PEN_FLAGS       penFlags;
	PEN_MASK        penMask;
	UINT32          pressure;
	UINT32          rotation;
	INT32           tiltX;
	INT32           tiltY;
} POINTER_PEN_INFO;

/*
* Flags that appear in pointer input message parameters
*/
#define POINTER_MESSAGE_FLAG_NEW                0x00000001 // New pointer
#define POINTER_MESSAGE_FLAG_INRANGE            0x00000002 // Pointer has not departed
#define POINTER_MESSAGE_FLAG_INCONTACT          0x00000004 // Pointer is in contact
#define POINTER_MESSAGE_FLAG_FIRSTBUTTON        0x00000010 // Primary action
#define POINTER_MESSAGE_FLAG_SECONDBUTTON       0x00000020 // Secondary action
#define POINTER_MESSAGE_FLAG_THIRDBUTTON        0x00000040 // Third button
#define POINTER_MESSAGE_FLAG_FOURTHBUTTON       0x00000080 // Fourth button
#define POINTER_MESSAGE_FLAG_FIFTHBUTTON        0x00000100 // Fifth button
#define POINTER_MESSAGE_FLAG_PRIMARY            0x00002000 // Pointer is primary
#define POINTER_MESSAGE_FLAG_CONFIDENCE         0x00004000 // Pointer is considered unlikely to be accidental
#define POINTER_MESSAGE_FLAG_CANCELED           0x00008000 // Pointer is departing in an abnormal manner

/*
* Macros to retrieve information from pointer input message parameters
*/
#define GET_POINTERID_WPARAM(wParam)                (LOWORD(wParam))
#define IS_POINTER_FLAG_SET_WPARAM(wParam, flag)    (((DWORD)HIWORD(wParam) & (flag)) == (flag))
#define IS_POINTER_NEW_WPARAM(wParam)               IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_NEW)
#define IS_POINTER_INRANGE_WPARAM(wParam)           IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_INRANGE)
#define IS_POINTER_INCONTACT_WPARAM(wParam)         IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_INCONTACT)
#define IS_POINTER_FIRSTBUTTON_WPARAM(wParam)       IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_FIRSTBUTTON)
#define IS_POINTER_SECONDBUTTON_WPARAM(wParam)      IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_SECONDBUTTON)
#define IS_POINTER_THIRDBUTTON_WPARAM(wParam)       IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_THIRDBUTTON)
#define IS_POINTER_FOURTHBUTTON_WPARAM(wParam)      IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_FOURTHBUTTON)
#define IS_POINTER_FIFTHBUTTON_WPARAM(wParam)       IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_FIFTHBUTTON)
#define IS_POINTER_PRIMARY_WPARAM(wParam)           IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_PRIMARY)
#define HAS_POINTER_CONFIDENCE_WPARAM(wParam)       IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_CONFIDENCE)
#define IS_POINTER_CANCELED_WPARAM(wParam)          IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_CANCELED)

/*
* WM_POINTERACTIVATE return codes
*/
#define PA_ACTIVATE                     MA_ACTIVATE
#define PA_NOACTIVATE                   MA_NOACTIVATE

#define UIA_CHECK_ELEMENT(element) if(!element) return UIA_E_ELEMENTNOTAVAILABLE

#define UIA_CHECK_ELEMENT_RETURN(element)          \
	if(!element) return UIA_E_ELEMENTNOTAVAILABLE; \
	else return S_OK 
}
#endif // UI_MACROS_H_