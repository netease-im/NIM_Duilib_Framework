// Copyright 2017, NetEase (Hangzhou) Network Co., Ltd. All rights reserved.
//
// zqw
// 2015/7/22
//
// virtual ListBox class
#pragma once

namespace ui
{

/**
 * @brief 虚拟列表接口类
 * 提供开发者对子项数据管理个性化定制
 */
class VirtualListInterface
{
public:
	/**
	 * @brief 创建一个子项
	 * @return 返回创建后的子项指针
	 */
	virtual Control* CreateElement() = 0;

	/**
	 * @brief 填充指定子项
	 * @param[in] control 子项控件指针
	 * @param[in] index 索引
	 * @return 返回创建后的子项指针
	 */
	virtual void FillElement(Control *control, int index) = 0;

	/**
	 * @brief 获取子项总数
	 * @return 返回子项总数
	 */
	virtual int GetElementCount() = 0;
};

/**
 * @brief 虚拟列表
 */
class UILIB_API VirtualVLayout : public VLayout
{
protected:
	virtual ui::CSize ArrangeChild(const std::vector<ui::Control*>& items, ui::UiRect rc);
};

class UILIB_API VirtualHLayout : public HLayout
{
protected:
	virtual ui::CSize ArrangeChild(const std::vector<ui::Control*>& items, ui::UiRect rc);
};

class UILIB_API VirtualListBox : public ListBox
{
	friend class VirtualVLayout;
	friend class VirtualHLayout;
public:
	VirtualListBox(ui::Layout* pLayout = new VirtualVLayout);

	virtual std::wstring GetType() const override;

	/**
	 * @brief 设置数据代理对象
	 * @param[in] pProvider 开发者需要重写 VirtualListInterface 的接口来作为数据代理对象
	 * @return 无
	 */
	virtual void SetDataProvider(VirtualListInterface *pProvider);

	/**
	 * @brief 设置子项高度
	 * @param[in] nHeight 高度值
	 * @return 无
	 */
	virtual void SetElementHeight(int nHeight);

	/**
	 * @brief 初始化子项
	 * @param[in] nMaxItemCount 初始化数量，默认 50
	 * @return 无
	 */
	virtual void InitElement(int nMaxItemCount = 50);

	/**
	 * @brief 设置列表的纵横方向
	 * @param[in] direction 方向
	 * @return 无
	 */
	enum ListDirection
	{
		kListHorizontal = 0,
		kListVertical = 1
	};
	void SetDirection(ListDirection direction);

	/**
	 * @brief 刷新列表
	 * @return 无
	 */
	virtual void Refresh();

	/**
	 * @brief 删除所有子项
	 * @return 无
	 */
	virtual void RemoveAll() override;

	/**
	 * @brief 设置是否强制重新布局
	 * @param[in] bForce 设置为 true 为强制，否则为不强制
	 * @return 无
	 */
	void SetForceArrange(bool bForce);

	/**
	 * @brief 获取当前所有可见控件的索引
	 * @param[out] collection 索引列表
	 * @return 无
	 */
    void GetDisplayCollection(std::vector<int>& collection);

	/**
	 * @brief 让控件在可见范围内
	 * @param[in] iIndex 控件索引
	 * @param[in] bToTop 是否在最上方
	 * @return 无
	 */
    void EnsureVisible(int iIndex, bool bToTop = false);

protected:
	/// 重写父类接口，提供个性化功能
	virtual void SetScrollPos(ui::CSize szPos) override;
	virtual void HandleMessage(ui::EventArgs& event) override;
	virtual void SetPos(UiRect rc) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/**
	 * @brief 重新布局子项
	 * @param[in] bForce 是否强制重新布局
	 * @return 无
	 */
	void ReArrangeChild(bool bForce);

	/**
	 * @brief 添加一个子项
	 * @param[in] iIndex 要插入的位置
	 * @return 无
	 */
	void AddElement(int iIndex);

	/**
	 * @brief 移除一个子项
	 * @param[in] iIndex 要移除的子项索引
	 * @return 无
	 */
	void RemoveElement(int iIndex);

	/**
	 * @brief 键盘按下通知
	 * @param[in] ch 按键
	 * @return 无
	 */
	virtual void OnKeyDown(TCHAR ch) {}

	/**
	 * @brief 键盘弹起通知
	 * @param[in] ch 按键
	 * @return 无
	 */
	virtual void OnKeyUp(TCHAR ch) {}

private:
	enum ScrollDirection
	{
		kScrollUp = -1,
		kScrollDown = 1
	};

	/**
	 * @brief 创建一个子项
	 * @return 返回创建后的子项指针
	 */
	Control* CreateElement();

	/**
	 * @brief 填充指定子项
	 * @param[in] control 子项控件指针
	 * @param[in] index 索引
	 * @return 返回创建后的子项指针
	 */
	void FillElement(Control *pControl, int iIndex);

	/**
	 * @brief 获取元素总数
	 * @return 返回元素总指数
	 */
	int GetElementCount();

	/**
	 * @brief 使用默认布局
	 * @return 成功返回 true，否则返回 false
	 */
	bool UseDefaultLayout();

	/**
	 * @brief 得到n个元素对应的高度和，
	 * @param[in] nCount 要得到多少元素的高度，-1表示全部元素
	 * @return 返回指定数量元素的高度和
	 */
    int CalcElementsHeight(int nCount);

	/**
	 * @brief 得到可见范围内第一个元素的前一个元素索引
	 * @param[out] bottom 返回上一个元素的 bottom 值
	 * @return 返回上一个元素的索引
	 */
	int GetTopElementIndex(int &bottom);

	/**
	 * @brief 判断某个元素是否在可见范围内
	 * @param[in] iIndex 元素索引
	 * @return 返回 true 表示可见，否则为不可见
	 */
    bool IsElementDisplay(int iIndex);

    /**
     * @brief 判断是否要重新布局
	 * @param[out] direction 向上滚动还是向下滚动
	 * @return true 为需要重新布局，否则为 false
     */
	bool NeedReArrange(ScrollDirection &direction);

private:
	VirtualListInterface *m_pDataProvider;
	int m_nElementHeight;	// 每个项的在列表方向的长度
	int m_nMaxItemCount;	// 列表真实控件数量上限  
	int m_nOldYScrollPos;
	bool m_bArrangedOnce;
	bool m_bForceArrange;	// 强制布局标记

	ListDirection m_eDirection = kListVertical;
};

}