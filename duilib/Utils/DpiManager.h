#ifndef UI_UTILS_DPIMANAGER_H_
#define UI_UTILS_DPIMANAGER_H_

#pragma once

namespace ui
{
/** @class DpiManager
  * @brief DPI适配管理类
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2016/10/10
  */
class UILIB_API DpiManager
{
public:
	static DpiManager* GetInstance();

	/**
	* @brief 获取系统DPI，开启DPI感知后有效
	* @return UINT DPI
	*/
	static UINT GetSystemDPI();

	/**
	* @brief 获取某个显示器的DPI，开启DPI感知后有效
	* @param[in] HMONITOR句柄
	* @return UINT DPI
	*/
	static UINT GetMonitorDPI(HMONITOR hMonitor);

	/**
	* @brief 获取主显示器DPI，开启DPI感知后有效
	* @return UINT DPI
	*/
	static UINT GetMainMonitorDPI();
		
	/**
	* @brief 从注册表获取DPI，不开启DPI感知也有效
	* @return UINT DPI
	*/
	static UINT GetDPIFromRegistry();

	/**
	* @brief 是否程序自己适配DPI缩放效果
	* @return bool true 是，false 否
	*/
	bool IsAdaptDPI();

	/**
	* @brief 设置是否程序自己适配DPI缩放，默认不自己适配
	* @return bool true 设置成功，false 设置失败
	*/
	bool SetAdaptDPI();

	/**
	* @brief 获取当前界面缩放比
	* @return UINT	缩放比
	*/
	UINT GetScale();

	/**
	* @brief 根据DPI值设置界面缩放比，只有程序自己处理DPI缩放时才有效
	* @param[in] uDPI DPI值
	* @return void	无返回值
	*/
	void SetScale(UINT uDPI);

	/**
	* @brief 根据界面缩放比来缩放整数，只有程序自己处理DPI缩放时才有效
	* @param[in] iValue 整数
	* @return int	缩放后的值
	*/
	int ScaleInt(int &iValue);

	/**
	* @brief 根据界面缩放比来缩放SIZE，只有程序自己处理DPI缩放时才有效
	* @param[in] pSize SIZE指针
	* @return void	无返回值
	*/
	void ScaleSize(SIZE &size);

	void ScaleSize(CSize &size);

	/**
	* @brief 根据界面缩放比来缩放POINT，只有程序自己处理DPI缩放时才有效
	* @param[in] pSize SIZE指针
	* @return void	无返回值
	*/
	void ScalePoint(POINT &point);

	void ScalePoint(CPoint &point);

	/**
	* @brief 根据界面缩放比来缩放RECT，只有程序自己处理DPI缩放时才有效
	* @param[in] pSize SIZE指针
	* @return void	无返回值
	*/
	void ScaleRect(RECT &rect);

	void ScaleRect(UiRect &rect);

	/**
	* @brief 设置最大缩放因子,在没有完成自适应之前需要限定最大缩放因子
	* @param[in] nScaleFactor 最大缩放因子,-1 表示不做限制
	* @return void	无返回值
	*/
	void LimitScaleFactor(unsigned int nScaleFactor);

private:
	DpiManager();
	~DpiManager() {};	
	DpiManager(const DpiManager&) = delete;
	DpiManager& operator = (const DpiManager&) = delete;

private:
	int		m_nScaleFactor;
	int   m_nLimitScaleFactor;
	bool	m_bAdaptDPI;
};
}
#endif //UI_UTILS_DPIMANAGER_H_