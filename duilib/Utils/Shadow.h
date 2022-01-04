#ifndef UI_UTILS_SHADOW_H_
#define UI_UTILS_SHADOW_H_

#pragma once

namespace ui 
{

class Box;
class UILIB_API Shadow
{
public:
	Shadow();

	/**
	 * @brief 设置是否附加阴影效果
	 * @param[in] bShadowAttached 设置 true 为是，false 为否
	 * @return 无
	 */
	void SetShadowAttached(bool bShadowAttached) { m_bShadowAttached = bShadowAttached; }

	/**
	 * @brief 判断是否已经附加了阴影
	 * @return 返回 true 表示已经附加，否则为 false
	 */
	bool IsShadowAttached() const { return m_bShadowAttached; }

	/**
	 * @brief 设置阴影的九宫格属性
	 * @param[in] rect 要设置的九宫格属性
	 * @param[in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
	 * @return 无
	 */
	void SetShadowCorner(const UiRect &rect, bool bNeedDpiScale = true);

	/**
	 * @brief 获取阴影的九宫格属性
	 * @return 返回阴影的九宫格属性
	 */
	UiRect GetShadowCorner() const;

	/**
	* @brief 重置为默认阴影效果
	* @return 无
	*/
	void ResetDefaultShadow();

	/**
	 * @brief 设置阴影图片
	 * @param[in] image 图片路径
	 * @return 无
	 */
	void SetShadowImage(const std::wstring &image);

	/**
	 * @brief 获取阴影图片位置
	 * @return 返回阴影图片位置
	 */
	std::wstring GetShadowImage() const;

	/**
	 * @brief 将阴影附加到窗口
	 * @param[in] pRoot 窗口的顶层容器
	 * @return 返回附加阴影后的容器指针
	 */
	Box* AttachShadow(Box* pRoot);

	/**
	 * @brief 设置窗口最大化还是还原状态
	 * @param[in] isMaximized 设置为 true 表示最大化，false 为还原初始状态
	 * @return 无
	 */
	void MaximizedOrRestored(bool isMaximized);

	/**
	 * @brief 获取附加阴影后的容器指针
	 * @return 返回容器指针
	 */
	Control* GetRoot();

	/**
	 * @brief 清理图片缓存
	 * @return 无
	 */
	void ClearImageCache();
private:
	bool m_bShadowAttached;
	bool m_bUseDefaultImage;
	std::wstring m_strImage;
	UiRect m_rcShadowCorner;
	UiRect m_rcShadowCornerBackup;

	Box* m_pRoot;
};

}

#endif // UI_UTILS_SHADOW_H_