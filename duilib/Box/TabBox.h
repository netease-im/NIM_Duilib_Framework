#ifndef UI_CORE_TABBOX_H_
#define UI_CORE_TABBOX_H_

#pragma once

namespace ui
{

class UILIB_API TabBox : public Box
{
public:
	TabBox(Layout* pLayout = new Layout());

	// 用于初始化xml属性
	virtual void DoInit() override;

	/// 重写父类方法，提供个性化功能，请参考父类声明
	virtual std::wstring GetType() const override;
	virtual bool Add(Control* pControl) override;
	virtual bool AddAt(Control* pControl, std::size_t iIndex) override;
	virtual bool Remove(Control* pControl) override;
	virtual bool RemoveAt(std::size_t iIndex) override;
	virtual void RemoveAll() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/**
	 * @brief 获取当前选择项
	 * @return 返回当前选择项索引
	 */
	int GetCurSel() const;

	/**
	 * @brief 根据控件索引选择一个子项
	 * @param[in] iIndex 要选择的子项索引
	 * @return 成功返回 true，否则返回 false
	 */
	bool SelectItem(int iIndex);

	/**
	 * @brief 根据控件指针选择一个子项
	 * @param[in] pControl 要选择的子项指针
	 * @return 成功返回 true，否则返回 false
	 */
	bool SelectItem(Control* pControl);

	/**
	 * @brief 根据控件名称选择一个子项
	 * @param[in] pControlName 要选择的控件名称
	 * @return 成功返回 true，否则返回 false
	 */
	bool SelectItem(const std::wstring& pControlName);

	/**
	 * @brief 设置是否显示动画效果
	 * @param[in] bFadeSwitch 设置为 true 则显示，false 为不显示动画
	 * @return 无
	 */
	void SetFadeSwitch(bool bFadeSwitch);

	/**
	 * @brief 判断是否需要显示动画效果
	 * @return 返回 true 显示动画，false 为不显示动画效果
	 */
	bool IsFadeSwitch() { return m_bFadeSwith; }
	
	/** @brief 监听选择时间
	  * @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	  * @return 无
	  */
	void AttachSelect(const EventCallback& callback) { OnEvent[kEventSelect] += callback; }
protected:
	/**
	 * @brief 显示一个 TAB 项
	 * @param[in] it TAB 项索引
	 * @return 无
	 */
	void ShowTabItem(std::size_t it);

	/**
	 * @brief 隐藏一个 TAB 项
	 * @param[in] it TAB 项索引
	 * @return 无
	 */
	void HideTabItem(std::size_t it);

protected:
	int m_iCurSel;
	int m_iInitSel;
	bool m_bIsInit;
	bool m_bFadeSwith;
};

}
#endif // UI_CORE_TABBOX_H_
