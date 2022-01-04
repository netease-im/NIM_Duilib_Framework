#ifndef UI_CORE_CONTROL_H_
#define UI_CORE_CONTROL_H_

#pragma once

namespace ui 
{

typedef Control* (CALLBACK* FINDCONTROLPROC)(Control*, LPVOID);

class UILIB_API UserDataBase
{
public:
	virtual ~UserDataBase()
	{

	}
};

class UIAControlProvider;

class UILIB_API Control : public PlaceHolder
{
	typedef std::map<int, CEventSource> GifEventMap;
public:
	Control();
	Control(const Control& r);
	Control& operator=(const Control& r) = delete;
    virtual ~Control();

	virtual std::wstring GetType() const;

    /// 图形相关
	/**
	 * @brief 获取背景颜色
	 * @return 返回背景颜色的字符串，该值在 global.xml 中定义
	 */
	std::wstring GetBkColor() const;

	/**
	 * @brief 设置背景颜色
	 * @param[in] strColor 要设置的背景颜色值，该值必须在 global.xml 中存在
	 * @return 无
	 */
	void SetBkColor(const std::wstring& strColor);

	/**
	 * @brief 获取某个状态下的字体颜色
	 * @param[in] stateType 要获取何种状态下的颜色值，参考 ControlStateType 枚举
	 * @return 指定状态下设定的颜色字符串，对应 global.xml 中指定色值
	 */
	std::wstring GetStateColor(ControlStateType stateType);

	/**
	 * @brief 设置某个状态下的字体颜色
	 * @param[in] stateType 要设置何种状态下的颜色值，参考 ControlStateType 枚举
	 * @param[in] strColor 要设置的颜色值，该值必须在 global.xml 中存在
	 * @return 无
	 */
	void SetStateColor(ControlStateType stateType, const std::wstring& strColor);

	/**
	 * @brief 获取背景图片位置
	 * @return 背景图片位置  
	 */
    std::wstring GetBkImage() const;

	/**
	 * @brief 获取 UTF8 格式的背景图片位置
	 * @return 背景图片位置
	 */
	std::string GetUTF8BkImage() const;

	/**
	 * @brief 设置背景图片
	 * @param[in] strImage 要设置的图片路径
	 * @return 无
	 */
	void SetBkImage(const std::wstring& strImage);

	/**
	 * @brief 设置背景图片（UTF8 格式字符串）
	 * @param[in] strImage 要设置的图片路径
	 * @return 无
	 */
	void SetUTF8BkImage(const std::string& strImage);

  /**
  * @brief 获取loading状态图片位置
  * @return loading图片位置
  */
  std::wstring GetLoadingImage() const;

  /**
  * @brief 设置loading图片
  * @param[in] strImage 要设置的图片路径
  * @return 无
  */
  void SetLoadingImage(const std::wstring& strImage);

  /**
  * @brief 设置loading背景色
  * @param[in] strColor 背景色
  * @return 无
  */
  void SetLoadingBkColor(const std::wstring& strColor);

	/**
	 * @brief 获取指定状态下的图片位置
	 * @param[in] 要获取何种状态下的图片，参考 ControlStateType 枚举
	 * @return 指定状态下的图片位置
	 */
	std::wstring GetStateImage(ControlStateType stateType);

	/**
	 * @brief 设置某个状态下的图片
	 * @param[in] stateType 要设置何种状态下的图片，参考 ControlStateType 枚举
	 * @param[in] strImage 要设置的图片路径
	 * @return 无
	 */
	void SetStateImage(ControlStateType stateType, const std::wstring& strImage);

	/**
	 * @brief 获取指定状态下的前景图片
	 * @param[in] 要获取何种状态下的图片，参考 `ControlStateType` 枚举
	 * @return 指定状态下前景图片位置
	 */
	std::wstring GetForeStateImage(ControlStateType stateType);

	/**
	 * @brief 设置某个状态下前景图片
	 * @param[in] stateType 要设置何种状态下的图片，参考 `ControlStateType` 枚举
	 * @param[in] strImage 要设置的前景图片路径
	 * @return 无
	 */
	void SetForeStateImage(ControlStateType stateType, const std::wstring& strImage);

	/**
	 * @brief 获取控件状态
	 * @return 控件的状态，请参考 `ControlStateType` 枚举
	 */
	ControlStateType GetState() const;

