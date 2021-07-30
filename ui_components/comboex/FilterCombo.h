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

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual bool Add(Control* pControl) override;
	virtual bool Remove(Control* pControl) override;
	virtual bool RemoveAt(std::size_t iIndex) override;
	virtual void RemoveAll() override;
	virtual void Activate() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	//virtual void PaintText(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;

	/**
	 * @brief ��ȡ��ǰѡ�����ı�
	 * @return ���ص�ǰѡ�����ı�
	 */
    std::wstring GetText() const;

	/**
	 * @brief ��ȡ��ǰ������ List ����
	 * @return ���������� List ����ָ��
	 */
	FilterListBox* GetListBox();

	/**
	 * @brief ��ȡ������������Ϣ
	 * @return �����ַ�����ʽ��������Ϣ
	 */
    std::wstring GetDropBoxAttributeList();

	/**
	 * @brief �����������������Ϣ
	 * @param[in] pstrList ת���� XML ��ʽ�����б�
	 * @return ��
	 */
    void SetDropBoxAttributeList(const std::wstring& pstrList);

	/**
	 * @brief ��ȡ������������С
	 * @return ����������С
	 */
    CSize GetDropBoxSize() const;

	/**
	 * @brief ����������������С
	 * @param[in] szDropBox Ҫ���õĴ�С��Ϣ
	 * @return ��
	 */
    void SetDropBoxSize(CSize szDropBox);
	
	/**
	 * @brief ���� Combobox �Ƿ����ϵ���
	 * @param[in] top Ϊ true �����ϵ�����false ΪĬ�����µ���
	 * @return ��
	 */
	void SetPopupTop(bool top) { m_bPopupTop = top; };
	
	/**
	 * @brief �ж� Combobox ����ģʽ�Ƿ������ϵ���
	 * @return ���� true ��ʾ���ϵ���������Ϊ false
	 */
	bool IsPopupTop() const { return m_bPopupTop; };

	/**
	 * @brief ѡ��һ������
	 * @param[in] iIndex Ҫѡ�����������
	 * @return ���� true ��ʾ�ɹ�������Ϊ false
	 */
	bool SelectItem(int iIndex);

	/**
	 * @brief ��ȡָ�������µ�����ؼ�
	 * @param[in] iIndex Ҫ��ȡ����������
	 * @return ���ؿؼ�ָ��
	 */
	Control* GetItemAt(int iIndex);

	/**
	 * @brief ��ȡ��ǰѡ��������
	 * @return ���ص�ǰѡ��������
	 */
	int GetCurSel() const { return m_iCurSel; }

	/**
	 * @brief ��ȡ������������
	 * @return ����������������
	 */
	virtual int GetCount() const;
    
	/**
	 * @brief �������ѡ���¼�
	 * @param[in] callback ���ѡ��󴥷��Ļص�����
	 * @return ��
	 */
	void AttachSelect(const EventCallback& callback);

private:
	/**
	 * @brief Ĭ�ϵ����ѡ������
	 * @param[in] args �����б�
	 * @return ʼ�շ��� true
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

