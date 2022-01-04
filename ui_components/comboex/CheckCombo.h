#pragma once

namespace nim_comp
{
	using namespace ui;
	class CCheckComboWnd;
	class UILIB_API CheckCombo : public Box
	{
		friend class CCheckComboWnd;
	public:
		CheckCombo();
		CheckCombo(const CheckCombo& r) = delete;
		CheckCombo& operator=(const CheckCombo& r) = delete;
		virtual ~CheckCombo();

		/// 重写父类方法，提供个性化功能，请参考父类声明
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
		* @brief 获取当前所属的 List 对象
		* @return 返回所属的 List 对象指针
		*/
		ScrollableBox* GetListBox() { return m_pDropList.get(); }

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
		* @brief 获取所有子项数量
		* @return 返回所有子项数量
		*/
		virtual int GetCount() const { return m_pDropList->GetCount(); }

		///**
		//* @brief 监听子项被选择事件
		//* @param[in] callback 子项被选择后触发的回调函数
		//* @return 无
		//*/
		//void AttachSelect(const EventCallback& callback) { m_pLayout->AttachSelect(callback); }

	private:
		/**
		* @brief 默认的子项被选择处理函数
		* @param[in] args 参数列表
		* @return 始终返回 true
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