	/**
	 * @brief 设置控件状态
	 * @param[in] pStrState 要设置的控件状态，请参考 `ControlStateType` 枚举
	 * @return 无
	 */
	void SetState(ControlStateType pStrState);

	/**
	 * @brief 获取控件图片指针
	 * @return 返回图片对象指针
	 */
	virtual Image* GetEstimateImage();

	/// 边框相关
	/**
	 * @brief 获取边框大小
	 * @return 返回边框的大小
	 */
	int GetBorderSize() const;

	/**
	 * @brief 设置边框大小
	 * @param[in] nSize 要设置的边框大小
	 * @return 无
	 */
	void SetBorderSize(int nSize);

	/**
	 * @brief 获取边框颜色
	 * @return 边框的颜色字符串，对应 global.xml 中的具体颜色值
	 */
	std::wstring GetBorderColor() const;

	/**
	 * @brief 设置边框颜色
	 * @param[in] strBorderColor 设置边框的颜色字符串值，该值必须在 global.xml 中存在
	 * @return 无
	 */
	void SetBorderColor(const std::wstring& strBorderColor);

	/**
	 * @brief 设置边框的大小
	 * @param[in] rc 一个 `UiRect` 结构的边框大小集合
	 * @return 无
	 */
	void SetBorderSize(UiRect rc);

	/**
	 * @brief 获取左侧边框大小
	 * @return 左侧边框的大小  
	 */
	int GetLeftBorderSize() const;

	/**
	 * @brief 设置左侧边框大小
	 * @param[in] nSize 要设置的左侧边框大小
	 * @return 无
	 */
	void SetLeftBorderSize(int nSize);

	/**
	 * @brief 获取顶部边框大小
	 * @return 顶部边框大小
	 */
	int GetTopBorderSize() const;

	/**
	 * @brief 设置顶部边框大小
	 * @param[in] nSize 要设置的上方边框大小
	 * @return 无
	 */
	void SetTopBorderSize(int nSize);

	/**
	 * @brief 获取右侧边框大小
	 * @return 右侧的边框大小
	 */
	int GetRightBorderSize() const;

	/**
	 * @brief 设置右侧边框大小
	 * @param[in] nSize 要设置的右侧边框大小
	 * @return 无
	 */
	void SetRightBorderSize(int nSize);

	/**
	 * @brief 获取下方边框大小
	 * @return 下方边框大小
	 */
	int GetBottomBorderSize() const;

	/**
	 * @brief 设置下方边框大小
	 * @param[in] nSize 要设置的下方边框大小
	 * @return 无
	 */
	void SetBottomBorderSize(int nSize);

	/**
	 * @brief 获取边框大小
	 * @return 四个方向的边框大小
	 */
	CSize GetBorderRound() const;

	/**
	 * @brief 设置边框大小
	 * @param[in] cxyRound 一个 CSize 结构表示了四个方向边框的大小
	 * @return 无
	 */
	void SetBorderRound(CSize cxyRound);

	/**
	 * @brief 设置边框阴影
	 * @param[in] 要设置的阴影属性
	 * @return 无
	 */
	void SetBoxShadow(const std::wstring& strShadow);

    /// 鼠标相关
	/**
	 * @brief 获取鼠标指针类型
	 * @return 当前鼠标类型
	 */
	virtual CursorType GetCursorType() const;

	/**
	 * @brief 设置当前鼠标指针类型
	 * @param[in] flag 要设置的鼠标类型，参考 CursorType 枚举
	 * @return 无
	 */
	void SetCursorType(CursorType flag);

	/**
	 * @brief 获取控件在鼠标悬浮状态下的提示文本
	 * @return 返回当前鼠标悬浮状态提示的文本
	 */
    virtual std::wstring GetToolTipText() const;

	/**
	 * @brief 获取控件在鼠标悬浮状态下的提示文本（UTF8 格式）
	 * @return 返回当前鼠标悬浮状态提示的文本
	 */
	virtual std::string GetUTF8ToolTipText() const;

	/**
	 * @brief 设置鼠标悬浮到控件显示的提示文本
	 * @param[in] strText 要设置的文本
	 * @return 无
	 */
	virtual void SetToolTipText(const std::wstring& strText);

	/**
	 * @brief 设置鼠标悬浮到控件显示的提示文本（UTF8 格式）
	 * @param[in] strText 要设置的文本
	 * @return 无
	 */
	virtual void SetUTF8ToolTipText(const std::string& strText);

