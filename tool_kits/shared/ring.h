#pragma once
#include <Vfw.h>

enum RingType
{
	RING_NEW_MESSAGE = 0,
	RING_VOIP_CONNTETING,
	RING_VOIP_NORESPONSE,
	RING_VOIP_PEERBUSY,
	RING_VOIP_PEERREJECT,
	RING_VOIP_RING,
};

namespace shared
{
/** @class Ring
	* @brief 提示音播放类
	* @copyright (c) 2016, NetEase Inc. All rights reserved
	* @date 2016/09/18
	*/
class Ring
{
public:
	Ring();
	virtual ~Ring();

	/**
	* 初始化提示音播放窗口
	* @param[in] hwnd 父窗口句柄
	* @return bool true 成功，false 失败
	*/
	bool Init(HWND hwnd);

	/**
	* 播放提示音
	* @param[in] type 提示类型
	* @param[in] replay 是否重复提示
	* @return void	无返回值
	*/
	void Play(RingType type, bool replay = false);

	/**
	* 重放提示音
	* @return void	无返回值
	*/
	void Replay();

	/**
	* 停止提示音
	* @return void	无返回值
	*/
	void Stop();

	/**
	* 提示窗口消息处理函数
	* @param[in] uMsg 消息类型
	* @param[in] wParam 附加参数
	* @param[in] lParam 附加参数
	* @return LRESULT 返回值
	*/
	LRESULT Notify(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	/**
	* 消息提示音窗口
	* @return void	无返回值
	*/
	void Destroy();
private:
	HWND hwnd_;
	bool replay_;
};
}