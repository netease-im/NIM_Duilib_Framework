#pragma once

#include "msgbox/msgbox.h"
#include "menu/ui_menu.h"
#include "modal_wnd/async_do_modal.h"
#include "modal_wnd/file_dialog_ex.h"
#include "windows_manager/windows_manager.h"
#include "windows_manager/window_ex.h"
#include "shadow_wnd/shadow_wnd.h"
#include "toast/toast.h"
#include "popover/popover.h"
#include "popover/notification.h"

#if 0
// CEF 控件需要使用第三方 libcef_dll_wrapper 封装层
// 需要引入时请从项目模块直接引入
#include "cef_control/control/cef_control.h"
#include "cef_control/control/cef_native_control.h"
#include "cef_control/control/cef_control_base.h"
#include "cef_control/manager/cef_manager.h"
#endif