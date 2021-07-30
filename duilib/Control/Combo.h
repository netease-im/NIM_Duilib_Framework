#ifndef UI_CONTROL_COMBO_H_
#define UI_CONTROL_COMBO_H_

#pragma once

namespace ui 
{

class CComboWnd;
class UILIB_API Combo : public Box
{
    friend class CComboWnd;
public:
	Combo();
	Combo(const Combo& r) = delete;
	Combo& operator=(const Combo& r) = delete;

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual UIAControlProvider* GetUIAProvider() override;
	virtual bool Add(Control* pControl) override;
	virtual bool Remove(Control* pControl) override;
	virtual bool RemoveAt(std::size_t iIndex) override;
	virtual void RemoveAll() override;
	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual bool IsActivated() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintText(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;

	/**
	 * @brief ��ȡ��ǰѡ�����ı�
	 * @return ���ص�ǰѡ�����ı�
	 */
    std::wstring GetText() const;

	/**
	* @brief ��ȡ���ֱ߾�
	* @return �������ֵı߾���Ϣ
	*/
	UiRect GetTextPadding() const;

	/**
	* @brief �������ֱ߾���Ϣ
	* @param[in] rc �߾���Ϣ
	* @return ��
	*/
	void SetTextPadding(UiRect rc);

	/**
	 * @brief ��ȡ��ǰ������ List ����
	 * @return ���������� List ����ָ��
	 */
	ListBox* GetListBox() { return m_pLayout.get(); }

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
	 * @param[in] bTrigger �Ƿ񴥷�ѡ���¼�
	 * @return ���� true ��ʾ�ɹ�������Ϊ false
	 */
	bool SelectItem(int iIndex, bool bTrigger = false);

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
	virtual int GetCount() const { return m_pLayout->GetCount(); }
    
	/**
	 * @brief �������ѡ���¼�
	 * @param[in] callback ���ѡ��󴥷��Ļص�����
	 * @return ��
	 */
	void AttachSelect(const EventCallback& callback) { OnEvent[kEventSelect] += callback;/*m_pLayout->AttachSelect(callback);*/ }	//mod by djj

	/**
	 * @brief �����������ر��¼�
	 * @param[in] callback �������رպ󴥷��Ļص�����
	 * @return ��
	 */
	void AttachWindowClose(const EventCallback& callback) { OnEvent[kEventWindowClose] += callback; };

private:
	/**
	 * @brief Ĭ�ϵ����ѡ������
	 * @param[in] args �����б�
	 * @return ʼ�շ��� true
	 */
	bool OnSelectItem(EventArgs* args);

	/**
	 * @brief ѡ��һ������
	 * @param[in] iIndex Ҫѡ�����������
	 * @return ���� true ��ʾ�ɹ�������Ϊ false
	 */
	bool SelectItemInternal(int iIndex);

protected:
	CComboWnd *m_pWindow;
	std::unique_ptr<ListBox> m_pLayout;
    int m_iCurSel;  
	ControlStateType m_uButtonState;
	CSize m_szDropBox;
	std::wstring m_sDropBoxAttributes;
	bool m_bPopupTop;
	UiRect	m_rcTextPadding;	//add by djj
};

} // namespace ui

#endif // UI_CONTROL_COMBO_H_
