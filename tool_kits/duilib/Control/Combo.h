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
	virtual bool Add(Control* pControl) override;
	virtual bool Remove(Control* pControl) override;
	virtual bool RemoveAt(std::size_t iIndex) override;
	virtual void RemoveAll() override;
	virtual void Activate() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintText(IRenderContext* pRender) override;

	/**
	 * @brief ��ȡ��ǰѡ����ָ��״̬�µ�ͼƬλ��
	 * @param[in] Ҫ��ȡ��ǰѡ�������״̬�µ�ͼƬ���ο� ControlStateType ö��
	 * @return ���ص�ǰѡ����ָ��״̬�µ�ͼƬλ��
	 */
	std::wstring GetStateImage(ControlStateType stateType) const;

	/**
	 * @brief ��ȡ��ǰѡ�����ı�
	 * @return ���ص�ǰѡ�����ı�
	 */
    std::wstring GetText() const;

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
	virtual int GetCount() const { return m_pLayout->GetCount(); }
    
	/**
	 * @brief �������ѡ���¼�
	 * @param[in] callback ���ѡ��󴥷��Ļص�����
	 * @return ��
	 */
	void AttachSelect(const EventCallback& callback) { m_pLayout->AttachSelect(callback); }

private:
	/**
	 * @brief Ĭ�ϵ����ѡ������
	 * @param[in] args �����б�
	 * @return ʼ�շ��� true
	 */
	bool OnSelectItem(EventArgs* args);

protected:
    CComboWnd *m_pWindow;
	std::unique_ptr<ListBox> m_pLayout;
    int m_iCurSel;  
	ControlStateType m_uButtonState;
	CSize m_szDropBox;
	std::wstring m_sDropBoxAttributes;
	bool m_bPopupTop;
};

} // namespace ui

#endif // UI_CONTROL_COMBO_H_
