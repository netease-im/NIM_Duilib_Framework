#ifndef UI_UTILS_ONSCREENKEYBOARDMANAGER_H_
#define UI_UTILS_ONSCREENKEYBOARDMANAGER_H_

#pragma once

namespace ui
{
/** @class OnScreenKeyboardManager
  * @brief D¨¦?a?¨¹?¨¬1¨¹¨¤¨ª¨¤¨¤
  * @copyright (c) 2018, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2018/08/28
  */
class UILIB_API OnScreenKeyboardManager : public virtual nbase::SupportWeakCallback
{
public:
	static OnScreenKeyboardManager* GetInstance();

	/**
	  * D¨¦?a?¨¹?¨¬¨º?¡¤??y?¨²??¨º?
	  * @return bool
	  */
	bool IsVisible();

	/**
	  * ??¨º?D¨¦?a?¨¹?¨¬
	  * @param[in] show ¨º?¡¤???¨º?
	  * @return void
	  */
	void ShowOSK(bool show);

private:
	OnScreenKeyboardManager();
	~OnScreenKeyboardManager() {};
	OnScreenKeyboardManager(const OnScreenKeyboardManager&) = delete;
	OnScreenKeyboardManager& operator = (const OnScreenKeyboardManager&) = delete;

	void TimerCallback();
	void StartTabTip();

private:
	bool m_bShow = false;
};
}
#endif //UI_UTILS_ONSCREENKEYBOARDMANAGER_H_