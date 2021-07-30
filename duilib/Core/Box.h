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
	 * @brief ����������
	 * @return ��
	 */
	void SetOwner(Box* pOwner);

	/**
	 * @brief ���ø���״̬�µ�������Ϣ
	 * @param[in] pControl �ؼ����
	 * @param[in] rcContainer Ҫ���õ�λ����Ϣ
	 * @return ���ؿؼ����յ�λ����Ϣ
	 */
	static CSize SetFloatPos(Control* pControl, UiRect rcContainer);

	/**
	 * @brief ���ò�������
	 * @param[in] strName Ҫ���õ�������
	 * @param[in] strValue Ҫ���õ�����ֵ
	 * @return true ���óɹ���false ���Բ�����
	 */
	virtual bool SetAttribute(const std::wstring& strName, const std::wstring& strValue);

	/**
	 * @brief �����ڲ����пؼ���λ����Ϣ
	 * @param[in] items �ؼ��б�
	 * @param[in] rc ��ǰ����λ����Ϣ
	 * @return �������к����պ��ӵĿ�Ⱥ͸߶���Ϣ
	 */
	virtual CSize ArrangeChild(const std::vector<Control*>& items, UiRect rc);

	/**
	 * @brief �����ڲ��ӿؼ���С�������������С
	 * @param[in] items �ؼ��б�
	 * @param[in] szAvailable �ӿؼ�����������
	 * @return �������к����պ��ӵĿ�Ⱥ͸߶���Ϣ
	 */
	virtual CSize AjustSizeByChild(const std::vector<Control*>& items, CSize szAvailable);

	/**
	 * @brief ��ȡ�ڱ߾�
	 * @return �����ڱ߾��ıߵĴ�С
	 */
	virtual UiRect GetPadding() const;

	/**
	 * @brief �����ڱ߾࣬�൱�����ÿͻ���
	 * @param[in] rcPadding �ڱ߾�����
	 * @param[in] bNeedDpiScale �Ƿ���� DPI ����Ӧ��Ĭ��Ϊ true
	 * @return ��
	 */
	virtual void SetPadding(UiRect rcPadding, bool bNeedDpiScale = true);

	/**
	 * @brief ��ȡ�ӿؼ�֮��Ķ���߾�
	 * @return ���ض��������ֵ
	 */
	virtual int GetChildMargin() const;

	/**
	 * @brief �����ӿؼ�֮��Ķ���߾�
	 * @param[in] iMargin Ҫ���õı߾���ֵ
	 * @return ��
	 */
	virtual void SetChildMargin(int iMargin);

	/**
	 * @brief ��ȡ�����ڱ߾���Ŀ��÷�Χ
	 * @return ���ؿ��÷�Χλ����Ϣ
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
	/// ��д����ӿڣ��ṩ���Ի����ܡ���������˵����鿴 Control �ؼ�             */
	virtual std::wstring GetType() const override;
	virtual UIAControlProvider* GetUIAProvider() override;
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void SetPos(UiRect rc) override;
	virtual void HandleMessageTemplate(EventArgs& msg) override;
	virtual void SetReceivePointerMsg(bool bRecv) override;
	virtual void PaintChild(dui::common::dui_refptr<dui::render::IRenderContext> pRender, const UiRect& rcPaint) override;
	virtual void SetVisible_(bool bVisible) override;
	virtual void SetInternVisible(bool bVisible = true) override;
	virtual void SetEnabled(bool bEnabled) override;
	virtual CSize EstimateSize(CSize szAvailable) override;
	virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, CPoint scrollPos = CPoint()) override;
	virtual void InvokeLoadImageCache() override;
	virtual void UnLoadImageCache() override;
	virtual void ClearImageCache() override;
	virtual UINT GetControlFlags() const override;

	/// �������з���
	/**
	 * @brief ����ָ���ӿؼ�
	 * @param[in] pstrSubControlName �ӿؼ�����
	 * @return �����ӿؼ�ָ��
	 */
	Control* FindSubControl(const std::wstring& pstrSubControlName);

	/**
	 * @brief ������һ����ѡ�ؼ������������� list��combo��
	 * @param[in] iIndex ָ��Ҫ��ʼ���ҵ�����
	 * @param[in] bForward true Ϊ�������ң� false Ϊ�ݼ�����
	 * @return ��һ����ѡ�ؼ������������� -1 Ϊû�п�ѡ�ؼ�
	 */
	virtual int FindSelectable(int iIndex, bool bForward = true) const;

	/**
	 * @brief ������������ָ���ؼ�
	 * @param[in] iIndex �ؼ�����
	 * @return ���ؿؼ�ָ��
	 */
	virtual Control* GetItemAt(std::size_t iIndex) const;

	/**
	 * @brief ���ݿؼ�ָ���ȡ����
	 * @param[in] pControl �ؼ�ָ��
	 * @return ���� pControl ��ָ��Ŀؼ�����
	 */
	virtual int GetItemIndex(Control* pControl) const;

	/**
	 * @brief ���ÿؼ��������ڲ����������пؼ�λ�ã�
	 * @param[in] pControl �ؼ�ָ��
	 * @param[in] iIndex Ҫ���õ�����ֵ
	 * @return ���� true ���óɹ���false ����ʧ��
	 */
	virtual bool SetItemIndex(Control* pControl, std::size_t iIndex);

	/**
	 * @brief ��ȡ�ӿؼ�����
	 * @return �����ӿؼ�����
	 */
	virtual int GetCount() const;

	/**
	 * @brief ���һ���ؼ���������
	 * @param[in] pControl �ؼ�ָ��
	 * @return ���� true ��ӳɹ���false Ϊ���ʧ��
	 */
	virtual bool Add(Control* pControl);

	/**
	 * @brief ��ָ��λ�����һ���ؼ�
	 * @param[in] pControl �ؼ�ָ��
	 * @param[in] iIndex �ڸ�����֮�����ؼ�
	 * @return ���� true Ϊ��ӳɹ���false Ϊ���ʧ��
	 */
	virtual bool AddAt(Control* pControl, std::size_t iIndex);

	/**
	 * @brief ���ݿؼ�ָ����������Ƴ�һ���ؼ�
	 * @param[in] pControl �ؼ���ָ��
	 * @return ���� true Ϊ�Ƴ��ɹ���false Ϊ�Ƴ�ʧ�ܣ��ؼ����ܲ����ڣ�
	 */
	virtual bool Remove(Control* pControl);

	/**
	 * @brief ���ݿؼ��������������Ƴ�һ���ؼ�
	 * @param[in] iIndex Ҫ�Ƴ��Ŀؼ�����
	 * @return ���� true Ϊ�Ƴ��ɹ���false Ϊ�Ƴ�ʧ�ܣ�����ֵ̫С�򳬳����ӿؼ�������
	 */
	virtual bool RemoveAt(std::size_t iIndex);

	/**
	 * @brief �Ƴ������ӿؼ�
	 * @return ��
	 */
	virtual void RemoveAll();

	/**
	 * @brief ����Ƿ����ĳһ���ؼ�
	 * @param[in] pControl �ؼ���ָ��
	 * @return ���� true ������false ������
	 */
	bool HasItem(Control* pControl) const;

	/**
	 * @brief �����ӿؼ�λ��
	 * @param[in] pChild1 �ӿؼ�1 ָ��
	 * @param[in] pChild2 �ӿؼ�2 ָ��
	 * @return ��
	 */
	void SwapChild(Control* pChild1, Control* pChild2);

	/**
	 * @brief ����һ���ӿؼ�λ��
	 * @param[in] pChild �ؼ�ָ��
	 * @param[in] iIndex Ҫ���õ�λ��
	 * @return ��
	 */
	void ResetChildIndex(Control* pChild, std::size_t iIndex);

	/**
	 * @brief �ж��Ƿ��Զ�����
	 * @return true Ϊ�Զ����٣�false Ϊ���Զ�����
	 */
    virtual bool IsAutoDestroy() const;

	/**
	 * @brief ���ÿؼ��Ƿ��Զ�����
	 * @param[in] bAuto true Ϊ�Զ����٣�false Ϊ���Զ�����
	 * @return ��
	 */
    virtual void SetAutoDestroyChild(bool bAuto);

	/**
	 * @brief �жϴ��ڹرպ��Ƿ��Զ�����
	 * @return true Ϊ�Զ����٣�false Ϊ���Զ�����
	 */
    virtual bool IsDelayedDestroy() const;

	/**
	 * @brief ���ô��ڹرպ��Ƿ��Զ�����
	 * @param[in] bDelayed true Ϊ�Զ����٣�false Ϊ���Զ�����
	 * @return ��
	 */
    virtual void SetDelayedDestroy(bool bDelayed);

	/**
	 * @brief ��ȡ�����Ƿ���Ӧ������
	 * @return true Ϊ��Ӧ��false Ϊ����Ӧ
	 */
    virtual bool IsMouseChildEnabled() const;

	/**
	 * @brief ����������Ӧ������
	 * @param[in] bEnable ����Ϊ true Ϊ��Ӧ������������Ϊ false Ϊ����Ӧ��Ĭ��Ϊ true
	 * @return ��
	 */
    virtual void SetMouseChildEnabled(bool bEnable = true);

	/**
	 * @brief ��ȡ�������ֶ���ָ��
	 * @return �������������Ĳ��ֶ���ָ��
	 */
	virtual Layout* GetLayout() const;

	/**
	 * @brief ���¹������ֶ���
	 * @param[in] pLayout ���ֶ���ָ��
	 * @return ��
	 */
	virtual void ReSetLayout(Layout* pLayout);

	/**
	 * @brief ��ȡ�ڱ߾��λ����Ϣ
	 * @return �����ڱ߾��λ����Ϣ
	 */
	virtual	UiRect GetPaddingPos() const;

	/**
	 * @brief ���¼�������
	 * @param[in] eventType �¼�����
	 * @param[in] callback ָ���ص�����
	 * @return void ��
	 */
 	void AttachBubbledEvent(EventType eventType, const EventCallback& callback)	{ OnBubbledEvent[eventType] += callback; }
 
	/**
	 * @brief ����¼�������
	 * @param[in] eventType �¼�����
	 * @return void ��
	 */
	void DetachBubbledEvent(EventType eventType);
