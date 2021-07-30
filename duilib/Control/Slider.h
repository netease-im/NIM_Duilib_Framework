#ifndef UI_CONTROL_SLIDER_H_
#define UI_CONTROL_SLIDER_H_

#pragma once

namespace ui
{

class UILIB_API Slider : public Progress
{
public:
	Slider();

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetType() const override;
	virtual UiRect GetProgressPos() override;
	virtual void HandleMessage(EventArgs& event) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintStatusImage(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;
	virtual void PaintBkColor(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;
	virtual void ClearImageCache() override;

	/**
	 * @brief ��ȡ��������
	 * @return ���ز�������
	 */
	int GetChangeStep();

	/**
	 * @brief ���ò�������
	 * @param[in] step Ҫ���õĲ���
	 * @return ��
	 */
	void SetChangeStep(int step);

	/**
	 * @brief ���öද����Ĵ�С
	 * @param[in] szXY Ҫ���õĴ�С
	 * @return ��
	 */
	void SetThumbSize(CSize szXY);

	/**
	 * @brief ��ȡ����ľ�����Ϣ
	 * @return ���ػ���ľ�����Ϣ
	 */
	UiRect GetThumbRect() const;

	/**
	 * @brief ��ȡָ��״̬�»����ͼƬ
	 * @param[in] stateType Ҫ��ȡ��״̬��ʶ���ο� ControlStateType ö��
	 * @return ����ͼƬ·��
	 */
	std::wstring GetThumbStateImage(ControlStateType stateType);

	/**
	 * @brief ����ָ��״̬�»����ͼƬ
	 * @param[in] stateType Ҫ���õ�״̬��ʶ���ο� ControlStateType ö��
	 * @param[in] pStrImage Ҫ���õ�ͼƬλ��
	 * @return ��
	 */
	void SetThumbStateImage(ControlStateType stateType, const std::wstring& pStrImage);

	/**
	 * @brief ��ȡ�������ڱ߾�
	 * @return �����ڱ߾���Ϣ
	 */
	UiRect GetProgressBarPadding() const;

	/**
	 * @brief ���ý������ڱ߾�
	 * @param[in] rc Ҫ���õı߾���Ϣ
	 * @return ��
	 */
	void SetProgressBarPadding(UiRect rc);

	/**
	 * @brief �������������ȸı��¼�
	 * @param[in] callback ���������ȸı����õĻص�����
	 * @return ��
	 */
	void AttachValueChange(const EventCallback& callback) {	OnEvent[kEventValueChange] += callback;	}

protected:
	int m_nStep;
	CSize m_szThumb;
	StateImage m_thumbStateImage;
	UiRect	m_rcProgressBarPadding;
	ControlStateType m_uButtonState;
	std::wstring m_sImageModify;
};

}

#endif // UI_CONTROL_SLIDER_H_