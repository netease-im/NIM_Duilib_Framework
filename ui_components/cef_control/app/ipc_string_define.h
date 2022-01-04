/** @file ipc_string_define.h
  * @brief 定义Render进程与Browser进程通信所用常量
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2016/7/19
*/
#pragma once

namespace nim_comp
{
	static const char kFocusedNodeChangedMessage[]	= "FocusedNodeChanged";		// web页面中获取焦点的元素改变
	static const char kExecuteCppCallbackMessage[]	= "ExecuteCppCallback";		// 执行 C++ 的消息回调函数
	static const char kCallCppFunctionMessage[]		= "CallCppFunction";		// web调用C++接口接口的通知
	static const char kExecuteJsCallbackMessage[]	= "ExecuteJsCallback";		// web调用C++接口接口的通知
	static const char kCallJsFunctionMessage[]		= "CallJsFunction";			// C++ 调用 JavaScript 通知
}
