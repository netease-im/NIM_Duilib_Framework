#pragma once

/** @class ITrayIconDelegate
  * @brief 托盘图标事件处理接口
  * @copyright (c) 2015, NetEase Inc. All rights reserved
  * @date 2016/10/12
  */
class ITrayIconDelegate
{
public:
	virtual void LeftClick(){}
	virtual void LeftDoubleClick(){}
	virtual void RightClick(){}
	virtual void RightDoubleClick(){}
};

/** @class TrayIcon
  * @brief 托盘图标管类
  * @copyright (c) 2015, NetEase Inc. All rights reserved
  * @date 2016/10/12
  */
class TrayIcon
{
public:
	SINGLETON_DEFINE(TrayIcon);
	TrayIcon();
	virtual ~TrayIcon();

public:
	/**
	* 初始化托盘图标
	* @param[in] tray_icon_delegate 指定托盘图标事件处理接口
	* @return void	无返回值
	*/
	void Init(ITrayIconDelegate* tray_icon_delegate);

	/**
	* 销毁托盘图标
	* @return void	无返回值
	*/
	void Destroy();

	/**
	* 设置托盘图标和提示文本
	* @param[in] icon 图标句柄
	* @param[in] tray_icon_text 提示文本
	* @return void	无返回值
	*/
	void SetTrayIcon(HICON icon, const std::wstring& tray_icon_text);

	/**
	* 从资源中加载一组图标资源并缓存起来
	* @param[in] icon_res_start_index 要图标资源的第一个资源的ID
	* @param[in] count 要加载的图标的数量
	* @return void	无返回值
	*/
	void LoadIconList(int icon_res_start_index, int count);

	/**
	* 设置托盘动画中，用于动画效果图标帧的索引号
	* @param[in] aniIndex 图标帧索引号数组
	* @param[in] anim_escape_time 动画间隔时间
	* @return void	无返回值
	*/
	void SetAnimateTray(const std::vector<int>& aniIndex, int anim_escape_time);

	/**
	* 开始托盘图标动画效果
	* @return void	无返回值
	*/
	void StartTrayIconAnimate();

	/**
	* 停止托盘图标动画效果
	* @return void	无返回值
	*/
	void StopTrayIconAnimate();

	/**
	* 获取任务栏图标创建完成的消息的消息值
	* @return UINT 消息值
	*/
	UINT GetTaskbarCreatedMsgId(){return trayicon_msgid_;}

private:
	/**
	* 处理内部窗口消息
	* @param[in] hWnd 窗口句柄
	* @param[in] uMsg 消息值
	* @param[in] wParam 附加参数
	* @param[in] lParam 附加参数
	* @return LRESULT 消息处理结果
	*/
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	* 清除托盘动画相关信息
	* @return void	无返回值
	*/
	void ClearAnimateInfo();

	/**
	* 处理WM_TIMER消息
	* @param[in] event_id 事件id
	* @return void	无返回值
	*/
	void OnTimer(UINT event_id);

	/**
	* 根据图标索引设置托盘图标
	* @param[in] index
	* @return void	无返回值
	*/
	void SetTrayIconIndex(int index);

	/**
	* 恢复托盘图标到默认值
	* @return BOOL	TRUE：回复成功， FALSE：恢复失败
	*/
	BOOL RestoreTrayIcon();

	/**
	* 修改托盘图标
	* @param[in] uTrayID 托盘ID
	* @param[in] dwTrayMsg 指定用于接收托盘消息的回调消息值
	* @param[in] hTrayIcon 图标句柄
	* @param[in] strTrayText 托盘提示文本
	* @return void	无返回值
	*/
	void ModifyTrayIcon(UINT uTrayID, DWORD dwTrayMsg, HICON hTrayIcon, const std::wstring& strTrayText);

private:
	HWND	wnd_;

	HICON			icon_;
	std::wstring	tray_icon_text_;
	UINT			trayicon_msgid_;

	std::map<int, HICON>	index_icon_map_;
	std::vector<int>		anim_index_array_;
	int						anim_escape_time_;
	int						anim_current_index_;

	ITrayIconDelegate		*tray_icon_delegate_;

};
