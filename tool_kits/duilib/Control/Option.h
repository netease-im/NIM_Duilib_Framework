#ifndef UI_CONTROL_OPTION_H_
#define UI_CONTROL_OPTION_H_

#pragma once

namespace ui
{

template<typename InheritType = Control>
class UILIB_API OptionTemplate : public CheckBoxTemplate<InheritType>
{
public:
	OptionTemplate();
	~OptionTemplate();
		
	/// 重写父类方法，提供个性化功能，请参考父类声明
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void Selected(bool bSelected, bool bTriggerEvent = false) override;
	virtual void Activate() override;

	/**
	 * @brief 获取所属组名称
	 * @return 返回组名称
	 */
	virtual  std::wstring GetGroup() const;

	/**
	 * @brief 设置所属组
	 * @param[in] strGroupName 组名称
	 * @return 无
	 */
	virtual void SetGroup(const std::wstring& strGroupName);

protected:
	std::wstring	m_sGroupName;
};

#include "OptionImpl.h"

typedef OptionTemplate<Control> Option;
typedef OptionTemplate<Box> OptionBox;

}

#endif // UI_CONTROL_OPTION_H_