	/**
	 * @brief 设置鼠标悬浮到控件显示的提示文本在语言文件中对应的文字
	 * @param[in] strTextId 在语言文件中对应的提示文字 ID
	 * @return 无
	 */
	virtual void SetToolTipTextId(const std::wstring& strTextId);

	/**
	 * @brief 设置鼠标悬浮到控件显示的提示文本在语言文件中对应的文字（UTF8 格式）
	 * @param[in] strTextId 在语言文件中对应的提示文字 ID
	 * @return 无
	 */
	virtual void SetUTF8ToolTipTextId(const std::string& strTextId);

	/**
	 * @brief 设置鼠标悬浮到控件上提示的文本单行最大宽度
	 * @param[in] nWidth 要设置的宽度值
	 * @return 无
	 */
	virtual void SetToolTipWidth(int nWidth);

	/**
	 * @brief 获取鼠标悬浮到控件上提示的文本单行最大宽度
	 * @return 当前提示文本最大宽度
	 */
	virtual int GetToolTipWidth(void) const;

    /// 菜单
	/**
	 * @brief 控件是否响应右键菜单消息
	 * @return 返回结果表示了是否响应右键菜单消息，true 响应右键菜单消息，false 不响应右键菜单消息
	 */
    virtual bool IsContextMenuUsed() const;

	/**
	 * @brief 设置控件响应右键菜单消息
	 * @param[in] bMenuUsed 是否响应右键菜单消息，true 为是，false 为否
	 * @return 无
	 */
    virtual void SetContextMenuUsed(bool bMenuUsed);

	/**
	  * @brief 获取控件右键菜单的弹出位置信息
	  * @return 位置信息
	  */
	virtual std::wstring GetMenuPopup() const;

	/**
	  * @brief 设置控件右键菜单的弹出位置信息
	 * @param[in] strPopup 位置信息，参见Menu.h StringToMenuPopup()
	  * @return 无
	  */
	virtual void SetMenuPopup(const std::wstring& strPopup);

	/**
	* @brief 获取控件右键菜单的对齐信息
	* @return 对齐信息
	*/
	virtual std::wstring GetMenuAlign() const;

	/**
	  * @brief 设置控件右键菜单的对齐信息
	  * @param[in] strAlign 对齐信息，参见Menu.h StringToMenuAlign()
	  * @return 无
	  */
	virtual void SetMenuAlign(const std::wstring& strAlign);

    /// 用户数据，辅助函数，供用户使用
	/**
	 * @brief 获取用户绑定到控件的数据字符串
	 * @return 返回具体数据字符串
	 */
    virtual std::wstring GetDataID() const;

	/**
	 * @brief 获取用户绑定到控件的数据字符串（UTF8 格式）
	 * @return 返回具体数据字符串
	 */
	virtual std::string GetUTF8DataID() const;				

	/**
	 * @brief 绑定一个字符串数据到控件
	 * @param[in] strText 要绑定的字符串数据
	 * @return 无
	 */
    virtual void SetDataID(const std::wstring& strText);

	/**
	 * @brief 绑定一个字符串数据到控件（UTF8 格式）
	 * @param[in] strText 要绑定的字符串数据
	 * @return 无
	 */
	virtual void SetUTF8DataID(const std::string& strText);	

	/**
	 * @brief 获取用户绑定的自定义数据结构
	 * @return 用户绑定的自定义结构数据指针
	 */
	virtual UserDataBase* GetUserDataBase() const;

	/**
	 * @brief 绑定自定义数据到控件，用户可继承 UserDataBase 来补充需要绑定的数据
	 * @param[in] pUserDataBase 数据指针
	 * @return 无
	 */
	virtual void SetUserDataBase(UserDataBase* pUserDataBase);

    /// 一些重要的属性
	/**
	 * @brief 设置控件是否可见
	 * @param[in] bVisible 为 true 时控件可见，为 false 时控件被隐藏
	 * @return 无
	 */
    virtual void SetVisible(bool bVisible = true);

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
    virtual void SetInternVisible(bool bVisible = true); // 仅供内部调用，有些ui拥有窗口句柄，需要重写此函数

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	virtual void SetVisible_(bool bVisible);

	/**
	 * @brief 检查控件是否可用
	 * @return 控件可用状态，返回 true 控件可用，否则为 false
	 */
    virtual bool IsEnabled() const;

