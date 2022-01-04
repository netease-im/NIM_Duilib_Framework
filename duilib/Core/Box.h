#ifndef UI_CORE_BOX_H_
#define UI_CORE_BOX_H_

#pragma once

namespace ui 
{

class Box;
class UILIB_API Layout
{
public:
	Layout();
	Layout& operator=(const Layout& r) = delete;
	virtual ~Layout() {}

	/**
	 * @brief 设置所有者
	 * @return 无
	 */
	void SetOwner(Box* pOwner);

	/**
	 * @brief 设置浮动状态下的坐标信息
	 * @param[in] pControl 控件句柄
	 * @param[in] rcContainer 要设置的位置信息
	 * @return 返回控件最终的位置信息
	 */
	static CSize SetFloatPos(Control* pControl, UiRect rcContainer);

	/**
	 * @brief 设置布局属性
	 * @param[in] strName 要设置的属性名
	 * @param[in] strValue 要设置的属性值
	 * @return true 设置成功，false 属性不存在
	 */
	virtual bool SetAttribute(const std::wstring& strName, const std::wstring& strValue);

	/**
	 * @brief 调整内部所有控件的位置信息
	 * @param[in] items 控件列表
	 * @param[in] rc 当前容器位置信息
	 * @return 返回排列后最终盒子的宽度和高度信息
	 */
	virtual CSize ArrangeChild(const std::vector<Control*>& items, UiRect rc);

	/**
	 * @brief 根据内部子控件大小调整容器自身大小
	 * @param[in] items 控件列表
	 * @param[in] szAvailable 子控件允许的最大宽度
	 * @return 返回排列后最终盒子的宽度和高度信息
	 */
	virtual CSize AjustSizeByChild(const std::vector<Control*>& items, CSize szAvailable);

	/**
	 * @brief 获取内边距
	 * @return 返回内边距四边的大小
	 */
	virtual UiRect GetPadding() const;

	/**
	 * @brief 设置内边距，相当于设置客户区
	 * @param[in] rcPadding 内边距数据
	 * @param[in] bNeedDpiScale 是否根据 DPI 自适应，默认为 true
	 * @return 无
	 */
	virtual void SetPadding(UiRect rcPadding, bool bNeedDpiScale = true);

	/**
	 * @brief 获取子控件之间的额外边距
	 * @return 返回额外间距的数值
	 */
	virtual int GetChildMargin() const;

	/**
	 * @brief 设置子控件之间的额外边距
	 * @param[in] iMargin 要设置的边距数值
	 * @return 无
	 */
	virtual void SetChildMargin(int iMargin);

	/**
	 * @brief 获取除了内边距外的可用范围
	 * @return 返回可用范围位置信息
	 */
	UiRect GetInternalPos() const;

protected:
	UiRect m_rcPadding;
	int m_iChildMargin;
	Box *m_pOwner;
};


/////////////////////////////////////////////////////////////////////////////////////
//
class ScrollBar;

class UILIB_API Box : public Control
{
public:
	Box(Layout* pLayout = new Layout());
	Box(const Box& r);
	Box& operator=(const Box& r) = delete;
    virtual ~Box();

public:
	/// 重写父类接口，提供个性化功能。方法具体说明请查看 Control 控件             */
	virtual std::wstring GetType() const override;
	virtual UIAControlProvider* GetUIAProvider() override;
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void SetPos(UiRect rc) override;
	virtual void HandleMessageTemplate(EventArgs& msg) override;
	virtual void SetReceivePointerMsg(bool bRecv) override;
	virtual void PaintChild(IRenderContext* pRender, const UiRect& rcPaint) override;
	virtual void SetVisible_(bool bVisible) override;
	virtual void SetInternVisible(bool bVisible = true) override;
	virtual void SetEnabled(bool bEnabled) override;
	virtual CSize EstimateSize(CSize szAvailable) override;
	virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, CPoint scrollPos = CPoint()) override;
	virtual void InvokeLoadImageCache() override;
	virtual void UnLoadImageCache() override;
	virtual void ClearImageCache() override;
	virtual UINT GetControlFlags() const override;