private:
	friend WindowBuilder;

	/**
	 * @brief �� XML �б�д�� Event �� BubbleEvent �¼��Ĵ�����
	 * @param[in] eventType �¼�����
	 * @param[in] callback ָ���ص�����
	 * @return void ��
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

 /// ���д�ֱ��ˮƽ��������������ʹ�����������ɸ�������
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
	virtual void PaintChild(dui::common::dui_refptr<dui::render::IRenderContext> pRender, const UiRect& rcPaint) override;
	virtual void SetMouseEnabled(bool bEnable = true) override;
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit) override;
	virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, CPoint scrollPos = CPoint()) override;
	virtual void ClearImageCache() override;

	/**
	 * @brief ��ȡ������λ��
	 * @return ���ع�������λ����Ϣ
	 */
	virtual CSize GetScrollPos() const;

	/**
	 * @brief ��ȡ�������ķ�Χ
	 * @return ���ع������ķ�Χ��Ϣ
	 */
	virtual CSize GetScrollRange() const;

	/**
	 * @brief ���ù�����λ��
	 * @param[in] szPos Ҫ���õ�λ������
	 * @return ��
	 */
	virtual void SetScrollPos(CSize szPos);

	/**
	 * @brief ���ù����� Y ������
	 * @param[in] y Ҫ���õ� Y ��������ֵ
	 * @return ��
	 */
	virtual void SetScrollPosY(int y);
	
	/**
	 * @brief ���ù����� X ������
	 * @param[in] x Ҫ���õ� X ��������ֵ
	 * @return ��
	 */
    virtual void SetScrollPosX(int x);
	
	/**
	 * @brief ���Ϲ���������
	 * @param[in] deltaValue �������룬Ĭ��Ϊ DUI_NOSET_VALUE
 	 * @param[in] withAnimation �Ƿ񸽴�����Ч����Ĭ��Ϊ true
	 * @return ��
	 */
	virtual void LineUp(int detaValue = DUI_NOSET_VALUE, bool withAnimation = true);
	
	/**
	 * @brief ���¹���������
	 * @param[in] deltaValue �������룬Ĭ��Ϊ DUI_NOSET_VALUE
 	 * @param[in] withAnimation �Ƿ񸽴�����Ч����Ĭ��Ϊ true
	 * @return ��
	 */
	virtual void LineDown(int detaValue = DUI_NOSET_VALUE, bool withAnimation = true);
	
	/**
	 * @brief �������������
	 * @param[in] deltaValue �������룬Ĭ��Ϊ DUI_NOSET_VALUE
	 * @return ��
	 */
    virtual void LineLeft(int detaValue = DUI_NOSET_VALUE);
	
	/**
	 * @brief ���ҹ���������
	 * @param[in] deltaValue �������룬Ĭ��Ϊ DUI_NOSET_VALUE
	 * @return ��
	 */
    virtual void LineRight(int detaValue = DUI_NOSET_VALUE);
	/**
	 * @brief ���Ϲ���һ��ҳ���С�ľ���
	 * @return ��
	 */
	virtual void PageUp();

	/**
	 * @brief ���¹���һ��ҳ���С�ľ���
	 * @return ��
	 */
	virtual void PageDown();

	/**
	 * @brief �ص����������Ϸ�
	 * @return ��
	 */
	virtual void HomeUp();

	/**
	 * @brief ���������·�λ��
	 * @param[in] arrange �Ƿ����ù�����λ�ã�Ĭ��Ϊ true
	 * @param[in] withAnimation �Ƿ����������Ч��Ĭ��Ϊ true
	 * @return ��
	 */
	virtual void EndDown(bool arrange = true, bool withAnimation = true);
	/**
	 * @brief �������һ��ҳ���С�ľ���
	 * @return ��
	 */
	virtual void PageLeft();

	/**
	 * @brief ���ҹ���һ��ҳ���С�ľ���
	 * @return ��
	 */
	virtual void PageRight();

	/**
	 * @brief �����������
	 * @return ��
	 */
	virtual void HomeLeft();

	/**
	 * @brief ���������Ҳ�
	 * @return ��
	 */
	virtual void EndRight();

	/**
	 * @brief �������Ϲ�������Ӧ WM_TOUCH ��Ϣ��
	 * @param[in] deltaValue ��������
	 * @return ��
	 */
	virtual void TouchUp(int deltaValue);

	/**
	 * @brief �������¹�������Ӧ WM_TOUCH ��Ϣ��
	 * @param[in] deltaValue ��������
	 * @return ��
	 */
	virtual void TouchDown(int deltaValue);

	/**
	 * @brief ���ù�����
	 * @param[in] bEnableVertical �Ƿ����ô�ֱ��������Ĭ��Ϊ true
	 * @param[in] bEnableHorizontal �Ƿ�����ˮƽ��������Ĭ��Ϊ true
	 * @return ��
	 */
	virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);

	/**
	 * @brief ��ȡ��ֱ����������ָ��
	 * @return ���ش�ֱ����������ָ��
	 */
	virtual ScrollBar* GetVerticalScrollBar() const;

	/**
	 * @brief ��ȡˮƽ����������ָ��
	 * @return ����ˮƽ����������ָ��
	 */
	virtual ScrollBar* GetHorizontalScrollBar() const;

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	virtual void ProcessVScrollBar(UiRect rc, int cyRequired);

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	virtual void ProcessHScrollBar(UiRect rc, int cxRequired);

	/**
	 * @brief �жϴ�ֱ�������Ƿ���Ч
	 * @return ���� true ��ʾ��Ч������ false Ϊ��Ч
	 */
	bool IsVScrollBarValid() const;

	/**
	 * @brief �ж�ˮƽ�������Ƿ���Ч
	 * @return ���� true ��ʾ��Ч������ false Ϊ��Ч
	 */
	bool IsHScrollBarValid() const;

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	void ReomveLastItemAnimation();

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	void PlayRenderOffsetYAnimation(int nRenderY);

	/**
	 * @brief �Ƿ��Ѿ��ڵײ�
	 * @return ���� true ��ʾ�Ѿ��ڵײ�������Ϊ false
	 */
	bool IsAtEnd() const;

	/**
	 * @brief �Ƿ��������ײ�
	 * @return �� true ��ʾ�����ڵײ�������Ϊ false
	 */
	bool IsHoldEnd() const;

	/**
	 * @brief ���ù������Ƿ�ʼ���������ײ�λ��
	 * @param[in] bHoldEnd ���� true ��ʾ������false Ϊ������
	 * @return ��
	 */
	void SetHoldEnd(bool bHoldEnd);
	
	/**
	 * @brief ��ȡ��ֱ��������������
	 * @return ���ع�������
	 */
	int GetVerScrollUnitPixels() const;

	/**
	 * @brief ���ô�ֱ��������������
	 * @param[in] nUnitPixels Ҫ���õĲ���
	 * @return ��
	 */
	void SetVerScrollUnitPixels(int nUnitPixels);

	/**
	* @brief ��ȡ�����������������
	* @return ���ع�������
	*/
	int GetHorScrollUnitPixels() const;

	/**
	* @brief ���ú����������������
	* @param[in] nUnitPixels Ҫ���õĲ���
	* @return ��
	*/
	void SetHorScrollUnitPixels(int nUnitPixels);

	/**
	 * @brief ��ȡ�����Ĺ������Ƿ��������ӿؼ�����
	 * @return ���� true ��ʾ�����ڹ������ϣ�����Ϊ false
	 */
	bool GetScrollBarFloat() const;

	/**
	 * @brief ���������Ĺ������Ƿ��������ӿؼ�����
	 * @param[in] bScrollBarFloat true ��ʾ�����ڹ������ϣ�false ��ʾ�������ڿؼ���
	 * @return ��
	 */
	void SetScrollBarFloat(bool bScrollBarFloat);

	/**
	 * @brief ��ȡ�����Ĺ������Ƿ��������ʾ
	 * @return ���� true ��ʾ����࣬false Ϊ�Ҳ�
	 */
	bool GetVScrollBarLeftPos() const;

	/**
	* @brief ���������Ĺ������Ƿ��������ʾ
	* @param[in] bLeftPos true ��ʾ����࣬false Ϊ�Ҳ�
	* @return ��
	*/
	void SetVScrollBarLeftPos(bool bLeftPos);

	/**
	 * @brief ��ȡ����������߾�
	 * @return ���ر߾���Ϣ 
	 */
	UiRect GetScrollBarPadding() const;

	/**
	 * @brief ���ù���������߾࣬�����ù�������ռ������
	 * @param[in] rcScrollBarPadding Ҫ���õı߾�
	 * @return ��
	 */
	void SetScrollBarPadding(UiRect rcScrollBarPadding);

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	bool GetDefaultDisplayScrollbar() const;

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	void SetDefaultDisplayScrollbar(bool bDefaultDisplay);

	/**
	 * @brief ����������λ�ñ仯�¼�
	 * @param[in] callback �б仯��֪ͨ�Ļص�����
	 * @return ��
	 */
	void AttachScrollChange(const EventCallback& callback) { OnEvent[kEventScrollChange] += callback; }
protected:

	/**
	 * @brief ��������ĳߴ�
	 * @param[in] rc ��ǰλ����Ϣ
	 * @return ��������ߴ��С
	 */
	virtual CSize CalcRequiredSize(const UiRect& rc);

	/**
	 * @brief ����ͼƬ���棬���� ScrollableBox �ڲ�ʹ��
	 * @param[in] bFromTopLeft ��������
	 * @return ��
	 */
	virtual void LoadImageCache(bool bFromTopLeft);
private:
	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	void SetPosInternally(UiRect rc);

protected:
	std::unique_ptr<ScrollBar> m_pVerticalScrollBar;
	std::unique_ptr<ScrollBar> m_pHorizontalScrollBar;

	int m_nVerScrollUnitPixels;
    int m_nHerScrollUnitPixels;
	bool m_bScrollProcess; // ��ֹSetPosѭ������
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