	/**
	 * @brief 设置控件可用状态
	 * @param[in] bEnable 为 true 时控件可用，为 false 时控件为禁用状态则不可用
	 * @return 无
	 */
    virtual void SetEnabled(bool bEnable = true);

	/**
	 * @brief 检查控件是否响应鼠标事件
	 * @return 返回控件是否响应鼠标事件，返回 true 响应鼠标事件，false 为不响应
	 */
    virtual bool IsMouseEnabled() const;

	/**
	 * @brief 设置控件是否响应鼠标事件
	 * @param[in] bEnable 为 true 响应鼠标事件，为 false 时不响应鼠标事件
	 * @return 无
	 */
    virtual void SetMouseEnabled(bool bEnable = true);

	/**
	 * @brief 检查控件是否响应键盘事件
	 * @return 返回控件是否响应键盘事件，返回 true 响应键盘事件，false 不响应键盘事件
	 */
    virtual bool IsKeyboardEnabled() const;

	/**
	 * @brief 设置控件是否响应键盘事件
	 * @param[in] bEnable 为 true 响应键盘事件，为 false 时不响应键盘事件
	 * @return 无
	 */
    virtual void SetKeyboardEnabled(bool bEnable = true);

	/**
	 * @brief 检查控件是否具有焦点
	 * @return 返回控件是否具有检点，为 true 时是当前具有焦点，为 false 时控件没有焦点
	 */
    virtual bool IsFocused() const;

	/**
	 * @brief 让控件获取焦点
	 * @return 无
	 */
    virtual void SetFocus();

	/**
	 * @brief 返回控件的标识，用于判断是否可以响应 TAB 切换事件
	 * @return 返回控件的标识类型
	 */
	virtual UINT GetControlFlags() const;

	/**
	 * @brief 让控件设置永远获取不到焦点
	 * @return 无
	 */
	void SetNoFocus(); // 控件永远不要焦点，与KillFocus不一样

	/**
	 * @brief 判断当前鼠标焦点是否在控件上
	 * @return 返回鼠标焦点是否在控件上，true 鼠标焦点在控件上，false 鼠标焦点不在控件上
	 */
	virtual bool IsMouseFocused() const { return m_bMouseFocused;}

	/**
	 * @brief 设置是否将鼠标焦点到控件上
	 * @param[in] bMouseFocused 为 true 时设置鼠标焦点到控件上，为 false 时让控件失去鼠标焦点
	 * @return 无
	 */
	virtual void SetMouseFocused(bool bMouseFocused) { m_bMouseFocused = bMouseFocused; }

	/**
	 * @brief 判断控件当前是否是激活状态
	 * @return 返回控件状态，true 控件当前是激活状态，可见并可用，false 控件当前非激活状态，可能不可见或被禁用
	 */
	virtual bool IsActivatable() const;

	/**
	 * @brief 激活控件，如点击、选中、展开等操作
	 * @param[in] 待补充
	 * @return 待补充
	 */
	virtual void Activate();

	/**
	 * @brief 取消激活控件，如反选、隐藏等操作
	 * @param[in] 待补充
	 * @return 待补充
	 */
	virtual void Deactivate();

	/**
	 * @brief 是否激活，如选中、展开等
	 * @param[in] 待补充
	 * @return 待补充
	 */
	virtual bool IsActivated();

	/// 控件搜索
	/**
	 * @brief 根据坐标查找指定控件
	 * @param[in] Proc
	 * @param[in] pData
	 * @param[in] uFlags
	 * @param[in] scrollPos
	 * @return 返回控件的指针
	 */
    virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, CPoint scrollPos = CPoint());

	/// 位置相关
	/**
	 * @brief 获取控件位置
	 * @param[in] bContainShadow 是否包含阴影，默认为 true 则包含阴影范围，否则 false 为不包含
	 * @return 返回控件的位置信息
	 */
	virtual	UiRect GetPos(bool bContainShadow = true) const override;

	/**
	 * @brief 设置控件位置
	 * @param[in] rc 控件位置的描述信息
	 * @return 无
	 */
	virtual void SetPos(UiRect rc) override;

	/**
	 * @brief 获取控件的外边距
	 * @return 返回控件的外边距
	 */
	virtual UiRect GetMargin() const;