	/// 容器自有方法
	/**
	 * @brief 查找指定子控件
	 * @param[in] pstrSubControlName 子控件名称
	 * @return 返回子控件指针
	 */
	Control* FindSubControl(const std::wstring& pstrSubControlName);

	/**
	 * @brief 查找下一个可选控件的索引（面向 list、combo）
	 * @param[in] iIndex 指定要起始查找的索引
	 * @param[in] bForward true 为递增查找， false 为递减查找
	 * @return 下一个可选控件的索引，返回 -1 为没有可选控件
	 */
	virtual int FindSelectable(int iIndex, bool bForward = true) const;

	/**
	 * @brief 根据索引查找指定控件
	 * @param[in] iIndex 控件索引
	 * @return 返回控件指针
	 */
	virtual Control* GetItemAt(std::size_t iIndex) const;

	/**
	 * @brief 根据控件指针获取索引
	 * @param[in] pControl 控件指针
	 * @return 返回 pControl 所指向的控件索引
	 */
	virtual int GetItemIndex(Control* pControl) const;

	/**
	 * @brief 设置控件索引（内部会重新排列控件位置）
	 * @param[in] pControl 控件指针
	 * @param[in] iIndex 要设置的索引值
	 * @return 返回 true 设置成功，false 设置失败
	 */
	virtual bool SetItemIndex(Control* pControl, std::size_t iIndex);

	/**
	 * @brief 获取子控件数量
	 * @return 返回子控件数量
	 */
	virtual int GetCount() const;

	/**
	 * @brief 添加一个控件到容器中
	 * @param[in] pControl 控件指针
	 * @return 返回 true 添加成功，false 为添加失败
	 */
	virtual bool Add(Control* pControl);

	/**
	 * @brief 向指定位置添加一个控件
	 * @param[in] pControl 控件指针
	 * @param[in] iIndex 在该索引之后插入控件
	 * @return 返回 true 为添加成功，false 为添加失败
	 */
	virtual bool AddAt(Control* pControl, std::size_t iIndex);

	/**
	 * @brief 根据控件指针从容器中移除一个控件
	 * @param[in] pControl 控件的指针
	 * @return 返回 true 为移除成功，false 为移除失败（控件可能不存在）
	 */
	virtual bool Remove(Control* pControl);

	/**
	 * @brief 根据控件索引从容器中移除一个控件
	 * @param[in] iIndex 要移除的控件索引
	 * @return 返回 true 为移除成功，false 为移除失败（索引值太小或超出了子控件总数）
	 */
	virtual bool RemoveAt(std::size_t iIndex);

	/**
	 * @brief 移除所有子控件
	 * @return 无
	 */
	virtual void RemoveAll();

	/**
	 * @brief 检查是否包含某一个控件
	 * @param[in] pControl 控件的指针
	 * @return 返回 true 包含，false 不包含
	 */
	bool HasItem(Control* pControl) const;

	/**
	 * @brief 交换子控件位置
	 * @param[in] pChild1 子控件1 指针
	 * @param[in] pChild2 子控件2 指针
	 * @return 无
	 */
	void SwapChild(Control* pChild1, Control* pChild2);

	/**
	 * @brief 重置一个子控件位置
	 * @param[in] pChild 控件指针
	 * @param[in] iIndex 要重置的位置
	 * @return 无
	 */
	void ResetChildIndex(Control* pChild, std::size_t iIndex);

	/**
	 * @brief 判断是否自动销毁
	 * @return true 为自动销毁，false 为不自动销毁
	 */
    virtual bool IsAutoDestroy() const;

	/**
	 * @brief 设置控件是否自动销毁
	 * @param[in] bAuto true 为自动销毁，false 为不自动销毁
	 * @return 无
	 */
    virtual void SetAutoDestroyChild(bool bAuto);

	/**
	 * @brief 判断窗口关闭后是否自动销毁
	 * @return true 为自动销毁，false 为不自动销毁
	 */
    virtual bool IsDelayedDestroy() const;

	/**
	 * @brief 设置窗口关闭后是否自动销毁
	 * @param[in] bDelayed true 为自动销毁，false 为不自动销毁
	 * @return 无
	 */
    virtual void SetDelayedDestroy(bool bDelayed);

