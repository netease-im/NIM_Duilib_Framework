#ifndef UI_CORE_PLACE_HOLDER_H_
#define UI_CORE_PLACE_HOLDER_H_

#pragma once

namespace ui 
{

class UILIB_API PlaceHolder : public virtual nbase::SupportWeakCallback
{
public:
	PlaceHolder();
	PlaceHolder(const PlaceHolder& r);
	PlaceHolder& operator=(const PlaceHolder& r) = delete;
	virtual ~PlaceHolder();

	/**
	 * @brief 获取父容器指针
	 * @return 返回父容器指针
	 */
	Box* GetParent() const { return m_pParent;}

	/**
	 * @brief 根据名称获取祖先容器指针
	 * @param[in] strName 要获取的祖先容器名称
	 * @return 返回祖先容器指针
	 */
	Box* GetAncestor(const std::wstring& strName);

	/**
	 * @brief 获取控件名称，对应 xml 中 name 属性
	 * @return 返回控件名称
	 */
	std::wstring GetName() const;

	/**
	 * @brief 获取控件名称，对应 xml 中 name 属性
	 * @return 返回控件名称（UTF8 编码）
	 */
	std::string GetUTF8Name() const;

	/**
	 * @brief 设置控件名称，内存中设置不会写入 xml 中
	 * @param[in] strName 要设置的名称
	 * @return 无
	 */
	void SetName(const std::wstring& strName);

	/**
	 * @brief 设置控件名称，内存中设置不会写入 xml 中（UTF8 编码）
	 * @param[in] strName 要设置的名称
	 * @return 无
	 */
	void SetUTF8Name(const std::string& strName);

	/**
	 * @brief 获取关联的窗口指针
	 * @return 返回关联窗口的指针
	 */
	virtual Window* GetWindow() const;

	/**
	 * @brief 设置容器所属窗口
	 * @param[in] pManager 窗口指针
	 * @param[in] pParent 父容器
	 * @param[in] bInit 设置后是否调用 Init 初始化控件
	 * @return 无
	 */
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true);

	/**
	 * @brief 设置容器所属窗口
	 * @param[in] pManager 窗口指针
	 * @return 无
	 */
	virtual void SetWindow(Window* pManager);

	/**
	 * @brief 初始化函数
	 * @return 无
	 */
	virtual void Init();

	/**
	 * @brief 由 Init 调用，功能与 Init 相同
	 * @return 无
	 */
	virtual void DoInit();

	/**
	 * @brief 判断是否可见
	 * @return 返回 true 为可见，否则 false 为不可见
	 */
	virtual bool IsVisible() const;

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	bool IsInternVisible() const;

	/**
	 * @brief 判断控件是否浮动，对应 xml 中 float 属性
	 * @return 返回 true 是浮动状态，否则 false 为非浮动状态
	 */
	bool IsFloat() const;

	/**
	 * @brief 设置控件是否浮动
	 * @param[in] bFloat 设置为 true 为浮动，false 为不浮动
	 * @return 无
	 */
	void SetFloat(bool bFloat = true);

	/**
	 * @brief 获取固定宽度，对应 xml 中 width 属性
	 * @return 返回控件宽度
	 */
	int GetFixedWidth() const;

	/**
	 * @brief 设置控件固定宽度
	 * @param[in] cx 要设置的宽度
	 * @param[in] bArrange 是否重新排列，默认为 true
	 * @param[in] bNeedDpiScale 兼容 DPI 缩放，默认为 true
	 * @return 无
	 */
	void SetFixedWidth(int cx, bool bArrange = true, bool bNeedDpiScale = true);

	/**
	 * @brief 获取固定高度
	 * @return 返回固定高度
	 */
	int GetFixedHeight() const;

	/**
	 * @brief 设置固定高度
	 * @param[in] cy 要设置的固定高度
	 * @param[in] bNeedDpiScale 兼容 DPI 缩放，默认为 true
	 * @return 无
	 */
	void SetFixedHeight(int cy, bool bNeedDpiScale = true);

	/**
	 * @brief 获取最小宽度
	 * @return 返回最小宽度
	 */
	int GetMinWidth() const;

	/**
	 * @brief 设置最小宽度
	 * @param[in] cx 要设置的最小宽度
	 * @return 无
	 */
	void SetMinWidth(int cx);

	/**
	 * @brief 获取最大宽度
	 * @return 返回最大宽度
	 */
	int GetMaxWidth() const;

