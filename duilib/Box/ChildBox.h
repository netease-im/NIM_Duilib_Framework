#ifndef UI_CORE_CHILDBOX_H_
#define UI_CORE_CHILDBOX_H_

#pragma once

namespace ui
{

class UILIB_API ChildBox : public Box
{
public:
	ChildBox();

	/// 重写父类方法，提供个性化功能，请参考父类声明
	virtual std::wstring GetType() const override;
	virtual void Init() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/**
	 * @brief 设置 ChildBox 关联的 XML 文件
	 * @param[in] strXML XML 文件路径
	 * @return 无
	 */
	void SetChildLayoutXML(std::wstring strXML);

	/**
	 * @brief 获取 ChildBox 关联的 XML 文件
	 * @return 返回 ChildBox 关联的 XML 文件
	 */
	std::wstring GetChildLayoutXML();

private:
	std::wstring m_strXMLFile;
};

} // namespace ui
#endif // UI_CORE_CHILDBOX_H_