	/**
	 * @brief 获取容器是否响应鼠标操作
	 * @return true 为响应，false 为不响应
	 */
    virtual bool IsMouseChildEnabled() const;

	/**
	 * @brief 设置容器响应鼠标操作
	 * @param[in] bEnable 设置为 true 为响应鼠标操作，设置为 false 为不响应，默认为 true
	 * @return 无
	 */
    virtual void SetMouseChildEnabled(bool bEnable = true);

	/**
	 * @brief 获取容器布局对象指针
	 * @return 返回容器关联的布局对象指针
	 */
	virtual Layout* GetLayout() const;

	/**
	 * @brief 重新关联布局对象
	 * @param[in] pLayout 布局对象指针
	 * @return 无
	 */
	virtual void ReSetLayout(Layout* pLayout);

	/**
	 * @brief 获取内边距的位置信息
	 * @return 返回内边距的位置信息
	 */
	virtual	UiRect GetPaddingPos() const;

	/**
	 * @brief 绑定事件处理函数
	 * @param[in] eventType 事件类型
	 * @param[in] callback 指定回调函数
	 * @return void 无
	 */
 	void AttachBubbledEvent(EventType eventType, const EventCallback& callback)	{ OnBubbledEvent[eventType] += callback; }
 
	/**
	 * @brief 解绑事件处理函数
	 * @param[in] eventType 事件类型
	 * @return void 无
	 */
	void DetachBubbledEvent(EventType eventType);
private:
	friend WindowBuilder;

	/**
	 * @brief 绑定 XML 中编写的 Event 和 BubbleEvent 事件的处理函数
	 * @param[in] eventType 事件类型
	 * @param[in] callback 指定回调函数
	 * @return void 无
	 */
	void AttachXmlBubbledEvent(EventType eventType, const EventCallback& callback) { OnXmlBubbledEvent[eventType] += callback; }

	EventMap OnXmlBubbledEvent;

protected:
	std::unique_ptr<Layout> m_pLayout;
	bool m_bAutoDestroy;
	bool m_bDelayedDestroy;
	bool m_bMouseChildEnabled;
	std::vector<Control*> m_items;
	EventMap OnBubbledEvent;
};

 /// 带有垂直或水平滚动条的容器，使容器可以容纳更多内容
class UILIB_API ScrollableBox : public Box
{
public:
	ScrollableBox(Layout* pLayout);
	ScrollableBox(const ScrollableBox& r);
	ScrollableBox& operator=(const ScrollableBox& r) = delete;

