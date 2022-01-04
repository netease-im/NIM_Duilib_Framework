#ifndef UI_CONTROL_LIST_H_
#define UI_CONTROL_LIST_H_

#pragma once

#include "Label.h"
#include "Box/VBox.h"
#include "Box/HBox.h"
#include "Button.h"
#include "CheckBox.h"
#include "Option.h"

namespace ui 
{

typedef int (CALLBACK *PULVCompareFunc)(UINT_PTR, UINT_PTR, UINT_PTR);

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API IListOwner
{
public:
	virtual int GetCurSel() const = 0;
	virtual bool SelectItem(int iIndex, bool bTakeFocus = false, bool bTrigger = true) = 0;
	virtual void HandleMessageTemplate(EventArgs& event) = 0;
	virtual void EnsureVisible(const UiRect& rcItem) = 0;
	virtual void StopScroll() {}
};


/////////////////////////////////////////////////////////////////////////////////////
//

/// 列表容器，用于展示一组数据使用
class UILIB_API ListBox : public ScrollableBox, public IListOwner
{
public:
	ListBox(Layout* pLayout = new VLayout);
	ListBox(const ListBox& r) = delete;
	ListBox& operator=(const ListBox& r) = delete;

	/// 重写父类方法，提供个性化功能，请参考父类声明
	virtual std::wstring GetType() const override;
	virtual UIAControlProvider* GetUIAProvider() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void HandleMessage(EventArgs& event) override;	
	virtual void HandleMessageTemplate(EventArgs& event) override;
	virtual int GetCurSel() const override;
	virtual bool SelectItem(int iIndex, bool bTakeFocus = false, bool bTrigger = true) override;
	virtual void EnsureVisible(const UiRect& rcItem) override;
	virtual void StopScroll() override;
	virtual bool ButtonDown(EventArgs& msg) override;

	/**
	 * @brief 滚动到指定子项位置
	 * @param[in] strItemName 子项名称
	 * @return 成功返回 true，否则为 false，可能控件不存在
	 */
	virtual bool ScrollItemToTop(const std::wstring& strItemName);

	/**
	 * @brief 获取当前位置第一个子项
	 * @return 返回第一个子项指针
	 */
	virtual Control* GetTopItem();
	
	/**
	 * @brief 设置子项的位置索引
	 * @param[in] pControl 子项指针
	 * @param[in] iIndex 索引号
	 * @return 成功返回 true，否则返回 false
	 */
	bool SetItemIndex(Control* pControl, std::size_t iIndex);

	/**
	 * @brief 选中上一项
	 * @return 无
	 */
	void Previous();

	/**
	 * @brief 选中下一项
	 * @return 无
	 */
	void Next();

	/**
	 * @brief 触发选中项的双击事件
	 * @return 无
	 */
	void ActiveItem();

	/**
	 * @brief 追加一个子项到末尾
	 * @param[in] pControl 子项指针
	 * @return 成功返回 true，失败返回 false
	 */
	virtual bool Add(Control* pControl) override;

	/**
	 * @brief 在指定位置之后插入一个子项
	 * @param[in] pControl 子项指针
	 * @param[in] iIndex 要插入的位置索引
	 * @return 成功返回 true，失败返回 false
	 */
    virtual bool AddAt(Control* pControl, std::size_t  iIndex) override;

	/**
	 * @brief 根据子项指针
	 * @param[in] pControl 子项指针
	 * @return 成功返回 true，失败返回 false
	 */
    virtual bool Remove(Control* pControl) override;

	/**
	 * @brief 根据索引移除一个子项
	 * @param[in] iIndex 子项索引
	 * @return 成功返回 true，失败返回 false
	 */
    virtual bool RemoveAt(std::size_t  iIndex) override;

	/**
	 * @brief 移除所有子项
	 * @return 无
	 */
    virtual void RemoveAll() override;

	/**
	 * @brief 排列子项
	 * @param[in] pfnCompare 自定义排序函数
	 * @param[in] dwData 用于子项对比时的数据
	 * @return 成功返回 true，失败返回 false
	 */
	bool SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData);

	/**
	 * @brief 默认的子项对比方法
	 * @param[in] pvlocale 保存 List 指针
	 * @param[in] item1 子项1
	 * @param[in] item2 子项2
	 * @return 返回对比结果
	 */
	static int __cdecl ItemComareFunc(void *pvlocale, const void *item1, const void *item2);
	int __cdecl ItemComareFunc(const void *item1, const void *item2);

	/**
	 * @brief 获取是否随滚动改变选中项设置
	 * @return 返回 true 表示跟随滚动条改变选择项，否则为 false
	 */
	bool GetScrollSelect();

	/**
	 * @brief 设置是否随滚动改变选中项设置
	 * @param[in] bScrollSelect 为 true 是为跟随滚动条改变选中项，false 为不跟随
	 * @return 无
	 */
	void SetScrollSelect(bool bScrollSelect);

	/**
	 * @brief 监听选择子项的事件
	 * @param[in] callback 选择子项时的回调函数
	 * @return 无
	 */
	void AttachSelect(const EventCallback& callback) { OnEvent[kEventSelect] += callback; }

	/**
	 * @brief 在移除一个子项后自动选择下一项
	 * @param[in] bSelectNextItem 为 true 时自动选择下一项，false 为不自动选择
	 * @return 无
	 */
	void SelectNextWhenActiveRemoved(bool bSelectNextItem);
protected:
	bool m_bScrollSelect;
    int m_iCurSel;
	bool m_bSelNextWhenRemoveActive;
	PULVCompareFunc m_pCompareFunc;
	UINT_PTR m_pCompareData;
};

/////////////////////////////////////////////////////////////////////////////////////
//


/// 列表项，用于在列表中展示数据的子项
class UILIB_API ListContainerElement : public OptionTemplate<Box>
{
public:
	ListContainerElement();

	/// 重写父类方法，提供个性化功能，请参考父类声明
	virtual std::wstring GetType() const override;
	virtual UIAControlProvider* GetUIAProvider() override;
	virtual void SetVisible(bool bVisible = true) override;
	virtual void Selected(bool bSelect, bool trigger) override;
	virtual void HandleMessage(EventArgs& event) override;

	/**
	 * @brief 获取父容器
	 * @return 返回父容器指针
	 */
	IListOwner* GetOwner();

	/**
	 * @brief 设置父容器
	 * @param[in] pOwner 父容器指针
	 * @return 无
	 */
	void SetOwner(IListOwner* pOwner);

	/**
	 * @brief 获取当前索引
	 * @return 返回当前索引
	 */
	int GetIndex() const;

	/**
	 * @brief 设置索引
	 * @param[in] iIndex 索引值
	 * @return 无
	 */
	void SetIndex(int iIndex);

	/**
	 * @brief 触发双击事件
	 * @return 无
	 */
	void InvokeDoubleClickEvent();

	/**
	 * @brief 监听控件双击事件
	 * @param[in] callback 收到双击消息时的回调函数
	 * @return 无
	 */
	void AttachDoubleClick(const EventCallback& callback) { OnEvent[kEventMouseDoubleClick] += callback; }

	/**
	 * @brief 监听回车事件
	 * @param[in] callback 收到回车时的回调函数
	 * @return 无
	 */
	void AttachReturn(const EventCallback& callback) { OnEvent[kEventReturn] += callback; }

protected:
	int m_iIndex;
	IListOwner* m_pOwner;
};

} // namespace ui

#endif // UI_CONTROL_LIST_H_
