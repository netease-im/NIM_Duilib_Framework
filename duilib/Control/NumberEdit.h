#ifndef UI_CONTROL_NUMBEREDIT_H_
#define UI_CONTROL_NUMBEREDIT_H_

#pragma once

namespace ui 
{

	class UILIB_API NumberEdit : public RichEdit
	{
	public:

	public:
		NumberEdit();
		NumberEdit(const NumberEdit& r) = delete;
		NumberEdit& operator=(const NumberEdit& r) = delete;

		/**
		 * @brief 判断是否仅支持整数
		 * @return 返回 true 表示是
		 */
		bool IsIntOnly();

		/**
		 * @brief 设置是否仅支持整数
		 * @param[in] bIntOnly 为 true 则只能输入整数，false 为实数
		 * @return 无
		 */
		void SetIntOnly(bool bIntOnly = false);

		/**
		 * @brief 判断是否允许负数
		 * @return 返回 true 表示允许
		 */
		bool IsAllowNegative();

		/**
		 * @brief 设置是否允许负数
		 * @param[in] bIntOnly 为 true 则允许输入负数
		 * @return 无
		 */
		void SetAllowNegative(bool bAllowNegative = false);

		 /**
		  * @brief 返回文本的整数值
		  */
		int GetIntValue(int def = 0);

		/**
		 * @brief 返回文本的实数值
		 */
		double GetValue(double def=0);

		/**
		 * @brief 给文本框设置一个整数值
		 */
		void SetValue(int value);

		/**
		 * @brief 给文本框设置一个实数值
		 */
		void SetValue(double value);

		/**
         * @brief 判断是否开启最大值控制
         * @return 返回 true 表示是
        */
		bool IsMaxValueEnabled();

		/**
		 * @brief 设置是否开启最大值控制
		 * @param[in] bMaxValueEnabled 为 true 开启
		 * @return 无
		 */
		void SetMaxValueEnabled(bool bMaxValueEnabled = false);

		/**
		 * @brief 判断是否开启最小值控制
		 * @return 返回 true 表示是
		*/
		bool IsMinValueEnabled();

		/**
		 * @brief 设置是否开启最小值控制
		 * @param[in] bMinValueEnabled 为 true 开启
		 * @return 无
		 */
		void SetMinValueEnabled(bool bMinValueEnabled = false);

		/**
		 * @brief 返回最大值设定
		 */
		int GetMaxValue();

		/**
		 * @brief 设置最大值
		 */
		void SetMaxValue(int value);

		/**
		 * @brief 返回最小值设定
		 */
		int GetMinValue();

		/**
		 * @brief 设置最小值
		 */
		void SetMinValue(int value);

		 /**
		  * @brief 设置控件的文本
		  * @param[in] strText 要设置的文本内容
		  * @return 无
		  */
		virtual void SetText(const std::wstring& strText) override;

		virtual void OnChar(EventArgs& event) override;
		virtual void OnKeyDown(EventArgs& event) override;
		virtual bool OnTxTextChanged() override;

		virtual void SetAttribute(const std::wstring& pstrName, const std::wstring& pstrValue) override;

	protected:
		bool m_bIntOnly; //仅支持整数 默认否
		bool m_bAllowNegative; //允许负数 默认否
		bool m_bMaxValueEnabled; //启用最大值控制
		bool m_bMinValueEnabled; //启用最小值控制
		int m_iMaxValue; //最大值
		int m_iMinValue; //最小值
	private:
		std::wstring _ToNumberStr(std::wstring str);
	};
	

} // namespace ui

#endif // UI_CONTROL_RICHEDIT_H_