	virtual std::wstring GetType() const override;
	virtual UIAControlProvider* GetUIAProvider() override;
	virtual void SetAttribute(const std::wstring& pstrName, const std::wstring& pstrValue) override;
	virtual void SetPos(UiRect rc) override;
	virtual void HandleMessage(EventArgs& event) override;
	virtual bool MouseEnter(EventArgs& msg) override;
	virtual bool MouseLeave(EventArgs& msg) override;
	virtual void PaintChild(IRenderContext* pRender, const UiRect& rcPaint) override;
	virtual void SetMouseEnabled(bool bEnable = true) override;
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit) override;
	virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, CPoint scrollPos = CPoint()) override;
	virtual void ClearImageCache() override;

	/**
	 * @brief 获取滚动条位置
	 * @return 返回滚动条的位置信息
	 */
	virtual CSize GetScrollPos() const;

	/**
	 * @brief 获取滚动条的范围
	 * @return 返回滚动条的范围信息
	 */
	virtual CSize GetScrollRange() const;

	/**
	 * @brief 设置滚动条位置
	 * @param[in] szPos 要设置的位置数据
	 * @return 无
	 */
	virtual void SetScrollPos(CSize szPos);

	/**
	 * @brief 设置滚动条 Y 轴坐标
	 * @param[in] y 要设置的 Y 轴坐标数值
	 * @return 无
	 */
	virtual void SetScrollPosY(int y);
	
	/**
	 * @brief 设置滚动条 X 轴坐标
	 * @param[in] x 要设置的 X 轴坐标数值
	 * @return 无
	 */
    virtual void SetScrollPosX(int x);
	
	/**
	 * @brief 向上滚动滚动条
	 * @param[in] deltaValue 滚动距离，默认为 DUI_NOSET_VALUE
 	 * @param[in] withAnimation 是否附带动画效果，默认为 true
	 * @return 无
	 */
	virtual void LineUp(int detaValue = DUI_NOSET_VALUE, bool withAnimation = true);
	
	/**
	 * @brief 向下滚动滚动条
	 * @param[in] deltaValue 滚动距离，默认为 DUI_NOSET_VALUE
 	 * @param[in] withAnimation 是否附带动画效果，默认为 true
	 * @return 无
	 */
	virtual void LineDown(int detaValue = DUI_NOSET_VALUE, bool withAnimation = true);
	
	/**
	 * @brief 向左滚动滚动条
	 * @param[in] deltaValue 滚动距离，默认为 DUI_NOSET_VALUE
	 * @return 无
	 */
    virtual void LineLeft(int detaValue = DUI_NOSET_VALUE);
	
	/**
	 * @brief 向右滚动滚动条
	 * @param[in] deltaValue 滚动距离，默认为 DUI_NOSET_VALUE
	 * @return 无
	 */
    virtual void LineRight(int detaValue = DUI_NOSET_VALUE);
	/**
	 * @brief 向上滚动一个页面大小的距离
	 * @return 无
	 */
	virtual void PageUp();

	/**
	 * @brief 向下滚动一个页面大小的距离
	 * @return 无
	 */
	virtual void PageDown();

	/**
	 * @brief 回到滚动条最上方
	 * @return 无
	 */
	virtual void HomeUp();

	/**
	 * @brief 滚动到最下方位置
	 * @param[in] arrange 是否重置滚动条位置，默认为 true
	 * @param[in] withAnimation 是否包含动画特效，默认为 true
	 * @return 无
	 */
	virtual void EndDown(bool arrange = true, bool withAnimation = true);
	/**
	 * @brief 向左滚动一个页面大小的距离
	 * @return 无
	 */
	virtual void PageLeft();

	/**
	 * @brief 向右滚动一个页面大小的距离
	 * @return 无
	 */
	virtual void PageRight();

	/**
	 * @brief 滚动到最左侧
	 * @return 无
	 */
	virtual void HomeLeft();

	/**
	 * @brief 滚动到最右侧
	 * @return 无
	 */
	virtual void EndRight();

	/**
	 * @brief 触摸向上滚动（响应 WM_TOUCH 消息）
	 * @param[in] deltaValue 滚动距离
	 * @return 无
	 */
	virtual void TouchUp(int deltaValue);

	/**
	 * @brief 触摸向下滚动（响应 WM_TOUCH 消息）
	 * @param[in] deltaValue 滚动距离
	 * @return 无
	 */
	virtual void TouchDown(int deltaValue);

	/**
	 * @brief 启用滚动条
	 * @param[in] bEnableVertical 是否启用垂直滚动条，默认为 true
	 * @param[in] bEnableHorizontal 是否启用水平滚动条，默认为 true
	 * @return 无
	 */
	virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);

	/**
	 * @brief 获取垂直滚动条对象指针
	 * @return 返回垂直滚动条对象指针
	 */
	virtual ScrollBar* GetVerticalScrollBar() const;

	/**
	 * @brief 获取水平滚动条对象指针
	 * @return 返回水平滚动条对象指针
	 */
	virtual ScrollBar* GetHorizontalScrollBar() const;

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	virtual void ProcessVScrollBar(UiRect rc, int cyRequired);

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	virtual void ProcessHScrollBar(UiRect rc, int cxRequired);

	/**
	 * @brief 判断垂直滚动条是否有效
	 * @return 返回 true 表示有效，否则 false 为无效
	 */
	bool IsVScrollBarValid() const;

	/**
	 * @brief 判断水平滚动条是否有效
	 * @return 返回 true 表示有效，否则 false 为无效
	 */
	bool IsHScrollBarValid() const;

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	void ReomveLastItemAnimation();

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	void PlayRenderOffsetYAnimation(int nRenderY);

	/**
	 * @brief 是否已经在底部
	 * @return 返回 true 表示已经在底部，否则为 false
	 */
	bool IsAtEnd() const;

	/**
	 * @brief 是否锁定到底部
	 * @return 回 true 表示锁定在底部，否则为 false
	 */
	bool IsHoldEnd() const;

	/**
	 * @brief 设置滚动条是否始终锁定到底部位置
	 * @param[in] bHoldEnd 设置 true 表示锁定，false 为不锁定
	 * @return 无
	 */
	void SetHoldEnd(bool bHoldEnd);
	
	/**
	 * @brief 获取垂直滚动条滚动步长
	 * @return 返回滚动步长
	 */
	int GetVerScrollUnitPixels() const;

	/**
	 * @brief 设置垂直滚动条滚动步长
	 * @param[in] nUnitPixels 要设置的步长
	 * @return 无
	 */
	void SetVerScrollUnitPixels(int nUnitPixels);

	/**
	* @brief 获取横向滚动条滚动步长
	* @return 返回滚动步长
	*/
	int GetHorScrollUnitPixels() const;

	/**
	* @brief 设置横向滚动条滚动步长
	* @param[in] nUnitPixels 要设置的步长
	* @return 无
	*/
	void SetHorScrollUnitPixels(int nUnitPixels);

	/**
	 * @brief 获取容器的滚动条是否悬浮在子控件上面
	 * @return 返回 true 表示悬浮在滚动条上，否则为 false
	 */
	bool GetScrollBarFloat() const;

	/**
	 * @brief 设置容器的滚动条是否悬浮在子控件上面
	 * @param[in] bScrollBarFloat true 表示悬浮在滚动条上，false 表示不悬浮在控件上
	 * @return 无
	 */
	void SetScrollBarFloat(bool bScrollBarFloat);

	/**
	 * @brief 获取容器的滚动条是否在左侧显示
	 * @return 返回 true 表示在左侧，false 为右侧
	 */
	bool GetVScrollBarLeftPos() const;

	/**
	* @brief 设置容器的滚动条是否在左侧显示
	* @param[in] bLeftPos true 表示在左侧，false 为右侧
	* @return 无
	*/
	void SetVScrollBarLeftPos(bool bLeftPos);

	/**
	 * @brief 获取滚动条的外边距
	 * @return 返回边距信息 
	 */
	UiRect GetScrollBarPadding() const;

	/**
	 * @brief 设置滚动条的外边距，可以让滚动条不占满容器
	 * @param[in] rcScrollBarPadding 要设置的边距
	 * @return 无
	 */
	void SetScrollBarPadding(UiRect rcScrollBarPadding);

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	bool GetDefaultDisplayScrollbar() const;

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	void SetDefaultDisplayScrollbar(bool bDefaultDisplay);

	/**
	 * @brief 监听滚动条位置变化事件
	 * @param[in] callback 有变化后通知的回调函数
	 * @return 无
	 */
	void AttachScrollChange(const EventCallback& callback) { OnEvent[kEventScrollChange] += callback; }
