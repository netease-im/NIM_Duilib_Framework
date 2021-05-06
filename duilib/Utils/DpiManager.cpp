#include "StdAfx.h"
#include "DpiManager.h"

namespace ui
{

DpiManager* DpiManager::GetInstance()
{
	static DpiManager dpiManager;
	return &dpiManager;
}

DpiManager::DpiManager()
{
	m_nScaleFactor = 100;
	m_nLimitScaleFactor = (UINT)-1;
	m_bAdaptDPI = false;
}

UINT DpiManager::GetSystemDPI()
{
	UINT dpi = 96;
	if (IsWindows8OrGreater()) {
		if (!GetDpiForSystemWrapper(dpi)) {
			dpi = 96;
		}
	}
	else {
		HDC desktopDc = GetDC(NULL);
		dpi = (UINT)GetDeviceCaps(desktopDc, LOGPIXELSX);
		ReleaseDC(0, desktopDc);
	}

	return dpi;
}

UINT DpiManager::GetMonitorDPI(HMONITOR hMonitor)
{
	UINT dpix = 96, dpiy = 96;
	if (IsWindows8OrGreater()) {
		if (!GetDpiForMonitorWrapper(hMonitor, MDT_EFFECTIVE_DPI, &dpix, &dpiy)) {
			dpix = 96;
		}
	}
	else {
		HDC desktopDc = GetDC(NULL);
		dpix = (UINT)GetDeviceCaps(desktopDc, LOGPIXELSX);
		ReleaseDC(0, desktopDc);
	}

	return dpix;
}

UINT DpiManager::GetMainMonitorDPI()
{
	POINT pt = { 1, 1 };
	HMONITOR hMonitor;
	hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);

	return GetMonitorDPI(hMonitor);
}

UINT DpiManager::GetDPIFromRegistry()
{
	int dpi_value = 96;
	HKEY hKEY;
	long ret = 0;
	ret = ::RegOpenKeyEx(HKEY_CURRENT_USER, L"CONTROL PANEL\\DESKTOP\\WINDOWMETRICS", 0, KEY_ALL_ACCESS, &hKEY);
	if (ret != ERROR_SUCCESS)
	{
		::RegCloseKey(hKEY);
	}
	else
	{
		DWORD data_size = 4;
		int data;
		DWORD type = REG_DWORD;
		ret = ::RegQueryValueEx(hKEY, L"AppliedDPI", NULL, &type, (LPBYTE)&data, &data_size);
		if (ret != ERROR_SUCCESS)
		{
			::RegCloseKey(hKEY);
		}
		else
		{
			if (data > 0)
			{
				dpi_value = data;
			}
			::RegCloseKey(hKEY);
		}
	}

	return dpi_value;
}

bool DpiManager::IsAdaptDPI()
{
	if (m_bAdaptDPI)
		return true;

	if(IsWindowsVistaOrGreater()) {
		PROCESS_DPI_AWARENESS awareness = PROCESS_DPI_UNAWARE;
		bool bAware = false;
		if (GetProcessDPIAwarenessWrapper(awareness)) {
			m_bAdaptDPI = awareness != PROCESS_DPI_UNAWARE;
		}
		else if (IsProcessDPIAwareWrapper(bAware)) {
			m_bAdaptDPI = bAware;
		}
		else{
			m_bAdaptDPI = false;
		}
	}

	return m_bAdaptDPI;
}

bool DpiManager::SetAdaptDPI()
{
	if (m_bAdaptDPI)
		return true;
	
	if(IsWindowsVistaOrGreater()) {
		if (SetProcessDPIAwarenessWrapper(PROCESS_PER_MONITOR_DPI_AWARE)) {
			m_bAdaptDPI = true;
		}
		else if(SetProcessDPIAwareWrapper()){
			m_bAdaptDPI = true;
		}
		else{
			m_bAdaptDPI = false;
		}
	}
	else {
		m_bAdaptDPI = true;
	}

	return m_bAdaptDPI;
}

UINT DpiManager::GetScale()
{
	if (!m_bAdaptDPI)
		return 100;

	return m_nScaleFactor;
}

void DpiManager::SetScale(UINT uDPI)
{
	if (m_bAdaptDPI)
		m_nScaleFactor = MulDiv(uDPI, 100, 96);

	if (m_nLimitScaleFactor != (UINT)-1)
		m_nScaleFactor = min(m_nScaleFactor, m_nLimitScaleFactor);

	ASSERT(m_nScaleFactor >= 100);
}

int DpiManager::ScaleInt(int &iValue)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100)
		return iValue;

	iValue = MulDiv(iValue, m_nScaleFactor, 100);
	return iValue;
}

void DpiManager::ScaleSize(SIZE &size)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100)
		return;

	size.cx = MulDiv(size.cx, m_nScaleFactor, 100);
	size.cy = MulDiv(size.cy, m_nScaleFactor, 100);
}

void DpiManager::ScaleSize(CSize &size)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100)
		return;

	size.cx = MulDiv(size.cx, m_nScaleFactor, 100);
	size.cy = MulDiv(size.cy, m_nScaleFactor, 100);
}

void DpiManager::ScalePoint(POINT &point)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100)
		return;

	point.x = MulDiv(point.x, m_nScaleFactor, 100);
	point.y = MulDiv(point.y, m_nScaleFactor, 100);
}


void DpiManager::ScalePoint(CPoint &point)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100)
		return;

	point.x = MulDiv(point.x, m_nScaleFactor, 100);
	point.y = MulDiv(point.y, m_nScaleFactor, 100);
}

void DpiManager::ScaleRect(RECT &rect)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100)
		return;

	int width = MulDiv(rect.right - rect.left, m_nScaleFactor, 100);
	int height = MulDiv(rect.bottom - rect.top, m_nScaleFactor, 100);
	rect.left = MulDiv(rect.left, m_nScaleFactor, 100);
	rect.top = MulDiv(rect.top, m_nScaleFactor, 100);
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;
}

void DpiManager::ScaleRect(UiRect &rect)
{
	if (!m_bAdaptDPI || m_nScaleFactor == 100)
		return;

	int width = MulDiv(rect.right - rect.left, m_nScaleFactor, 100);
	int height = MulDiv(rect.bottom - rect.top, m_nScaleFactor, 100);
	rect.left = MulDiv(rect.left, m_nScaleFactor, 100);
	rect.top = MulDiv(rect.top, m_nScaleFactor, 100);
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;
}

void DpiManager::LimitScaleFactor(unsigned int nScaleFactor)
{
	m_nLimitScaleFactor = nScaleFactor;
}

}