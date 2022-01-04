#pragma once

namespace nim_comp
{
	using namespace ui;

class CFilterComboWnd;
class ui::RichEdit;

class UILIB_API IMatch {
	virtual bool StringMatch(const std::string& utf8str) = 0;
};

class UILIB_API ListElementMatch : public ui::ListContainerElement, public IMatch
{
public:
	virtual bool StringMatch(const std::string& utf8str) override;
};

class FilterListBox : public ListBox
{
public:
	FilterListBox(Layout* pLayout = new VLayout) : ListBox(pLayout), m_pFilterComboWnd(nullptr){};
	virtual ~FilterListBox(){};

	virtual bool SelectItem(int iIndex, bool bTakeFocus = false, bool bTrigger = true) override;
	void Filter(std::string utf8_str);

	void SetFilterComboWnd(CFilterComboWnd *pFilterComboWnd){ m_pFilterComboWnd = pFilterComboWnd; };
	CFilterComboWnd *GetFilterComboWnd(){ return m_pFilterComboWnd; };
private:
	CFilterComboWnd *m_pFilterComboWnd;
};

class UILIB_API FilterCombo : public Box
{
	friend class CFilterComboWnd;
public:
	FilterCombo();
	FilterCombo(const FilterCombo& r) = delete;
	FilterCombo& operator=(const FilterCombo& r) = delete;
	virtual ~FilterCombo(){};

	virtual void HandleMessage(EventArgs& args) override;

	/// 重写父类方法，提供个性化功能，请参考父类声明
	virtual bool Add(Control* pControl) override;
	virtual bool Remove(Control* pControl) override;
	virtual bool RemoveAt(std::size_t iIndex) override;
	virtual void RemoveAll() override;
	virtual void Activate() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	//virtual void PaintText(IRenderContext* pRender) override;

	/**
	 * @brief 获取当前选择项文本
	 * @return 返回当前选择项文本
	 */
    std::wstring GetText() const;

	/**
	 * @brief 获取当前所属的 List 对象
	 * @return 返回所属的 List 对象指针
	 */
	FilterListBox* GetListBox();

	/**
	 * @brief 获取下拉框属性信息
	 * @return 返回字符串形式的属性信息
	 */
    std::wstring GetDropBoxAttributeList();

	/**
	 * @brief 设置下拉框的属性信息
	 * @param[in] pstrList 转义后的 XML 格式属性列表
	 * @return 无
	 */
    void SetDropBoxAttributeList(const std::wstring& pstrList);

	/**
	 * @brief 获取下拉框容器大小
	 * @return 返回容器大小
	 */
    CSize GetDropBoxSize() const;

	/**
	 * @brief 设置下拉框容器大小
	 * @param[in] szDropBox 要设置的大小信息
	 * @return 无
	 */
    void SetDropBoxSize(CSize szDropBox);
	
	/**
	 * @brief 设置 Combobox 是否向上弹出
	 * @param[in] top 为 true 则向上弹出，false 为默认向下弹出
	 * @return 无
	 */
	void SetPopupTop(bool top) { m_bPopupTop = top; };
	
	/**
	 * @brief 判断 Combobox 弹出模式是否是向上弹出
	 * @return 返回 true 表示向上弹出，否则为 false
	 */
	bool IsPopupTop() const { return m_bPopupTop; };

	/**
	 * @brief 选择一个子项
	 * @param[in] iIndex 要选择的子项索引
	 * @return 返回 true 表示成功，否则为 false
	 */
	bool SelectItem(int iIndex);

	/**
	 * @brief 获取指定索引下的子项控件
	 * @param[in] iIndex 要获取的子项索引
	 * @return 返回控件指针
	 */
	Control* GetItemAt(int iIndex);

	/**
	 * @brief 获取当前选择项索引
	 * @return 返回当前选择项索引
	 */
	int GetCurSel() const { return m_iCurSel; }

	/**
	 * @brief 获取所有子项数量
	 * @return 返回所有子项数量
	 */
	virtual int GetCount() const;
    
	/**
	 * @brief 监听子项被选择事件
	 * @param[in] callback 子项被选择后触发的回调函数
	 * @return 无
	 */
	void AttachSelect(const EventCallback& callback);

private:
	/**
	 * @brief 默认的子项被选择处理函数
	 * @param[in] args 参数列表
	 * @return 始终返回 true
	 */
	bool OnSelectItem(EventArgs* args);

	bool OnRichEditTextChanged(EventArgs* args);
	bool OnRichEditButtonDown(EventArgs* args);

protected:
	CFilterComboWnd *m_pComboWnd;
	std::unique_ptr<FilterListBox> m_pLayout;

	RichEdit* m_pRichEdit;
    int m_iCurSel;  
	ControlStateType m_uButtonState;
	CSize m_szDropBox;
	std::wstring m_sDropBoxAttributes;
	bool m_bPopupTop;
};

} // namespace ui

