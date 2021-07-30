#pragma once

namespace nim_comp
{
using namespace ui;
class CShadowComboWnd;
class ShadowCombo;
 
class CShadowComboWnd : public ui::Window
{
public:
  CShadowComboWnd();
  ~CShadowComboWnd() override;

  void Init(ShadowCombo* pOwner);
  virtual std::wstring GetWindowClassName() const override;
  virtual void OnFinalMessage(HWND hWnd) override;
  virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

  void OnSeleteItem();

private:
  ShadowCombo* m_pOwner = nullptr;
  int m_iOldSel = 0;
  bool m_bClosing = false;
};

class ShadowCombo : public ui::Box {
public:
  friend class CShadowComboWnd;

  ShadowCombo();
  ShadowCombo(const ShadowCombo& r) = delete;
  ShadowCombo& operator=(const ShadowCombo& r) = delete;
  ~ShadowCombo() override;

  // ��д���෽�����ṩ���Ի����ܣ���ο���������
  void DoInit() override;
  std::wstring GetType() const override;
  UIAControlProvider* GetUIAProvider() override;
  bool Add(Control* pControl) override;
  bool Remove(Control* pControl) override;
  bool RemoveAt(std::size_t iIndex) override;
  void RemoveAll() override;
  void Activate() override;
  void Deactivate() override;
  bool IsActivated() override;
  void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
  void PaintText(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;
  void PaintChild(dui::common::dui_refptr<dui::render::IRenderContext> pRender, const UiRect& rcPaint) override;

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
  * @brief ����������Ĵ���
  * @param[in] combo_wnd ���õ������򴰿�
   */
  void SetCShadowComboWnd(CShadowComboWnd* comboWnd) { m_pWindow = comboWnd; }

  /**
  * @brief ��ȡ������Ĵ���
  * @return ���������ʱ���������򴰿ڣ�������ʱ����nullptr
  */
  CShadowComboWnd* GetShadowComboWnd() { return m_pWindow; }

  /**
  * @brief ������������Ӱ�Ĵ�С
  * @param[in] rect ���õ���������Ӱ��С
  */
  void SetShadowCorner(const UiRect& rect, bool bNeedDpiScale = true);

  /**
  * @brief ��ȡ��������Ӱ��С
  * @return ������������Ӱ�Ĵ�С
  */
  UiRect GetShadowCorner() const { return m_rcShadowCorner; }

  /**
  * @brief ������������ӰͼƬ
  * @param[in] rect ���õ���������ӰͼƬ
  */
  void SetShadowImage(const std::wstring& ShadowImage) { m_sShadowImage = ShadowImage; }

  /**
  * @brief ��ȡ��������ӰͼƬ
  * @return ������������ӰͼƬ
  */
  std::wstring GetShadowImage() const { return m_sShadowImage; }

  /**
  * @brief ��ȡLayout
  * @return ����Layout
  */
  ui::ListBox* GetCustomLayout() const { return m_pLayout.get(); }

  void SetArrowOffset(int offset, bool bNeedDpiScale = true);

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
  CShadowComboWnd* m_pWindow;
  std::unique_ptr<ListBox> m_pLayout;
  int m_iCurSel;
  CSize m_szDropBox;
  std::wstring m_sDropBoxAttributes;
  bool m_bPopupTop;
  UiRect	m_rcTextPadding;
  UiRect m_rcShadowCorner;
  std::wstring m_sShadowImage;
  ui::Control* m_cArrow;
  bool m_bInit;
  int m_iArrowOffset;
};

}