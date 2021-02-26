#ifndef UI_CONTROL_BUTTON_H_
#define UI_CONTROL_BUTTON_H_

#pragma once

namespace ui
{
template<typename InheritType = Control>
class UILIB_API ButtonTemplate : public LabelTemplate<InheritType>
{
public:
	ButtonTemplate();

	/// 重写父类方法，提供个性化功能，请参考父类声明
	virtual void Activate() override;
	virtual void HandleMessage(EventArgs& event) override;
	virtual UINT GetControlFlags() const override;

	/**
	 * @brief 绑定鼠标点击处理函数
	 * @param[in] callback 要绑定的回调函数
	 * @return 无
	 */
	void AttachClick(const EventCallback& callback)	{ this->OnEvent[kEventClick] += callback;	}
};

#include "ButtonImpl.h"

typedef ButtonTemplate<Control> Button;
typedef ButtonTemplate<Box> ButtonBox;

}	// namespace ui

#endif // UI_CONTROL_BUTTON_H_