	/**
	 * @brief 设置控件的外边距
	 * @param[in] rcMargin 控件的外边距信息
	 * @param[in] bNeedDpiScale 是否让外边距根据 DPI 适配，默认为 true，false 不适配 DPI
	 * @return 无
	 */
	virtual void SetMargin(UiRect rcMargin, bool bNeedDpiScale = true);

	/**
	 * @brief 计算控件大小
	 * @param[in] szAvailable 暂无意义
	 * @return szAvailable 控件实际大小（如果设置了图片并设置 width 或 height 任意一项为 auto，将根据图片来计算最终大小）
	 */
	virtual CSize EstimateSize(CSize szAvailable);

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	virtual CSize EstimateText(CSize szAvailable, bool& bReEstimateSize);

	/**
	 * @brief 检查指定坐标是否在滚动条当前滚动位置的范围内
	 * @param[in] point 具体坐标
	 * @return 返回是否在范围内，true 在滚动条当前滚动位置范围内，false 不在滚动条当前滚动位置范围内
	 */
	virtual bool IsPointInWithScrollOffset(const CPoint& point) const;

	/**
	 * @brief Get ui automation provider 
	 * @return nullptr or pointer
	 */
	virtual UIAControlProvider* GetUIAProvider();

	// 消息处理
	/**
	 * @brief 控件统一的消息处理入口，将传统 Windows 消息转换为自定义格式的消息
	 * @param[in] eventType	消息内容
	 * @param[in] wParam	消息附加内容
	 * @param[in] lParam	消息附加内容
	 * @param[in] tChar		按键信息
	 * @param[in] mousePos	鼠标信息
	 * @return 无
	 */
	void HandleMessageTemplate(EventType eventType, WPARAM wParam = 0, LPARAM lParam = 0, TCHAR tChar = 0, CPoint mousePos = CPoint(), FLOAT pressure = 0.0f);

	/**
	 * @brief 将转换后的消息派发到消息处理函数
	 * @param[in] msg 消息内容
	 * @return 无
	 */
    virtual void HandleMessageTemplate(EventArgs& msg);

	/**
	 * @brief 判断控件是否有 HOT 状态
	 * @return 返回 true 有 HOT 状态， 否则为 false
	 */
	virtual bool HasHotState();

	/**
	 * @brief 设置控件是否响应触控消息
	 * @param[in] bRecv 设置为 true 表示响应触控消息，false 为不响应
	 * @return 无
	 */
	virtual void SetReceivePointerMsg(bool bRecv) { m_bReceivePointerMsg = bRecv; };
	
	/**
	 * @brief 判断控件是否响应触控消息
	 * @return 返回 true 为响应，否则为 false
	 */
	bool IsReceivePointerMsg() const { return m_bReceivePointerMsg; };

	/**
	 * @brief 设置控件失去焦点时是否发送鼠标弹起消息
	 * @param[in] bNeed 设置为 true 表示响应触控消息，false 为不响应
	 * @return 无
	 */
	void SetNeedButtonUpWhenKillFocus(bool bNeed) { m_bNeedButtonUpWhenKillFocus = bNeed; };

	/**
	 * @brief 判断控件失去焦点时是否发送鼠标弹起消息
	 * @return 返回 true 为需要，false 为不需要
	 */
	bool IsNeedButtonUpWhenKillFocus() const { return m_bNeedButtonUpWhenKillFocus; };

