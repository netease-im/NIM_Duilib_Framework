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

  // 重写父类方法，提供个性化功能，请参考父类声明
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
  void PaintText(IRenderContext* pRender) override;
  void PaintChild(IRenderContext* pRender, const UiRect& rcPaint) override;

  /**
   * @brief 获取当前选择项文本
   * @return 返回当前选择项文本
   */
  std::wstring GetText() const;

  /**
  * @brief 获取文字边距
  * @return 返回文字的边距信息
  */
  UiRect GetTextPadding() const;

  /**
  * @brief 设置文字边距信息
  * @param[in] rc 边距信息
  * @return 无
  */
  void SetTextPadding(UiRect rc);

  /**
   * @brief 获取当前所属的 List 对象
   * @return 返回所属的 List 对象指针
   */
  ListBox* GetListBox() { return m_pLayout.get(); }

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
   * @param[in] bTrigger 是否触发选择事件
   * @return 返回 true 表示成功，否则为 false
   */
  bool SelectItem(int iIndex, bool bTrigger = false);

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
  virtual int GetCount() const { return m_pLayout->GetCount(); }

  /**
  * @brief 设置下拉框的窗口
  * @param[in] combo_wnd 设置的下拉框窗口
   */
  void SetCShadowComboWnd(CShadowComboWnd* comboWnd) { m_pWindow = comboWnd; }

  /**
  * @brief 获取下拉框的窗口
  * @return 下拉框存在时返回下拉框窗口，不存在时返回nullptr
  */
  CShadowComboWnd* GetShadowComboWnd() { return m_pWindow; }

  /**
  * @brief 设置下拉框阴影的大小
  * @param[in] rect 设置的下拉框阴影大小
  */
  void SetShadowCorner(const UiRect& rect, bool bNeedDpiScale = true);

  /**
  * @brief 获取下拉框阴影大小
  * @return 返回下拉框阴影的大小
  */
  UiRect GetShadowCorner() const { return m_rcShadowCorner; }

  /**
  * @brief 设置下拉框阴影图片
  * @param[in] rect 设置的下拉框阴影图片
  */
  void SetShadowImage(const std::wstring& ShadowImage) { m_sShadowImage = ShadowImage; }

  /**
  * @brief 获取下拉框阴影图片
  * @return 返回下拉框阴影图片
  */
  std::wstring GetShadowImage() const { return m_sShadowImage; }

  /**
  * @brief 获取Layout
  * @return 返回Layout
  */
  ui::ListBox* GetCustomLayout() const { return m_pLayout.get(); }

  void SetArrowOffset(int offset, bool bNeedDpiScale = true);

  /**
   * @brief 监听子项被选择事件
   * @param[in] callback 子项被选择后触发的回调函数
   * @return 无
   */
  void AttachSelect(const EventCallback& callback) { OnEvent[kEventSelect] += callback;/*m_pLayout->AttachSelect(callback);*/ }	//mod by djj

  /**
   * @brief 监听下拉窗关闭事件
   * @param[in] callback 下拉窗关闭后触发的回调函数
   * @return 无
   */
  void AttachWindowClose(const EventCallback& callback) { OnEvent[kEventWindowClose] += callback; };

private:
  /**
   * @brief 默认的子项被选择处理函数
   * @param[in] args 参数列表
   * @return 始终返回 true
   */
  bool OnSelectItem(EventArgs* args);

  /**
   * @brief 选择一个子项
   * @param[in] iIndex 要选择的子项索引
   * @return 返回 true 表示成功，否则为 false
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