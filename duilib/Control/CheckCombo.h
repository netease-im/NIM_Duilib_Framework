#pragma once

namespace ui
{

	class CCheckComboWnd;
	class UILIB_API CheckCombo : public Box
	{
		friend class CCheckComboWnd;
	public:
		CheckCombo();
		CheckCombo(const CheckCombo& r) = delete;
		CheckCombo& operator=(const CheckCombo& r) = delete;
		virtual ~CheckCombo();

		/// ��д���෽�����ṩ���Ի����ܣ���ο���������
		virtual bool Add(Control* pControl) override;
		virtual bool Remove(Control* pControl) override;
		virtual bool RemoveAt(std::size_t iIndex) override;
		virtual void RemoveAll() override;
		virtual void Activate() override;
		virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

		virtual ui::UiRect GetOrgPos() const;
		//virtual void PaintText(IRenderContext* pRender) override;

		const std::vector<std::string>& GetAllDate() const{ return m_vecDate; };
		void ClearAllDate();

		/**
		* @brief ��ȡ��ǰ������ List ����
		* @return ���������� List ����ָ��
		*/
		ScrollableBox* GetListBox() { return m_pDropList.get(); }

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
		* @brief ��ȡ������������
		* @return ����������������
		*/
		virtual int GetCount() const { return m_pDropList->GetCount(); }

		///**
		//* @brief �������ѡ���¼�
		//* @param[in] callback ���ѡ��󴥷��Ļص�����
		//* @return ��
		//*/
		//void AttachSelect(const EventCallback& callback) { m_pLayout->AttachSelect(callback); }

	private:
		/**
		* @brief Ĭ�ϵ����ѡ������
		* @param[in] args �����б�
		* @return ʼ�շ��� true
		*/
		bool OnSelectItem(EventArgs* args);
		bool OnUnSelectItem(EventArgs* args);

		bool OnListButtonDown(EventArgs* args);

	protected:
		CCheckComboWnd *m_pCheckComboWnd;
		std::unique_ptr<ScrollableBox> m_pDropList;

		std::unique_ptr<ScrollableBox> m_pList;
		std::vector<std::string> m_vecDate;
		
		ControlStateType m_uButtonState;
		CSize m_szDropBox;
		std::wstring m_sDropBoxAttributes;
		bool m_bPopupTop;

		int m_iOrgHeight;
	};
}