	// 属性设置
	/**
	 * @brief 设置控件指定属性
	 * @param[in] strName 要设置的属性名称（如 width）
	 * @param[in] strValue 要设置的属性值（如 100）
	 * @return 无
	 */
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue);

	/**
	 * @brief 设置控件的 class 全局属性
	 * @param[in] strClass 要设置的 class 名称，该名称必须在 global.xml 中存在
	 * @return 无
	 */
	void SetClass(const std::wstring& strClass);

	/**
	 * @brief 应用一套属性列表
	 * @param[in] strList 属性列表的字符串表示，如 `width="100" height="30"`
	 * @return 无
	 */
    void ApplyAttributeList(const std::wstring& strList);

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	bool OnApplyAttributeList(const std::wstring& strReceiver, const std::wstring& strList, EventArgs* eventArgs);

	/// 绘制操作
	/**
	 * @brief 根据图片路径缓存图片信息
	 * @param[in，out] duiImage 传入时标注图片的路径信息，如果成功则会缓存图片并记录到该参数的成员中
	 * @return 无
	 */
	void GetImage(Image& duiImage) const;

	/**
	 * @brief 绘制图片
	 * @param[in] pRender 绘制上下文
	 * @param[in] duiImage 图片对象
	 * @param[in] strModify 图片的附加属性
	 * @param[in] nFade 控件的透明度，如果启用动画效果该值在绘制时是不断变化的
	 * @return 成功返回 true，失败返回 false
	 */
	bool DrawImage(IRenderContext* pRender, Image& duiImage, const std::wstring& strModify = L"", int nFade = DUI_NOSET_VALUE);

	/**
	* @brief 获取绘制上下文对象
	* @return 返回绘制上下文对象
	*/
	IRenderContext* GetRenderContext();

	/**
	* @brief 清理绘制上下文对象
	* @return 无
	*/
	void ClearRenderContext();

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	virtual void AlphaPaint(IRenderContext* pRender, const UiRect& rcPaint);
	
	/**
	* @brief 绘制控件的入口函数
	* @param[in] pRender 指定绘制区域
	* @param[in] rcPaint 指定绘制坐标
	* @return 无
	*/
	virtual void Paint(IRenderContext* pRender, const UiRect& rcPaint);

	/**
	* @brief 绘制控件子项入口函数
	* @param[in] pRender 指定绘制区域
	* @param[in] rcPaint 指定绘制坐标
	* @return 无
	*/
	virtual void PaintChild(IRenderContext* pRender, const UiRect& rcPaint) {};

	/**
	* @brief 设置是否对绘制范围做剪裁限制
	* @param[in] clip 设置 true 为需要，否则为不需要，见绘制函数
	* @return 无
	*/
	void SetClip(bool clip) { m_bClip = clip; };

	/**
	* @brief 判断是否对绘制范围做剪裁限制
	* @return 返回 true 为需要，false 为不需要
	*/
	bool IsClip() const { return m_bClip; }

	/**
	 * @brief 设置控件透明度
	 * @param[in] alpha 0 ~ 255 的透明度值，255 为不透明
	 * @return 无
	 */
	void SetAlpha(int alpha);

	/**
	 * @brief 获取控件透明度
	 * @return 返回控件的透明度
	 */
	int GetAlpha() const { return m_nAlpha;	}

	/**
	 * @brief 检查控件是否有透明属性
	 * @return 返回控件是否透明，true 控件当前有透明属性，false 控件没有透明属性
	 */
	bool IsAlpha() const { return m_nAlpha != 255; }

	/**
	 * @brief 设置焦点状态透明度
	 * @param[in] alpha 0 ~ 255 的透明度值，255 为不透明
	 * @return 无
	 */
	void SetHotAlpha(int nHotAlpha);

	/**
	 * @brief 设置是否接受TAB键切换焦点
	 * @param[in] enable
	 * @return 无
	 */
	void SetTabStop(bool enable);

	/**
	 * @brief 检查是否接受TAB键切换焦点
	 * @return 返回控件是否接受TAB键切换焦点
	 */
	bool IsAllowTabStop() const { return m_bAllowTabstop; }

	/**
	 * @brief 获取焦点状态透明度
	 * @return 返回控件焦点状态的透明度
	 */
	int GetHotAlpha() const { return m_nHotAlpha; }

	/**
	 * @brief 获取控件绘制偏移量
	 * @return 返回当前控件的绘制偏移量
	 */
	CPoint GetRenderOffset() const { return m_renderOffset;	}

	/**
	 * @brief 设置控件绘制偏移量
	 * @param[in] renderOffset 控件偏移数据
	 * @return 无
	 */
	void SetRenderOffset(CPoint renderOffset);

	/**
	 * @brief 设置控件偏移的 X 坐标
	 * @param[in] renderOffsetX X 坐标值
	 * @return 无
	 */
	void SetRenderOffsetX(int renderOffsetX);

	/**
	 * @brief 设置控件偏移的 Y 坐标
	 * @param[in] renderOffsetY Y 坐标值
	 * @return 无
	 */
	void SetRenderOffsetY(int renderOffsetY);

	/// Gif图片
	/**
	 * @brief 播放 GIF
	 * @param[in] 播放完成停止在哪一帧，可设置第一帧、当前帧和最后一帧。请参考 GifStopType 枚举
	 * @return 无
	 */
	void StartGifPlayForUI(GifStopType frame = kGifStopFirst,int playcount = -1);

	/**
	 * @brief 停止播放 GIF
	 * @param[in] transfer 是否将停止事件通知给订阅者，参考 AttachGifPlayStop 方法
	 * @param[frame] frame 播放结束停止在哪一帧，可设置第一帧、当前帧和最后一帧。请参考 GifStopType 枚举
	 * @return 无
	 */
	void StopGifPlayForUI(bool transfer = false, GifStopType frame = kGifStopCurrent);

	/**
	 * @brief 监听 GIF 播放完成通知
	 * @param[in] callback 要监听 GIF 停止播放的回调函数
	 * @return 无
	 */
	void AttachGifPlayStop(const EventCallback& callback){ OnGifEvent[m_nVirtualEventGifStop] += callback; };

  /**
  * @brief 开启loading状态
  * @param[in] start_angle loading图片旋转的角度
  * @return 无
  */
  void StartLoading(int fStartAngle = -1);

  /**
  * @brief 关闭loading状态
  * @param[in] frame 播放完成停止在哪一帧，可设置第一帧、当前帧和最后一帧。请参考 GifStopType 枚举
  * @return 无
  */
  void StopLoading(GifStopType frame = kGifStopFirst);

  /**
  * @brief 计算loading图片的旋转角度
  * @return 无
  */
  void Loading();
  /**
  * @brief 是否正在loading
  * @return 在loading返回true, 反之返回false
  */
  bool IsLoading();

	/// 动画管理
	/**
	 * @brief 获取动画管理器指针
	 * @return 返回动画管理器指针
	 */
	AnimationManager& GetAnimationManager()	{ return m_animationManager; }

	/// 图片缓存
	/**
	 * @brief 缓存图片
	 * @return 无
	 */
	virtual void InvokeLoadImageCache();

	/**
	 * @brief 取消加载图片缓存
	 * @return 无
	 */
	virtual void UnLoadImageCache();

	/**
	 * @brief 清理图片缓存
	 * @return 无
	 */
	virtual void ClearImageCache();

	/**
	 * @brief 监听控件所有事件
	 * @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	 * @return 无
	 */
	void AttachAllEvents(const EventCallback& callback)	{ OnEvent[kEventAll] += callback; }

	/**
	 * @brief 监听鼠标进入事件
	 * @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	 * @return 无
	 */
	void AttachMouseEnter(const EventCallback& callback) { OnEvent[kEventMouseEnter] += callback; }

	/**
	 * @brief 监听鼠标离开事件
	 * @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	 * @return 无
	 */
	void AttachMouseLeave(const EventCallback& callback) { OnEvent[kEventMouseLeave] += callback; }

	/**
	 * @brief 监听鼠标悬浮事件
	 * @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	 * @return 无
	 */
	void AttachMouseHover(const EventCallback& callback) { OnEvent[kEventMouseHover] += callback; }

	/**
	 * @brief 监听鼠标按下事件
	 * @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	 * @return 无
	 */
	void AttachButtonDown(const EventCallback& callback) { OnEvent[kEventMouseButtonDown] += callback; }

	/**
	 * @brief 监听鼠标弹起事件
	 * @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	 * @return 无
	 */
	void AttachButtonUp(const EventCallback& callback) { OnEvent[kEventMouseButtonUp] += callback; }

	/**
	 * @brief 监听获得焦点事件
	 * @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	 * @return 无
	 */
	void AttachSetFocus(const EventCallback& callback) { OnEvent[kEventSetFocus] += callback; }

	/**
	 * @brief 监听失去焦点事件
	 * @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	 * @return 无
	 */
	void AttachKillFocus(const EventCallback& callback) { OnEvent[kEventKillFocus] += callback; }

	/**
	 * @brief 监听右键菜单事件
	 * @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	 * @return 无
	 */
	void AttachMenu(const EventCallback& callback) { OnEvent[kEventMouseMenu] += callback; }

	/**
	 * @brief 监听控件大小改变事件
	 * @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	 * @return 无
	 */
	void AttachResize(const EventCallback& callback) { OnEvent[kEventResize] += callback; }

	/**
	 * @brief 监听双击事件
	 * @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	 * @return 无
	 */
	void AttachDoubleClick(const EventCallback& callback) { OnEvent[kEventMouseDoubleClick] += callback; }

	/**
	* @brief 监听控件关闭前最后一条消息
	* @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	* @return 无
	*/
	void AttachLastEvent(const EventCallback& callback) { OnEvent[kEventLast] += callback; }

	/**
	* @brief 监听控件显示或隐藏事件
	* @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	* @return 无
	*/
	void AttachVisibleChange(const EventCallback& callback) { OnEvent[kEventVisibleChange] += callback; }

	/**
	 * @brief 取消监听指定事件，见 EventType 枚举
	 * @param[in] callback 事件处理的回调函数，请参考 EventCallback 声明
	 * @return 无
	 */
	void DetachEvent(EventType type);