	/**
	 * @brief 设置最大宽度
	 * @param[in] cx 要设置的最大宽度
	 * @return 无
	 */
	void SetMaxWidth(int cx);

	/**
	 * @brief 获取最小高度
	 * @return 返回最小高度
	 */
	int GetMinHeight() const;

	/**
	 * @brief 设置最小高度
	 * @param[in] cy 要设置的最小高度
	 * @return 无
	 */
	void SetMinHeight(int cy);

	/**
	 * @brief 获取最大高度
	 * @return 返回最大高度
	 */
	int GetMaxHeight() const;

	/**
	 * @brief 设置最大高度
	 * @param[in] cy 要设置的最大高度
	 * @return 无
	 */
	void SetMaxHeight(int cy);

	/**
	 * @brief 获取实际宽度
	 * @return 返回实际宽度
	 */
	int GetWidth() const;

	/**
	 * @brief 获取实际高度
	 * @return 返回实际高度
	 */
	int GetHeight() const;

	/**
	 * @brief 获取水平对齐方式
	 * @return 返回水平对齐方式，参考 HorAlignType 枚举
	 */
	HorAlignType GetHorAlignType() const;

	/**
	 * @brief 设置水平对齐方式
	 * @param[in] horAlignType 要设置的对齐方式，参考 HorAlignType 枚举
	 * @return 无
	 */
	void SetHorAlignType(HorAlignType horAlignType);

	/**
	 * @brief 获取垂直对齐方式
	 * @return 返回垂直对齐方式，参见 VerAlignType 枚举
	 */
	VerAlignType GetVerAlignType() const;

	/**
	 * @brief 设置垂直对齐方式
	 * @param[in] vorAlignType 要设置的对齐方式，参考 VerAlignType 枚举
	 * @return 无
	 */
	void SetVerAlignType(VerAlignType verAlignType);

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	bool IsReEstimateSize() const;

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	void SetReEstimateSize(bool bReEstimateSize);

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	virtual CSize EstimateSize(CSize szAvailable);

	/**
	 * @brief 获取控件位置
	 * @param[in] bContainShadow 是否包含阴影位置，默认为 true
	 * @return 返回控件位置
	 */
	virtual	UiRect GetPos(bool bContainShadow = true) const;

	/**
	 * @brief 设置控件位置
	 * @param[in] rc 要设置的位置信息
	 * @return 无
	 */
	virtual void SetPos(UiRect rc);

	/**
	 * @brief 进行布局
	 * @return 无
	 */
	virtual void Arrange();

	/**
	 * @brief 让父容器重排
	 * @return 无
	 */
	virtual void ArrangeAncestor();

	/**
	 * @brief 判断是否已经排列过
	 * @return true 为已经排列过，false 为尚未排列
	 */
	bool IsArranged() const;

	void SetUseCache(bool cache);
	bool IsUseCache() { return m_bUseCache; }
	void SetCacheDirty(bool dirty);
	bool IsCacheDirty() { return m_bUseCache && m_bCacheDirty; }

	/**
	 * @brief 重绘控件
	 * @return 无
	 */
	virtual void Invalidate();

	/**
	 * @brief 获取控件实际的位置（布局位置加外层滚动后的偏移位置）
	 * @param[in] bContainShadow 是否考虑窗口阴影尺寸
	 * @return UiRect 控件实际的位置
	 */
	UiRect GetPosWithScrollOffset(bool bContainShadow = true) const;

	/**
	 * @brief 获取外层滚动偏移
	 * @return CPoint
	 */
	CPoint GetScrollOffset() const;

	static bool IsChild(PlaceHolder* pAncestor, PlaceHolder* pChild);
protected:
	/**
	 * @brief 让自己重排
	 * @return void 无
	 */
	virtual void ArrangeSelf();

protected:
	std::wstring m_sName;
	Window *m_pWindow;
	Box* m_pParent;
	CSize m_cxyFixed;
	CSize m_cxyMin;
	CSize m_cxyMax;
	UiRect m_rcItem;
	HorAlignType m_horAlignType;
	VerAlignType m_verAlignType;
	bool m_bFloat;
	bool m_bReEstimateSize;
	bool m_bVisible;
	bool m_bInternVisible;
	bool m_bIsArranged;

	bool m_bUseCache;
	bool m_bCacheDirty;
	std::unique_ptr<IRenderContext> m_renderContext;
};

} // namespace ui

#endif // UI_CORE_PLACE_HOLDER_H_
