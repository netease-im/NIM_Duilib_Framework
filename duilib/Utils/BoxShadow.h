#ifndef UI_UTILS_BOXSHADOW_H_
#define UI_UTILS_BOXSHADOW_H_

#pragma once

namespace ui {

// 控件阴影属性类
// 待优化：
// 1.实现真正高斯模糊
//   参考资料：
//           https://www.cnblogs.com/mfbzr/p/14154156.html
//           https://stackoverflow.com/questions/42314939/diagonal-shadow-with-gdi
//           https://blog.csdn.net/aoshilang2249/article/details/45153375
//   算法:
//           http://blog.ivank.net/fastest-gaussian-blur.html
//           https://software.intel.com/content/www/us/en/develop/articles/iir-gaussian-blur-filter-implementation-using-intel-advanced-vector-extensions.html
//           https://software.intel.com/content/dam/develop/public/us/en/downloads/gaussian_blur_0311.cpp
// 2.阴影应提前绘制好并进行缓存，而不是实时绘制
class UILIB_API BoxShadow {
public:
	BoxShadow();
	~BoxShadow();

	/**
	 * @brief 设置阴影属性
	 * @param[in] strBoxShadow 要设置的属性,如 "color='black' offset='1,1' blursize='2'"
	 * @return 无
	 */
	void SetBoxShadowString(const std::wstring& strBoxShadow);

	/**
	 * @brief 是否有阴影
	 * @return 是或否
	 */
	bool HasShadow() const;

	// 阴影属性
	// 暂时仅仅支持 color offset blurSize
	// https://codersblock.com/blog/creating-glow-effects-with-css/

	std::wstring m_strColor;
	CPoint m_cpOffset;
	int m_nBlurRadius;
	int m_nBlurSize;
	int m_nSpreadSize;
	bool m_bExclude;
};

} // namespace ui

#endif // !UI_UTILS_BOXSHADOW_H_