protected:
	friend StateColorMap;
	friend WindowBuilder;
	void AttachXmlEvent(EventType eventType, const EventCallback& callback) { OnXmlEvent[eventType] += callback; }
	/// Gif图片
	void GifPlay();
	void StopGifPlay(GifStopType frame = kGifStopCurrent);

	
	/// 消息处理的保护成员函数，不允许外部直接调用
	virtual void HandleMessage(EventArgs& msg);
	virtual bool MouseEnter(EventArgs& msg);
	virtual bool MouseLeave(EventArgs& msg);
	virtual bool ButtonDown(EventArgs& msg);
	virtual bool ButtonUp(EventArgs& msg);

	/// 绘制相关保护成员函数，不允许外部直接调用
	virtual void PaintShadow(IRenderContext* pRender);
	virtual void PaintBkColor(IRenderContext* pRender);
	virtual void PaintBkImage(IRenderContext* pRender);
	virtual void PaintStatusColor(IRenderContext* pRender);
	virtual void PaintStatusImage(IRenderContext* pRender);
	virtual void PaintText(IRenderContext* pRender);
	virtual void PaintBorder(IRenderContext* pRender);
  virtual void PaintLoading(IRenderContext* pRender);

	/**
	* @brief 获取某个颜色对应的值，优先获取窗口颜色
	* @param[in] strName 颜色名字
	* @return DWORD ARGB颜色值
	*/
	DWORD GetWindowColor(const std::wstring& strName);