protected:

	/**
	 * @brief 计算所需的尺寸
	 * @param[in] rc 当前位置信息
	 * @return 返回所需尺寸大小
	 */
	virtual CSize CalcRequiredSize(const UiRect& rc);

	/**
	 * @brief 加载图片缓存，仅供 ScrollableBox 内部使用
	 * @param[in] bFromTopLeft 暂无意义
	 * @return 无
	 */
	virtual void LoadImageCache(bool bFromTopLeft);
private:
	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	void SetPosInternally(UiRect rc);

protected:
	std::unique_ptr<ScrollBar> m_pVerticalScrollBar;
	std::unique_ptr<ScrollBar> m_pHorizontalScrollBar;

	int m_nVerScrollUnitPixels;
    int m_nHerScrollUnitPixels;
	bool m_bScrollProcess; // 防止SetPos循环调用
	bool m_bHoldEnd;
	bool m_bScrollBarFloat;
	bool m_bDefaultDisplayScrollbar;
	bool m_bVScrollBarLeftPos;
	UiRect m_rcScrollBarPadding;

	CPoint m_ptLastTouchPos;
	AnimationPlayer m_scrollAnimation;
	AnimationPlayer m_renderOffsetYAnimation;
};

} // namespace ui

#endif // UI_CORE_BOX_H_