private:
	void BroadcastGifEvent(int nVirtualEvent);
	int GetGifFrameIndex(GifStopType frame);

protected:
	EventMap OnXmlEvent;
	EventMap OnEvent;
	GifEventMap OnGifEvent;
	std::unique_ptr<UserDataBase> m_pUserDataBase;
	bool m_bMenuUsed;
	bool m_bEnabled;
	bool m_bMouseEnabled;
	bool m_bKeyboardEnabled;
	bool m_bFocused;
	bool m_bMouseFocused;
	bool m_bSetPos;		// 防止SetPos循环调用
	bool m_bNoFocus;	//控件不需要焦点
	bool m_bClip;
	bool m_bGifPlay;
	bool m_bReceivePointerMsg;
	bool m_bNeedButtonUpWhenKillFocus;
	bool m_bAllowTabstop;
  bool m_bIsLoading;
	int m_nBorderSize;
	int m_nTooltipWidth;
	int m_nAlpha;
	int m_nHotAlpha;
  int m_fCurrrentAngele;
	CSize m_szEstimateSize;
	CPoint m_renderOffset;
	CSize m_cxyBorderRound;
	UiRect m_rcMargin;
	UiRect m_rcPaint;
	UiRect m_rcBorderSize;
	CursorType m_cursorType;	//影响控件的鼠标形状
	ControlStateType m_uButtonState;
	std::wstring m_sToolTipText;
	std::wstring m_sToolTipTextId;
	std::wstring m_sUserData;
	std::wstring m_sMenuPopup;
	std::wstring m_sMenuAlign;
	std::wstring m_strBkColor;
  std::wstring m_strLoadingBkColor;
	StateColorMap m_colorMap;
	Image m_bkImage;
  Image m_loadingImage;
	StateImageMap m_imageMap;
	std::wstring m_strBorderColor;
	nbase::WeakCallbackFlag m_gifWeakFlag;
	AnimationManager m_animationManager;
	nbase::WeakCallbackFlag m_loadBkImageWeakFlag;
  nbase::WeakCallbackFlag m_loadingImageFlag;
	static const int m_nVirtualEventGifStop;

	UIAControlProvider* m_pUIAProvider;
	BoxShadow m_boxShadow;
};

} // namespace ui

#endif // UI_CORE_CONTROL_H_
